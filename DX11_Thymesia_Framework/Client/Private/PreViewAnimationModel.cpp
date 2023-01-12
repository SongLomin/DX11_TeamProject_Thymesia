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
#include "BoneNode.h"
#include "MeshContainer.h"

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

#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
#endif // _USE_THREAD_


	return S_OK;
}

void CPreviewAnimationModel::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

#ifdef _DEBUG
	/*if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		++m_iContainerIndex;
		cout << "m_iContainerIndex : " << m_iContainerIndex << endl;
	}*/
#endif // _DEBUG
}

void CPreviewAnimationModel::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, Weak_Cast<CGameObject>(m_this));
}

void CPreviewAnimationModel::Thread_PreTick(_float fTimeDelta)
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->Update_BoneMatrices();
}

void CPreviewAnimationModel::Thread_PreBeforeRender(_float fTimeDelta)
{
	__super::Thread_PreBeforeRender(fTimeDelta);

	if (!m_pTargetBoneNode.lock())
		return;

	

	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();

	_matrix		BoneMatrix;
	_matrix		InverseMatrix;
	_vector		vGravity;


	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Head").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	_vector vSpherePos = XMVectorSet(0.2f, 0.f, 0.f, 1.f);
	vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);
	PxVec4 spheres[5];

	spheres[0] = PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.15f);

	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Neck").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	vSpherePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);

	spheres[1] = PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.2f);

	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Neck").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	vSpherePos = XMVectorSet(0.1f, 0.f, 0.f, 1.f);
	vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);

	spheres[2] = PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.2f);

	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Spine2").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	vSpherePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);

	spheres[3] = PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.3f);

	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	vSpherePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);

	spheres[4] = PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.3f);

	nv::cloth::Range<const physx::PxVec4> sphereRange(spheres, spheres + 5);

	m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->setSpheres(sphereRange, 0, m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->getNumSpheres());
	m_pModelCom.lock()->Get_MeshContainer(3).lock()->Get_NvCloth()->setSpheres(sphereRange, 0, m_pModelCom.lock()->Get_MeshContainer(3).lock()->Get_NvCloth()->getNumSpheres());

	//Bip001-Ponytail1
	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Ponytail1").lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//InverseMatrix = XMMatrixInverse(nullptr, BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix());

	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), XMMatrixRotationX(XMConvertToRadians(90.f)) * InverseMatrix);
	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), InverseMatrix * XMMatrixRotationX(XMConvertToRadians(-90.f)));

	m_pModelCom.lock()->Get_MeshContainer(1).lock()->Update_NvClothVertices(pDeferredContext,
		m_pModelCom.lock()->Find_BoneNode("Bip001-Ponytail1").lock()->Get_OffsetMatrix() * BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix(),
		XMVectorSet(0.f, -9.81f , 0.f, 0.f));



	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Xtra10").lock()->Get_CombinedMatrix()
		* XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//InverseMatrix = XMMatrixInverse(nullptr, BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix());

	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), XMMatrixRotationX(XMConvertToRadians(0.f)) * InverseMatrix);
	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), InverseMatrix);

	// "Bip001-Xtra10"

	m_pModelCom.lock()->Get_MeshContainer(3).lock()->Update_NvClothVertices(pDeferredContext,
		m_pModelCom.lock()->Find_BoneNode("Bip001-Xtra10").lock()->Get_OffsetMatrix() * BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix(),
		XMVectorSet(0.f, -9.81f, 0.f, 0.f));

	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);

}

HRESULT CPreviewAnimationModel::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	_uint iNumMeshContainers(m_pModelCom.lock()->Get_NumMeshContainers());

#ifdef _DEBUG
	//if (m_iContainerIndex >= iNumMeshContainers)
	//	m_iContainerIndex = 0;
#endif //_DEBUG

	_flag BindTextureFlag;

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		BindTextureFlag = 0;


		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			BindTextureFlag |= (1 << aiTextureType_NORMALS);
		}

		if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
		{
			BindTextureFlag |= (1 << aiTextureType_SPECULAR);
		}

		// DiffuseTexture	NO.
		if (!((1 << aiTextureType_DIFFUSE) & BindTextureFlag))
		{
#ifdef _DEBUG
			cout << "DiffuseTexture is not binded. : " << m_pModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG
			continue;
		}

		// NormalTexture	OK.
		// ORMTexture		OK.
		// NoneCulling		OK.
		else if (
			(1 << aiTextureType_NORMALS) & BindTextureFlag &&
			(1 << aiTextureType_SPECULAR) & BindTextureFlag)
		{
			m_iPassIndex = 8;
		}

		// NormalTexture	OK.
		// ORMTexture		NO.
		// NoneCulling		OK.
		else if (
			(1 << aiTextureType_NORMALS) & BindTextureFlag &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			m_iPassIndex = 11;
		}

		// NormalTexture	NO.
		// ORMTexture		NO.
		else if (
			!((1 << aiTextureType_NORMALS) & BindTextureFlag) &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			m_iPassIndex = 12;
		}
		else
		{
#ifdef _DEBUG
			cout << "The correct pass does not define. : " << m_pModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG

			m_iPassIndex = 12;
		}

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, m_iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

