#include "stdafx.h"
#include "BossVarg/Varg.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "VargWeapon.h"
#include "VargStates.h"
#include "Effect_Trail_EyeLight.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "MonsterHPBar_Boss.h"
#include "Status_Monster.h"
#include "Status_Boss.h"
#include "BoneNode.h"
#include "UIManager.h"

GAMECLASS_C(CVarg);
CLONE_C(CVarg, CGameObject);

HRESULT CVarg::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CVarg::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	CModel::NVCLOTH_MODEL_DESC NvClothDesc;
	Preset::NvClothMesh::VargSetting(NvClothDesc, XMMatrixIdentity());

	m_pModelCom.lock()->Init_Model("Boss_Varg", "", (_uint)TIMESCALE_LAYER::MONSTER, &NvClothDesc);
	m_TransformationMatrix = m_pModelCom.lock()->Get_TransformationMatrix();

	m_pStandState = Add_Component<CVargBossState_Start>();
	Add_Component<CVargBossState_Attack1a>();
	Add_Component<CVargBossState_Attack1b>();
	Add_Component<CVargBossState_Attack2a>();
	Add_Component<CVargBossState_Attack2b>();
	Add_Component<CVargBossState_Attack3a>();
	Add_Component<CVargBossState_Attack3b>();
	Add_Component<CVargBossState_AvoidAttack>();
	Add_Component<CVargBossState_AvoidB>();
	Add_Component<CVargBossState_AvoidL>();
	Add_Component<CVargBossState_AvoidR>();
	Add_Component<CVargBossState_Hurt>();
	Add_Component<CVargBossState_Parry>();
	Add_Component<CVargBossState_Run>();
	Add_Component<CVargBossState_RunAttack>();
	Add_Component<CVargBossState_RunStart>();
	Add_Component<CVargBossState_SPA_Catch>();
	Add_Component<CVargBossState_SPA_CatchFail>();
	Add_Component<CVargBossState_SPA_Roar>();
	Add_Component<CVargBossState_SPA_Roar_Getup>();
	Add_Component<CVargBossState_SPA_Run>();
	Add_Component<CVargBossState_Idle>();
	Add_Component<CVargBossState_Stun_End>();
	Add_Component<CVargBossState_Exe_Dead>();
	Add_Component<CVargBossState_Exe_NoDeadEnd>();
	Add_Component<CVargBossState_Exe_SitLoop>();
	Add_Component<CVargBossState_Exe_Start>();
	Add_Component<CVargBossState_Stun_Loop>();
	Add_Component<CVargBossState_Stun_Start>();
	Add_Component<CVargBossState_TurnL>();
	Add_Component<CVargBossState_TurnR>();
	Add_Component<CVargBossState_WalkB>();
	Add_Component<CVargBossState_WalkF>();
	Add_Component<CVargBossState_WalkL>();
	Add_Component<CVargBossState_WalkR>();
	Add_Component<CVargBossState_RaidAttack>();
	Add_Component<CVargBossState_TurnAttack>();
	Add_Component<CVargBossState_Attack2b1>();
	Add_Component<CVargBossState_Attack2b2>();
	Add_Component<CVargBossState_Exe_End>();
	Add_Component<CVargBossState_IdleGeneral>();

	TRAIL_DESC TrailDesc;
	ZeroMemory(&TrailDesc, sizeof(TRAIL_DESC));

	TrailDesc.iMaxCnt = 30;
	//position 0.163, 0.12,0.055 , z 0.1씩
	TrailDesc.vPos_0 = _float3(0.163, 0.17, 0.075);
	TrailDesc.vPos_1 = _float3(0.163, 0.17, 0.035);
	m_pTrailEffect = GAMEINSTANCE->Add_GameObject<CEffect_Trail_EyeLight>(m_CreatedLevel, &TrailDesc);
	m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTrailBoneNode, m_pModelCom.lock()->Get_ModelData());

	if (!m_pTrailEffect.lock())
		assert(0);

	m_pTrailEffect.lock()->Set_Enable(false);

