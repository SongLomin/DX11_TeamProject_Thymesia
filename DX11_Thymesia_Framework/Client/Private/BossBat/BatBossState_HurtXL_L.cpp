#include "stdafx.h"
#include "BossBat/BatBossState_HurtXL_L.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_HurtXL_L);
CLONE_C(CBatBossState_HurtXL_L, CComponent)

HRESULT CBatBossState_HurtXL_L::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_HurtXL_L::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_HurtXL_L::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_HurtXL_L");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_HurtXL_L::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_HurtXL_L::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_HurtXL_L::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}

void CBatBossState_HurtXL_L::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 87:
	{
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	case 94:
	{
		_vector vPosition = m_pOwner.lock()->Get_Transform()->Get_Position();
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	}
}

void CBatBossState_HurtXL_L::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_HurtXL_L::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_HurtXL_L::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_HurtXL_L::Call_NextAnimationKey, this, placeholders::_1);

}



void CBatBossState_HurtXL_L::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_HurtXL_L::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_HurtXL_L::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_HurtXL_L::Free()
{

}

_bool CBatBossState_HurtXL_L::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;




	return false;
}

