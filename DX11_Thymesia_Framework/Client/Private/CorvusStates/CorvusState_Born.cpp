#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_Born.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "LuxiyaStates/LuxiyaStates.h"
#include "GameManager.h"
#include "Camera_Target.h"
#include "SkillChain.h"

GAMECLASS_C(CLuxiyaState_Born);
CLONE_C(CLuxiyaState_Born, CComponent)

HRESULT CLuxiyaState_Born::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CLuxiyaState_Born::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 16;

	

	return S_OK;
}

void CLuxiyaState_Born::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_Born::Call_AnimationEnd, this);
}

void CLuxiyaState_Born::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	if (!m_bFirst)
	{
		m_bFirst = true;
		
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
		m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
		return;
	}

	if (!m_bSecond)
	{
		m_bSecond = true;
		if(GET_SINGLE(CGameManager)->Get_TargetCamera().lock())
			GET_SINGLE(CGameManager)->Get_TargetCamera().lock()->Cinema_LuxiyaStartBattle();
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
}

void CLuxiyaState_Born::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CLuxiyaState_Born::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>(0.2f);
}

void CLuxiyaState_Born::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_bFirst = false;
	m_bSecond = false;

	Get_OwnerPlayer()->Get_Component<CSkillChain>().lock()->Set_Enable(false);

	GAMEINSTANCE->BGMVolumeDown(0.03f);
 	GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_Born.wav"), 1.f);

#ifdef _DEBUG
	cout << "LuxiyaState: Stand -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_Born::OnStateEnd()
{
	__super::OnStateEnd();

	if (!Get_OwnerCharacter().lock()->Is_Edit())
	{
		GET_SINGLE(CGameManager)->Set_GameState(GAME_STATE::BATTLE);
		Get_OwnerPlayer()->Get_Component<CSkillChain>().lock()->Set_Enable(true);
	}

	GAMEINSTANCE->BGMVolumeUp(0.03f);
}

_bool CLuxiyaState_Born::Check_AndChangeNextState()
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

void CLuxiyaState_Born::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_Born::Call_AnimationEnd, this);
}

