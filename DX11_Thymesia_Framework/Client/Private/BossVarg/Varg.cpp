#include "stdafx.h"
#include "BossVarg/Varg.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "MobWeapon.h"
#include "VargStates.h"
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


	memcpy(&m_tLinkStateDesc, pArg, sizeof(STATE_LINK_MONSTER_DESC));

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
	


	
	


	//TODO 위치이동 야매임
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tLinkStateDesc.vYame.x, m_tLinkStateDesc.vYame.y, m_tLinkStateDesc.vYame.z, 1.f));
	m_pTransformCom.lock()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.0f));
	//TODO 여기서하는 이유는 몬스터가 배치되고 원점에서 우리가 피킹한위치만큼더해지고 난뒤에 그월드포지션값저장하기위해서 여기서함

	m_pModelCom.lock()->Set_RootNode("root");
	
	m_pStandState = Add_Component<CVargBossState_Start>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack1a>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack1b>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack2a>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack2b>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack3a>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack3b>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_AvoidAttack>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_AvoidB>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_AvoidL>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_AvoidR>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Hurt>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Parry>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Run>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_RunAttack>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_RunStart>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_SPA_Catch>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_SPA_CatchFail>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_SPA_Roar>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_SPA_Roar_Getup>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_SPA_Run>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Idle>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_End>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_Exe_Dead>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_Exe_SitDown>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_Exe_SitLoop>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_Exe_Start>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_Loop>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Stun_Start>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_TurnL>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_TurnR>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_WalkB>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_WalkF>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_WalkR>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_RaidAttack>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_TurnAttack>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack2b1>(&m_tLinkStateDesc);
	Add_Component<CVargBossState_Attack2b2>(&m_tLinkStateDesc);

	
	
	GET_SINGLE(CGameManager)->Bind_KeyEvent("Boss_Varg", m_pModelCom, bind(&CVarg::Call_NextAnimationKey, this, placeholders::_1));

	m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom));

	m_fCullingRange = 999.f;

	return S_OK;
}

HRESULT CVarg::Start()
{
	__super::Start();


	Change_State<CVargBossState_Start>();


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
