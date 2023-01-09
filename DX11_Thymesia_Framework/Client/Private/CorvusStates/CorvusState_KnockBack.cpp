#include "stdafx.h"
#include "CorvusStates/CorvusState_KnockBack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXController.h"


GAMECLASS_C(CCorvusState_KnockBack);
CLONE_C(CCorvusState_KnockBack, CComponent)

HRESULT CCorvusState_KnockBack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_KnockBack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_KnockBack::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Defense_Hurt_Large");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_KnockBack::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_KnockBack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_KnockBack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_KnockBack::OnDisable()
{

}

void CCorvusState_KnockBack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Weak_StaticCast<CCorvus>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.f, 2.f, 2.f));

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif
	

}

void CCorvusState_KnockBack::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CCorvus>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));

	
}

void CCorvusState_KnockBack::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_KnockBack::OnDestroy()
{

	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_KnockBack::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_KnockBack::Free()
{
}

_bool CCorvusState_KnockBack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}


