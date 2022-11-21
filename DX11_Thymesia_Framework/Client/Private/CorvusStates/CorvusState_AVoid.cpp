#include "stdafx.h"
#include "CorvusStates/CorvusState_AVoid.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_AVoid);
CLONE_C(CCorvusState_AVoid, CComponent)

HRESULT CCorvusState_AVoid::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_AVoid::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAvoidIndex = 131;
	return S_OK;
}

void CCorvusState_AVoid::Start()
{
	__super::Start();
	
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Corvus.ao|Corvus_SD_AvoidF");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_AVoid::Call_AnimationEnd, this);
}

void CCorvusState_AVoid::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_AVoid::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputAgainAvoid();

	Check_AndChangeNextState();
}

void CCorvusState_AVoid::Check_InputAgainAvoid()
{
	if (!KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}

	switch (m_iAvoidIndex)
	{
	case 131:
		if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(3, 999))
		{
			m_IsAgainAvoid = true;
		}
		break;



	}
}

void CCorvusState_AVoid::OnDisable()
{

}

void CCorvusState_AVoid::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(3.f);
}

void CCorvusState_AVoid::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CCorvusState_AVoid::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_AVoid::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	m_pModelCom.lock()->Set_AnimationSpeed(m_fDebugAnimationSpeed);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAvoidIndex);
	m_pModelCom.lock()->Set_AnimationSpeed(3.f);
}

void CCorvusState_AVoid::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_AVoid::Call_AnimationEnd, this);
}

_bool CCorvusState_AVoid::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (Check_RuquireMnetRepeatAvoidkState())
	{
		if (KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
		{

			if (!Rotation_InputToLookDir())
				Rotation_TargetToLookDir();

			m_IsAgainAvoid = false;
			Play_AttackWithIndex(m_iAvoidIndex);
			return false;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.23f)
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

_bool CCorvusState_AVoid::Check_RuquireMnetRepeatAvoidkState()
{
	_uint iTargetKeyFrameMin = 999;
	_uint iTargetKeyFrameMax = 999;



	switch (m_iAvoidIndex)
	{
	case 131:
		iTargetKeyFrameMin = 35;
		iTargetKeyFrameMax = 80;
		break;

	}


	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) == true
		&& m_IsAgainAvoid)
	{
		return true;
	}



	return false;
}

