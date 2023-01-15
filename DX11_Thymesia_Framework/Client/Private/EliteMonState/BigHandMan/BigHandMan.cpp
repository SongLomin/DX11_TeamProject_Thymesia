#include "stdafx.h"
#include "EliteMonState/BigHandMan/BigHandMan.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "Client_Components.h"
#include "Status_Monster.h"
#include "MonsterHPBar_Elite.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"

GAMECLASS_C(CBigHandMan);
CLONE_C(CBigHandMan, CGameObject);

HRESULT CBigHandMan::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CBigHandMan::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_pStatus = Add_Component<CStatus_Monster>(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pModelCom.lock()->Init_Model("Elite_BigHandman", "", (_uint)TIMESCALE_LAYER::MONSTER);

	m_pStandState = Add_Component<CBigHandManState_ComboA01>();
	Add_Component<CBigHandManState_ComboA02>();
	Add_Component<CBigHandManState_ComboB_End>();
	Add_Component<CBigHandManState_ComboB_Loop>();
	Add_Component<CBigHandManState_ComboB_Start>();
	Add_Component<CBigHandManState_HurtCounter>();
	Add_Component<CBigHandManState_HurtL>();
	Add_Component<CBigHandManState_HurtR>();
	Add_Component<CBigHandManState_Idle>();
	Add_Component<CBigHandManState_SP_Idle1>();
	Add_Component<CBigHandManState_SP_Idle1_End>();
	Add_Component<CBigHandManState_Stun_End>();
	Add_Component<CBigHandManState_Stun_Loop>();
	Add_Component<CBigHandManState_Stun_Start>();
	Add_Component<CBigHandManState_TurnL90>();
	Add_Component<CBigHandManState_TurnR90>();
	Add_Component<CBigHandManState_VS_TakeExecution_01>();
	Add_Component<CBigHandManState_Walk>();


	m_fCullingRange = 999.f;
	return S_OK;
}

HRESULT CBigHandMan::Start()
{
	__super::Start();

	Change_State<CBigHandManState_Idle>();

	Bind_HPBar();
	
	return S_OK;
}

void CBigHandMan::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CBigHandMan::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBigHandMan::Render(ID3D11DeviceContext* pDeviceContext)
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


		//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, m_iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CBigHandMan::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}



void CBigHandMan::Init_Desc()
{
	__super::Init_Desc();

	m_pModelCom.lock()->Init_Model("Elite_BigHandman", "", (_uint)TIMESCALE_LAYER::MONSTER);
	


	m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);

	INIT_STATE(CBigHandManState_ComboA01);
	INIT_STATE(CBigHandManState_ComboA02);
	INIT_STATE(CBigHandManState_ComboB_End);
	INIT_STATE(CBigHandManState_ComboB_Loop);
	INIT_STATE(CBigHandManState_ComboB_Start);
	INIT_STATE(CBigHandManState_HurtCounter);
	INIT_STATE(CBigHandManState_HurtL);
	INIT_STATE(CBigHandManState_HurtR);
	INIT_STATE(CBigHandManState_Idle);
	INIT_STATE(CBigHandManState_SP_Idle1);
	INIT_STATE(CBigHandManState_SP_Idle1_End);
	INIT_STATE(CBigHandManState_Stun_End);
	INIT_STATE(CBigHandManState_Stun_Loop);
	INIT_STATE(CBigHandManState_Stun_Start);
	INIT_STATE(CBigHandManState_TurnL90);
	INIT_STATE(CBigHandManState_TurnR90);
	INIT_STATE(CBigHandManState_VS_TakeExecution_01);
	INIT_STATE(CBigHandManState_Walk);
	

	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));

	Bind_KeyEvent("Elite_BigHandman");

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);
}


void CBigHandMan::Move_RootMotion_Internal()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
}

void CBigHandMan::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CBigHandMan::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CBigHandMan::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CBigHandMan::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);


	if ((_uint)EVENT_TYPE::ON_ENTER_SECTION == iArg)
	{
		Set_Enable(true);
	}

	if ((_uint)EVENT_TYPE::ON_BIGHANDMANEXECUTIONSTART == iArg)
	{
		m_bEliteExecutionStartOnOff = true;
	}



	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->Set_Position(XMLoadFloat4(&m_tLinkStateDesc.m_fStartPositon), 0.f, Filters);

		Change_State<CBigHandManState_Idle>();
		Set_Enable(false);
		m_pStatus.lock()->Full_Recovery();
	}

}

void CBigHandMan::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CBigHandMan::OnDisable()
{
	__super::OnDisable();
}



void CBigHandMan::Free()
{
}
