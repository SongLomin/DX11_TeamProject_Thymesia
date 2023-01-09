#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"



GAMECLASS_C(CBigHandManState_HurtCounter);
CLONE_C(CBigHandManState_HurtCounter, CComponent)

HRESULT CBigHandManState_HurtCounter::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_HurtCounter::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_HurtCounter::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_HurtCounter");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_HurtCounter::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_HurtCounter::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_HurtCounter::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_HurtCounter::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);



	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_HurtCounter::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_HurtCounter::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
}

void CBigHandManState_HurtCounter::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_HurtCounter::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_HurtCounter::Free()
{

}

_bool CBigHandManState_HurtCounter::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리



	return false;
}

