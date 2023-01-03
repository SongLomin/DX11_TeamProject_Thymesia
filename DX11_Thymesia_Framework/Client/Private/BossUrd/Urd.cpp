#include "stdafx.h"
#include "BossUrd/Urd.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "BossUrd/UrdStates.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "UrdWeapon.h"
#include "JavelinWeapon.h"

GAMECLASS_C(CUrd);
CLONE_C(CUrd, CGameObject);

HRESULT CUrd::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUrd::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxAnimModel"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pModelCom.lock()->Init_Model("Boss_Urd", "", (_uint)TIMESCALE_LAYER::MONSTER);


	m_pStandState = Add_Component<CUrdBossState_Attack01>();
	Add_Component<CUrdBossState_Attack01LV2>();
	Add_Component<CUrdBossState_Attack01LV2>();
	Add_Component<CUrdBossState_Attack02>();
	Add_Component<CUrdBossState_Attack02LV2C0>();
	Add_Component<CUrdBossState_Attack02LV2C1>();
	Add_Component<CUrdBossState_Attack03_DashSting_L>();
	Add_Component<CUrdBossState_Attack03_DashSting_R>();
	Add_Component<CUrdBossState_Attack05>();
	Add_Component<CUrdBossState_Attack06>();
	Add_Component<CUrdBossState_Attack07>();
	Add_Component<CUrdBossState_Dead>();
	Add_Component<CUrdBossState_Equip_L>();
	Add_Component<CUrdBossState_Equip_R>();
	Add_Component<CUrdBossState_FightStart>();
	Add_Component<CUrdBossState_HurtCounter>();
	Add_Component<CUrdBossState_HurtM_FL>();
	Add_Component<CUrdBossState_HurtM_FR>();
	Add_Component<CUrdBossState_HurtS_FL>();
	Add_Component<CUrdBossState_HurtS_FR>();
	Add_Component<CUrdBossState_Idle>();
	Add_Component<CUrdBossState_Attack_Idle>();
	Add_Component<CUrdBossState_Step_Idle>();
	Add_Component<CUrdBossState_Walk_Idle>();
	Add_Component<CUrdBossState_Skill_Idle>();
	Add_Component<CUrdBossState_Parry_L>();
	Add_Component<CUrdBossState_Parry_R>();
	Add_Component<CUrdBossState_Parry_RQuick>();
	Add_Component<CUrdBossState_Run>();
	Add_Component<CUrdBossState_Skill01>();
	Add_Component<CUrdBossState_Skill02_1>();
	Add_Component<CUrdBossState_Skill03_L>();
	Add_Component<CUrdBossState_Skill03_R>();
	Add_Component<CUrdBossState_SPSkill01>();
	Add_Component<CUrdBossState_Start>();
	Add_Component<CUrdBossState_StepL>();
	Add_Component<CUrdBossState_StepR>();
	Add_Component<CUrdBossState_StepFR>();
	Add_Component<CUrdBossState_StepFL>();
	Add_Component<CUrdBossState_StepFR45>();
	Add_Component<CUrdBossState_StepFL45>();
	Add_Component<CUrdBossState_StepB>();
	Add_Component<CUrdBossState_StunStart>();
	Add_Component<CUrdBossState_StunLoop>();
	Add_Component<CUrdBossState_StunEnd>();
	Add_Component<CUrdBossState_VS_TakeExecution>();
	Add_Component<CUrdBossState_WalkB>();
	Add_Component<CUrdBossState_WalkBL>();
	Add_Component<CUrdBossState_WalkBR>();
	Add_Component<CUrdBossState_WalkL>();
	Add_Component<CUrdBossState_WalkR>();
	Add_Component<CUrdBossState_WalkF>();
	Add_Component<CUrdBossState_WalkFR>();
	Add_Component<CUrdBossState_WalkFL>();
	Add_Component<CUrdBossState_TurnL>();
	Add_Component<CUrdBossState_TurnR>();



	m_fCullingRange = 999.f;


	return S_OK;
}

