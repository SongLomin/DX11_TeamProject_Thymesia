#include "stdafx.h"
#include "CorvusStates/CorvusState_HurtBlown.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"


GAMECLASS_C(CCorvusState_HurtBlown);
CLONE_C(CCorvusState_HurtBlown, CComponent)

HRESULT CCorvusState_HurtBlown::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_HurtBlown::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_HurtBlown::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_HurtBlown");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_HurtBlown::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtBlown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_HurtBlown::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_HurtBlown::OnDisable()
{

}

void CCorvusState_HurtBlown::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Weak_StaticCast<CCorvus>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.f, 2.f, 2.f));

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 34);

	_float3 vRandDir = SMath::vRandom(_float3(-1.f, -1.f, 0.f), _float3(1.f, 1.f, 0.f));

	_matrix WorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	GET_SINGLE(CGameManager)->Add_Shaking(XMVector3TransformNormal(XMLoadFloat3(&vRandDir), WorldMatrix), 0.25f, 1.f, 9.f, 0.4f);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif
}

void CCorvusState_HurtBlown::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

	Weak_StaticCast<CCorvus>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}

void CCorvusState_HurtBlown::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_HurtBlown::OnDestroy()
{

	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_HurtBlown::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtBlown::Free()
{
}

_bool CCorvusState_HurtBlown::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}


