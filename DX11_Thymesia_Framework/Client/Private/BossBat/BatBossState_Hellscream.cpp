#include "stdafx.h"
#include "BossBat/BatBossState_Hellscream.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Hellscream);
CLONE_C(CBatBossState_Hellscream, CComponent)

HRESULT CBatBossState_Hellscream::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Hellscream::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Hellscream::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Hellscream");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Hellscream::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Hellscream::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}
	
	if (m_bShaking)
	{
		GET_SINGLE(CGameManager)->Add_Shaking(XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.15f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.05f);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Hellscream::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);


	Check_AndChangeNextState();
}

void CBatBossState_Hellscream::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 28:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.05f, 9.f, 3.f);
		break;
	}
	case 45:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 428:
		m_bShaking = true;
		break;

	case 653:
		m_bShaking = false;
		break;
	}
}

void CBatBossState_Hellscream::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;
	m_bShaking = false;
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Hellscream::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Hellscream::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Hellscream::Call_NextAnimationKey, this, placeholders::_1);

}



void CBatBossState_Hellscream::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_HeelScream(true);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Hellscream::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Hellscream::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Hellscream::Free()
{

}

_bool CBatBossState_Hellscream::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.97f)
	{
		Rotation_TargetToLookDir();
		m_bAttackLookAtLimit = false;
	}

	

	return false;
}

