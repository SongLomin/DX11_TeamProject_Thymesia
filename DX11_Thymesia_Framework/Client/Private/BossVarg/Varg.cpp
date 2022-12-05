#include "stdafx.h"
#include "BossVarg/Varg.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "VargStates.h"
#include "Effect_Trail_EyeLight.h"
//#include "MonsterWeapon.h"
//#include "Monster1States/Monster1States.h"
#include "Client_Components.h"
#include "MonsterHPBar_Boss.h"
#include "Status_Monster.h"

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

	weak_ptr<CMonsterHPBar_Boss> pHPBar = GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Boss>(LEVEL_STATIC);
	pHPBar.lock()->Set_Owner(Weak_Cast<CMonster>(m_this));
	
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
	Add_Component<CVargBossState_Stun_Exe_Dead>();
	Add_Component<CVargBossState_Stun_Exe_SitDown>();
	Add_Component<CVargBossState_Stun_Exe_SitLoop>();
	Add_Component<CVargBossState_Stun_Exe_Start>();
	Add_Component<CVargBossState_Stun_Loop>();
	Add_Component<CVargBossState_Stun_Start>();
	Add_Component<CVargBossState_TurnL>();
	Add_Component<CVargBossState_TurnR>();
	Add_Component<CVargBossState_WalkB>();
	Add_Component<CVargBossState_WalkF>();
	Add_Component<CVargBossState_WalkR>();
	Add_Component<CVargBossState_RaidAttack>();
	Add_Component<CVargBossState_TurnAttack>();
	Add_Component<CVargBossState_Attack2b1>();
	Add_Component<CVargBossState_Attack2b2>();

	TRAIL_DESC TrailDesc;
	ZeroMemory(&TrailDesc, sizeof(TRAIL_DESC));

	TrailDesc.iMaxCnt = 100;
	//position 0.163, 0.12,0.055 , z 0.1씩
	TrailDesc.vPos_0 = _float3(0.163, 0.12, 0.155);
	TrailDesc.vPos_1 = _float3(0.163, 0.12, -0.045);
	m_pTrailEffect = GAMEINSTANCE->Add_GameObject<CEffect_Trail_EyeLight>(LEVEL_GAMEPLAY, &TrailDesc);

	//GET_SINGLE(CGameManager)->Bind_KeyEvent("Boss_Varg", m_pModelCom, bind(&CVarg::Call_NextAnimationKey, this, placeholders::_1));

	m_fCullingRange = 999.f;


	return S_OK;
}

HRESULT CVarg::Start()
{
	__super::Start();

	m_pTrailBoneNode = m_pModelCom.lock()->Find_BoneNode("Bip001-Head");



	Change_State<CVargBossState_Start>();

	//weak_ptr<CBoneNode> pTargetBoneNode = m_pModelCom.lock()->Find_BoneNode();
	//m_pTrailEffect.lock()->Set_OwnerDesc(m_pTransformCom, m_pTargetBoneNode, m_pModelCom.lock()->Get_ModelData());
	//m_EffectIndexList.emplace_back("Character_Target", GET_SINGLE(CGameManager)->Use_EffectGroup("Character_Target", m_pTransformCom));

	return S_OK;
}

void CVarg::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root", true, XMMatrixRotationX(XMConvertToRadians(-90.f)));

	PxControllerFilters Filters = Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);

	m_pTrailEffect.lock()->Update(fTimeDelta, m_pTransformCom, m_pTrailBoneNode, m_pModelCom.lock()->Get_ModelData());


}

void CVarg::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CVarg::Render()
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

void CVarg::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

}

void CVarg::Init_Desc()
{
	m_pModelCom.lock()->Init_Model("Boss_Varg", "", (_uint)TIMESCALE_LAYER::MONSTER);
	m_pWeapons.push_back(GAMEINSTANCE->Add_GameObject<CMobWeapon>(m_CreatedLevel));
	m_pWeapons.back().lock()->Init_Model("Boss_VargWeapon", TIMESCALE_LAYER::MONSTER);
	m_pWeapons.back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");

	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.9f,-2.4f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.8f,-2.2f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.7f,-2.0f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.6f,-1.8f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.5f,-1.6f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.4f,-1.4f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.3f,-1.2f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.2f,-1.f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.1f,-0.8f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.0f,-0.6f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);
	m_pWeapons.back().lock()->Add_Collider({ 0.f,0.0f,-0.4f,1.0f }, 0.4f, COLLISION_LAYER::MONSTER_ATTACK);

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
	INIT_STATE(CVargBossState_Stun_Exe_Dead);
	INIT_STATE(CVargBossState_Stun_Exe_SitDown);
	INIT_STATE(CVargBossState_Stun_Exe_SitLoop);
	INIT_STATE(CVargBossState_Stun_Exe_Start);
	INIT_STATE(CVargBossState_Stun_Loop);
	INIT_STATE(CVargBossState_Stun_Start);
	INIT_STATE(CVargBossState_TurnL);
	INIT_STATE(CVargBossState_TurnR);
	INIT_STATE(CVargBossState_WalkB);
	INIT_STATE(CVargBossState_WalkF);
	INIT_STATE(CVargBossState_WalkR);
	INIT_STATE(CVargBossState_RaidAttack);
	INIT_STATE(CVargBossState_TurnAttack);
	INIT_STATE(CVargBossState_Attack2b1);
	INIT_STATE(CVargBossState_Attack2b2);

	GET_SINGLE(CGameManager)->Bind_KeyEvent("Boss_Varg", m_pModelCom, bind(&CVarg::Call_NextAnimationKey, this, placeholders::_1));

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom));
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
