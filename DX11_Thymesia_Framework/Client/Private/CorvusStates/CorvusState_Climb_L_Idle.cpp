#include "stdafx.h"
#include "CorvusStates/CorvusState_Climb_L_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_Climb_L_Idle);
CLONE_C(CCorvusState_Climb_L_Idle, CComponent)

HRESULT CCorvusState_Climb_L_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Climb_L_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Climb_L_Idle::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_L_Idle");
	
}

void CCorvusState_Climb_L_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Climb_L_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CCorvusState_Climb_L_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 0, 0.05f);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Stop -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Climb_L_Idle::OnStateEnd()
{
	__super::OnStateEnd();
}


void CCorvusState_Climb_L_Idle::Free()
{
	
}

_bool CCorvusState_Climb_L_Idle::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	const _flag CollisionObjectFlags = Get_OwnerPlayer()->Get_CollisionObjectFlags();

	if (CollisionObjectFlags & (_flag)COLISIONOBJECT_FLAG::LADDERUP) //위쪽사다리랑충돌중이냐
	{
		if (Check_RequirementUpLadderState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_R_UP_End>();
			return true;
		}
	}
	else
	{
		if (Check_RequirementUpLadderState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_R_UP>();
			return true;
		}
	}


	//아래에서내려가는거

	if (CollisionObjectFlags & (_flag)COLISIONOBJECT_FLAG::LADDERDOWN)
	{
		if (Check_RequirementDownLadderState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_L_Down_End>();
			return true;
		}
	}
	else
	{
		if (Check_RequirementDownLadderState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Climb_L_Down>();
			return true;
		}
	}


	//위에서올라가는거
	
	//위에서내려가는거
	



	if (Check_RequirementObjectInteraction())
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_Fall_Start>();
		return true;
	}



	return false;
}

