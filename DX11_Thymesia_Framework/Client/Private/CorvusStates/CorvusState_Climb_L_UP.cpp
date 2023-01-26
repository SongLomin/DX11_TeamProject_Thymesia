#include "stdafx.h"
#include "CorvusStates/CorvusState_Climb_L_UP.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CCorvusState_Climb_L_UP);
CLONE_C(CCorvusState_Climb_L_UP, CComponent)

HRESULT CCorvusState_Climb_L_UP::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Climb_L_UP::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Climb_L_UP::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_L_UP");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Climb_L_UP::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Climb_L_UP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);	
}

void CCorvusState_Climb_L_UP::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}

void CCorvusState_Climb_L_UP::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	_vector vTransformPositon = Get_OwnerPlayer()->Get_Transform()->Get_Position();

	cout << "x" << vTransformPositon.m128_f32[0] << "," << "y" << vTransformPositon.m128_f32[1] << "," << "z" << vTransformPositon.m128_f32[2] << endl;

	
	Get_OwnerPlayer()->Change_State<CCorvusState_Climb_L_Idle>();

}

void CCorvusState_Climb_L_UP::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);



	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 0, 0.05f);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CCorvusState_Climb_L_UP::Call_NextKeyFrame, this, placeholders::_1);
}

void CCorvusState_Climb_L_UP::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	_vector fOffSet = { 0.f, 0.00164142857f ,0.f };

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation(fOffSet, 0.f, GAMEINSTANCE->Get_DeltaTime(),
		Filters, nullptr, m_pTransformCom);
}


void CCorvusState_Climb_L_UP::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CCorvusState_Climb_L_UP::Call_NextKeyFrame, this, placeholders::_1);
}

void CCorvusState_Climb_L_UP::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Climb_L_UP::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Climb_L_UP::Free()
{
	
}

_bool CCorvusState_Climb_L_UP::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	return false;
}

