#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_Varg_Execution.h"
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

GAMECLASS_C(CCorvusState_Varg_Execution);
CLONE_C(CCorvusState_Varg_Execution, CComponent)

HRESULT CCorvusState_Varg_Execution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Varg_Execution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Varg_Execution::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_VSVarg_Execution_Blending");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Varg_Execution::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Varg_Execution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));


	GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::EXECUTION);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Varg_Execution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Varg_Execution::OnDisable()
{

}

void CCorvusState_Varg_Execution::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CCorvusState_Varg_Execution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,60);


	//m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	//m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_Varg_Execution::Call_NextAnimationKey, this, placeholders::_1);


	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_Varg_Execution::OnStateEnd()
{
	__super::OnStateEnd();

	GET_SINGLE(CGameManager)->End_Cinematic();
}

void CCorvusState_Varg_Execution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CVarg_Execution>();

}

void CCorvusState_Varg_Execution::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;


}

void CCorvusState_Varg_Execution::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_Varg_Execution::Free()
{
	
}

void CCorvusState_Varg_Execution::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Varg_Execution::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_Varg_Execution::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 23)
	//{
	//	Get_OwnerPlayer()->Change_State<CCorvusState_Bat_Execution>();
	//	return true;
	//}


	return false;
}

