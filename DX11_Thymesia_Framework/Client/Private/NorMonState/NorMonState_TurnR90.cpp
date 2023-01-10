#include "stdafx.h"
#include "NorMonState/NorMonState_TurnR90.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_TurnR90);
CLONE_C(CNorMonState_TurnR90, CComponent)

HRESULT CNorMonState_TurnR90::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_TurnR90::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CNorMonState_TurnR90::Start()
{
	__super::Start();

	DirCheckTurnSpeed();
	m_bStartAnimation = true;

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_TurnR90|BaseLayer|Armature|A");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_TurnR90");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_TurnR90");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_TurnR90");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_TurnR90|BaseLayer|Armature|A");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_TurnR90");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_TurnR90");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_TurnR90");
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_TurnR90::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_TurnR90::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_TurnR90::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	_float fTurnValue = 1.57f / 1.333f;

	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * 1.5f);

	Check_AndChangeNextState();
}

void CNorMonState_TurnR90::DirCheckTurnSpeed()
{
	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 3.f)
		m_fTurnSpeed = 3.f;

	if (fPToMDistance > 3.f && fPToMDistance <= 7.f)
		m_fTurnSpeed = 2.f;

	if (fPToMDistance > 7.f)
		m_fTurnSpeed = 1.f;
}



void CNorMonState_TurnR90::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: turnrrrrrr -> trurnrrrr" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
}

void CNorMonState_TurnR90::OnStateEnd()
{
	__super::OnStateEnd();
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}




void CNorMonState_TurnR90::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_TurnR90::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_TurnR90::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_TurnR90::Free()
{
}

_bool CNorMonState_TurnR90::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.94f)
	{
		Rotation_TargetToLookDir();
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}

	return false;
}


	



