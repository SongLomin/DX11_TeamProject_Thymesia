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



	// Corvus_SD_Ladder_Climb_R_UP_End_Reverse
	m_pModelCom.lock()->Add_ReverseAnimation(m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_R_UP_End"), (_uint)TIMESCALE_LAYER::PLAYER);
	// Corvus_SD_Ladder_Climb_L_UP_End_Reverse
	m_pModelCom.lock()->Add_ReverseAnimation(m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_L_UP_End"), (_uint)TIMESCALE_LAYER::PLAYER);

	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);


	m_iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	this->Ready_Weapon();
	this->Ready_States();

	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));

#ifdef _CORVUS_EFFECT_
	// Key Frame Effect ON
	GET_SINGLE(CGameManager)->Bind_KeyEvent("Corvus", m_pModelCom, bind(&CCorvus::Call_NextAnimationKey, this, placeholders::_1));

	// TODO : need to disable at Destroy/Disable
	// Passive Effect ON
	m_EffectIndexList.push_back
	({
		"Corvus_PassiveFeather",
		GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_PassiveFeather", m_pTransformCom, (_uint)TIMESCALE_LAYER::PLAYER)
	});

	GET_SINGLE(CGameManager)->Set_CurrentPlayer(Weak_StaticCast<CPlayer>(m_this));

#endif // _CORVUS_EFFECT_
	return S_OK;
}

HRESULT CCorvus::Start()
{
	__super::Start();
	CCharacter::Change_State<CCorvusState_Idle>();

	m_pCamera = GET_SINGLE(CGameManager)->Get_TargetCamera();

	if (m_pCamera.lock())
		m_pCameraTransform = m_pCamera.lock()->Get_Component<CTransform>();

	return S_OK;
}

void CCorvus::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	this->RootMove();

	// TODO : get rid of this
	this->Debug_KeyInput(fTimeDelta);
}

void CCorvus::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCorvus::Before_Render(_float fTimeDelta)
{
	__super::Before_Render(fTimeDelta);
}

void CCorvus::Custom_Thread1(_float fTimeDelta)
{
	if (RENDERGROUP::RENDER_END != m_eRenderGroup)
		m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
}

HRESULT CCorvus::Render()
{
	__super::Render();
	 _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i(0); i < m_iNumMeshContainers; ++i)
	{
#ifdef _DEBUG
		/*if (i == m_iContainerIndex)
			continue;*/
#endif // _DEBUG
		if (4 == i || 5 == i || 9 == i || 10 == i || 11 == i|| 12 == i|| 13 == i)
		{
			unordered_map<_uint, DISSOLVE_DESC>::iterator iter = m_DissolveDescs.find(i);

			if (iter == m_DissolveDescs.end())
			{
				continue;
			}

			m_pShaderCom.lock()->Set_RawValue("g_bGlow", &iter->second.bGlow, sizeof(_bool));
			m_pShaderCom.lock()->Set_RawValue("g_bBloom", &iter->second.bBloom, sizeof(_bool));
			m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &iter->second.vGlowColor, sizeof(_float4));


			m_pShaderCom.lock()->Set_RawValue("g_vDissolveDir", &iter->second.vDirection, sizeof(_float3));
			m_pShaderCom.lock()->Set_RawValue("g_vDissolveStartPos", &iter->second.vStartPos, sizeof(_float3));
			m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &iter->second.fAmount, sizeof(_float));
	
			m_iPassIndex = 6;
		}
		else
		{
			_bool bFalse = false;
			m_pShaderCom.lock()->Set_RawValue("g_bGlow", &bFalse, sizeof(_bool));
			m_pShaderCom.lock()->Set_RawValue("g_bBloom", &bFalse, sizeof(_bool));
		
			if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
				m_iPassIndex = 4;
			else
				m_iPassIndex = 5;
		}
		
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			m_iPassIndex = 0;
	
		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, m_iPassIndex, "g_Bones");
	}

	m_DissolveDescs.clear();

	return S_OK;
}

void CCorvus::Debug_KeyInput(_float fTimeDelta)
{
	PxControllerFilters Filters;

	// TODO : test jump key R
	if (KEY_INPUT(KEY::R, KEY_STATE::HOLD))
		m_pPhysXControllerCom.lock()->Move(_vector{ 0.f, 500.f * fTimeDelta, 0.f }, 0.f, fTimeDelta, Filters);

	if (KEY_INPUT(KEY::NUM0, KEY_STATE::TAP))
	{
		static _bool bEnable;
		bEnable = !bEnable;
		m_pPhysXControllerCom.lock()->Set_EnableSimulation(bEnable);
	}

#ifdef _DEBUG
	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		++m_iContainerIndex;
		if (m_iContainerIndex >= m_iNumMeshContainers)\
			m_iContainerIndex = 0;
		cout << "m_iContainerIndex : " << m_iContainerIndex << endl;
	}
#endif // _DEBUG
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
	// Macro for convenience to add state component
	// undefines after use
