#include "stdafx.h"
#include "PreviewAnimationModel.h"
#include "Model.h"
#include "shader.h"
#include "Animation.h"
#include "GameManager.h"
#include "Corvus_DefaultSaber.h"
#include "Corvus_DefaultDagger.h"
#include "MobWeapon.h"
#include "Weapon.h"

GAMECLASS_C(CPreviewAnimationModel)
CLONE_C(CPreviewAnimationModel, CGameObject)

weak_ptr<CModel> CPreviewAnimationModel::Get_CurrentModel()
{
	return m_pModelCom;
}


HRESULT CPreviewAnimationModel::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}


HRESULT CPreviewAnimationModel::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_eAttackCollisionLayer = COLLISION_LAYER::PLAYER_ATTACK;

	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::MONSTER, Weak_Cast<CGameObject>(m_this));

	m_fCullingRange = FLT_MAX;
	return S_OK;
}

void CPreviewAnimationModel::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPreviewAnimationModel::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, Weak_Cast<CGameObject>(m_this));
}

void CPreviewAnimationModel::Thread_PreTick(_float fTimeDelta)
{
	if (m_pCurrentModelCom.lock())
		m_pCurrentModelCom.lock()->Update_BoneMatrices();
}

HRESULT CPreviewAnimationModel::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	if (!m_pCurrentModelCom.lock())
		return E_FAIL;

	_uint iNumMeshContainers(m_pCurrentModelCom.lock()->Get_NumMeshContainers());
	for (_uint i(0); i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pCurrentModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		m_pCurrentModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 0, "g_Bones", pDeviceContext);
	}

	return S_OK;
}

HRESULT CPreviewAnimationModel::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
	if (!m_pCurrentModelCom.lock())
		return E_FAIL;

	CallBack_Bind_SRV(m_pShaderCom, "");

	_float	fDissolveAmount(-1.1f);
	m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float));

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

	_uint iNumMeshContainers(m_pCurrentModelCom.lock()->Get_NumMeshContainers());
	for (_uint i(0); i < iNumMeshContainers; ++i)
	{
		//m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

			//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		m_pCurrentModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 1, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CPreviewAnimationModel::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
	_vector vShaderFlag(XMVectorSet(0.f, 0.f, 0.f, 0.f));
	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));
#ifndef _USE_THREAD_
	if(m_pCurrentModelCom.lock())
		m_pCurrentModelCom.lock()->Update_BoneMatrices();
#endif // !_USE_THREAD_
}

void CPreviewAnimationModel::Init_EditPreviewAnimationModel(const string& In_szModelKey)
{
	m_pModelCom.lock()->Init_Model(In_szModelKey.c_str(),"", (_uint)TIMESCALE_LAYER::EDITER);
	m_pCurrentModelCom = m_pModelCom;

	if (!strcmp(In_szModelKey.c_str(), "Corvus"))
	{
		this->Clear_ModelWeapon();
#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultSaber>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, m_pTransformCom, "weapon_r");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultDagger>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, m_pTransformCom, "weapon_l");
#endif // _ANIMATION_TOOL_WEAPON_

		m_pModelCom.lock()->Add_ReverseAnimation(m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_R_UP_End"), (_uint)TIMESCALE_LAYER::EDITER);
	}

	if (!strcmp(In_szModelKey.c_str(), "Boss_Varg"))
	{
		this->Clear_ModelWeapon();

#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_VargWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, m_pTransformCom, "weapon_r");
#endif // _ANIMATION_TOOL_WEAPON_
	}

	if (!strcmp(In_szModelKey.c_str(), "Elite_Joker"))
	{
		this->Clear_ModelWeapon();

#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Joker_Weapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, m_pTransformCom, "weapon_r");
#endif // _ANIMATION_TOOL_WEAPON_
	}
}

void CPreviewAnimationModel::Change_AnimationFromIndex(const _uint& In_iAnimIndex)
{
	m_pCurrentModelCom.lock()->Set_CurrentAnimation(In_iAnimIndex, 0, 0.f);
}

void CPreviewAnimationModel::Play_Animation(_float fTimeDelta)
{
	if (!m_pCurrentModelCom.lock())
		return;

	m_pCurrentModelCom.lock()->Play_Animation(fTimeDelta);
}

void CPreviewAnimationModel::Add_DebugWeapon(const string& In_szBoneName)
{
	m_pDebugWeapons.push_back(GAMEINSTANCE->Add_GameObject<CWeapon>(LEVEL_EDIT));
	m_pDebugWeapons.back().lock()->Init_Weapon(m_pCurrentModelCom, m_pTransformCom, In_szBoneName);
}

void CPreviewAnimationModel::Clear_DebugWeapon()
{
	for (auto& elem : m_pDebugWeapons)
		elem.lock()->Set_Dead();

	m_pDebugWeapons.clear();
}

void CPreviewAnimationModel::Clear_ModelWeapon()
{
	for (auto& elem : m_pModelWeapons)
		elem.lock()->Set_Dead();

	m_pModelWeapons.clear();
}


void CPreviewAnimationModel::Release_BeforeModel()
{

}

void CPreviewAnimationModel::Free()
{

}
