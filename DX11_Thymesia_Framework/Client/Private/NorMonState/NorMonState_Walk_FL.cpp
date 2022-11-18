#include "stdafx.h"
#include "NorMonState/NorMonState_Walk_FL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Walk_FL);
CLONE_C(CNorMonState_Walk_FL, CComponent)

HRESULT CNorMonState_Walk_FL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Walk_FL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Walk_FL::Start()
{
	__super::Start();

	switch (m_eNorMonType)
	{
	case Client::NORMONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkFL|BaseLayer|Armat");
		break;
	case Client::NORMONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|SK_C_LV1Villager_F.ao|Walk_F|SK_C_LV1Vill");
		break;
	case Client::NORMONSTERTYPE::SKULL:
		break;
	case Client::NORMONSTERTYPE::GARDENER:
		break;
	}

	
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Walk_FL::Call_AnimationEnd, this);
}

void CNorMonState_Walk_FL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	m_pTransformCom.lock()->Add_PositionWithRotation(XMVectorSet(m_fCurrentSpeed * fTimeDelta, 0.f, -m_fCurrentSpeed * fTimeDelta, 1.f), m_pNaviCom);
}

void CNorMonState_Walk_FL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Walk_FL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

	m_bAttackLookAtLimit = true;
}

void CNorMonState_Walk_FL::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);


}



void CNorMonState_Walk_FL::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Walk_FL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Walk_FL::Call_AnimationEnd, this);
}

void CNorMonState_Walk_FL::Free()
{

}

_bool CNorMonState_Walk_FL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.1f)
	{
		m_bAttackLookAtLimit = false;
	}

	return false;

}

