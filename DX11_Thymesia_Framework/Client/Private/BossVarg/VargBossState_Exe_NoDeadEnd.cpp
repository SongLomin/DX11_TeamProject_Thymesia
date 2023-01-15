#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_NoDeadEnd.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "Status_Boss.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_Exe_NoDeadEnd);
CLONE_C(CVargBossState_Exe_NoDeadEnd, CComponent)

HRESULT CVargBossState_Exe_NoDeadEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Exe_NoDeadEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Exe_NoDeadEnd::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_NoDeadEnd");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Exe_NoDeadEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_NoDeadEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Exe_NoDeadEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Exe_NoDeadEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,5);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CVargBossState_Exe_NoDeadEnd::Call_NextKeyFrame, this, placeholders::_1);

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CVargBossState_Exe_NoDeadEnd::Call_NextKeyFrame, this, placeholders::_1);

	if (Get_OwnerMonster()->Get_BossExecutionStartOnOff())
	{
		PxControllerFilters Filters;
		_matrix                    vDoorOpenPlayerMatrix = Get_Owner().lock()->Get_Component<CVargBossState_Start>().lock()->Get_PlayerTransform();
		m_pPhysXControllerCom.lock()->Set_Position(
			XMVectorSet(41.5f, 0.09f, 40.05f, 1.f),
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		Get_OwnerCharacter().lock()->Get_Transform()->Set_Look2D(-vDoorOpenPlayerMatrix.r[2]);

		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
		weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);


		_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
		vOtherWorldMatrix.r[3] = XMVectorSet(41.5f, 0.09f, 40.05f, 1.f);
		_matrix                    vResultOtherWorldMatrix;
		vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, 1.25f, 0.f));
		pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
			vResultOtherWorldMatrix.r[3],
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		pOtherCharacter.lock()->Get_Transform()->Set_Look2D(-vOtherWorldMatrix.r[2]);
		Get_OwnerMonster()->Set_BossExecutionStartOnOff(false);
	}

#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_SitDown -> OnStateStart" << endl;
#endif
}

void CVargBossState_Exe_NoDeadEnd::OnStateEnd()
{
	__super::OnStateEnd();


}



void CVargBossState_Exe_NoDeadEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CVargBossState_Exe_NoDeadEnd::Call_NextKeyFrame, this, placeholders::_1);


	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_SPA_Roar_Getup>(0.05f);
}

void CVargBossState_Exe_NoDeadEnd::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_bDeadChoice = false;
	}
}

void CVargBossState_Exe_NoDeadEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Exe_NoDeadEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Exe_NoDeadEnd::Free()
{

}

_bool CVargBossState_Exe_NoDeadEnd::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 40)
	{
		if (m_bDeadChoice)
		{
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Exe_Start>(0.05f);
			return true;
		}
	}

	return false;
}

