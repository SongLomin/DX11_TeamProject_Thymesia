#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_HitDown.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "LuxiyaStates/LuxiyaStates.h"

GAMECLASS_C(CLuxiyaState_HitDown);
CLONE_C(CLuxiyaState_HitDown, CComponent)

HRESULT CLuxiyaState_HitDown::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CLuxiyaState_HitDown::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 23;
	m_fFixedPlayRatio = 0.f;

	return S_OK;
}

void CLuxiyaState_HitDown::Start()
{
	__super::Start();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_HitDown::Call_AnimationEnd, this);
}

void CLuxiyaState_HitDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
}

void CLuxiyaState_HitDown::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CLuxiyaState_HitDown::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
}

void CLuxiyaState_HitDown::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	_int iRandom = rand() % 2;

	switch (iRandom)
	{
	case 0:
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_DownHit.wav"), 1.f);
		break;

	case 1:
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_DownHit2.wav"), 1.f);
		break;
	}


#ifdef _DEBUG
	cout << "LuxiyaState: HitDown -> OnStateStart" << endl;
#endif
}

void CLuxiyaState_HitDown::OnStateEnd()
{
	__super::OnStateEnd();
}

void CLuxiyaState_HitDown::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_StandUp>();

}

void CLuxiyaState_HitDown::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_HitDown::Call_AnimationEnd, this);
}

void CLuxiyaState_HitDown::Free()
{

}

_bool CLuxiyaState_HitDown::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}