#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"

GAMECLASS_C(CBigHandManState_ComboB_End);
CLONE_C(CBigHandManState_ComboB_End, CComponent)

void CBigHandManState_ComboB_End::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 16:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
	}
		return;	
	}
}

HRESULT CBigHandManState_ComboB_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_ComboB_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_ComboB_End::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_ComboB_End");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_ComboB_End::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_ComboB_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_ComboB_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_ComboB_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBigHandManState_ComboB_Loop>().lock())
	{

	}
	else
	{

	}

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBigHandManState_ComboB_End::Call_NextKeyFrame, this, placeholders::_1);
}

void CBigHandManState_ComboB_End::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBigHandManState_ComboB_End::Call_NextKeyFrame, this, placeholders::_1);
}

void CBigHandManState_ComboB_End::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBigHandManState_Idle>(0.05f);	
}

void CBigHandManState_ComboB_End::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_ComboB_End::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_ComboB_End::Free()
{

}

_bool CBigHandManState_ComboB_End::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}



	return false;
}