#ifdef _DEBUG
	m_fCullingRange = 999.f;
#endif // _DEBUG

	_uint iNvClothColliderCount;
	CNvClothCollider::NVCLOTH_COLLIDER_DESC* NvClothColliderDesc = (CNvClothCollider::NVCLOTH_COLLIDER_DESC*)Preset::NvClothCollider::VergSetting(iNvClothColliderCount);

	m_pNvClothColliderCom = Add_Component<CNvClothCollider>();
	m_pNvClothColliderCom.lock()->Init_NvClothColliders(m_pModelCom, NvClothColliderDesc, iNvClothColliderCount);

	Safe_Delete_Array(NvClothColliderDesc);

#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
#endif // _USE_THREAD_

	LIGHTDESC LightDesc;

	LightDesc.eActorType = LIGHTDESC::TYPE_SPOTLIGHT;
	LightDesc.vDiffuse = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vSpecular = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vAmbient = { 1.f,0.95f,0.8f,1.f };
	LightDesc.fIntensity = 10.f;
	LightDesc.fRange = 10.f;
	LightDesc.fCutOff = cosf(XMConvertToRadians(20.f));
	LightDesc.fOuterCutOff = cosf(XMConvertToRadians(30.f));
	_vector vOwnerPos = m_pTransformCom.lock()->Get_Position();

	_vector vLightPos = vOwnerPos + XMVectorSet(0.f, 5.f, 0.f, 0.f);
	_vector vLightLook = XMVectorSet(0.f,-1.f,0.f,0.f);

	XMStoreFloat4(&LightDesc.vPosition, vLightPos);
	XMStoreFloat4(&LightDesc.vDirection, vLightLook);

	LightDesc.bEnable = false;

	// m_LightDesc = GAMEINSTANCE->Add_Light(LightDesc);

	
	LightDesc.eActorType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vSpecular = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vAmbient = { 1.f,0.95f,0.8f,1.f };
	LightDesc.fIntensity = 1.f;
	LightDesc.bEnable = false;

	m_DirLightDesc = GAMEINSTANCE->Add_Light(LightDesc);

	
	LightDesc.eActorType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDiffuse = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vSpecular = { 1.f,0.95f,0.8f,1.f };
	LightDesc.vAmbient = { 1.f,0.95f,0.8f,1.f };
	LightDesc.fIntensity = 0.f;
	LightDesc.bEnable = false;

	m_DirLightDesc = GAMEINSTANCE->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CVarg::Start()
{
	__super::Start();

	m_pTrailEffect.lock()->Set_TextureIndex(1,869, 0);
	m_pTrailBoneNode = m_pModelCom.lock()->Find_BoneNode("Bip001-Head");

	//CBase::Set_Enable(true);

	Change_State<CVargBossState_IdleGeneral>();


	// weak_ptr<CBoneNode> pTargetBoneNode = m_pModelCom.lock()->Find_BoneNode();
	// m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());
	 m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CVarg::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pTrailEffect.lock()->Update(fTimeDelta, m_pTransformCom, m_pTrailBoneNode, m_pModelCom.lock()->Get_ModelData());


}

void CVarg::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_bool bEnd = false;

	CallBack_LightEvent(fTimeDelta, bEnd);
	if (bEnd)
	{
		CallBack_LightEvent.Clear();
	}

}

