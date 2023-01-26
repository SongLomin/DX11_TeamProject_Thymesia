#include "stdafx.h"
#include "BossVarg/VargBossState_SPA_Roar.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_SPA_Roar);
CLONE_C(CVargBossState_SPA_Roar, CComponent)

HRESULT CVargBossState_SPA_Roar::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_Roar::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_SPA_Roar::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Roar");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_Roar::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_Roar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (m_bShakingCamera)
	{
		_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position());
		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.15f, 2.f,3.f,0.95f);
		GAMEINSTANCE->Set_RadialBlur(0.3f, vPosition);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_SPA_Roar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_SPA_Roar::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CVargBossState_SPA_Roar::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_Roar -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_SPA_Roar::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CVargBossState_SPA_Roar::Call_NextKeyFrame, this, placeholders::_1);
}



void CVargBossState_SPA_Roar::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Run>(0.05f);
}

void CVargBossState_SPA_Roar::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 40:
		m_bShakingCamera = true;
		break;

	case 75:
		m_bShakingCamera = false;
		break;
	}
}

void CVargBossState_SPA_Roar::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_Roar::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_SPA_Roar::Free()
{

}

_bool CVargBossState_SPA_Roar::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

