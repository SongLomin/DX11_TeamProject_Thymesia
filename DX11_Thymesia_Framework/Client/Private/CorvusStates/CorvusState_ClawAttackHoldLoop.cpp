#include "stdafx.h"
#include "CorvusStates/CorvusState_ClawAttackHoldLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"


GAMECLASS_C(CCorvusState_ClawAttackHoldLoop);
CLONE_C(CCorvusState_ClawAttackHoldLoop, CComponent)

HRESULT CCorvusState_ClawAttackHoldLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_ClawAttackHoldLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_ClawAttackHoldLoop::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_Raven_ClawCommon_ChargeLoop");
	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ClawAttackHoldLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawAttackHoldLoop::Tick(_float fTimeDelta)
{
	CPlayerStateBase::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	DISSOLVE_DESC	ArmDissolveDesc;
	ZeroMemory(&ArmDissolveDesc, sizeof(DISSOLVE_DESC));
	DISSOLVE_DESC	ClawDissolveDesc;
	ZeroMemory(&ClawDissolveDesc, sizeof(DISSOLVE_DESC));

	ArmDissolveDesc.bBloom = true;
	ArmDissolveDesc.bGlow = true;
	ArmDissolveDesc.fAmount = 0.f;
	ArmDissolveDesc.vDirection = { 1.f,0.f,0.f };
	ArmDissolveDesc.vGlowColor = { 0.f, 1.f, 0.7f, 1.f };
	ArmDissolveDesc.vStartPos = { 3.f,0.f,0.f };

	ClawDissolveDesc.bBloom = true;
	ClawDissolveDesc.bGlow = true;
	ClawDissolveDesc.fAmount = 0.f;
	ClawDissolveDesc.vDirection = { 1.f,0.f,0.f };
	ClawDissolveDesc.vGlowColor = { 0.f, 1.f, 0.7f, 1.f };
	ClawDissolveDesc.vStartPos = { 3.f,0.f,0.f };


	Get_OwnerPlayer()->Set_DissolveAmount(5, ClawDissolveDesc);
	Get_OwnerPlayer()->Set_DissolveAmount(9, ArmDissolveDesc);



	Attack();
}

void CCorvusState_ClawAttackHoldLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//Check_InputNextAttack();

	Check_AndChangeNextState();
}

//void CCorvusState_ClawAttackHoldLoop::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//	Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHoldLoopLoop>();
//
//}

void CCorvusState_ClawAttackHoldLoop::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_ClawAttackHoldLoop::Attack()
{

}

void CCorvusState_ClawAttackHoldLoop::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	m_IsNextAttack = true;

}



void CCorvusState_ClawAttackHoldLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	if (!m_pModelCom.lock().get())
	{

		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
	//m_iAttackIndex = 7;
	//m_iEndAttackEffectIndex = -1;

	m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_ClawChargeLoop_Particle", m_pTransformCom, _uint(TIMESCALE_LAYER::PLAYER));

	//Disable_Weapons();

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Attack -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_ClawAttackHoldLoop::OnStateEnd()
{
	__super::OnStateEnd();

	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Corvus_ClawChargeLoop_Particle", m_iEffectIndex);

	//Disable_Weapons();
	m_IsNextAttack = false;

}

void CCorvusState_ClawAttackHoldLoop::OnEventMessage(_uint iArg)
{
	//__super::OnEventMessage(iArg);
	//
	//if ((_uint)EVENT_TYPE::ON_FIRSTHIT == iArg)
	//{
	//	switch (m_pModelCom.lock()->Get_CurrentAnimationIndex())
	//	{
	//	case 0:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack01.wav"), 1.f);
	//		break;
	//
	//	case 1:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack02.wav"), 1.f);
	//		break;
	//
	//	case 2:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack03.wav"), 1.f);
	//		break;
	//
	//	case 3:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack04.wav"), 1.f);
	//		break;
	//
	//	case 4:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack05.wav"), 1.f);
	//		break;
	//
	//	case 5:
	//		GAMEINSTANCE->PlaySoundW(TEXT("c_luciaRK3_atk_attack06.wav"), 1.f);
	//		break;
	//	}
	//}

}

void CCorvusState_ClawAttackHoldLoop::OnDestroy()
{
	//m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ClawAttackHoldLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawAttackHoldLoop::Free()
{

}

_bool CCorvusState_ClawAttackHoldLoop::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;
	
	Rotation_InputToLookDir();

	if (Check_RequirementAVoidState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		return true;
	}

	if (!Check_RequirementClawAttackHoldState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackAway>();
		return true;
	}

	if (Check_RequirementParryState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
		return true;
	}


	












	return false;
}

_bool CCorvusState_ClawAttackHoldLoop::Check_RequirementNextAttackState()
{

	_uint iTargetKeyFrameFirst = 15;
	_uint iTargetKeyFrameSecond = 50;



	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameFirst, iTargetKeyFrameSecond) && m_IsNextAttack)
	{
		return true;
	}

	return false;
}

_bool CCorvusState_ClawAttackHoldLoop::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 80;



	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsNextAttack)
	{
		return true;
	}



	return false;
}