void CVarg::Thread_PreBeforeRender(_float fTimeDelta)
{
	__super::Thread_PreBeforeRender(fTimeDelta);

	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();

	_matrix		BoneMatrix;
	_matrix		InverseMatrix;
	_vector		vGravity;


	//BoneMatrix = //m_pModelCom.lock()->Find_BoneNode("Bip001-Head").lock()->Get_OffsetMatrix() * 
	//	m_pModelCom.lock()->Find_BoneNode("Bip001-Head").lock()->Get_CombinedMatrix() *
	//	XMLoadFloat4x4(&m_TransformationMatrix) * m_pTransformCom.lock()->Get_WorldMatrix();

	//_vector vSpherePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	//vSpherePos = XMVector3TransformCoord(vSpherePos, BoneMatrix);
	//PxVec4 spheres[2]{
	//	PxVec4(SMath::Convert_PxVec3(vSpherePos), 0.3f),
	//	PxVec4(999.f, 999.f, 999.f, 1.0f),
	//};
	//nv::cloth::Range<const physx::PxVec4> sphereRange(spheres, spheres + 2);

	//m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->setSpheres(sphereRange, 0, m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->getNumSpheres());

	//Bip001-Ponytail1

	/*BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Head").lock()->Get_CombinedMatrix() *
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

	nv::cloth::Range<const physx::PxVec4> sphereRange(spheres, spheres + 5);*/

	m_pNvClothColliderCom.lock()->Update_Colliders(m_pTransformCom.lock()->Get_WorldMatrix());
	m_pNvClothColliderCom.lock()->Set_Spheres(m_pModelCom.lock()->Get_MeshContainer(1));
	m_pNvClothColliderCom.lock()->Set_Spheres(m_pModelCom.lock()->Get_MeshContainer(3));

	//m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->setSpheres(sphereRange, 0, m_pModelCom.lock()->Get_MeshContainer(1).lock()->Get_NvCloth()->getNumSpheres());
	//m_pModelCom.lock()->Get_MeshContainer(3).lock()->Get_NvCloth()->setSpheres(sphereRange, 0, m_pModelCom.lock()->Get_MeshContainer(3).lock()->Get_NvCloth()->getNumSpheres());

	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Ponytail1").lock()->Get_OffsetMatrix() *
		m_pModelCom.lock()->Find_BoneNode("Bip001-Ponytail1").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//InverseMatrix = XMMatrixInverse(nullptr, BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix());

	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), XMMatrixRotationX(XMConvertToRadians(90.f)) * InverseMatrix);
	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), InverseMatrix * XMMatrixRotationX(XMConvertToRadians(-90.f)));

	m_pModelCom.lock()->Get_MeshContainer(1).lock()->Update_NvClothVertices(pDeferredContext,
		BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix(),
		XMVectorSet(0.f, -9.81f, 0.f, 0.f));



	BoneMatrix = m_pModelCom.lock()->Find_BoneNode("Bip001-Xtra10").lock()->Get_OffsetMatrix() *
		m_pModelCom.lock()->Find_BoneNode("Bip001-Xtra10").lock()->Get_CombinedMatrix() *
		XMLoadFloat4x4(&m_TransformationMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//InverseMatrix = XMMatrixInverse(nullptr, BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix());

	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), XMMatrixRotationX(XMConvertToRadians(0.f)) * InverseMatrix);
	//vGravity = XMVector3TransformNormal(XMVectorSet(0.f, -9.81f, 0.f, 0.f), InverseMatrix);

	// "Bip001-Xtra10"

	m_pModelCom.lock()->Get_MeshContainer(3).lock()->Update_NvClothVertices(pDeferredContext,
		BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix(),
		XMVectorSet(0.f, -9.81f, 0.f, 0.f));

	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);

}

HRESULT CVarg::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();


	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{

		_flag BindTextureFlag = 0;

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
			continue;
		}

		m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", m_pTransformCom.lock()->Get_WorldMatrix());

		// Cloth
		if ((1 == i || 3 == i) &&
			(1 << aiTextureType_NORMALS) & BindTextureFlag &&
			(1 << aiTextureType_SPECULAR) & BindTextureFlag)
		{
			m_iPassIndex = 9;
			m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", XMMatrixIdentity());
		}

		// NormalTexture	OK.
		// ORMTexture		OK.
		else if ((1 << aiTextureType_NORMALS) & BindTextureFlag &&
			(1 << aiTextureType_SPECULAR) & BindTextureFlag)
		{
			m_iPassIndex = 5;
		}

		// NormalTexture	OK.
		// ORMTexture		NO.
		else if ((1 << aiTextureType_NORMALS) & BindTextureFlag &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			m_iPassIndex = 4;
		}

		// NormalTexture	NO.
		// ORMTexture		NO.
		else if (
			!((1 << aiTextureType_NORMALS) & BindTextureFlag) &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			m_iPassIndex = 0;
		}

		if (1 == i || 3 == i)
		{
			m_iPassIndex = 9;

			m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", XMMatrixIdentity());
		}



		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, m_iPassIndex, "g_Bones", pDeviceContext);

		/*if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}
		else
		{
			iPassIndex = 4;
		}

		if (1 == i || 3 == i)
		{
			iPassIndex = 9;

			m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", XMMatrixIdentity());
		}
		else
		{
			m_pShaderCom.lock()->Set_Matrix("g_WorldMatrix", m_pTransformCom.lock()->Get_WorldMatrix());
		}*/

		//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CVarg::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CVarg::Set_TrailEnable(_bool In_bEnable)
{
	Weak_Cast<CVargWeapon>(m_pWeapons.front()).lock()->Set_TrailEnable(In_bEnable);
}

