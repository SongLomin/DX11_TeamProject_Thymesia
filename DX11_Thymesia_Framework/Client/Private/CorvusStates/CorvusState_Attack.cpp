#include "stdafx.h"
#include "CorvusStates/CorvusState_Attack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"


GAMECLASS_C(CCorvusState_Attack);
CLONE_C(CCorvusState_Attack, CComponent)

HRESULT CCorvusState_Attack::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CCorvusState_Attack::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	//m_vCurrentIndex[0].push_back("Corvus_SD_LAttack1");
	//m_vCurrentIndex[1].push_back("Corvus_SD_LAttack2");
	//m_vCurrentIndex[2].push_back("Corvus_SD_LAttack3_New");


	return S_OK;
}

void CCorvusState_Attack::Start()
{
	__super::Start();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Attack::Call_AnimationEnd, this);
}

void CCorvusState_Attack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);

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

void CCorvusState_Attack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputNextAttack();

	if (Check_AndChangeNextState())
	{
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	}
}

void CCorvusState_Attack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	
	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
	
}

void CCorvusState_Attack::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{

	m_vCurrentIndex = In_iAttackIndex;

	if (6 == m_iAttackIndex)
	{
		m_iAttackIndex = 2;
	}

	m_pModelCom.lock()->Set_AnimationSpeed(m_fDebugAnimationSpeed);

	cout << "AttackIndex: " << m_iAttackIndex << endl;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAttackIndex);
}

void CCorvusState_Attack::Attack()
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

void CCorvusState_Attack::Check_InputNextAttack()
{
	if (!KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		return;
	}

	switch (m_iAttackIndex)
	{
	case 0:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}

		break;

	case 1:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}

		break;

	case 2:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}
		break;

	case 3:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}
		break;

	case 5:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsNextAttack = true;
		}
		break;
	}

}

void CCorvusState_Attack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_LAttack1");
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_iAttackIndex = 0;
	//m_iEndAttackEffectIndex = -1;
	

	//Disable_Weapons();


	

#ifdef _DEBUG
	cout << "LuxiyaState: Attack -> OnStateStart" << endl;

#endif
}

void CCorvusState_Attack::OnStateEnd()
{
	__super::OnStateEnd();

	//Disable_Weapons();
	m_IsNextAttack = false;
	m_iAttackIndex = 0;
}

void CCorvusState_Attack::OnEventMessage(_uint iArg)
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

void CCorvusState_Attack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Attack::Call_AnimationEnd, this);
}

void CCorvusState_Attack::Free()
{
	
}

_bool CCorvusState_Attack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.9f)
	{

		if (Check_RequirementAttackState())
		{
			if (!Rotation_InputToLookDir())
				Rotation_NearToLookDir();

			Get_OwnerPlayer()->Change_State<CCorvusState_Attack>(0.05f);
			Get_OwnerPlayer()->Get_Component<CCorvusState_Attack>().lock()->Play_AttackWithIndex(0);
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
	//if (Check_RequirementNextAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_TargetToLookDir();
	//
	//	Disable_Weapons();
	//
	//	m_IsNextAttack = false;
	//	
	//	Play_AttackWithIndex(m_iAttackIndex + 1);
	//
	//	//스테이트 자체가 변하지 않았기 때문에 false다.
	//	return false;
	//}
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

_bool CCorvusState_Attack::Check_RequirementNextAttackState()
{
	_uint iTargetKeyFrame = 999;

	switch (m_cAttackIndex)
	{
	case 0:
		iTargetKeyFrame = 28;
		break;

	case 1:
		iTargetKeyFrame = 31;
		break;

	case 2:
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

