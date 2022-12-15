#include "stdafx.h"
#include "BossBat/Bat.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "Client_Components.h"
#include "BossBat/BatStates.h"
#include "MonsterHPBar_Boss.h"
#include "Status_Monster.h"
#include "Status_Boss.h"

GAMECLASS_C(CBat);
CLONE_C(CBat, CGameObject);

HRESULT CBat::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CBat::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pModelCom.lock()->Init_Model("Boss_Bat", "", (_uint)TIMESCALE_LAYER::MONSTER);

	m_pStandState = Add_Component<CBatBossState_Start>();
	Add_Component<CBatBossState_Idle>();
	Add_Component<CBatBossState_Car>();
	Add_Component<CBatBossState_Atk_L01_1>();
	Add_Component<CBatBossState_Atk_L01_2a>();
	Add_Component<CBatBossState_Atk_L01_2b>();
	Add_Component<CBatBossState_Atk_L01_3a>();
	Add_Component<CBatBossState_Atk_R01_1>();
	Add_Component<CBatBossState_Atk_R01_2a>();
	Add_Component<CBatBossState_Atk_R01_2b>();
	Add_Component<CBatBossState_Bite_1>();
	Add_Component<CBatBossState_Bite_2>();
	Add_Component<CBatBossState_Car>();
	Add_Component<CBatBossState_Charge>();
	Add_Component<CBatBossState_FTurnL>();
	Add_Component<CBatBossState_FTurnR>();
	Add_Component<CBatBossState_Hellscream>();
	Add_Component<CBatBossState_HurtXL_F>();
	Add_Component<CBatBossState_HurtXL_L>();
	Add_Component<CBatBossState_JumpSmash_Chest>();
	Add_Component<CBatBossState_JumpSmash_ForwardL>();
	Add_Component<CBatBossState_JumpSmash_SmarhL>();
	Add_Component<CBatBossState_SonicBoom>();
	Add_Component<CBatBossState_Sp>();
	Add_Component<CBatBossState_Start_Loop>();
	Add_Component<CBatBossState_Storm>();
	Add_Component<CBatBossState_Stun_End>();
	Add_Component<CBatBossState_Stun_Start>();
	Add_Component<CBatBossState_Stun_Loop>();
	Add_Component<CBatBossState_TakeExecution_End>();
	Add_Component<CBatBossState_TakeExecution_Start>();
	Add_Component<CBatBossState_TakeExecution_Loop>();
	Add_Component<CBatBossState_TurnL>();
	Add_Component<CBatBossState_TurnR>();
	Add_Component<CBatBossState_WalkF>();


	GET_SINGLE(CGameManager)->Bind_KeyEvent("Boss_Bat", m_pModelCom, bind(&CBat::Call_NextAnimationKey, this, placeholders::_1));

	m_fCullingRange = 999.f;

	USE_START(CBat);
	return S_OK;
}

HRESULT CBat::Start()
{
	__super::Start();

	CBase::Set_Enable(true);

	Change_State<CBatBossState_Start_Loop>();


	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CBat::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CBat::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBat::Render()
{
	__super::Render();

	_int iPassIndex = 0;
	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();


	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{


		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			iPassIndex = 0;
		}
		else
		{
			iPassIndex = 4;
		}

		//m_pShaderCom.lock()->Begin(m_iPassIndex);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones");
		//m_pModelCom.lock()->Render_Mesh(i);
	}

	return S_OK;
}

void CBat::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CBat::Init_Desc()
{

	__super::Init_Desc();

	m_pModelCom.lock()->Init_Model("Boss_Bat", "", (_uint)TIMESCALE_LAYER::MONSTER);


	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-140.f));
	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);

	//GET_SINGLE(CGameManager)->Bind_KeyEvent("Monster1", m_pModelCom, bind(&CBat::Call_NextAnimationKey, this, placeholders::_1));
	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);

	INIT_STATE(CBatBossState_Start);
	INIT_STATE(CBatBossState_Idle);
	INIT_STATE(CBatBossState_Car);
	INIT_STATE(CBatBossState_Atk_L01_1);
	INIT_STATE(CBatBossState_Atk_L01_2a);
	INIT_STATE(CBatBossState_Atk_L01_2b);
	INIT_STATE(CBatBossState_Atk_L01_3a);
	INIT_STATE(CBatBossState_Atk_R01_1);
	INIT_STATE(CBatBossState_Atk_R01_2a);
	INIT_STATE(CBatBossState_Atk_R01_2b);
	INIT_STATE(CBatBossState_Bite_1);
	INIT_STATE(CBatBossState_Bite_2);
	INIT_STATE(CBatBossState_Car);
	INIT_STATE(CBatBossState_Charge);
	INIT_STATE(CBatBossState_FTurnL);
	INIT_STATE(CBatBossState_FTurnR);
	INIT_STATE(CBatBossState_Hellscream);
	INIT_STATE(CBatBossState_HurtXL_F);
	INIT_STATE(CBatBossState_HurtXL_L);
	INIT_STATE(CBatBossState_JumpSmash_Chest);
	INIT_STATE(CBatBossState_JumpSmash_ForwardL);
	INIT_STATE(CBatBossState_JumpSmash_SmarhL);
	INIT_STATE(CBatBossState_SonicBoom);
	INIT_STATE(CBatBossState_Sp);
	INIT_STATE(CBatBossState_Start_Loop);
	INIT_STATE(CBatBossState_Storm);
	INIT_STATE(CBatBossState_Stun_End);
	INIT_STATE(CBatBossState_Stun_Start);
	INIT_STATE(CBatBossState_Stun_Loop);
	INIT_STATE(CBatBossState_TakeExecution_End);
	INIT_STATE(CBatBossState_TakeExecution_Start);
	INIT_STATE(CBatBossState_TakeExecution_Loop);
	INIT_STATE(CBatBossState_TurnL);
	INIT_STATE(CBatBossState_TurnR);
	INIT_STATE(CBatBossState_WalkF);

	GET_SINGLE(CGameManager)->Bind_KeyEvent("Boss_Bat", m_pModelCom, bind(&CBat::Call_NextAnimationKey, this, placeholders::_1));

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);
}

void CBat::Move_RootMotion_Internal()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));

	PxControllerFilters Filters = Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
}

void CBat::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CBat::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CBat::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CBat::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}

void CBat::OnDisable()
{
	__super::OnDisable();
}



void CBat::Free()
{
}
