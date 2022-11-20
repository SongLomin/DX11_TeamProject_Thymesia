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

	switch (m_eNorMonType)
	{
	case Client::NORMONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_RunF1|BaseLayer|Armature|Arm");
		break;
	case Client::NORMONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_RunRoot");
		break;
	case Client::NORMONSTERTYPE::SKULL:
		break;
	case Client::NORMONSTERTYPE::GARDENER:
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

	//5����ã��ʹ� 90�� ������5���Ѵ� �׷� 18�̳��´� ����ã������� 5���̴� �׷��� 90-5 85 85 �� 5 7


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


	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�
	_float fMToMDistance = GetStartPositionToCurrentPositionDir(); // ���ͽ�ŸƮ�����ǰ� �������� ������ ������ �Ÿ�

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


	if (fPToMDistance < 1.f && m_bClosePlayer) //6���� ������ �����ϰų� �¿�Ʒ��� �ӽñ�ο����δ�  �̋� �׾ִϸ��̼� �ٽ� �Ÿ��Ի��ϰ� ���� �ϴ°ɷ� �ϰԱ�
	{
		_int iMovRand = rand() % 3;

		if (!m_bRunCheck)
		{
			switch (m_eNorMonType)
			{
			case Client::NORMONSTERTYPE::AXEMAN:
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
			case Client::NORMONSTERTYPE::KNIFEWOMAN:
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
			case Client::NORMONSTERTYPE::SKULL:
				break;
			case Client::NORMONSTERTYPE::GARDENER:
				break;
			}
			return true;
		}
		
	}
	//
	if (fPToMDistance <= 1.5f && m_bClosePlayer) //6���� ������ �����ϰų� �¿�Ʒ��� �ӽñ�ο����δ�  �̋� �׾ִϸ��̼� �ٽ� �Ÿ��Ի��ϰ� ���� �ϴ°ɷ� �ϰԱ�
	{
		if (!m_bRunCheck)
		{
			_int iMovRand = rand() % 2;
			switch (m_eNorMonType)
			{
			case Client::NORMONSTERTYPE::AXEMAN:
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
			case Client::NORMONSTERTYPE::KNIFEWOMAN:
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
			    case Client::NORMONSTERTYPE::SKULL:
			    	break;
			    case Client::NORMONSTERTYPE::GARDENER:
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

			switch (m_eNorMonType)
			{
			case Client::NORMONSTERTYPE::AXEMAN:
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
			case Client::NORMONSTERTYPE::KNIFEWOMAN:
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
			case Client::NORMONSTERTYPE::SKULL:
				break;
			case Client::NORMONSTERTYPE::GARDENER:
				break;
			}
			return true;
		}
	

		

	
	}




	return false;
}

