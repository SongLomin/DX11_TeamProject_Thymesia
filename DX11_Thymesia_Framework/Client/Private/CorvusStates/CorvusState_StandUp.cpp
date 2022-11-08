#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_StandUp.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "LuxiyaStates/LuxiyaStates.h"

GAMECLASS_C(CLuxiyaState_StandUp);
CLONE_C(CLuxiyaState_StandUp, CComponent)

HRESULT CLuxiyaState_StandUp::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CLuxiyaState_StandUp::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 38;
	m_fFixedPlayRatio = 0.f;

	return S_OK;
}

void CLuxiyaState_StandUp::Start()
{
	__super::Start();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_StandUp::Call_AnimationEnd, this);
}

void CLuxiyaState_StandUp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
}

void CLuxiyaState_StandUp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CLuxiyaState_StandUp::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);




#ifdef _DEBUG
	cout << "LuxiyaState: StandUp -> OnStateStart" << endl;
#endif
}

void CLuxiyaState_StandUp::OnStateEnd()
{
	__super::OnStateEnd();
}

void CLuxiyaState_StandUp::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>();

}

void CLuxiyaState_StandUp::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_StandUp::Call_AnimationEnd, this);
}

void CLuxiyaState_StandUp::Free()
{

}

_bool CLuxiyaState_StandUp::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (Check_RequirementDashState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
		return true;
	}

	return false;
}