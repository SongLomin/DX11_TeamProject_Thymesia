#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_QTE.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "LuxiyaStates/LuxiyaStates.h"
#include "GameManager.h"

GAMECLASS_C(CLuxiyaState_QTE);
CLONE_C(CLuxiyaState_QTE, CComponent)

HRESULT CLuxiyaState_QTE::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_iAnimIndex = 14;
	m_fFixedPlayRatio = 0.5f;

	return S_OK;
}

HRESULT CLuxiyaState_QTE::Initialize(void* pArg)
{
	__super::Initialize(pArg);




	return S_OK;
}

void CLuxiyaState_QTE::Start()
{
	__super::Start();

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_QTE::Call_AnimationEnd, this);
}

void CLuxiyaState_QTE::Tick(_float fTimeDelta)
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

void CLuxiyaState_QTE::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	if (Check_AndChangeNextState())
	{
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);

	}
}

void CLuxiyaState_QTE::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>();

}


void CLuxiyaState_QTE::Attack()
{

}

void CLuxiyaState_QTE::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	Disable_Weapons();
	Enable_Weapons(HIT_TYPE::NORMAL_HIT, 50.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	Get_OwnerCharacter().lock()->Set_RigidColliderEnable(false);

	Set_WeaponsScale(5.f);

#ifdef _DEBUG
	cout << "LuxiyaState: ReSkill -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_QTE::OnStateEnd()
{
	__super::OnStateEnd();

	Disable_Weapons();
	m_IsNextAttack = false;
	m_iAttackIndex = 0;
	Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	Set_OriginalWeaponsScale();
}

void CLuxiyaState_QTE::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_QTE::Call_AnimationEnd, this);
}

void CLuxiyaState_QTE::Free()
{

}

_bool CLuxiyaState_QTE::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}



