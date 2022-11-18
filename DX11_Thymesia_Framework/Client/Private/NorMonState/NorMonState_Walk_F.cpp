#include "stdafx.h"
#include "NorMonState/NorMonState_Walk_F.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Walk_F);
CLONE_C(CNorMonState_Walk_F, CComponent)

HRESULT CNorMonState_Walk_F::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Walk_F::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Walk_F::Start()
{
	__super::Start();

	switch (m_eNorMonType)
	{
	case Client::NORMONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::NORMONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|Walk_F|SK_C_LV1Vill");
		break;
	case Client::NORMONSTERTYPE::SKULL:
		break;
	case Client::NORMONSTERTYPE::GARDENER:
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Walk_F::Call_AnimationEnd, this);
}

void CNorMonState_Walk_F::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	m_pTransformCom.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta, m_pNaviCom);
}

void CNorMonState_Walk_F::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Walk_F::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: RunStart -> Walk_F" << endl;
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

	
}

void CNorMonState_Walk_F::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CNorMonState_Walk_F::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Walk_F::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Walk_F::Call_AnimationEnd, this);
}

void CNorMonState_Walk_F::Free()
{

}

_bool CNorMonState_Walk_F::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fDistance = Get_DistanceWithPlayer();

	if (fDistance < 1.f) //6보다 작을떄 공격하거나 좌우아래옆 머시기로움직인다  이떄 그애니메이션 다시 거리게산하고 공격 하는걸로 하게금
	{


		if (m_eNorMonType == NORMONSTERTYPE::AXEMAN && !m_bWalkCheck)
		{
			_int iMovRand = rand() % 7;

			switch (iMovRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
				m_bWalkCheck = true;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
				m_bWalkCheck = true;
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_B>(0.05f);
				m_bWalkCheck = true;
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
				m_bWalkCheck = false;
				break;
			}
			return true;

		}
	}

	if (fDistance < 3.f && m_bWalkCheck)
	{

		if (m_eNorMonType == NORMONSTERTYPE::AXEMAN && m_bWalkCheck)
		{
			_int iAttRand = rand() % 4;

			switch (iAttRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
				m_bWalkCheck = false;
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
				m_bWalkCheck = false;
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
				m_bWalkCheck = false;
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
				m_bWalkCheck = false;
				break;
			}
			return true;
		}


	}


	return false;
}