HRESULT CUrd::Start()
{
	__super::Start();


	CBase::Set_Enable(true);
	Change_State<CUrdBossState_Start>();


	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CUrd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CUrd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUrd::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

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

		//m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

		m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, iPassIndex, "g_Bones", pDeviceContext);
		//m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CUrd::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CUrd::Init_Desc()
{
	__super::Init_Desc();

	m_pModelCom.lock()->Init_Model("Boss_Urd", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CUrdWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "AnimTargetPoint");
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.9f,-2.4f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.9f,-2.4f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.8f,-2.2f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.7f,-2.0f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.6f,-1.8f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.5f,-1.6f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.4f,-1.4f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.3f,-1.2f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.2f,-1.f,1.0f },  0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.1f,-0.8f,1.0f }, 0.2f, COLLISION_LAYER::MONSTER_ATTACK);
	
	m_pDecoWeapons.push_back(GAMEINSTANCE->Add_GameObject<CUrdWeapon>(m_CreatedLevel));
	m_pDecoWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword02_Point");
	m_pDecoWeapons.push_back(GAMEINSTANCE->Add_GameObject<CUrdWeapon>(m_CreatedLevel));
	m_pDecoWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword03_Point");
	m_pDecoWeapons.push_back(GAMEINSTANCE->Add_GameObject<CUrdWeapon>(m_CreatedLevel));
	m_pDecoWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "SK_W_UrdSword04_Point");

	m_m_pJavelinWeapon.push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));
	m_m_pJavelinWeapon.push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));
	m_m_pJavelinWeapon.push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));

	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X + (_byte)ROOTNODE_FLAG::Z);


	INIT_STATE(CUrdBossState_Attack01);
	INIT_STATE(CUrdBossState_Attack01LV2);
	INIT_STATE(CUrdBossState_Attack01LV2);
	INIT_STATE(CUrdBossState_Attack02);
	INIT_STATE(CUrdBossState_Attack02LV2C0);
	INIT_STATE(CUrdBossState_Attack02LV2C1);
	INIT_STATE(CUrdBossState_Attack03_DashSting_L);
	INIT_STATE(CUrdBossState_Attack03_DashSting_R);
	INIT_STATE(CUrdBossState_Attack05);
	INIT_STATE(CUrdBossState_Attack06);
	INIT_STATE(CUrdBossState_Attack07);
	INIT_STATE(CUrdBossState_Dead);
	INIT_STATE(CUrdBossState_Equip_L);
	INIT_STATE(CUrdBossState_Equip_R);
	INIT_STATE(CUrdBossState_FightStart);
	INIT_STATE(CUrdBossState_HurtCounter);
	INIT_STATE(CUrdBossState_HurtM_FL);
	INIT_STATE(CUrdBossState_HurtM_FR);
	INIT_STATE(CUrdBossState_HurtS_FL);
	INIT_STATE(CUrdBossState_HurtS_FR);
	INIT_STATE(CUrdBossState_Idle);
	INIT_STATE(CUrdBossState_Attack_Idle);
	INIT_STATE(CUrdBossState_Step_Idle);
	INIT_STATE(CUrdBossState_Walk_Idle);
	INIT_STATE(CUrdBossState_Skill_Idle);
	INIT_STATE(CUrdBossState_Parry_L);
	INIT_STATE(CUrdBossState_Parry_R);
	INIT_STATE(CUrdBossState_Parry_RQuick);
	INIT_STATE(CUrdBossState_Run);
	INIT_STATE(CUrdBossState_Skill01);
	INIT_STATE(CUrdBossState_Skill02_1);
	INIT_STATE(CUrdBossState_Skill03_L);
	INIT_STATE(CUrdBossState_Skill03_R);
	INIT_STATE(CUrdBossState_SPSkill01);
	INIT_STATE(CUrdBossState_Start);
	INIT_STATE(CUrdBossState_StepL);
	INIT_STATE(CUrdBossState_StepR);
	INIT_STATE(CUrdBossState_StepFR);
	INIT_STATE(CUrdBossState_StepFL);
	INIT_STATE(CUrdBossState_StepFR45);
	INIT_STATE(CUrdBossState_StepFL45);
	INIT_STATE(CUrdBossState_StepB);
	INIT_STATE(CUrdBossState_StunStart);
	INIT_STATE(CUrdBossState_StunLoop);
	INIT_STATE(CUrdBossState_StunEnd);
	INIT_STATE(CUrdBossState_VS_TakeExecution);
	INIT_STATE(CUrdBossState_WalkB);
	INIT_STATE(CUrdBossState_WalkBL);
	INIT_STATE(CUrdBossState_WalkBR);
	INIT_STATE(CUrdBossState_WalkL);
	INIT_STATE(CUrdBossState_WalkR);
	INIT_STATE(CUrdBossState_WalkF);
	INIT_STATE(CUrdBossState_WalkFR);
	INIT_STATE(CUrdBossState_WalkFL);
	INIT_STATE(CUrdBossState_TurnL);
	INIT_STATE(CUrdBossState_TurnR);

	Bind_KeyEvent("Boss_Urd");

	m_pPhysXControllerCom.lock()->Init_Controller(
		Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
		(_uint)PHYSX_COLLISION_LAYER::MONSTER);
}

void CUrd::Move_RootMotion_Internal()
{
	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root") * XMLoadFloat3(&m_vMoveScale);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
}

void CUrd::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionEnter(pMyCollider, pOtherCollider);
}

void CUrd::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionStay(pMyCollider, pOtherCollider);
}

void CUrd::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
	__super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CUrd::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_BOSS_EXECUTIONSTART == iArg)
	{
		m_bBossExecutionStartOnOff = true;
	}
}

void CUrd::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);


}
void CUrd::Use_DecoWeapon()
{
	m_pDecoWeapons[m_iDecoIndex].lock()->Set_Enable(false);
	++m_iDecoIndex;
}

void CUrd::OnDisable()
{
	__super::OnDisable();
}

void CUrd::Reset_JavelinWeapon()
{
	for (auto& elem : m_m_pJavelinWeapon)
	{
		elem.lock()->Set_Enable(false);
	}
}

void CUrd::Free()
{
}
