#include "stdafx.h"
#include "CorvusStates/CorvusState_CheckPointEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_CheckPointEnd);
CLONE_C(CCorvusState_CheckPointEnd, CComponent)

HRESULT CCorvusState_CheckPointEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_CheckPointEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_CheckPointEnd::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_ArchiveGetup");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_CheckPointEnd::Call_AnimationEnd, this);
}

void CCorvusState_CheckPointEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_CheckPointEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_CheckPointEnd::OnDisable()
{

}

void CCorvusState_CheckPointEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

}

void CCorvusState_CheckPointEnd::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_CheckPointEnd::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_CheckPointEnd::Free()
{
	
}

void CCorvusState_CheckPointEnd::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_CheckPointEnd::Call_AnimationEnd, this);
}

_bool CCorvusState_CheckPointEnd::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}

