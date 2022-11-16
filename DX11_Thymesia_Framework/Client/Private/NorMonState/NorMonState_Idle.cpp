#include "stdafx.h"
#include "NorMonState/NorMonState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateBase.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Idle);
CLONE_C(CNorMonState_Idle, CComponent)

void CNorMonState_Idle::Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE)
{
	m_eNorMonIdleType = IDLETYPE;
}

HRESULT CNorMonState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Idle::Start()
{
	__super::Start();


	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|LV1Villager_M_IdleGeneral|BaseLayer");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|LV1Villager_M_Sit_Idle|BaseLayer");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|LV1Villager_M_SP_Idle1|BaseLayer");
			break;
		}
	}

	if (m_eNorMonType == NORMONSTERTYPE::KNIFEWOMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("LV1Villager_F_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("DemoM02_Idle1");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("DemoM02_Idle1");
			break;
		}
	}

	
	
	

}

void CNorMonState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	
	Check_AndChangeNextState();



}



void CNorMonState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: Idle -> OnStateStart" << endl;
#endif


}

void CNorMonState_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}



void CNorMonState_Idle::Free()
{

}

_bool CNorMonState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리
	_float fMToMDistance = GetStartPositionToCurrentPositionDir(); // 몬스터스타트포지션과 몬스터현재 포지션 사이의 거리

	if (fMToMDistance >= 10.f) // 거리30보다멀어지면 다른거 다끄고 돌아가게금 30보다멀어지면 트루줌
	{
		m_bReturnStartPosition = true;
	
	}
	if (m_bReturnStartPosition && fMToMDistance <= 5.f)
	{
		m_bReturnStartPosition = false;
	}


	if (fPToMDistance <= 3.f && !m_bReturnStartPosition )
	{
		
		TurnMechanism();

		m_bFirstRun = true;
		m_bPlayerColision = true;
		Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
		if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
		{
			
			switch (m_eNorMonIdleType)
			{			
			case Client::NORMONSTERIDLETYPE::NORIDLE:
			{
				int iRunORWalk = rand() % 2;
				switch (iRunORWalk)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
					break;
				}
			}
				break;
			case Client::NORMONSTERIDLETYPE::SITIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_SitToIdle>(0.05f);
				break;
			case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Awake>(0.05f);
				break;
			}

			return true;

		}

		//if (m_eNorMonType == NORMONSTERTYPE::KNIFEWOMAN)
		//{
		//	if (m_eNorMonType == NORMONSTERTYPE::KNIFEWOMAN)
		//	{
		//		switch (m_eNorMonIdleType)
		//		{
		//		case Client::NORMONSTERIDLETYPE::NORIDLE:
		//			break;
		//		case Client::NORMONSTERIDLETYPE::SITIDLE:
		//			break;
		//		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
		//			break;
		//		}
		//	}
		//	
		//}
	}
	if (fPToMDistance > 3.f && !m_bReturnStartPosition && m_bPlayerColision)
	{
		if (m_eNorMonType == NORMONSTERTYPE::AXEMAN && m_bFirstRun)
		{
			TurnMechanism();

			switch (m_eNorMonIdleType)
			{
			case Client::NORMONSTERIDLETYPE::NORIDLE:
			{
				int iRunORWalk = rand() % 2;
				switch (iRunORWalk)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
					break;
				}
			}	
			break;				
			}
			
			return true;
		}
	}

	if (m_bReturnStartPosition && !m_bOneCheck)
	{
	 
		switch (m_eNorMonType)
		{
		case  NORMONSTERTYPE::AXEMAN:
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Stop>(0.05f);
			break;
		}
		Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(false);
		return true;
	}

	if (m_bReturnStartPosition && m_bOneCheck)
	{
		switch (m_eNorMonType)
		{
		case NORMONSTERTYPE::AXEMAN:
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
			break;
		}
		return true;
	}




	return false;
}

