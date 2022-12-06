#include "stdafx.h"
#include "CorvusStates/CorvusState_Climb_R_Down_End.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXCharacterController.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CCorvusState_Climb_R_Down_End);
CLONE_C(CCorvusState_Climb_R_Down_End, CComponent)

HRESULT CCorvusState_Climb_R_Down_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Climb_R_Down_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Climb_R_Down_End::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_R_Down_End");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Climb_R_Down_End::Call_AnimationEnd, this);
}

void CCorvusState_Climb_R_Down_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Climb_R_Down_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Climb_R_Down_End::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Climb_R_Down_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Y | (_byte)ROOTNODE_FLAG::Z);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Stop -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Climb_R_Down_End::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Z);
}

void CCorvusState_Climb_R_Down_End::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Climb_R_Down_End::Call_AnimationEnd, this);
}

void CCorvusState_Climb_R_Down_End::Free()
{
	
}

_bool CCorvusState_Climb_R_Down_End::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	//애니메이션이 끝나면 피직스키고 아이들로돌아가라

	return false;
}

