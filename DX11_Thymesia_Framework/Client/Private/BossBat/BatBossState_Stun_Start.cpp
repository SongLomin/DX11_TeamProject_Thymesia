#include "stdafx.h"
#include "BossBat/BatBossState_Stun_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_Stun_Start);
CLONE_C(CBatBossState_Stun_Start, CComponent)

HRESULT CBatBossState_Stun_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Stun_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Stun_Start::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_StunStart");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Stun_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CBatBossState_Stun_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Stun_Start::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 4:
	{
		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.1f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);
		break;
	}
	case 15:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 23:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	}
}

void CBatBossState_Stun_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Enable_Gravity(true);
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Stun_Start::Call_NextAnimationKey, this, placeholders::_1);

	Get_OwnerMonster()->Set_RimLightDesc(6.f, { 0.5f,1.f,0.9f }, 1.f);
}

void CBatBossState_Stun_Start::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Stun_Start::Call_NextAnimationKey, this, placeholders::_1);

}

void CBatBossState_Stun_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Stun_Loop>(0.05f);
}

void CBatBossState_Stun_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Stun_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Stun_Start::Free()
{

}

_bool CBatBossState_Stun_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

