#include "stdafx.h"
#include "NorMonState/NorMonState_Run.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateBase.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Run);
CLONE_C(CNorMonState_Run, CComponent)

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

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|DemoM02_RunF1|BaseLayer");
	}
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

	Check_AndChangeNextState();
}



void CNorMonState_Run::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_Run::OnStateEnd()
{
	__super::OnStateEnd();


}



void CNorMonState_Run::Free()
{

}

_bool CNorMonState_Run::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fDistance = Get_DistanceWithPlayer();

	if (fDistance > 5.f) //6보다 작을떄 공격하거나 좌우아래옆 머시기로움직인다  이떄 그애니메이션 다시 거리게산하고 공격 하는걸로 하게금
	{
		

		if (m_eNorMonType == NORMONSTERTYPE::AXEMAN && !m_bRunCheck)
		{
			_int iMovRand = rand() % 8;

			switch (iMovRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
				m_bRunCheck = true;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_FR>(0.05f);
				m_bRunCheck = true;
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_FL>(0.05f);
				m_bRunCheck = true;
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
				m_bRunCheck = true;
				break;
			case 4:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
				m_bRunCheck = true;
				break;
			case 5:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_B>(0.05f);
				m_bRunCheck = true;
				break;
			case 6:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_BL>(0.05f);
				m_bRunCheck = true;
				break;
			case 7:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_BR>(0.05f);
				m_bRunCheck = true;
				break;
			}
			return true;
		
		}

		if (m_eNorMonType == NORMONSTERTYPE::AXEMAN && m_bRunCheck)
		{
			_int iAttRand = rand() % 4;

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
			}
			return true;
		}

		if (m_eNorMonType == NORMONSTERTYPE::KNIFEWOMAN)
		{
			

		}
	}
	else
	{
		
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}

	return false;
}

