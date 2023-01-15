
#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_Execution_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CCorvusState_Execution_Start);
CLONE_C(CCorvusState_Execution_Start, CComponent)

HRESULT CCorvusState_Execution_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Execution_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Execution_Start::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_StunExecute_StartR_L");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Execution_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Execution_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Execution_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Execution_Start::OnDisable()
{

}

void CCorvusState_Execution_Start::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CCorvusState_Execution_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


	//m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	//m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_Execution_Start::Call_NextAnimationKey, this, placeholders::_1);


	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_Execution_Start::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_Execution_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Execution_Start::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 3:
		_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f));
		GAMEINSTANCE->Set_RadialBlur(0.2f, vPosition);
		GAMEINSTANCE->Set_Chromatic(0.1f);
		
		break;
	}
}

void CCorvusState_Execution_Start::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_Execution_Start::Free()
{
	
}

void CCorvusState_Execution_Start::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Execution_Start::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_Execution_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 23)
	{
		Get_OwnerPlayer()->Change_State<CCorvusState_Bat_Execution>();
		return true;
	}


	return false;
}