#define ADD_STATE_MACRO(state_class_name)\
		Add_Component<state_class_name>()

	m_pStandState = ADD_STATE_MACRO(CCorvusState_Idle);
	ADD_STATE_MACRO(CCorvusState_Jogging);
	ADD_STATE_MACRO(CCorvusState_JoggingStart);
	ADD_STATE_MACRO(CCorvusState_JoggingStartEnd);
	ADD_STATE_MACRO(CCorvusState_Run);
	ADD_STATE_MACRO(CCorvusState_Sprint);
	ADD_STATE_MACRO(CCorvusState_SprintStart);
	ADD_STATE_MACRO(CCorvusState_AVoid);
	ADD_STATE_MACRO(CVarg_Execution);
	ADD_STATE_MACRO(CCorvusState_SprintAttack);
	ADD_STATE_MACRO(CCorvusState_LAttack1);
	ADD_STATE_MACRO(CCorvusState_LAttack2);
	ADD_STATE_MACRO(CCorvusState_LAttack3);
	ADD_STATE_MACRO(CCorvusState_LAttack4);
	ADD_STATE_MACRO(CCorvusState_LAttack5);
	ADD_STATE_MACRO(CCorvusState_Parry1);
	ADD_STATE_MACRO(CCorvusState_Parry2);
	ADD_STATE_MACRO(CCorvusState_BasicHealing);
	ADD_STATE_MACRO(CCorvusState_ClawAttackTab);
	ADD_STATE_MACRO(CCorvusState_ClawAttackTab2);
	ADD_STATE_MACRO(CCorvusState_ClawAttackHold);
	ADD_STATE_MACRO(CCorvusState_ClawAttackAway);
	ADD_STATE_MACRO(CCorvusState_ClawAttackHoldLoop);
	ADD_STATE_MACRO(CCorvusState_Die);
	ADD_STATE_MACRO(CCorvusState_HurtL);
	ADD_STATE_MACRO(CCorvusState_HurtR);
	ADD_STATE_MACRO(CCorvusState_HurtXXL);
	ADD_STATE_MACRO(CCorvusState_NorMob_Execution);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectLeft);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectLeftup);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectRight);
	ADD_STATE_MACRO(CCorvusState_ParryDeflectRightup);
	ADD_STATE_MACRO(CCorvusState_PS_Axe);
	ADD_STATE_MACRO(CCorvusState_PS_CaneSword);
	ADD_STATE_MACRO(CCorvusState_PS_Knife);
	ADD_STATE_MACRO(CCorvusState_PS_Magician);
	ADD_STATE_MACRO(CCorvusState_PS_UrdSword);
	ADD_STATE_MACRO(CCorvusState_PS_VargSword);
	ADD_STATE_MACRO(CCorvusState_AVoidSalsh);
	ADD_STATE_MACRO(CCorvusState_AVoidThrust);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Down);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Down_End);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Idle);
	ADD_STATE_MACRO(CCorvusState_Climb_L_UP);
	ADD_STATE_MACRO(CCorvusState_Climb_L_UP_End);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Down);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Down_End);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Idle);
	ADD_STATE_MACRO(CCorvusState_Climb_R_UP);
	ADD_STATE_MACRO(CCorvusState_Climb_R_UP_End);
	ADD_STATE_MACRO(CCorvusState_Climb_Start);
	ADD_STATE_MACRO(CCorvusState_Fall_End);
	ADD_STATE_MACRO(CCorvusState_Fall_Loop);
	ADD_STATE_MACRO(CCorvusState_Fall_Start);
	ADD_STATE_MACRO(CCorvusState_Climb_Start);
	ADD_STATE_MACRO(CCorvusState_Climb_Fall_Attack);
	ADD_STATE_MACRO(CCorvusState_Climb_L_Up_Down_End);
	ADD_STATE_MACRO(CCorvusState_Climb_R_Up_Down_End);
	ADD_STATE_MACRO(CCorvusState_RaidAttack1Hurt);

#undef ADD_STATE_MACRO
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
	switch ((COLLISION_LAYER)pOtherCollider.lock()->Get_CollisionLayer())
	{
	case Client::COLLISION_LAYER::LADDER_UP:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::LADDERUP;
		m_bLadderCheck = true;
		break;
	case Client::COLLISION_LAYER::LADDER_DOWN:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::LADDERDOWN;
		m_bLadderCheck = true;
		break;
	case Client::COLLISION_LAYER::ELEVATOR:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::ELEVATOR;
		break;
	case Client::COLLISION_LAYER::DOOR:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::DOOR;
		break;

	case Client::COLLISION_LAYER::CHECKPOINT:
		m_CollisionObjectFlags |= (_flag)COLISIONOBJECT_FLAG::CHECKPOINT;
		break;
	}

	__super::OnCollisionEnter(pMyCollider,pOtherCollider);
}

void CCorvus::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CCorvus::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider,pOtherCollider);

	switch ((COLLISION_LAYER)pOtherCollider.lock()->Get_CollisionLayer())
	{
	case Client::COLLISION_LAYER::LADDER_UP:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::LADDERUP;
		m_bLadderCheck = false;
		break;
	case Client::COLLISION_LAYER::LADDER_DOWN:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::LADDERDOWN;
		m_bLadderCheck = false;
		break;
	case Client::COLLISION_LAYER::ELEVATOR:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::ELEVATOR;
		break;
	case Client::COLLISION_LAYER::DOOR:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::DOOR;
		break;
	case Client::COLLISION_LAYER::CHECKPOINT:
		m_CollisionObjectFlags &= !(_flag)COLISIONOBJECT_FLAG::CHECKPOINT;
		break;
	}
}

void CCorvus::RootMove()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");
	//m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviMeshCom);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, GAMEINSTANCE->Get_DeltaTime(), Filters, nullptr, m_pTransformCom);
}

void CCorvus::OnBattleEnd()
{
}

void CCorvus::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);
}

void CCorvus::OnDisable()
{
	__super::OnDisable();
}

void CCorvus::Free()
{
}

