#include "stdafx.h"
#include "CCorvus.h"
#include "Client_Components.h"
#include "CorvusStates/CorvusStates.h"
#include "Camera_Target.h"
#include "Corvus_DefaultSaber.h"
#include "Corvus_DefaultDagger.h"
#include "PhysXCollider.h"
#include "Light_Prop.h"
#include "Status_Player.h"

#include "PhysXController.h"

GAMECLASS_C(CCorvus)
CLONE_C(CCorvus, CGameObject)

HRESULT CCorvus::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvus::Initialize(void* pArg)
{

	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxAnimModel"), VTXANIM_DECLARATION::Element, VTXANIM_DECLARATION::iNumElements);

	m_pStatus = CGameObject::Add_Component<CStatus_Player>();

	m_pModelCom.lock()->Init_Model("Corvus", "", (_uint)TIMESCALE_LAYER::PLAYER);

	m_pModelCom.lock()->Set_RootNode("root");

	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	this->Ready_Weapon();
	this->Ready_States();

	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));
	
	// Key Frame Effect ON
	GET_SINGLE(CGameManager)->Bind_KeyEvent("Corvus", m_pModelCom, bind(&CCorvus::Call_NextAnimationKey, this, placeholders::_1));

	// Passive Effect ON
	GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_PassiveFeather", m_pTransformCom, (_uint)TIMESCALE_LAYER::PLAYER);

	//USE_START(CCorvus);
	return S_OK;
}

HRESULT CCorvus::Start()
{
	__super::Start();

	CCharacter::Change_State<CCorvusState_Idle>();
	
	m_pCamera = GET_SINGLE(CGameManager)->Get_TargetCamera();
	m_pCameraTransform = m_pCamera.lock()->Get_Component<CTransform>();

	
	return S_OK;
}

void CCorvus::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	this->RootMove();

	// TODO : test jump key R
	if (KEY_INPUT(KEY::R, KEY_STATE::HOLD))
		m_pPhysXControllerCom.lock()->Move(_vector{ 0.f, 10.f * fTimeDelta, 0.f }, 0.f, fTimeDelta, PxControllerFilters());

	// TODO : frame test
	//if (KEY_INPUT(KEY::DELETEKEY, KEY_STATE::TAP))
	//{
	//	for (_int i(0); i < 100; ++i)
	//	{
	//		GET_SINGLE(CGameManager)->Use_EffectGroup("ParryEffectParticle1", m_pTransformCom, _uint(TIMESCALE_LAYER::NONE));

	//		/*
	//		_vector PushPower = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

	//		weak_ptr<CGameObject> pGameObject = GAMEINSTANCE->Add_GameObject<CLight_Prop>(m_CreatedLevel);
	//		pGameObject.lock()->Get_Component<CTransform>().lock()->Set_Position(m_pTransformCom.lock()->Get_Position() + XMVectorSet(0.f, 0.5f, 0.f, 0.f) + PushPower);
	//		pGameObject.lock()->Get_Component<CPhysXCollider>().lock()->Add_Force(PushPower * 1000.f);
	//		*/
	//	}
	//}
}

void CCorvus::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	//m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, 1.5f, 0.f, 1.f));
	//m_pPhysXTriggerColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, 1.5f, 0.f, 1.f));
}

void CCorvus::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);
	//m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom, XMVectorSet(0.f, -1.5f, 0.f, 1.f));
	//m_pPhysXTriggerColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, 1.5f, 0.f, 1.f));
}

void CCorvus::Custom_Thread1(_float fTimeDelta)
{
	// 컬링 연산 방지
	if (RENDERGROUP::RENDER_END != m_eRenderGroup)
		m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
}

HRESULT CCorvus::Render()
{
	__super::Render();
	// _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i(0); i < m_iNumMeshContainers; ++i)
	{
		if (i == 4 || i == 9 || i == 12)
			continue;
			
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			m_iPassIndex = 0;
		else
		{
			if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			{
				m_iPassIndex = 4;
			}
			else
				m_iPassIndex = 5;
		}
		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, m_iPassIndex, "g_Bones");
		
	}

	return S_OK;
}

void CCorvus::Ready_Weapon()
{
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultSaber>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CCorvus_DefaultDagger>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");
}

void CCorvus::Ready_States()
{
#define MACRO(state_class_name)\
		Add_Component<state_class_name>()

	m_pStandState = MACRO(CCorvusState_Idle);
	MACRO(CCorvusState_Jogging);
	MACRO(CCorvusState_JoggingStart);
	MACRO(CCorvusState_JoggingStartEnd);
	MACRO(CCorvusState_Run);
	MACRO(CCorvusState_Sprint);
	MACRO(CCorvusState_SprintStart);
	MACRO(CCorvusState_AVoid);
	MACRO(CVarg_Execution);
	MACRO(CCorvusState_SprintAttack);
	MACRO(CCorvusState_LAttack1);
	MACRO(CCorvusState_LAttack2);
	MACRO(CCorvusState_LAttack3);
	MACRO(CCorvusState_Parry1);
	MACRO(CCorvusState_Parry2);
	MACRO(CCorvusState_BasicHealing);
	MACRO(CCorvusState_ClawAttack1);
	MACRO(CCorvusState_Die);
	MACRO(CCorvusState_HurtL);
	MACRO(CCorvusState_HurtR);
	MACRO(CCorvusState_HurtXXL);
	MACRO(CCorvusState_NorMob_Execution);

#undef MACRO
}

void CCorvus::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

#ifndef _USE_THREAD_
	m_pModelCom.lock()->Update_BoneMatrices();
#endif // !_USE_THREAD_
}

void CCorvus::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CCorvus::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CCorvus::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
}

void CCorvus::RootMove()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));
	//m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, PxControllerFilters(), nullptr, m_pTransformCom);
}

void CCorvus::OnBattleEnd()
{
}

void CCorvus::OnEnable(void* pArg)
{
}

void CCorvus::OnDisable()
{
}

void CCorvus::OnDestroy()
{
}

void CCorvus::Free()
{
}
