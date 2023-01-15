#include "stdafx.h"
#include "BossBat/BatBossState_Charge.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "Status.h"
#include "Status_Boss.h"

GAMECLASS_C(CBatBossState_Charge);
CLONE_C(CBatBossState_Charge, CComponent)

HRESULT CBatBossState_Charge::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Charge::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Charge::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Charge");

	m_pHeadBoneNode = m_pModelCom.lock()->Find_BoneNode("head");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Charge::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Charge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);

}


void CBatBossState_Charge::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Charge::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	if (In_iKeyIndex >= 80 && In_iKeyIndex <= 225)
	{
		_matrix CombinedMatrix = Get_HeadCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];

		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.03f, 9.f, 3.f);
	}

	switch (In_iKeyIndex)
	{
	case 294://ÀÌ¶§ Èú.
	{
 		weak_ptr<CStatus_Boss> pBossStatus = Get_OwnerMonster()->Get_Component<CStatus_Boss>();
		pBossStatus.lock()->Heal(pBossStatus.lock()->Get_Desc().m_fMaxHP_white * m_fHeallingRatio);
		break;
	}

	default:
		break;
	}

}

void CBatBossState_Charge::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_iHellSceram += 1;

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Charge::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Charge::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Charge::Call_NextAnimationKey, this, placeholders::_1);

}


void CBatBossState_Charge::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
	

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ZeroAttackCount(0);
	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ChargeCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Charge::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Charge::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Charge::Free()
{

}

_bool CBatBossState_Charge::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_iHellSceram == 1)
	{
		Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_ChestCheck(true);
	}

	if (m_iHellSceram == 2)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.99f)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_Hellscream>(0.05f);
			return true;
		}
	} 
	

	return false;
}

