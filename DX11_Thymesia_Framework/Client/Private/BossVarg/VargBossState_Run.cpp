#include "stdafx.h"
#include "BossVarg/VargBossState_Run.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "PhysXController.h"

GAMECLASS_C(CVargBossState_Run);
CLONE_C(CVargBossState_Run, CComponent)

void CVargBossState_Run::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 0:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.1f, 0.3f, 9.f, 0.1f);
		break;
	case 10:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.1f, 0.3f, 9.f, 0.1f);
		break;
	}
}

HRESULT CVargBossState_Run::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Run::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = { 0.f, 1.f, 0.f };
	return S_OK;
}

void CVargBossState_Run::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_RunF");
}

void CVargBossState_Run::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CVargBossState_Run::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CVargBossState_Run::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_Run::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Run -> OnStateStart" << endl;
#endif
#endif


}

void CVargBossState_Run::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_Run::Call_NextKeyFrame, this, placeholders::_1);
}


void CVargBossState_Run::Free()
{

}

_bool CVargBossState_Run::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리


	if (fPToMDistance <= 4.f)
	{
		int iRand = rand() % 3;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1a>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1b>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack3b>(0.05f);
			break;
		}

		return true;
	}


	return false;
}

