#include "stdafx.h"
#include "BossUrd/UrdBossState_Skill02_1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "JavelinWeapon.h"
#include "MobWeapon.h"

GAMECLASS_C(CUrdBossState_Skill02_1);
CLONE_C(CUrdBossState_Skill02_1, CComponent)

HRESULT CUrdBossState_Skill02_1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Skill02_1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Skill02_1::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Skill02_1|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Skill02_1::Call_AnimationEnd, this);
}

void CUrdBossState_Skill02_1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAtkPermitAndNoHurt)
	{
		TurnAttack(fTimeDelta * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Skill02_1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Skill02_1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.5f, 1.5f, 1.5f));

	m_bOne = true;

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Skill02_1::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_Skill02_1::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Skill02_1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Skill02_1::Call_AnimationEnd, this);
}

void CUrdBossState_Skill02_1::Free()
{

}

_bool CUrdBossState_Skill02_1::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 39 && m_bOne)
	{
		//여기서 첫번쨰꺼 안보이게하고 두번쨰거 여기서 바인딩하면될듯
		m_bOne = false;





		m_pJavelinWeapon = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CJavelinWeapon>(Get_Owner().lock()->Get_CreatedLevel());

		if (!m_pJavelinWeapon.lock())
		{
			DEBUG_ASSERT;
			return false;
		}



		//Get_OwnerMonster()->Get_JavelinWeapon().push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));
		m_pJavelinWeapon.lock()->Set_JavelinState(CJavelinWeapon::JAVELIN_STATE::BIND_HAND);
		m_pJavelinWeapon.lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_l");
		m_pJavelinWeapon.lock()->Set_Enable(true);
		//Get_OwnerMonster()->Get_JavelinWeapon().back().lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.0f));

	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 40 && !m_bOne)
	{
		if (!m_pJavelinWeapon.lock())
		{
			DEBUG_ASSERT;
			return false;
		}


		m_pJavelinWeapon.lock()->Set_JavelinState(CJavelinWeapon::JAVELIN_STATE::STAKE);
		m_pJavelinWeapon.lock()->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(150.0f));
		//m_pJavelinWeapon.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(300.0f));//90
		m_pJavelinWeapon.lock()->Get_Transform()->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(0.f));
	
	}

	return false;
}

