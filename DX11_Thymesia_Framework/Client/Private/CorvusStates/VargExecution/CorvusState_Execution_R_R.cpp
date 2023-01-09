#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_Execution_R_R.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CCorvusState_Execution_R_R);
CLONE_C(CCorvusState_Execution_R_R, CComponent)

HRESULT CCorvusState_Execution_R_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Execution_R_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Execution_R_R::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_StunExecute_StartR_R");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Execution_R_R::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Execution_R_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Execution_R_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Execution_R_R::OnDisable()
{

}

void CCorvusState_Execution_R_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


	//m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	//m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_Execution_R_R::Call_NextAnimationKey, this, placeholders::_1);


	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_Execution_R_R::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_Execution_R_R::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Execution_R_R::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;


}

void CCorvusState_Execution_R_R::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_Execution_R_R::Free()
{
	
}

void CCorvusState_Execution_R_R::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Execution_R_R::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Execution_R_R::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);


	if (EVENT_TYPE::ON_BIGHANDMANEXECUTION == (EVENT_TYPE)iArg)
	{
		eExeMonName = EXECUTIONMONSTERNAME::BIGHANDMAN;
	}

	if (EVENT_TYPE::ON_URDEXECUTON == (EVENT_TYPE)iArg)
	{
		eBossName = BOSSNAME::URD;
	}

	if ((_uint)EVENT_TYPE::ON_VARGEXECUTION == iArg)
	{
		eBossName = BOSSNAME::VARG;
	}

}

_bool CCorvusState_Execution_R_R::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 27)
	{
		
		if (eBossName != BOSSNAME::NAMEEND)
		{
			list<weak_ptr <CGameObject>> pBossMonsters = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::BOSSMONSTER);

			for (auto& elem : pBossMonsters)
			{
				elem.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_BOSS_EXECUTIONSTART);
			}
			switch (eBossName)
			{
			case Client::BOSSNAME::VARG:
				Get_OwnerPlayer()->Change_State<CCorvusState_Varg_Execution>();
				break;
			case Client::BOSSNAME::BAT:
				break;
			case Client::BOSSNAME::URD:
				Get_OwnerPlayer()->Change_State<CCorvusState_Urd_Execution>();
				break;
			}
		}

		if (eExeMonName != EXECUTIONMONSTERNAME::NAMEEND)
		{
			list<weak_ptr <CGameObject>> pBossMonsters = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::ELITEMONSTER);

			for (auto& elem : pBossMonsters)
			{
				elem.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_BIGHANDMANEXECUTION);
			}

			switch (eExeMonName)
			{
			case Client::EXECUTIONMONSTERNAME::BIGHANDMAN:
				Get_OwnerPlayer()->Change_State<CCorvusState_BigHandman_Execution>();
				break;
			case Client::EXECUTIONMONSTERNAME::ARMORMAN:
				break;
			}
		}
			
		return true;
	}

	return false;
}

