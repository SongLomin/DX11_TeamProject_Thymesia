#include "stdafx.h"
#include "CorvusStates/CorvusState_SprintAttack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"


GAMECLASS_C(CCorvusState_SprintAttack);
CLONE_C(CCorvusState_SprintAttack, CComponent)

HRESULT CCorvusState_SprintAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCorvusState_SprintAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	

	return S_OK;
}

void CCorvusState_SprintAttack::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Corvus.ao|Corvus_SD_SprintAttack_1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_SprintAttack::Call_AnimationEnd, this);
}

void CCorvusState_SprintAttack::Tick(_float fTimeDelta)
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

void CCorvusState_SprintAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();

	if (Check_AndChangeNextState())
	{
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	}
}

void CCorvusState_SprintAttack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}


void CCorvusState_SprintAttack::Attack()
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

void CCorvusState_SprintAttack::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	m_IsNextAttack = true;
	

}



void CCorvusState_SprintAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	if (!m_pModelCom.lock().get())
	{

		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}
	m_pModelCom.lock()->Set_AnimationSpeed(2.f);




#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Attack -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_SprintAttack::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	m_IsNextAttack = false;
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CCorvusState_SprintAttack::OnEventMessage(_uint iArg)
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

void CCorvusState_SprintAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_SprintAttack::Call_AnimationEnd, this);
}

void CCorvusState_SprintAttack::Free()
{

}

_bool CCorvusState_SprintAttack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	if(Check_RequirementNextAttackState())
	{
		if (Check_RequirementAttackState())
		{

			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_LAttack2>();

			return true;
		}
	}




	return false;
}

_bool CCorvusState_SprintAttack::Check_RequirementNextAttackState()
{
	_uint iTargetKeyFrameFirst = 17;
	_uint iTargetKeyFrameSecond = 50;



	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameFirst, iTargetKeyFrameSecond) 
&& m_IsNextAttack)
	{
		return true;
	}



	return false;
}

_bool CCorvusState_SprintAttack::Check_RuquireMnetFirstAttackState()
{
	_uint iTargetKeyFrameMin = 51;
	_uint iTargetKeyFrameMax = 80;






	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsNextAttack)
	{
		return true;
	}



	return false;
}

