#include "stdafx.h"
#include "BossVarg/VargBossState_Roar_Getup.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "Status_Monster.h"
#include "Status_Boss.h"

GAMECLASS_C(CVargBossState_SPA_Roar_Getup);
CLONE_C(CVargBossState_SPA_Roar_Getup, CComponent)

HRESULT CVargBossState_SPA_Roar_Getup::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_SPA_Roar_Getup::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_SPA_Roar_Getup::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_SPAttack1_Roar_GetUp");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_SPA_Roar_Getup::Call_AnimationEnd, this);
}

void CVargBossState_SPA_Roar_Getup::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (m_bShakingCamera)
	{
		_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position());
		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.15f, 2.f, 3.f, 0.95f);
		GAMEINSTANCE->Set_RadialBlur(0.3f, vPosition);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_SPA_Roar_Getup::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_SPA_Roar_Getup::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

	pStatus.lock()->Set_FullHp(100.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CVargBossState_SPA_Roar_Getup::Call_NextKeyFrame, this, placeholders::_1);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: SPA_Getup -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_SPA_Roar_Getup::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CVargBossState_SPA_Roar_Getup::Call_NextKeyFrame, this, placeholders::_1);



}
void CVargBossState_SPA_Roar_Getup::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 34:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_EyeTrailEnable(true);
		GET_SINGLE(CGameManager)->Store_EffectIndex("Varg_Eye", GET_SINGLE(CGameManager)->Use_EffectGroup("Varg_Eye", m_pTransformCom, _uint(TIMESCALE_LAYER::MONSTER)));
		break;
	case 62:
		m_bShakingCamera = true;
		break;

	case 97:
		m_bShakingCamera = false;
		break;
	}

}



void CVargBossState_SPA_Roar_Getup::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Run>(0.05f);
}

void CVargBossState_SPA_Roar_Getup::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_SPA_Roar_Getup::Call_AnimationEnd, this);
}

void CVargBossState_SPA_Roar_Getup::Free()
{

}

_bool CVargBossState_SPA_Roar_Getup::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