HRESULT CPreviewAnimationModel::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{

	CallBack_Bind_SRV(m_pShaderCom, "");

	_float	fDissolveAmount(-1.1f);
	m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float));

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

	_uint iNumMeshContainers(m_pModelCom.lock()->Get_NumMeshContainers());
	for (_uint i(0); i < iNumMeshContainers; ++i)
	{
		//m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

			//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 1, "g_Bones", pDeviceContext);
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
	this->Clear_ModelWeapon();

	if (strcmp(In_szModelKey.c_str(), "Boss_Varg") == 0)
	{
		CModel::NVCLOTH_MODEL_DESC NvModelDesc;
		//XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixRotationX(XMConvertToRadians(-90.f))
		Preset::NvClothMesh::VargSetting(NvModelDesc, XMMatrixIdentity());

		m_pModelCom.lock()->Init_Model(In_szModelKey.c_str(), "", (_uint)TIMESCALE_LAYER::EDITER, &NvModelDesc);
	}
	else
	{
		m_pModelCom.lock()->Init_Model(In_szModelKey.c_str(), "", (_uint)TIMESCALE_LAYER::EDITER);
	}

	if (!strcmp(In_szModelKey.c_str(), "Corvus"))
	{
		
#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultSaber>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultDagger>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");
#endif // _ANIMATION_TOOL_WEAPON_

		m_pModelCom.lock()->Add_ReverseAnimation(m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_R_UP_End"), (_uint)TIMESCALE_LAYER::EDITER);
	}

	if (!strcmp(In_szModelKey.c_str(), "Boss_Varg"))
	{
#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_VargWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
#endif // _ANIMATION_TOOL_WEAPON_

		m_pTargetBoneNode = m_pModelCom.lock()->Find_BoneNode("Bip001-Xtra10");
		m_TransformationMatrix = m_pModelCom.lock()->Get_TransformationMatrix();
	}

	if (!strcmp(In_szModelKey.c_str(), "Elite_Joker"))
	{
#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Joker_Weapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
#endif // _ANIMATION_TOOL_WEAPON_
	}

	if (!strcmp(In_szModelKey.c_str(), "Boss_Urd"))
	{
#ifdef _ANIMATION_TOOL_WEAPON_
		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "AnimTargetPoint");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword01_Point");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword02_Point");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword03_Point");

		m_pModelWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(LEVEL_STATIC));
		m_pModelWeapons.back().lock()->Init_Model("Boss_UrdWeapon", TIMESCALE_LAYER::MONSTER);
		m_pModelWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword04_Point");
#endif // _ANIMATION_TOOL_WEAPON_
	}
}

void CPreviewAnimationModel::Change_AnimationFromIndex(const _uint& In_iAnimIndex)
{
	m_pModelCom.lock()->Set_CurrentAnimation(In_iAnimIndex, 0, 0.f);
}

void CPreviewAnimationModel::Play_Animation(_float fTimeDelta)
{
	if (!m_pModelCom.lock()->Get_ModelData().lock())
		return;

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CPreviewAnimationModel::Add_DebugWeapon(const string& In_szBoneName, const _float3& In_vOffset, const _float In_fSize)
{
	_hashcode NameToHash = hash<string>()(In_szBoneName);

	weak_ptr<CWeapon> pWeapon = GAMEINSTANCE->Add_GameObject<CWeapon>(LEVEL_EDIT);

	m_pDebugWeapons[NameToHash].push_back(pWeapon);
	pWeapon.lock()->Init_Weapon(m_pModelCom, m_pTransformCom, In_szBoneName);
	pWeapon.lock()->Add_Collider(XMLoadFloat3(&In_vOffset), In_fSize, COLLISION_LAYER::NONE);
}

void CPreviewAnimationModel::Remove_DebugWeapon(const string& In_szBoneName)
{
	_hashcode NameToHash = hash<string>()(In_szBoneName);

	if (m_pDebugWeapons.find(NameToHash) != m_pDebugWeapons.end())
	{
		m_pDebugWeapons[NameToHash].back().lock()->Set_Dead();
		m_pDebugWeapons.erase(NameToHash);
	}
}

void CPreviewAnimationModel::Clear_DebugWeapon()
{
	for (auto& elem : m_pDebugWeapons)
	{
		for (auto& elem_Weapon : elem.second)
		{
			elem_Weapon.lock()->Set_Dead();
		}
	}
		

	m_pDebugWeapons.clear();
}

void CPreviewAnimationModel::Clear_ModelWeapon()
{
	for (auto& elem : m_pModelWeapons)
		elem.lock()->Set_Dead();

	m_pModelWeapons.clear();
}

void CPreviewAnimationModel::Bind_KeyFrameEvent(const string& In_szKeyFrameEventFileName)
{
	Unbind_KeyEvent(In_szKeyFrameEventFileName.c_str());
	Bind_KeyEvent(In_szKeyFrameEventFileName.c_str());
}


void CPreviewAnimationModel::Release_BeforeModel()
{

}

void CPreviewAnimationModel::Free()
{

}
