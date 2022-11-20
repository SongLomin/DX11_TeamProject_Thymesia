#include "stdafx.h"
#include "NorMonState/NorMonState_Run.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Run);
CLONE_C(CNorMonState_Run, CComponent)

void CNorMonState_Run::Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE)
{
	m_eNorMonIdleType = IDLETYPE;
}

HRESULT CNorMonState_Run::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Run::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Run::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_RunF1|BaseLayer|Armature|Arm");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_RunRoot");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Run::Call_AnimationEnd, this);
}

void CNorMonState_Run::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	m_pTransformCom.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta, m_pNaviCom);
}

void CNorMonState_Run::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bClosePlayer)
		Turn_ToThePlayer(fTimeDelta * 2.f);

	//5도를찾고싶다 90을 나누기5를한다 그럼 18이나온다 내가찾고싶은건 5도이다 그러면 90-5 85 85 을 5 7


	if(!m_bClosePlayer)
		StartPositonLookAt(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Run::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunRUNRUNRUNRUN -> RunRUNRUNRUN" << endl;
#endif
#endif


}

void CNorMonState_Run::OnStateEnd()
{
	__super::OnStateEnd();


}



void CNorMonState_Run::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;


	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Run::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Run::Call_AnimationEnd, this);
}

void CNorMonState_Run::Free()
{

}

_bool CNorMonState_Run::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리
	_float fMToMDistance = GetStartPositionToCurrentPositionDir(); // 몬스터스타트포지션과 몬스터현재 포지션 사이의 거리

	if (!m_bClosePlayer && fMToMDistance <= 0.1f)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_ClosePlayerCheck(false);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_CloseToRun(false);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_IdleType(0);
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}

	if (!m_bClosePlayer && fPToMDistance <= 3.f)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_CloseToRun(true);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_ClosePlayerCheck(true);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_IdleType(2);
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}


	if (fPToMDistance < 1.f && m_bClosePlayer) //6보다 작을떄 공격하거나 좌우아래옆 머시기로움직인다  이떄 그애니메이션 다시 거리게산하고 공격 하는걸로 하게금
	{
		_int iMovRand = rand() % 3;

		if (!m_bRunCheck)
		{
			switch (m_eMonType)
			{
			case Client::MONSTERTYPE::AXEMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_B>(0.05f);
					m_bRunCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_BL>(0.05f);
					m_bRunCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_BR>(0.05f);
					m_bRunCheck = true;
					break;
				}
				break;
			case Client::MONSTERTYPE::KNIFEWOMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_B>(0.05f);
					m_bRunCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_BL>(0.05f);
					m_bRunCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_BR>(0.05f);
					m_bRunCheck = true;
					break;
				}
				break;
			case Client::MONSTERTYPE::SKULL:
				break;
			case Client::MONSTERTYPE::GARDENER:
				break;
			}
			return true;
		}
		
	}
	//
	if (fPToMDistance <= 1.5f && m_bClosePlayer) //6보다 작을떄 공격하거나 좌우아래옆 머시기로움직인다  이떄 그애니메이션 다시 거리게산하고 공격 하는걸로 하게금
	{
		if (!m_bRunCheck)
		{
			_int iMovRand = rand() % 2;
			switch (m_eMonType)
			{
			case Client::MONSTERTYPE::AXEMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					m_bRunCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					m_bRunCheck = true;
					break;
				}
				break;
			case Client::MONSTERTYPE::KNIFEWOMAN:
				switch (iMovRand)
			    {
			    case 0:
			    	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
			    	m_bRunCheck = true;
			    	break;
			    case 1:
			    	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
			    	m_bRunCheck = true;
			    	break;
			    }
			    	break;
			    case Client::MONSTERTYPE::SKULL:
			    	break;
			    case Client::MONSTERTYPE::GARDENER:
			    	break;
			    }
			    return true;
		}	
	}

	if (fPToMDistance <= 1.5f  && m_bRunCheck && m_bClosePlayer)
	{

		if (m_bRunCheck)
		{
			_int iAttRand = rand() % 5;

			switch (m_eMonType)
			{
			case Client::MONSTERTYPE::AXEMAN:
				switch (iAttRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					m_bRunCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
					m_bRunCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					m_bRunCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
					m_bRunCheck = false;
					break;
				case 4:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
					m_bRunCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::KNIFEWOMAN:
				switch (iAttRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					m_bRunCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
					m_bRunCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					m_bRunCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
					m_bRunCheck = false;
					break;
				case 4:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
					m_bRunCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::SKULL:
				break;
			case Client::MONSTERTYPE::GARDENER:
				break;
			}
			return true;
		}
	

		

	
	}




	return false;
}

