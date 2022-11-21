#include "stdafx.h"
#include "NorMonState/NorMonState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Collider.h"
#include "Attack_Area.h"



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




	if (m_eMonType == MONSTERTYPE::AXEMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_Idle1|BaseLayer|Armature|Arm");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Sit_Idle|BaseLayer|Arm");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_SP_Idle1|BaseLayer|Arm");
			break;
		}
	}

	if (m_eMonType == MONSTERTYPE::KNIFEWOMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Sit_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_SP_Idle1");
			break;
		}
	}

	if (m_eMonType == MONSTERTYPE::GARDENER)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_Sit_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_SP_Idle1");
			break;
		}
	}


	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_HurtL>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_HurtR>().lock())
	{
		int iRand = rand() % 3 + 2;

		m_iCount += iRand;
	}



	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_LightAttack3>().lock())
	{
		m_iCount = 0;
		m_bGoAtk = false;
		m_bCloseToRun = true;
		m_iIdleType = 1;
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


	//TODO 지우면안돼요 화나요
	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_Awake>().lock())
	//{
	//	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_IdleGeneral|BaseLayer");
	//}
	//
	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_SitToIdle>().lock())
	//{
	//	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_IdleGeneral|BaseLayer");
	//}


	if (m_iCount >= 10)
	{
		m_bGoAtk = true;
	}

	Check_AndChangeNextState();

}


void CNorMonState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: IdleIDLEIDLE -> OnStateStart" << endl;
#endif
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

	if (fMToMDistance >= 20.f) // 거리20보다멀어지면 다른거 다끄고 돌아가게금 30보다멀어지면 트루줌
	{
		m_iIdleType = 3;
	}

	// 1. 아이들상태부터 검사  근데이떄가 거리가 가까워졌을대임 
	// 첫번째로아이들들어오면 무조건 이친구가 들어오게함 
	if (fPToMDistance <= 3.f && m_iIdleType == 0  )
	{

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
			switch (m_eNorMonIdleType)
			{
			case Client::NORMONSTERIDLETYPE::NORIDLE:
				TurnMechanism();
				break;
			case Client::NORMONSTERIDLETYPE::SITIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_SitToIdle>(0.05f);
				break;
			case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Awake>(0.05f);
				break;
			}
			break;
		case Client::MONSTERTYPE::KNIFEWOMAN:
			switch (m_eNorMonIdleType)
			{
			case Client::NORMONSTERIDLETYPE::NORIDLE:
				TurnMechanism();
				break;
			case Client::NORMONSTERIDLETYPE::SITIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_SitToIdle>(0.05f);
				break;
			case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Awake>(0.05f);
				break;
			}
			break;
		case Client::MONSTERTYPE::SKULL:
			break;
		case Client::MONSTERTYPE::GARDENER:
			switch (m_eNorMonIdleType)
			{
			case Client::NORMONSTERIDLETYPE::NORIDLE:
				TurnMechanism();
				break;
			case Client::NORMONSTERIDLETYPE::SITIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_SitToIdle>(0.05f);
				break;
			case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Awake>(0.05f);
				break;
			}
			break;
		}
		m_iIdleType = 1;
		m_bCloseToRun = true;
		return true;

	}

	// 2.재내들 끊나면 다시아이들로옴 어떤상태는 무조건 NORIDLE상태임
	// 아이들타입으로 들어왔고 이때 턴검사한번 더해야함 90보다 작은상태이거나
	// 90보다큰상태일떄 검사를해야하기에 아직 기본아이들도 룩앳이아님 
	// 기본아이들이면 각도를계산해 
	if (m_bCloseToRun && m_iIdleType == 1)
	{

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
			TurnMechanism();
			break;
		case Client::MONSTERTYPE::KNIFEWOMAN:
			TurnMechanism();
			break;
		case Client::MONSTERTYPE::SKULL:
			break;
		case Client::MONSTERTYPE::GARDENER:
			TurnMechanism();
			break;

		}


		m_iIdleType = 2;
		m_bClosePlayerCheck = true;
		return true;
	}
	//이러면 서있는거는 턴검사를했고 룩앳인상태고 나머지는 90보만큼돌았을거임 (그거보다작으면룩앳)
	//이제는 각도계산해서 90보다크면 다시턴검사하고 아니면 원래코드로가야됨

	if (fPToMDistance <= 3.f && m_iIdleType == 2 )
	{

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
			if (ComputeAngleWithPlayer() <= 0.f)
			{
				TurnMechanism();
			}
			else
			{
				Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
				m_bClosePlayerCheck = true;
				m_bCloseToRun = true;
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
		case Client::MONSTERTYPE::KNIFEWOMAN:
			if (ComputeAngleWithPlayer() <= 0.f)
			{
				TurnMechanism();
			}
			else
			{
				Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
				m_bClosePlayerCheck = true;
				m_bCloseToRun = true;
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
		case Client::MONSTERTYPE::SKULL:
			break;
		case Client::MONSTERTYPE::GARDENER:
			if (ComputeAngleWithPlayer() <= 0.f)
			{
				TurnMechanism();
			}
			else
			{
				Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
				m_bClosePlayerCheck = true;
				m_bCloseToRun = true;
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

	// 
	if (m_bCloseToRun && m_iIdleType == 2 && m_bClosePlayerCheck)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
		int iRunORWalk = rand() % 2;

		switch (m_eMonType)
		{
		case Client::MONSTERTYPE::AXEMAN:
			switch (iRunORWalk)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
				break;
			}
			break;
		case Client::MONSTERTYPE::KNIFEWOMAN:
			switch (iRunORWalk)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
				break;
			}
			break;
		case Client::MONSTERTYPE::SKULL:
			break;
		case Client::MONSTERTYPE::GARDENER:
			switch (iRunORWalk)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
				break;
			}
			break;
		}

		return true;
	}


	if (m_iIdleType == 3)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_B>(0.05f);
		return true;
	}


	if (m_bGoAtk)
	{
		
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
		return true;
	}

	return false;
}

