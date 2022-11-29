#include "stdafx.h"
#include "CorvusStates/CorvusState_Climb_L_Down.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_Climb_L_Down);
CLONE_C(CCorvusState_Climb_L_Down, CComponent)

HRESULT CCorvusState_Climb_L_Down::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Climb_L_Down::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Climb_L_Down::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Corvus.ao|Corvus_SD_Ladder_Climb_L_Down");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Climb_L_Down::Call_AnimationEnd, this);
}

void CCorvusState_Climb_L_Down::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Climb_L_Down::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Climb_L_Down::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Climb_L_Down::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Stop -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Climb_L_Down::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_Climb_L_Down::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Climb_L_Down::Call_AnimationEnd, this);
}

void CCorvusState_Climb_L_Down::Free()
{
	
}

_bool CCorvusState_Climb_L_Down::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}

