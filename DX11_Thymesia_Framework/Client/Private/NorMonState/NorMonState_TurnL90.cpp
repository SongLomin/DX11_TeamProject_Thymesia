#include "stdafx.h"
#include "NorMonState/NorMonState_TurnL90.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_TurnL90);
CLONE_C(CNorMonState_TurnL90, CComponent)

HRESULT CNorMonState_TurnL90::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_TurnL90::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_TurnL90::Start()
{
	__super::Start();


	switch (m_eNorMonType)
	{
	case Client::NORMONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_TurnL90|BaseLayer|Armature|A");
		break;
	case Client::NORMONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|TurnL90|SK_C_LV1Vil");
		break;
	case Client::NORMONSTERTYPE::SKULL:
		break;
	case Client::NORMONSTERTYPE::GARDENER:
		break;
	}


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_TurnL90::Call_AnimationEnd, this);
}

void CNorMonState_TurnL90::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float fTurnValue = 1.57 / 1.333f;


	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * -1.5f);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_TurnL90::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CNorMonState_TurnL90::DirCheckTurnSpeed()
{
	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 3.f)
		m_fTurnSpeed = 2.f;

	if (fPToMDistance > 3.f && fPToMDistance <= 7.f)
		m_fTurnSpeed = 1.5f;

	if (fPToMDistance > 7.f)
		m_fTurnSpeed = 1.f;
}




void CNorMonState_TurnL90::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: TurnLllllll -> Trurnllllll" << endl;
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

}

void CNorMonState_TurnL90::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_TurnL90::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_TurnL90::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_TurnL90::Call_AnimationEnd, this);
}

void CNorMonState_TurnL90::Free()
{

}

_bool CNorMonState_TurnL90::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	//코사인값으로 15일때 룩엣해라 
	//90도일떄 0 나힘들어  ??  0.83
	//띵깅중 
	if (ComputeAngleWithPlayer() > 0.94f)
	{
		Rotation_TargetToLookDir();
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}
	

	return false;

}




