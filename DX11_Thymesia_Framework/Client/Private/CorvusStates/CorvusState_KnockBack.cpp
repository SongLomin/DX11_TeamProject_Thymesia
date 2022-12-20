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
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_HurtKnockDown");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_KnockBack::Call_AnimationEnd, this);
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

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 34);

	_float3 vRandDir = SMath::vRandom(_float3(-1.f, -1.f, 0.f), _float3(1.f, 1.f, 0.f));

	_matrix WorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();

	GET_SINGLE(CGameManager)->Add_Shaking(XMVector3TransformNormal(XMLoadFloat3(&vRandDir), WorldMatrix), 0.25f, 1.f, 9.f, 0.4f);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif
	

}

void CCorvusState_KnockBack::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_KnockBack::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_KnockBack::OnDestroy()
{

	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_KnockBack::Call_AnimationEnd, this);
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


