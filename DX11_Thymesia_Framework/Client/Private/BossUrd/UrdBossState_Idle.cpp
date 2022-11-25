#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Idle);
CLONE_C(CUrdBossState_Idle, CComponent)

HRESULT CUrdBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Urd.ao|Urd_Idle");

	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Idle::Call_AnimationEnd, this);
}

void CUrdBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));



	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CUrdBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();



}



//void CUrdBossState_Idle::Call_AnimationEnd()
//{
//	if (!Get_Enable())
//		return;
//
//	Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkF>(0.05f);
//}
//
//void CUrdBossState_Idle::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Idle::Call_AnimationEnd, this);
//}

void CUrdBossState_Idle::Free()
{

}

_bool CUrdBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;





	return false;
}

