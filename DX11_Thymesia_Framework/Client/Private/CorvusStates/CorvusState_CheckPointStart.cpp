#include "stdafx.h"
#include "CorvusStates/CorvusState_CheckPointStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "UIManager.h"


GAMECLASS_C(CCorvusState_CheckPointStart);
CLONE_C(CCorvusState_CheckPointStart, CComponent)

HRESULT CCorvusState_CheckPointStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_CheckPointStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	
	return S_OK;
}

void CCorvusState_CheckPointStart::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_ArchiveSitStart");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_CheckPointStart::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_CheckPointStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_CheckPointStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_CheckPointStart::OnDisable()
{
	
}

void CCorvusState_CheckPointStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

}

void CCorvusState_CheckPointStart::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_CheckPointStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_CheckPointLoop>();
	/*
		여기서 처리해줘야함.
	
	*/
	GET_SINGLE(CUIManager)->OnEnterEvolveMenu();

}

void CCorvusState_CheckPointStart::Free()
{
	
}

void CCorvusState_CheckPointStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_CheckPointStart::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_CheckPointStart::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (KEY_INPUT(KEY::E, KEY_STATE::AWAY))
	{
		return true;
	}
	
	return false;
}

