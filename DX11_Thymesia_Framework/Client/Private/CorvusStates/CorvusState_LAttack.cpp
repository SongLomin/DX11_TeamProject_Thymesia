#include "stdafx.h"
#include "CorvusStates/CorvusState_LAttack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"


GAMECLASS_C(CCorvusState_LAttack);
CLONE_C(CCorvusState_LAttack, CComponent)

HRESULT CCorvusState_LAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_LAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_iAttackIndex = 7;
	return S_OK;
}

void CCorvusState_LAttack::Start()
{
	__super::Start();
	//m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_LAttack1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_LAttack::Call_AnimationEnd, this);
}

void CCorvusState_LAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	if (KEY_INPUT(KEY::N, KEY_STATE::TAP))
	{
		if (m_fDebugAnimationSpeed < 0.5f)
		{
			m_fDebugAnimationSpeed = 1.f;
		}

		else
		{
			m_fDebugAnimationSpeed = 0.1f;
		}
	}

	Attack();
}

void CCorvusState_LAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();

	if (Check_AndChangeNextState())
	{
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	}
}

void CCorvusState_LAttack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	
	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
	
}

void CCorvusState_LAttack::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{

	m_iAttackIndex = In_iAttackIndex;
	
	if (10 == m_iAttackIndex)
	{
		m_iAttackIndex = 7;
	}

	m_pModelCom.lock()->Set_AnimationSpeed(m_fDebugAnimationSpeed);

	cout << "AttackIndex: " << m_iAttackIndex << endl;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAttackIndex);
	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
}

void CCorvusState_LAttack::Attack()
{
#ifdef _DEBUG
	if (GAMEINSTANCE->Is_Debug())
	{
		tstring szDebugText;
		szDebugText += TEXT("Current Animation Index: ");
		szDebugText += to_wstring(m_pModelCom.lock()->Get_CurrentAnimationIndex());
		szDebugText += TEXT(", Current KeyFrame: ");
		szDebugText += to_wstring(m_pModelCom.lock()->Get_CurrentAnimationKeyIndex());
		_float2 vPosition(0.f, 3.f);
		_vector vColor = XMVectorSet(1.f, 1.f, 0.f, 1.f);

		GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, szDebugText, vPosition, vColor, false);
	}
#endif // _DEBUG

}

void CCorvusState_LAttack::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	switch (m_iAttackIndex)
	{
	case 7:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}

		break;

	case 8:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}

		break;

	case 9:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}
		break;

	//case 3:
	//	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
	//	{
	//		m_IsNextAttack = true;
	//	}
	//	break;
	//
	//case 5:
	//	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
	//	{
	//		m_IsNextAttack = true;
	//	}
	//	break;
	}

}



void CCorvusState_LAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pOwnerFromPlayer.lock()->Get_PreState().lock()->Get_StateIndex() == 11)
	{
		m_iAttackIndex = 8;
	}
	int i = 0;
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAttackIndex);

	if (!m_pModelCom.lock().get())
	{
		
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

	//m_iAttackIndex = 7;
	//m_iEndAttackEffectIndex = -1;
	

	//Disable_Weapons();


	

#ifdef _DEBUG
	cout << "LuxiyaState: Attack -> OnStateStart" << endl;

#endif
}

void CCorvusState_LAttack::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
	m_IsNextAttack = false;
	m_iAttackIndex = 7;
}

void CCorvusState_LAttack::OnEventMessage(_uint iArg)
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

void CCorvusState_LAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_LAttack::Call_AnimationEnd, this);
}

void CCorvusState_LAttack::Free()
{
	
}

_bool CCorvusState_LAttack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.6f)
	{
		if (Check_RequirementAttackState() && 10 == m_iAttackIndex)
		{
			if (Check_RequirementAttackState())
			{
				if (!Rotation_InputToLookDir())
					Rotation_NearToLookDir();

				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack>(0.05f);
				Get_OwnerPlayer()->Get_Component<CCorvusState_LAttack>().lock()->Play_AttackWithIndex(7);
				return true;
			}
		}
	}

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

	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
	//	return true;
	//}
	//
	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_UltimateSkill>();
	//	return true;
	//}
	//
	if (Check_RequirementNextAttackState())
	{
		if (!Rotation_InputToLookDir())
			Rotation_TargetToLookDir();
	
		//Disable_Weapons();
	
		m_IsNextAttack = false;
		
		Play_AttackWithIndex(m_iAttackIndex + 1);
	
		//스테이트 자체가 변하지 않았기 때문에 false다.
		return false;
	}

	if (Check_RuquireMnetFirstAttackState())
	{
		if (!Rotation_InputToLookDir())
			Rotation_TargetToLookDir();

		m_IsNextAttack = false;
		
		Play_AttackWithIndex(7);

		return false;
	}


	//
	//if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.33f)
	//{
	//
	//	if (Check_RequirementAttackState() && 4 == m_iAttackIndex)
	//	{
	//		if (!Rotation_InputToLookDir())
	//			Rotation_NearToLookDir();
	//
	//		Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>(0.05f);
	//		Get_OwnerPlayer()->Get_Component<CLuxiyaState_Attack>().lock()->Play_AttackWithIndex(0);
	//		return true;
	//	}
	//
	//	if (Check_RequirementRunState())
	//	{
	//		Rotation_InputToLookDir();
	//		Get_OwnerPlayer()->Change_State<CLuxiyaState_RunStart>(0.05f);
	//		return true;
	//	}
	//
	//	
	//}


	return false;
}

_bool CCorvusState_LAttack::Check_RequirementNextAttackState()
{
	_uint iTargetKeyFrame = 999;
	

	switch (m_iAttackIndex)
	{
	case 7:
		iTargetKeyFrame = 50;
	
		break;

	case 8:
		iTargetKeyFrame = 31;	
		break;

	case 9:
		iTargetKeyFrame = 37;
	
		break;

	//case 3:
	//	iTargetKeyFrame = 21;
	//	break;
	//
	//case 5:
	//	iTargetKeyFrame = 11;
	//	break;
	}


	if (m_pModelCom.lock()->Get_CurrentAnimationKeyIndex() == iTargetKeyFrame
		&& m_IsNextAttack)
	{
		return true;
	}



	return false;
}

_bool CCorvusState_LAttack::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 999;
	_uint iTargetKeyFrameMax = 999;



	switch (m_iAttackIndex)
	{
	case 7:
		iTargetKeyFrameMin = 51;
		iTargetKeyFrameMax = 80;

		break;

	case 8:
		iTargetKeyFrameMin = 32;
		iTargetKeyFrameMax = 62;

		break;

	case 9:
		iTargetKeyFrameMin = 37;
		iTargetKeyFrameMax = 62;
		break;

		//case 3:
		//	iTargetKeyFrame = 21;
		//	break;
		//
		//case 5:
		//	iTargetKeyFrame = 11;
		//	break;
	}


	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) == true
		&& m_IsNextAttack)
	{
		return true;
	}



	return false;
}

