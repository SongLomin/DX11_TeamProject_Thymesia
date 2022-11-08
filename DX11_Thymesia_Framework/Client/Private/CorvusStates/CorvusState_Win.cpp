#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_Win.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "LuxiyaStates/LuxiyaStates.h"
#include "Camera_Target.h"
#include "ClientLevel.h"

GAMECLASS_C(CLuxiyaState_Win);
CLONE_C(CLuxiyaState_Win, CComponent)

HRESULT CLuxiyaState_Win::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CLuxiyaState_Win::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 42;



	return S_OK;
}

void CLuxiyaState_Win::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_Win::Call_AnimationEnd, this);
}

void CLuxiyaState_Win::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bAnimEnd)
	{
		m_pModelCom.lock()->Play_Animation(fTimeDelta);

		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
		m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
	}
	
}

void CLuxiyaState_Win::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CLuxiyaState_Win::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	m_bAnimEnd = true;
	//Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>();
}

void CLuxiyaState_Win::Call_OnNextKeyFrame(const _uint& In_iKeyIndex)
{
	if (In_iKeyIndex == 90)
	{
		GET_SINGLE(CGameManager)->Change_NextLevel(nullptr);
		GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_WeaponIn.wav"), 1.f);
	}

}

void CLuxiyaState_Win::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	Get_OwnerPlayer()->Set_DodgeTime(20.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pModelCom.lock()->Get_CurrentAnimation().lock()->CallBack_NextChannelKey +=
		bind(&CLuxiyaState_Win::Call_OnNextKeyFrame, this, placeholders::_1);

	GET_SINGLE(CGameManager)->Get_TargetCamera().lock()->Cinema_BattleEnd();
	GAMEINSTANCE->BGMVolumeDown(0.07f);
	GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_Win.wav"), 1.f);
	m_bAnimEnd = false;

#ifdef _DEBUG
	cout << "LuxiyaState: Win -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_Win::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Get_CurrentAnimation().lock()->CallBack_NextChannelKey -=
		bind(&CLuxiyaState_Win::Call_OnNextKeyFrame, this, placeholders::_1);
}

_bool CLuxiyaState_Win::Check_AndChangeNextState()
{
	//if (!Check_Requirement())
	//	return false;

	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();

	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>();
	//	return true;
	//}

	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	//Open_HyperSpace();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
	//	return true;
	//}

	//if (Check_RequirementRunState())
	//{
	//	Rotation_InputToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_RunStart>();
	//	return true;
	//}

	return false;
}

void CLuxiyaState_Win::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_Win::Call_AnimationEnd, this);
}

