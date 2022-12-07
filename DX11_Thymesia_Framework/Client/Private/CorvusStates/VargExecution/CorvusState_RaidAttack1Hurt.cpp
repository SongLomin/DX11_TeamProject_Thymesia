#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_RaidAttack1Hurt.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "BoneNode.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CCorvusState_RaidAttack1Hurt);
CLONE_C(CCorvusState_RaidAttack1Hurt, CComponent)

HRESULT CCorvusState_RaidAttack1Hurt::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_RaidAttack1Hurt::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_RaidAttack1Hurt::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_VSVarg_TakeSPAttack");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_RaidAttack1Hurt::Call_AnimationEnd, this);
}

void CCorvusState_RaidAttack1Hurt::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));


	GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_RaidAttack1Hurt::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_RaidAttack1Hurt::Init_Varg(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode)
{

}

void CCorvusState_RaidAttack1Hurt::OnDisable()
{

}

void CCorvusState_RaidAttack1Hurt::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

}

void CCorvusState_RaidAttack1Hurt::OnStateEnd()
{
	__super::OnStateEnd();

	GET_SINGLE(CGameManager)->End_Cinematic();

}

void CCorvusState_RaidAttack1Hurt::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_RaidAttack1Hurt::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_RaidAttack1Hurt::Call_AnimationEnd, this);
}

_bool CCorvusState_RaidAttack1Hurt::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

