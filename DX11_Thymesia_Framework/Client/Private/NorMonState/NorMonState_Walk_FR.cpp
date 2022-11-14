#include "stdafx.h"
#include "NorMonState/NorMonState_Walk_FR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateBase.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Walk_FR);
CLONE_C(CNorMonState_Walk_FR, CComponent)

HRESULT CNorMonState_Walk_FR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Walk_FR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Walk_FR::Start()
{
	__super::Start();

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|LV1Villager_M_WalkFR|BaseLayer");
	}
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Walk_FR::Call_AnimationEnd, this);
}

void CNorMonState_Walk_FR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	m_pTransformCom.lock()->Go_Straight(m_fCurrentSpeed * fTimeDelta, m_pNaviCom);
}

void CNorMonState_Walk_FR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Walk_FR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_Walk_FR::OnStateEnd()
{
	__super::OnStateEnd();

	
}



void CNorMonState_Walk_FR::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	m_eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}

void CNorMonState_Walk_FR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Walk_FR::Call_AnimationEnd, this);
}

void CNorMonState_Walk_FR::Free()
{

}

_bool CNorMonState_Walk_FR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;

}

