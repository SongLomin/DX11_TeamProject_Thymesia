#include "stdafx.h"
#include "EliteMonState/Joker/Joker.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "JokerWeapon.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "Status_Monster.h"
#include "JokerStates.h"
#include "MonsterHPBar_Elite.h"

GAMECLASS_C(CJoker);
CLONE_C(CJoker, CGameObject);

HRESULT CJoker::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CJoker::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_pStatus = Add_Component<CStatus_Monster>(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pModelCom.lock()->Init_Model("Elite_Joker", "", (_uint)TIMESCALE_LAYER::MONSTER);

	m_pStandState = Add_Component<CJokerState_Sp_Open>();
	Add_Component<CJokerState_ComboA1>();
	Add_Component<CJokerState_ComboA2>();
	Add_Component<CJokerState_Combob1>();
	Add_Component<CJokerState_ComboB2>();
	Add_Component<CJokerState_Dead>();
	Add_Component<CJokerState_Execution_End>();
	Add_Component<CJokerState_Execution_Loop>();
	Add_Component<CJokerState_Idle>();
	Add_Component<CJokerState_JumpAttack>();
	Add_Component<CJokerState_RunAtkEnd>();
	Add_Component<CJokerState_RunAttackLoop>();
	Add_Component<CJokerState_ShockAttack>();
	Add_Component<CJokerState_StrongAttack>();
	Add_Component<CJokerState_Stun_End>();
	Add_Component<CJokerState_Stun_Loop>();
	Add_Component<CJokerState_Stun_Start>();
	Add_Component<CJokerState_TakeExecution_Start>();
	Add_Component<CJokerState_TurnAtkL>();
	Add_Component<CJokerState_TurnAtkR>();
	Add_Component<CJokerState_TurnL90>();
	Add_Component<CJokerState_TurnR90>();
	Add_Component<CJokerState_WalkB>();
	Add_Component<CJokerState_WalkR>();
	Add_Component<CJokerState_WalkF>();
	Add_Component<CJokerState_WalkL>();
	Add_Component<CJokerState_WheelAtkEnd>();
	Add_Component<CJokerState_WheelAtkEnd2>();
	Add_Component<CJokerState_WheelAtkLoop>();
	Add_Component<CJokerState_WheelAtkStart>();
	Add_Component<CJokerState_RunAttackStart>();

	m_fCullingRange = 999.f;
	return S_OK;
}

HRESULT CJoker::Start()
{
	__super::Start();

	//CBase::Set_Enable(true);
	
	Change_State<CJokerState_Sp_Open>();
	
	Bind_HPBar();
	// weak_ptr<CBoneNode> pTargetBoneNode = m_pModelCom.lock()->Find_BoneNode();
	// m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());


	return S_OK;
}

void CJoker::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CJoker::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CJoker::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

	_flag BindTextureFlag;
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		_uint iPassIndex = 0;
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

		// NormalTexture	OK.
		// ORMTexture		OK.
		else if ((1 << aiTextureType_NORMALS) & BindTextureFlag &&
			(1 << aiTextureType_SPECULAR) & BindTextureFlag)
		{
			iPassIndex = 5;
		}

		// NormalTexture	OK.
		// ORMTexture		NO.
		else if ((1 << aiTextureType_NORMALS) & BindTextureFlag &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			iPassIndex = 4;
		}

		// NormalTexture	NO.
		// ORMTexture		NO.
		else if (
			!((1 << aiTextureType_NORMALS) & BindTextureFlag) &&
			!((1 << aiTextureType_SPECULAR) & BindTextureFlag))
		{
			iPassIndex = 0;
		}
	
		//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		if (m_iPassIndex > 0)
		{
			iPassIndex = m_iPassIndex;
			m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float));
		}
		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CJoker::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}



void CJoker::Init_Desc()
{
	__super::Init_Desc();

	m_pModelCom.lock()->Init_Model("Elite_Joker", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CJokerWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Model("Joker_Weapon", TIMESCALE_LAYER::MONSTER);
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");

	m_pWeapons.back().lock()->Add_Collider({ 1.f,-0.5f,-0.2f,1.0f }, 1.4f, COLLISION_LAYER::MONSTER_ATTACK);

	//m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.0f));
	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);

	INIT_STATE(CJokerState_ComboA1);
	INIT_STATE(CJokerState_ComboA2);
	INIT_STATE(CJokerState_Combob1);
	INIT_STATE(CJokerState_ComboB2);
	INIT_STATE(CJokerState_Dead);
	INIT_STATE(CJokerState_Execution_End);
	INIT_STATE(CJokerState_Execution_Loop);
	INIT_STATE(CJokerState_Idle);
	INIT_STATE(CJokerState_JumpAttack);
	INIT_STATE(CJokerState_RunAtkEnd);
	INIT_STATE(CJokerState_RunAttackLoop);
	INIT_STATE(CJokerState_ShockAttack);
	INIT_STATE(CJokerState_Sp_Open);
	INIT_STATE(CJokerState_StrongAttack);
	INIT_STATE(CJokerState_Stun_End);
	INIT_STATE(CJokerState_Stun_Loop);
	INIT_STATE(CJokerState_Stun_Start);
	INIT_STATE(CJokerState_TakeExecution_Start);
	INIT_STATE(CJokerState_TurnAtkL);
	INIT_STATE(CJokerState_TurnAtkR);
	INIT_STATE(CJokerState_TurnL90);
	INIT_STATE(CJokerState_TurnR90);
	INIT_STATE(CJokerState_WalkB);
	INIT_STATE(CJokerState_WalkR);
	INIT_STATE(CJokerState_WalkF);
	INIT_STATE(CJokerState_WalkL);
	INIT_STATE(CJokerState_WheelAtkEnd);
	INIT_STATE(CJokerState_WheelAtkEnd2);
	INIT_STATE(CJokerState_WheelAtkLoop);
	INIT_STATE(CJokerState_WheelAtkStart);
	INIT_STATE(CJokerState_RunAttackStart);

	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));

	Bind_KeyEvent("Elite_Joker");

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);
}


void CJoker::Move_RootMotion_Internal()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
}

void CJoker::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CJoker::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CJoker::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CJoker::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);


	if ((_uint)EVENT_TYPE::ON_ENTER_SECTION == iArg)
	{
		Set_Enable(true);
	}

	if ((_uint)EVENT_TYPE::ON_GROGGY == iArg)
	{
		Change_State<CJokerState_Stun_Start>();
	}

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->Set_Position(XMLoadFloat4(&m_tLinkStateDesc.m_fStartPositon), 0.f, Filters);

		Change_State<CJokerState_Sp_Open>();
		Set_Enable(false);
		m_pStatus.lock()->Full_Recovery();
	}

	if ((_uint)EVENT_TYPE::ON_JOKEREXECUTIONSTART == iArg)
	{
		m_bEliteExecutionStartOnOff = true;
	}

}

void CJoker::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CJoker::OnDisable()
{
	__super::OnDisable();
}



void CJoker::Free()
{
}
