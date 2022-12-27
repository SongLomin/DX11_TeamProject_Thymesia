#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_VargSwordStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "Effect_Trail.h"
#include "NorMonStates.h"
#include "PhysXCharacterController.h"
#include "Collider.h"
#include "Attack_Area.h"
#include "Status_Monster.h"

GAMECLASS_C(CCorvusState_PS_VargSwordStart);
CLONE_C(CCorvusState_PS_VargSwordStart, CComponent)

HRESULT CCorvusState_PS_VargSwordStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_VargSwordStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_PS_VargSwordStart::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_VargSword_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_VargSwordStart::Call_AnimationEnd, this);
}

void CCorvusState_PS_VargSwordStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_PS_VargSwordStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_PS_VargSwordStart1Disable()
{

}

void CCorvusState_PS_VargSwordStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_VargSwordStart::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_VargSwordStart::Call_OtherControllerHit, this, placeholders::_1);



#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

}

void CCorvusState_PS_VargSwordStart::OnStateEnd()
{
	__super::OnStateEnd();
	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_VargSwordStart::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_VargSwordStart::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_VargSwordStart::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_PS_VargSwordStart::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 1:
	{
		GET_SINGLE(CGameManager)->Store_EffectIndex("Corvus_PW_EyeGlow", GET_SINGLE(CGameManager)->Use_EffectGroup("Corvus_PW_EyeGlow", m_pTransformCom, _uint(TIMESCALE_LAYER::PLAYER)));
	}
	return;
	case 41:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(-1.f, -0.2f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.2f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.2f);
	}
		return;
	case 97:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.3f);
	}
		return;
	case 178:
	{
		GET_SINGLE(CGameManager)->UnUse_EffectGroup("Corvus_PW_EyeGlow", GET_SINGLE(CGameManager)->Get_StoredEffectIndex("Corvus_PW_EyeGlow"));
	}
	return;
	}
}



void CCorvusState_PS_VargSwordStart::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_PS_VargSwordStart::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::MONSTER_ATTACK)
	{
		weak_ptr<CStatus_Player> pStatus = Weak_StaticCast<CStatus_Player>(m_pStatusCom);

		weak_ptr<CAttackArea>	pAttackArea = Weak_StaticCast<CAttackArea>(pOtherCollider.lock()->Get_Owner());
		weak_ptr<CCharacter>	pMonsterFromCharacter = pAttackArea.lock()->Get_ParentObject();
		weak_ptr<CStatus_Monster>	pMonsterStatusCom = Weak_StaticCast< CStatus_Monster>(pMonsterFromCharacter.lock()->Get_Status());

		pStatus.lock()->Add_Damage(In_fDamage * pMonsterStatusCom.lock()->Get_Desc().m_fAtk);
		_bool bGroggy = pMonsterStatusCom.lock()->Is_Groggy();

		if (bGroggy)
		{
			pMonsterFromCharacter.lock()->OnEventMessage((_uint)EVENT_TYPE::ON_GROGGY);
		}
	}
}

void CCorvusState_PS_VargSwordStart::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_VargSwordStart::Call_AnimationEnd, this);
}

_bool CCorvusState_PS_VargSwordStart::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

