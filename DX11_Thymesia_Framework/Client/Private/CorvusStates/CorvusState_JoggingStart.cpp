#include "stdafx.h"
#include "CorvusStates/CorvusState_JoggingStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_JoggingStart);
CLONE_C(CCorvusState_JoggingStart, CComponent)

HRESULT CCorvusState_JoggingStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_JoggingStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}

void CCorvusState_JoggingStart::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_WalkF_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_JoggingStart::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_JoggingStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_JoggingStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_JoggingStart::OnDisable()
{
	
}

void CCorvusState_JoggingStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

}

void CCorvusState_JoggingStart::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_JoggingStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Jogging>();
	
}

void CCorvusState_JoggingStart::Free()
{
	
}

void CCorvusState_JoggingStart::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_JoggingStart::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_JoggingStart::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	
	if (!KEY_INPUT(KEY::W, KEY_STATE::HOLD)
		&& !KEY_INPUT(KEY::A, KEY_STATE::HOLD)
		&& !KEY_INPUT(KEY::S, KEY_STATE::HOLD)
		&& !KEY_INPUT(KEY::D, KEY_STATE::HOLD))
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStartEnd>();
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

