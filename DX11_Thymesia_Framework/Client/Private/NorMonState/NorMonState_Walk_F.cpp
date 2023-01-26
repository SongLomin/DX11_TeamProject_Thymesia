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
#include "PhysXController.h"



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

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_WalkF");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkF");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkF");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_WalkF");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_WalkF");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_WalkF");
		break;

	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Walk_F::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Walk_F::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	//m_pTransformCom.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta, m_pNaviCom);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
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
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStart -> Walk_F" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(2.5f);
}

void CNorMonState_Walk_F::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CNorMonState_Walk_F::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_bWalkCheck = false;
		m_iGardnerAtkIndex = 0;
	}
}

void CNorMonState_Walk_F::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Walk_F::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Walk_F::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Walk_F::Free()
{

}

_bool CNorMonState_Walk_F::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fDistance = Get_DistanceWithPlayer();

	if (fDistance < 2.f && !m_bWalkCheck) //6보다 작을떄 공격하거나 좌우아래옆 머시기로움직인다  이떄 그애니메이션 다시 거리게산하고 공격 하는걸로 하게금
	{		
			_int iMovRand = rand() % 3;

			switch (m_eMonType)
			{
			case Client::MONSTERTYPE::AXEMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::KNIFEWOMAN:
			{
				_int iAttRand = rand() % 2;
				switch (iAttRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
			    }
				break;
			case Client::MONSTERTYPE::SKULL:
				break;
			case Client::MONSTERTYPE::GARDENER:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::ENHANCE_GARDENER:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::SHIELDAXEMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::SKULLSHIELDMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::SKULLSPEARMAN:
				switch (iMovRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::ARMORSHIELDMAN:
			{
				int iRand = rand() % 2;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				}
			}
				 
				break;
			case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
				{
				int iRand = rand() % 2;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				}
			}
				 
				break;
			case Client::MONSTERTYPE::ARMORSPEARMAN:
			{
				int iRand = rand() % 2;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				}
			}

			break;
			case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
			{
				int iRand = rand() % 2;

				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_L>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_R>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
					m_bWalkCheck = true;
					break;
				}
			}

			break;
			}
			return true;
	
		
	}

	if (fDistance < 2.f && m_bWalkCheck)
	{
		if (m_bWalkCheck)
		{
			_int iAttRand = rand() % 4;

			switch (m_eMonType)
			{
			case Client::MONSTERTYPE::AXEMAN:
				switch (iAttRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::KNIFEWOMAN:
				switch (iAttRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			case Client::MONSTERTYPE::SKULL:
				break;
			case Client::MONSTERTYPE::GARDENER:
				if (m_iGardnerAtkIndex > 1)
				{
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_GardnerAtkIndex(0);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_iGardnerAtkIndex = 0;
					m_bWalkCheck = false;
				}
				else
				{
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_GardnerPlusAtkIndex(1);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					++m_iGardnerAtkIndex;
					m_bWalkCheck = false;

				}	
				break;	
			case Client::MONSTERTYPE::ENHANCE_GARDENER:			
				{
				int iRand = rand() % 2;
				if (m_iGardnerAtkIndex > 1)
				{
					switch (iRand)
					{
					case 0:
						Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack2>(0.05f);
						Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_GardnerAtkIndex(0);
						Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
						m_iGardnerAtkIndex = 0;
						m_bWalkCheck = false;
						break;
					case 1:
						Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
						Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_GardnerAtkIndex(0);
						Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
						m_iGardnerAtkIndex = 0;
						m_bWalkCheck = false;
						break;
					}
					break;
				}
				else
				{
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_GardnerPlusAtkIndex(1);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					++m_iGardnerAtkIndex;
					m_bWalkCheck = false;

				}
				}
				break;
			case Client::MONSTERTYPE::SHIELDAXEMAN:
			{
				int iRand = rand() % 3;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack3>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}	
			break;
			case Client::MONSTERTYPE::SKULLSHIELDMAN:
			{
				int iRand = rand() % 4;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}
			break;
			case Client::MONSTERTYPE::SKULLSPEARMAN:
			{
				int iRand = rand() % 3;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}
			break;
			case Client::MONSTERTYPE::ARMORSHIELDMAN:
			{
				int iRand = rand() % 4;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}
			break;
			case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
			{
				int iRand = rand() % 4;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}
			break;
			case Client::MONSTERTYPE::ARMORSPEARMAN:
			{
				int iRand = rand() % 4;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 3:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack3>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}
			break;
			case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
			{
				int iRand = rand() % 3;
				switch (iRand)
				{
				case 0:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 1:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				case 2:
					Get_OwnerCharacter().lock()->Change_State<CNorMonState_HeavyAttack1>(0.05f);
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(false);
					m_bWalkCheck = false;
					break;
				}
				break;
			}
			break;
			}
			return true;
		}

		
	}


	return false;
}