void CVarg::Set_EyeTrailEnable(_bool In_bEnable)
{
	m_pTrailEffect.lock()->Set_Enable(In_bEnable);
}



void CVarg::Init_Desc()
{
	__super::Init_Desc();

	//m_pModelCom.lock()->Init_Model("Boss_Varg", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CVargWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");

	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.9f,-2.4f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.9f,-2.4f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.8f,-2.2f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.7f,-2.0f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.6f,-1.8f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.5f,-1.6f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.4f,-1.4f,1.0f }, 1.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.3f,-1.2f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.2f,-1.f,1.0f },  0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.1f,-0.8f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.0f,-0.6f,1.0f }, 0.8f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.0f,-0.4f,1.0f }, 1.8f, COLLISION_LAYER::MONSTER_ATTACK);

	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.0f));
	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);

	INIT_STATE(CVargBossState_Start);
	INIT_STATE(CVargBossState_Attack1a);
	INIT_STATE(CVargBossState_Attack1b);
	INIT_STATE(CVargBossState_Attack2a);
	INIT_STATE(CVargBossState_Attack2b);
	INIT_STATE(CVargBossState_Attack3a);
	INIT_STATE(CVargBossState_Attack3b);
	INIT_STATE(CVargBossState_AvoidAttack);
	INIT_STATE(CVargBossState_AvoidB);
	INIT_STATE(CVargBossState_AvoidL);
	INIT_STATE(CVargBossState_AvoidR);
	INIT_STATE(CVargBossState_Hurt);
	INIT_STATE(CVargBossState_Parry);
	INIT_STATE(CVargBossState_Run);
	INIT_STATE(CVargBossState_RunAttack);
	INIT_STATE(CVargBossState_RunStart);
	INIT_STATE(CVargBossState_SPA_Catch);
	INIT_STATE(CVargBossState_SPA_CatchFail);
	INIT_STATE(CVargBossState_SPA_Roar);
	INIT_STATE(CVargBossState_SPA_Roar_Getup);
	INIT_STATE(CVargBossState_SPA_Run);
	INIT_STATE(CVargBossState_Idle);
	INIT_STATE(CVargBossState_Stun_End);
	INIT_STATE(CVargBossState_Exe_Dead);
	INIT_STATE(CVargBossState_Exe_End);
	INIT_STATE(CVargBossState_Exe_NoDeadEnd);
	INIT_STATE(CVargBossState_Exe_SitLoop);
	INIT_STATE(CVargBossState_Exe_Start);
	INIT_STATE(CVargBossState_Stun_Loop);
	INIT_STATE(CVargBossState_Stun_Start);
	INIT_STATE(CVargBossState_TurnL);
	INIT_STATE(CVargBossState_TurnR);
	INIT_STATE(CVargBossState_WalkB);
	INIT_STATE(CVargBossState_WalkF);
	INIT_STATE(CVargBossState_WalkL);
	INIT_STATE(CVargBossState_WalkR);
	INIT_STATE(CVargBossState_RaidAttack);
	INIT_STATE(CVargBossState_TurnAttack);
	INIT_STATE(CVargBossState_Attack2b1);
	INIT_STATE(CVargBossState_Attack2b2);
	INIT_STATE(CVargBossState_IdleGeneral);

