#include "stdafx.h"
#include "BossUrd/UrdBossState_Skill01.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"
#include "JavelinWeapon.h"

GAMECLASS_C(CUrdBossState_Skill01);
CLONE_C(CUrdBossState_Skill01, CComponent)

HRESULT CUrdBossState_Skill01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Skill01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Skill01::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Skill01|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Skill01::Call_AnimationEnd, this);
}

void CUrdBossState_Skill01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAtkPermitAndNoHurt)
	{
		TurnAttack(fTimeDelta * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Skill01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Skill01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bOne = true;

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.5f, 1.5f, 1.5f));

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Skill01::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_Skill01::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Skill01::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Skill01::Call_AnimationEnd, this);
}

void CUrdBossState_Skill01::Free()
{

}

_bool CUrdBossState_Skill01::Check_AndChangeNextState()
{

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	//
	//for (auto& elem : pWeapons)
	//{
	//	if (elem.lock()->Get_WeaponNum() == 1)
	//	{
	//		elem.lock()->Set_RenderOnOff(false);
	//	}
	//}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 42 && m_bOne)
	{
		//여기서 첫번쨰꺼 안보이게하고 두번쨰거 여기서 바인딩하면될듯
		m_bOne = false;

		for (auto& elem : pWeapons)
		{
			if (elem.lock()->Get_WeaponNum() == 1)
			{
				elem.lock()->Set_RenderOnOff(false);
			}
		}

		//Get_OwnerMonster()->Get_JavelinWeapon().push_back(GAMEINSTANCE->Add_GameObject<CJavelinWeapon>(m_CreatedLevel));
		Get_OwnerMonster()->Get_JavelinWeapon().back().lock()->Init_Weapon(m_pModelCom, m_pTransformCom, "weapon_r");
		Get_OwnerMonster()->Get_JavelinWeapon().back().lock()->Get_Transform()->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-180.0f));

	}	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 43 && !m_bOne)
	{
		Get_OwnerMonster()->Get_JavelinWeapon().back().lock()->Set_BoneBindOff(false);
		//Get_OwnerMonster()->Get_JavelinWeapon().back().lock()->Get_Transform()->LookAt2D(pCurrentPlayer.lock()->Get_WorldPosition());
		//Get_OwnerMonster
	
	}
		
	
		


	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	return false;
}

