#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"



GAMECLASS_C(CBigHandManState_HurtL);
CLONE_C(CBigHandManState_HurtL, CComponent)

HRESULT CBigHandManState_HurtL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_HurtL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_HurtL::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_HurtL");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_HurtL::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_HurtL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_HurtL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_HurtL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_HurtL::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_HurtL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBigHandManState_Idle>(0.05f);
}


void CBigHandManState_HurtL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_HurtL::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_HurtL::Free()
{

}

_bool CBigHandManState_HurtL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리



	return false;
}

