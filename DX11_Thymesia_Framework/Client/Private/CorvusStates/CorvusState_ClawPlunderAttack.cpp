#include "stdafx.h"
#include "CorvusStates/CorvusState_ClawPlunderAttack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXController.h"
#include "Weapon.h"
#include "Player.h"
#include "Corvus.h"
#include "PlayerSkill_System.h"
#include "UIManager.h"


GAMECLASS_C(CCorvusState_ClawPlunderAttack);
CLONE_C(CCorvusState_ClawPlunderAttack, CComponent)

HRESULT CCorvusState_ClawPlunderAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_ClawPlunderAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_ClawPlunderAttack::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_Raven_ClawLong_PlunderAttack2TTT");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ClawPlunderAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawPlunderAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	
	//75정도에 값을  가져올 수 있도록 해야함.
	//Mons
	

	//DISSOLVE_DESC	ArmDissolveDesc;
	//ZeroMemory(&ArmDissolveDesc, sizeof(DISSOLVE_DESC));
	//DISSOLVE_DESC	ClawDissolveDesc;
	//ZeroMemory(&ClawDissolveDesc, sizeof(DISSOLVE_DESC));
	//
	//if (m_bDissolve)
	//{
	//	
	//	if (m_bDissolveAppear)
	//	{
	//		if (0.6f > m_fDissolveTimeArm)
	//		{
	//			m_fDissolveTimeClaw -= fTimeDelta;
	//			m_fDissolveAmountClaw = SMath::Lerp(0.f, 1.f, m_fDissolveTimeClaw / 0.7f);
	//			m_vDissolveDir = { -1.f,0.f,0.f };
	//
	//			cout << "m_fDissolveAmountClaw : " << m_fDissolveAmountClaw << endl;
	//		}
	//		
	//			m_fDissolveTimeArm -= fTimeDelta;
	//			m_fDissolveAmountArm = SMath::Lerp(0.f, 1.f, m_fDissolveTimeArm / 0.7f);
	//			m_vDissolveDir = { -1.f,0.f,0.f };
	//
	//			cout << "m_fDissolveAmountArm : " << m_fDissolveAmountArm << endl;
	//
	//		
	//
	//	}
	//	else
	//	{
	//
	//		if(0.6f > m_fDissolveTimeClaw)
	//		{
	//			m_fDissolveTimeArm -= fTimeDelta;
	//			m_fDissolveAmountArm = SMath::Lerp(1.f, 0.f, m_fDissolveTimeArm / 0.7f);
	//			m_vDissolveDir = { 1.f,0.f,0.f };
	//		}
	//		
	//			m_fDissolveTimeClaw -= fTimeDelta;
	//			m_fDissolveAmountClaw = SMath::Lerp(1.f, 0.f, m_fDissolveTimeClaw / 0.7f);
	//			m_vDissolveDir = { 1.f,0.f,0.f };
	//		
	//	}
	//}
	//
	//ArmDissolveDesc.bBloom = true;
	//ArmDissolveDesc.bGlow = true;
	//ArmDissolveDesc.fAmount = m_fDissolveAmountArm;
	//ArmDissolveDesc.vDirection= m_vDissolveDir;
	//ArmDissolveDesc.vGlowColor = { 0.f, 1.f, 0.7f, 1.f };
	//ArmDissolveDesc.vStartPos = { -3.f,0.f,0.f };
	//
	//ClawDissolveDesc.bBloom = true;
	//ClawDissolveDesc.bGlow = true;
	//ClawDissolveDesc.fAmount = m_fDissolveAmountClaw;
	//ClawDissolveDesc.vDirection = m_vDissolveDir;
	//ClawDissolveDesc.vGlowColor = { 0.f, 1.f, 0.7f, 1.f };
	//ClawDissolveDesc.vStartPos = { -3.f,0.f,0.f };
	//
	//
	//Get_OwnerPlayer()->Set_DissolveAmount(5, ClawDissolveDesc);
	//Get_OwnerPlayer()->Set_DissolveAmount(9, ArmDissolveDesc);
	//


	Attack();
}

void CCorvusState_ClawPlunderAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();

	Check_AndChangeNextState();
}

void CCorvusState_ClawPlunderAttack::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_ClawPlunderAttack::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

}

void CCorvusState_ClawPlunderAttack::Attack()
{

}

void CCorvusState_ClawPlunderAttack::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	m_IsNextAttack = true;

}



void CCorvusState_ClawPlunderAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	if (!m_pModelCom.lock().get())
	{

		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
	m_bStealCompleteCurrentState = false;
	//m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	//m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += 
	//	bind(&CCorvusState_ClawPlunderAttack::Call_NextAnimationKey, this, placeholders::_1);
	//
	//m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
	//	bind(&CCorvusState_ClawPlunderAttack::Call_OtherControllerHit, this, placeholders::_1);

	//m_iAttackIndex = 7;
	//m_iEndAttackEffectIndex = -1
	//Disable_Weapons();

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Attack -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_ClawPlunderAttack::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	//m_IsNextAttack = false;
	//m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -= 
	//	bind(&CCorvusState_ClawPlunderAttack::Call_NextAnimationKey, this, placeholders::_1);
	//
	//m_pPhysXControllerCom.lock()->Callback_ControllerHit -=
	//	bind(&CCorvusState_ClawPlunderAttack::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_ClawPlunderAttack::OnEventMessage(_uint iArg)
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

void CCorvusState_ClawPlunderAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ClawPlunderAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ClawPlunderAttack::Free()
{

}

_bool CCorvusState_ClawPlunderAttack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
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



	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.9f)
	{
		weak_ptr<CPlayer> pPlayer = Weak_Cast<CPlayer>(m_pOwner);
		list<weak_ptr<CWeapon>>	pWeapons = pPlayer.lock()->Get_Weapon();

		pWeapons.front().lock()->Set_RenderOnOff(true);
	}


	



	return false;
}

_bool CCorvusState_ClawPlunderAttack::Check_RequirementNextAttackState()
{

	_uint iTargetKeyFrameFirst = 15;
	_uint iTargetKeyFrameSecond = 50;

	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameFirst, iTargetKeyFrameSecond) && m_IsNextAttack)
	{
		return true;
	}

	return false;
}

_bool CCorvusState_ClawPlunderAttack::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 80;



	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsNextAttack)
	{
		return true;
	}



	return false;
}

