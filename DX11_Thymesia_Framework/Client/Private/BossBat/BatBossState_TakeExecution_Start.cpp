#include "stdafx.h"
#include "BossBat/BatBossState_TakeExecution_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "Status_Boss.h"

GAMECLASS_C(CBatBossState_TakeExecution_Start);
CLONE_C(CBatBossState_TakeExecution_Start, CComponent)

HRESULT CBatBossState_TakeExecution_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_TakeExecution_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_TakeExecution_Start::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_TakeExecution_Start01");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");
	m_pHeadBoneNode = m_pModelCom.lock()->Find_BoneNode("head");
	
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_TakeExecution_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_TakeExecution_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_TakeExecution_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_TakeExecution_Start::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	if (In_iKeyIndex >= 203 && In_iKeyIndex <= 248)
	{
	
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.03f, 9.f, 3.f);
		
		CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.03f, 9.f, 3.f);

	}

	switch (In_iKeyIndex)
	{
	case 35:
	{
		_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 57:
	{
		_matrix CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
		break;
	}
	case 204:
	{
		_matrix CombinedMatrix = Get_HeadCombinedWorldMatrix();

		_vector vPosition = CombinedMatrix.r[3];//XMVector3TransformCoord(vPosition, m_pRightHandBoneNode.lock()->Get_CombinedMatrix());
		GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.2f, 9.f, 3.f);
		break;
	}
	}
}

void CBatBossState_TakeExecution_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_TakeExecution_Start::Call_NextAnimationKey, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BatState: Start -> OnStateStart" << endl;
#endif
#endif

	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);


}	


void CBatBossState_TakeExecution_Start::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_TakeExecution_Start::Call_NextAnimationKey, this, placeholders::_1);


	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
}



void CBatBossState_TakeExecution_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_TakeExecution_Loop>(0.05f);
}

void CBatBossState_TakeExecution_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_TakeExecution_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_TakeExecution_Start::Free()
{

}

_bool CBatBossState_TakeExecution_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_bDieType)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 314)
		{
			m_pOwner.lock()->Get_Component<CStatus_Boss>().lock()->Set_NextPhase();
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_TakeExecution_End>(0.05f);
			return true;
		}
	}

	return false;
}

