#include "stdafx.h"
#include "CorvusStates/CorvusState_Climb_R_Down.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CCorvusState_Climb_R_Down);
CLONE_C(CCorvusState_Climb_R_Down, CComponent)

HRESULT CCorvusState_Climb_R_Down::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Climb_R_Down::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Climb_R_Down::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_R_Down");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Climb_R_Down::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Climb_R_Down::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Climb_R_Down::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	_vector fOffSet = { 0.f, -0.00088095238f ,0.f };

	PxControllerFilters Filters;

	m_pPhysXControllerCom.lock()->MoveWithRotation(fOffSet, 0.f, GAMEINSTANCE->Get_DeltaTime(),
		Filters, nullptr, m_pTransformCom);
}


void CCorvusState_Climb_R_Down::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}

void CCorvusState_Climb_R_Down::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	_vector vTransformPositon = Get_OwnerPlayer()->Get_Transform()->Get_Position();

	cout << "x" << vTransformPositon.m128_f32[0] << "," << "y" << vTransformPositon.m128_f32[1] << "," << "z" << vTransformPositon.m128_f32[2] << endl;

	Get_OwnerPlayer()->Change_State<CCorvusState_Climb_L_Idle>();

}

void CCorvusState_Climb_R_Down::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

;

	_vector vTransformPositon = Get_OwnerPlayer()->Get_Transform()->Get_Position();

	cout << "x" << vTransformPositon.m128_f32[0] << "," << "y" << vTransformPositon.m128_f32[1] << "," << "z" << vTransformPositon.m128_f32[2] << endl;

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Y | (_byte)ROOTNODE_FLAG::Z);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 0, 0.05f);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CCorvusState_Climb_R_Down::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Stop -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Climb_R_Down::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CCorvusState_Climb_R_Down::Call_NextKeyFrame, this, placeholders::_1);
	
}

void CCorvusState_Climb_R_Down::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Climb_R_Down::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Climb_R_Down::Free()
{
	
}

_bool CCorvusState_Climb_R_Down::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

