#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_BlueSkill.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "LuxiyaStates/LuxiyaStates.h"
#include "GameManager.h"

GAMECLASS_C(CLuxiyaState_BlueSkill);
CLONE_C(CLuxiyaState_BlueSkill, CComponent)

HRESULT CLuxiyaState_BlueSkill::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_iAnimIndex = 7;
	m_fFixedPlayRatio = 0.5f;

	return S_OK;
}

HRESULT CLuxiyaState_BlueSkill::Initialize(void* pArg)
{
	__super::Initialize(pArg);




	return S_OK;
}

void CLuxiyaState_BlueSkill::Start()
{
	__super::Start();

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_BlueSkill::Call_AnimationEnd, this);
}

void CLuxiyaState_BlueSkill::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);

	if (KEY_INPUT(KEY::N, KEY_STATE::TAP))
	{
		if (m_fDebugAnimationSpeed < 0.5f)
		{
			m_fDebugAnimationSpeed = 1.f;
		}

		else
		{
			m_fDebugAnimationSpeed = 0.1f;
		}
	}

	Attack();
}

void CLuxiyaState_BlueSkill::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (Check_AndChangeNextState())
	{
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);

	}
}

void CLuxiyaState_BlueSkill::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>();

}

void CLuxiyaState_BlueSkill::Call_OnEffect(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		DEBUG_ASSERT;

	if (In_iKeyIndex == 6)
	{
		Weak_StaticCast<CLuxiya>(m_pOwner).lock()->Set_BlueSkillState(20.f);
		GET_SINGLE(CGameManager)->Add_Shaking(SHAKE_DIRECTION::LOOK, 0.5f, 0.6f);
		GAMEINSTANCE->Add_MotionBlurScale(0.11f);
	}

}


void CLuxiyaState_BlueSkill::Attack()
{

}

void CLuxiyaState_BlueSkill::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	Rotation_TargetToLookDir();

	Disable_Weapons();

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pModelCom.lock()->Get_CurrentAnimation().lock()->CallBack_NextChannelKey +=
		bind(&CLuxiyaState_BlueSkill::Call_OnEffect, this, placeholders::_1);

	GAMEINSTANCE->PlaySoundW(TEXT("Luxiya_BlueSkill.wav"), 1.f);


#ifdef _DEBUG
	cout << "LuxiyaState: ReSkill -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_BlueSkill::OnStateEnd()
{
	__super::OnStateEnd();

	Disable_Weapons();
	m_IsNextAttack = false;
	m_iAttackIndex = 0;
	Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	Set_OriginalWeaponsScale();

	m_pModelCom.lock()->Get_CurrentAnimation().lock()->CallBack_NextChannelKey -=
		bind(&CLuxiyaState_BlueSkill::Call_OnEffect, this, placeholders::_1);
}

void CLuxiyaState_BlueSkill::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_BlueSkill::Call_AnimationEnd, this);
}

void CLuxiyaState_BlueSkill::Free()
{

}

_bool CLuxiyaState_BlueSkill::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (Check_RequirementUltimateState())
	{
		Rotation_NearToLookDir();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_UltimateSkill>();
		return true;
	}

	if (Check_RequirementAttackState())
	{
		if (!Rotation_InputToLookDir())
			Rotation_NearToLookDir();

		Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>();
		Get_OwnerPlayer()->Get_Component<CLuxiyaState_Attack>().lock()->Play_AttackWithIndex(0);
		return true;
	}

	if (Check_RequirementDashState())
	{
		Rotation_InputToLookDir();
		//Open_HyperSpace();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
		return true;
	}

	if (Check_RequirementRunState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_RunStart>();
		return true;
	}

	return false;
}



