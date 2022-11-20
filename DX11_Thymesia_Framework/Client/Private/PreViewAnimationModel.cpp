#include "stdafx.h"
#include "PreViewAnimationModel.h"
#include "Model.h"
#include "shader.h"
#include "Animation.h"
#include "GameManager.h"
#include "Corvus_DefaultSaber.h"
#include "Corvus_DefaultDagger.h"
#include "Weapon.h"

GAMECLASS_C(CPreViewAnimationModel)
CLONE_C(CPreViewAnimationModel, CGameObject)


weak_ptr<CModel> CPreViewAnimationModel::Get_CurrentModel()
{
	return m_pCurrentModelCom;
}


HRESULT CPreViewAnimationModel::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}


HRESULT CPreViewAnimationModel::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_eAttackCollisionLayer = COLLISION_LAYER::PLAYER_ATTACK;

	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::MONSTER, Weak_Cast<CGameObject>(m_this));

	return S_OK;
}

void CPreViewAnimationModel::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
}

void CPreViewAnimationModel::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, Weak_Cast<CGameObject>(m_this));
}

void CPreViewAnimationModel::Custom_Thread0(_float fTimeDelta)
{
	if (m_pCurrentModelCom.lock())
		m_pCurrentModelCom.lock()->Update_BoneMatrices();
}

HRESULT CPreViewAnimationModel::Render()
{
		__super::Render();

	if (!m_pCurrentModelCom.lock())
		return E_FAIL;

	_uint iNumMeshContainers = m_pCurrentModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pCurrentModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		m_pCurrentModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 0, "g_Bones");
	}

	return S_OK;
}

HRESULT CPreViewAnimationModel::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
	if (!m_pCurrentModelCom.lock())
		return E_FAIL;

	CallBack_Bind_SRV(m_pShaderCom, "");

	_float	fDissolveAmount = -1.1f;
	m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float));

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

	

	_uint iNumMeshContainers = m_pCurrentModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		//m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

			//m_pShaderCom.lock()->Begin(m_iPassIndex);

		m_pCurrentModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 1, "g_Bones");
		//m_pModelCom.lock()->Render_Mesh(i);
	}

	return S_OK;
}

void CPreViewAnimationModel::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	_vector vLightFlag = { 1.f, 0.f, 0.f, 0.f };

	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));

#ifndef _USE_THREAD_
	if(m_pCurrentModelCom.lock())
		m_pCurrentModelCom.lock()->Update_BoneMatrices();
#endif // !_USE_THREAD_
	

}

void CPreViewAnimationModel::Init_EditPreViewAnimationModel(const string& In_szModelKey)
{
	auto iter = find_if(m_pModelComs.begin(), m_pModelComs.end(), CTag_Finder_c_str(In_szModelKey.c_str()));

	if (m_pModelComs.end() != iter)
	{
		m_pCurrentModelCom = iter->second;
		m_pCurrentModelCom.lock()->Set_CurrentAnimation(0);
		return;
	}


	Clear_DebugWeapon();

	shared_ptr<MODEL_DATA> pModelData = GAMEINSTANCE->Get_ModelFromKey(In_szModelKey.c_str());

	//모델키와 일치하는 모델이 없음.
	if (!pModelData)
	{
		DEBUG_ASSERT;
		return;
	}

	weak_ptr<CModel> pModel = Add_Component<CModel>();

	pModel.lock()->Init_Model(In_szModelKey.c_str(),"", (_uint)TIMESCALE_LAYER::EDITER);

	m_pModelComs.emplace(In_szModelKey, pModel);
	m_pCurrentModelCom = pModel;
	m_pCurrentModelCom.lock()->Set_CurrentAnimation(0);

	if (strcmp(In_szModelKey.c_str(), "Corvus") == 0)
	{
		Clear_ModelWeapon();

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultSaber>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, Weak_Cast<CGameObject>(m_this), "weapon_r");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultDagger>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, Weak_Cast<CGameObject>(m_this), "weapon_l");
	}
}

void CPreViewAnimationModel::Change_AnimationFromIndex(const _uint& In_iAnimIndex)
{
	m_pCurrentModelCom.lock()->Set_CurrentAnimation(In_iAnimIndex, 0, 0.f);
}

void CPreViewAnimationModel::Play_Animation(_float fTimeDelta)
{
	if (!m_pCurrentModelCom.lock())
		return;

	m_pCurrentModelCom.lock()->Play_Animation(fTimeDelta);

}

void CPreViewAnimationModel::Add_DebugWeapon(const string& In_szBoneName)
{
	m_pDebugWeapons.push_back(GAMEINSTANCE->Add_GameObject<CWeapon>(LEVEL_EDIT));
	m_pDebugWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, Weak_Cast<CGameObject>(m_this), In_szBoneName);
}

void CPreViewAnimationModel::Set_WeaponDesc(const _float& In_fScale, const _float3& In_vOffset, const _float& In_fDamage, const HIT_TYPE& In_eHitType)
{
	/*for (auto& elem : m_pDebugWeapons)
	{
		elem.lock()->Set_WeaponDesc(In_fScale, In_vOffset, In_eHitType, In_fDamage);
	}*/

}

void CPreViewAnimationModel::Clear_DebugWeapon()
{
	for (auto& elem : m_pDebugWeapons)
	{
		elem.lock()->Set_Dead();
	}
	m_pDebugWeapons.clear();
}

void CPreViewAnimationModel::Clear_ModelWeapon()
{
	for (auto& elem : m_pModelWeapons)
	{
		elem.lock()->Set_Dead();
	}
}


void CPreViewAnimationModel::Release_BeforeModel()
{

}

void CPreViewAnimationModel::Free()
{

}