#ifdef _VARG_EFFECT_
	Bind_KeyEvent("Boss_Varg");
#endif // _VARG_EFFECT_

	m_pPhysXControllerCom.lock()->Init_Controller(
		Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);
}


void CVarg::Move_RootMotion_Internal()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
}

void CVarg::TurnOn_Light(_float fTimeDelta, _bool& In_bEnd)
{
	_vector vOwnerPos = m_pTransformCom.lock()->Get_Position();

	_vector vLightPos = vOwnerPos + XMVectorSet(0.f, 5.f, 0.f, 0.f);
	_vector vLightLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);

	XMStoreFloat4(&m_LightDesc.vPosition, vLightPos);
	XMStoreFloat4(&m_LightDesc.vDirection, vLightLook);

	//GAMEINSTANCE->Set_LightDesc(m_LightDesc);
}

void CVarg::TurnOff_Light(_float fTimeDelta, _bool& In_bEnd)
{
	//m_LightDesc.fIntensity -= fTimeDelta*5.f;
	m_DirLightDesc.fIntensity = min(m_DirLightDesc.fIntensity + fTimeDelta * 0.2f , 1.f);
	GAMEINSTANCE->Set_IrradianceColorScale(_float3(m_DirLightDesc.fIntensity, m_DirLightDesc.fIntensity, m_DirLightDesc.fIntensity));
	if (0.f > m_LightDesc.fIntensity)
	{
		m_LightDesc.bEnable = false;
		In_bEnd = true;
	}
	//GAMEINSTANCE->Set_LightDesc(m_LightDesc);
	GAMEINSTANCE->Set_LightDesc(m_DirLightDesc);
}

void CVarg::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CVarg::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CVarg::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CVarg::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_CATCH == iArg)
	{
		Change_State<CVargBossState_SPA_Catch>();
	}

	if ((_uint)EVENT_TYPE::ON_VARGEXECUTION == iArg)
	{
		Change_State<CVargBossState_Exe_NoDeadEnd>();
	}

	if ((_uint)EVENT_TYPE::ON_GROGGY == iArg)
	{
		Change_State<CVargBossState_Stun_Start>();
	}

	if ((_uint)EVENT_TYPE::ON_ENTER_SECTION == iArg)
	{
		Set_Enable(true);
	}

	if ((_uint)EVENT_TYPE::ON_BOSS_EXECUTIONSTART == iArg)
	{
		m_bBossExecutionStartOnOff = true;
	}

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{

		if (Weak_Cast<CStatus_Boss>(m_pStatus).lock()->Get_WhiteRatio() > 0.99f)
		{
			return;
		}

		GAMEINSTANCE->PlayBGM("BGM_STAGE_3.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM));


		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->Set_Position(XMLoadFloat4(&m_tLinkStateDesc.m_fStartPositon), 0.f, Filters);

		Change_State<CVargBossState_IdleGeneral>();
		Set_Enable(false);
		
		m_pStatus.lock()->Full_Recovery();

	}
	//켜지는 조건
	if ((_uint)EVENT_TYPE::ON_VARGTURNONSPOTLIGHT == iArg)
	{
		m_LightDesc.bEnable = true;
		GAMEINSTANCE->Set_LightDesc(m_LightDesc);
		CallBack_LightEvent+=bind(&CVarg::TurnOn_Light,this, placeholders::_1, placeholders::_2);
	}
	//꺼지는 조건
	if ((_uint)EVENT_TYPE::ON_VARGTURNOFFSPOTLIGHT == iArg)
	{
		m_DirLightDesc.bEnable = true;
		CallBack_LightEvent.Clear();
		CallBack_LightEvent+= bind(&CVarg::TurnOff_Light, this, placeholders::_1, placeholders::_2);
	}

}

void CVarg::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}

void CVarg::OnDisable()
{
	__super::OnDisable();
}



void CVarg::Free()
{
}
