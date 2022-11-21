#include "stdafx.h"
#include "CorvusStates/CorvusState_Hit.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_Hit);
CLONE_C(CCorvusState_Hit, CComponent)

HRESULT CCorvusState_Hit::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCorvusState_Hit::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 19;
	m_fFixedPlayRatio = 0.f;

	return S_OK;
}

void CCorvusState_Hit::Start()
{
	__super::Start();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Hit::Call_AnimationEnd, this);
}

void CCorvusState_Hit::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Hit::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Hit::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	_int iRandom = rand() % 3;

	switch (iRandom)
	{
	case 0:
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_OnHit.wav"), 1.f);
		break;

	case 1:
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_OnHit2.wav"), 1.f);
		break;

	case 2:
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_OnHit3.wav"), 1.f);
		break;
	}


#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Hit -> OnStateStart" << endl;
#endif
#endif
}

void CCorvusState_Hit::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_Hit::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
	
}

void CCorvusState_Hit::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Hit::Call_AnimationEnd, this);
}

void CCorvusState_Hit::Free()
{

}

_bool CCorvusState_Hit::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Dash>();
	//	return true;
	//}

	return false;
}



