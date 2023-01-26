#include "stdafx.h"
#include "CorvusStates/CorvusState_ClawAttackHold.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXController.h"
#include "Weapon.h"


GAMECLASS_C(CCorvusState_ClawAttackHold);
CLONE_C(CCorvusState_ClawAttackHold, CComponent)

HRESULT CCorvusState_ClawAttackHold::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_ClawAttackHold::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_ClawAttackHold::Start()
{
	__super::Start();

	m_fDissolveAmountArm = 1.f;
	m_fDissolveAmountClaw = 1.f;

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_Raven_ClawCommonV2_ChargeStart");
	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ClawAttackHold::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawAttackHold::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	DISSOLVE_DESC	ArmDissolveDesc;
	ZeroMemory(&ArmDissolveDesc, sizeof(DISSOLVE_DESC));
	DISSOLVE_DESC	ClawDissolveDesc;
	ZeroMemory(&ClawDissolveDesc, sizeof(DISSOLVE_DESC));

	if (m_bDissolve)
	{

		
			if (0.7f > m_fDissolveTimeArm)
			{
				m_fDissolveTimeClaw -= fTimeDelta;
				m_fDissolveAmountClaw = SMath::Lerp(0.f, 1.f, m_fDissolveTimeClaw / 0.7f);
				m_vDissolveDir = { 1.f,0.f,0.f };

				cout << "m_fDissolveAmountClaw : " << m_fDissolveAmountClaw << endl;
			}
		
			m_fDissolveTimeArm -= fTimeDelta;
			m_fDissolveAmountArm = SMath::Lerp(0.f, 1.f, m_fDissolveTimeArm / 1.f);
			m_vDissolveDir = { 1.f,0.f,0.f };

			cout << "m_fDissolveAmountArm : " << m_fDissolveAmountArm << endl;	
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

void CCorvusState_ClawAttackHold::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();

	Check_AndChangeNextState();
}

//void CCorvusState_ClawAttackHold::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//	Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHoldLoop>();
//
//}

void CCorvusState_ClawAttackHold::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_ClawAttackHold::Attack()
{

}

void CCorvusState_ClawAttackHold::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	m_IsNextAttack = true;

}

void CCorvusState_ClawAttackHold::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 0:
		m_fDissolveTimeArm = 1.f;
		m_fDissolveTimeClaw = 0.7f;
		m_bDissolve = true;
		m_fDissolveAmountArm = 1.f;
		m_fDissolveAmountClaw = 1.f;
		break;
	case 20:
		GET_SINGLE(CGameManager)->Activate_Zoom(1.5f, 0.5f, EASING_TYPE::INOUT_BACK);
		return;
	case 33:
		_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 1.3f, 0.f, 0.f));
		GAMEINSTANCE->Set_RadialBlur(0.25f, vPosition);
		return;

	}
}


void CCorvusState_ClawAttackHold::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CPlayer> pPlayer = Weak_Cast<CPlayer>(m_pOwner);
	list<weak_ptr<CWeapon>>	pWeapons = pPlayer.lock()->Get_Weapon();

	pWeapons.front().lock()->Set_RenderOnOff(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	if (!m_pModelCom.lock().get())
	{

		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	

	//m_iAttackIndex = 7;
	//m_iEndAttackEffectIndex = -1;

	m_fDissolveAmountClaw = 1.f;
	m_fDissolveAmountArm = 1.f;
	m_bDissolve = false;

	//Disable_Weapons();


	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_ClawAttackHold::Call_NextKeyFrame, this, placeholders::_1);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Attack -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_ClawAttackHold::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	m_IsNextAttack = false;
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_ClawAttackHold::Call_NextKeyFrame, this, placeholders::_1);

}

void CCorvusState_ClawAttackHold::OnEventMessage(_uint iArg)
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

void CCorvusState_ClawAttackHold::OnDestroy()
{
	//m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ClawAttackHold::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawAttackHold::Free()
{

}

_bool CCorvusState_ClawAttackHold::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;




	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 30)
	{
		if (!Check_RequirementClawAttackHoldState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackAway>();
			return true;
		}
		
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() <= 30)
	{
		if (Check_RequirementClawAttackState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackTab>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.99f)
	{
		
	    Rotation_InputToLookDir();
	    Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHoldLoop>();
	    return true;
		
	}

	




	return false;
}

_bool CCorvusState_ClawAttackHold::Check_RequirementNextAttackState()
{

	_uint iTargetKeyFrameFirst = 15;
	_uint iTargetKeyFrameSecond = 50;



	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameFirst, iTargetKeyFrameSecond) && m_IsNextAttack)
	{
		return true;
	}

	return false;
}

_bool CCorvusState_ClawAttackHold::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 80;



	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsNextAttack)
	{
		return true;
	}



	return false;
}

