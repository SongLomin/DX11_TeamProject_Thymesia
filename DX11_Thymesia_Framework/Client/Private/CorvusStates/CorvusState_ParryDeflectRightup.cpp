#include "stdafx.h"
#include "CorvusStates/CorvusState_ParryDeflectRightup.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_ParryDeflectRightup);
CLONE_C(CCorvusState_ParryDeflectRightup, CComponent)

HRESULT CCorvusState_ParryDeflectRightup::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_ParryDeflectRightup::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_ParryDeflectRightup::Start()
{
	__super::Start();

	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Corvus.ao|Corvus_SD_ParryDeflect_RUp");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ParryDeflectRightup::Call_AnimationEnd, this);
}

void CCorvusState_ParryDeflectRightup::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_ParryDeflectRightup::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}


void CCorvusState_ParryDeflectRightup::OnDisable()
{

}

void CCorvusState_ParryDeflectRightup::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_ParryDeflectRightup>().lock())
	//{
	//	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 5);
	//}
	//else
	//{
	//	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	//}
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

	//m_pModelCom.lock()->Set_AnimationSpeed(3.f);
}

void CCorvusState_ParryDeflectRightup::OnStateEnd()
{
	__super::OnStateEnd();

	//m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CCorvusState_ParryDeflectRightup::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_ParryDeflectRightup::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	m_pModelCom.lock()->Set_AnimationSpeed(m_fDebugAnimationSpeed);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_ParryDeflectRightup::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ParryDeflectRightup::Call_AnimationEnd, this);
}

_bool CCorvusState_ParryDeflectRightup::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	return false;
}


