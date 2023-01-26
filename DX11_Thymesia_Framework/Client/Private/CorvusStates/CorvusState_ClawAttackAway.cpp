#include "stdafx.h"
#include "CorvusStates/CorvusState_ClawAttackAway.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CCorvusState_ClawAttackAway);
CLONE_C(CCorvusState_ClawAttackAway, CComponent)

HRESULT CCorvusState_ClawAttackAway::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_ClawAttackAway::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_ClawAttackAway::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_Raven_ClawLong_ChargeFull");
	
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ClawAttackAway::Call_AnimationEnd, this, placeholders::_1);

	m_fDissolveAmountArm = 0.f;
	m_fDissolveAmountClaw = 0.f;
	m_bDissolve = false;
}

void CCorvusState_ClawAttackAway::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);


	DISSOLVE_DESC	ArmDissolveDesc;
	ZeroMemory(&ArmDissolveDesc, sizeof(DISSOLVE_DESC));
	DISSOLVE_DESC	ClawDissolveDesc;
	ZeroMemory(&ClawDissolveDesc, sizeof(DISSOLVE_DESC));

	if (m_bDissolve)
	{
		
			if (0.6f > m_fDissolveTimeClaw)
			{
				m_fDissolveTimeArm -= fTimeDelta;
				m_fDissolveAmountArm = SMath::Lerp(1.f, 0.f, m_fDissolveTimeArm / 0.7f);
				m_vDissolveDir = { 1.f,0.f,0.f };
			}
		
			m_fDissolveTimeClaw -= fTimeDelta;
			m_fDissolveAmountClaw = SMath::Lerp(1.f, 0.f, m_fDissolveTimeClaw / 0.7f);
			m_vDissolveDir = { 1.f,0.f,0.f };
		
		
	}

	ArmDissolveDesc.bBloom = true;
	ArmDissolveDesc.bGlow = true;
	ArmDissolveDesc.fAmount = m_fDissolveAmountArm;
	ArmDissolveDesc.vDirection = m_vDissolveDir;
	ArmDissolveDesc.vGlowColor = { 0.f, 1.f, 0.7f, 1.f };
	ArmDissolveDesc.vStartPos = { 3.f,0.f,0.f };

	ClawDissolveDesc.bBloom = true;
	ClawDissolveDesc.bGlow = true;
	ClawDissolveDesc.fAmount = m_fDissolveAmountClaw;
	ClawDissolveDesc.vDirection = m_vDissolveDir;
	ClawDissolveDesc.vGlowColor = { 0.f, 1.f, 0.7f, 1.f };
	ClawDissolveDesc.vStartPos = { 3.f,0.f,0.f };


	Get_OwnerPlayer()->Set_DissolveAmount(5, ClawDissolveDesc);
	Get_OwnerPlayer()->Set_DissolveAmount(9, ArmDissolveDesc);

	Attack();
}

void CCorvusState_ClawAttackAway::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();
	Check_AndChangeNextState();
}

void CCorvusState_ClawAttackAway::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
}

void CCorvusState_ClawAttackAway::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_ClawAttackAway::Attack()
{

}

void CCorvusState_ClawAttackAway::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
		return;

	m_IsNextAttack = true;
}

void CCorvusState_ClawAttackAway::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 10:
		GET_SINGLE(CGameManager)->Deactivate_Zoom(1.f, EASING_TYPE::LINEAR);
	/*	_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 1.3f, 0.f, 0.f));
		GAMEINSTANCE->Set_RadialBlur(0.3f, vPosition);
	*/	GAMEINSTANCE->Set_MotionBlur(0.05f);
		break;
	case 45:
		m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_ClawChargeAtk_FollowParticle", m_pTransformCom, _uint(TIMESCALE_LAYER::PLAYER));
		return;

	case 74:
		GET_SINGLE(CGameManager)->UnUse_EffectGroup("Corvus_ClawChargeAtk_FollowParticle", m_iEffectIndex);
		return;

	case 130:
		m_fDissolveTimeArm = 0.7f;
		m_fDissolveTimeClaw = 0.7f;
		m_bDissolve = true;
		m_fDissolveAmountArm = 0.f;
		m_fDissolveAmountClaw = 0.f;
		return;
	}
}

void CCorvusState_ClawAttackAway::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_ClawAttackHold>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_ClawAttackHoldLoop>().lock())
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 26);
	}
	else
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}

	if (!m_pModelCom.lock().get())
	{

		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_ClawAttackAway::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CCorvusState_ClawAttackAway::Call_OtherControllerHit, this, placeholders::_1);

	m_fDissolveAmountArm = 0.f;
	m_fDissolveAmountClaw = 0.f;
	m_bDissolve = false;

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Attack -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
#endif // _DEBUG
}

void CCorvusState_ClawAttackAway::OnStateEnd()
{
	__super::OnStateEnd();
	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Corvus_ClawChargeAtk_FollowParticle", m_iEffectIndex);
	//Disable_Weapons();

	m_IsNextAttack = false;
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_ClawAttackAway::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -=
		bind(&CCorvusState_ClawAttackAway::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_ClawAttackAway::OnEventMessage(_uint iArg)
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

void CCorvusState_ClawAttackAway::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ClawAttackAway::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawAttackAway::Free()
{

}

_bool CCorvusState_ClawAttackAway::Check_AndChangeNextState()
{
	if (!CPlayerStateBase::Check_Requirement())
		return false;

	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		if (Check_RequirementAVoidState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		if (Check_RequirementParryState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		if (Check_RequirementAttackState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();
			return true;
		}
	}

	return false;
}

_bool CCorvusState_ClawAttackAway::Check_RequirementNextAttackState()
{
	_uint iTargetKeyFrameFirst = 15;
	_uint iTargetKeyFrameSecond = 50;

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameFirst, iTargetKeyFrameSecond) && m_IsNextAttack)
		return true;

	return false;
}

_bool CCorvusState_ClawAttackAway::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 80;

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsNextAttack)
		return true;

	return false;
}
