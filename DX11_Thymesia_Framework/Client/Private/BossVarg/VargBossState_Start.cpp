#include "stdafx.h"
#include "BossVarg/VargBossState_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "UI_ScriptQueue.h"
#include "MonsterHPBar_Boss.h"
GAMECLASS_C(CVargBossState_Start);
CLONE_C(CVargBossState_Start, CComponent)

HRESULT CVargBossState_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Start::Start()
{
	__super::Start();

	switch (m_eBossStartType)
	{
	case Client::BOSSSTARTTYPE::BEGINSTART:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Seq_TutorialBossFightStart");
		break;
	case Client::BOSSSTARTTYPE::NORMALSTART:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_Seq_BossFightStart");
		break;
	}


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Start::Call_AnimationEnd, this);
}

void CVargBossState_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));

	if (m_fSinematic == 4.f)
	{
		GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pModelCom.lock()->Set_AnimationSpeed(m_fSinematic);

	Check_AndChangeNextState();
}



void CVargBossState_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	m_pModelCom.lock()->Set_AnimationSpeed(m_fSinematic);

}


void CVargBossState_Start::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

	if (m_fSinematic == 4.f)
		GET_SINGLE(CGameManager)->End_Cinematic();



}



void CVargBossState_Start::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	

	weak_ptr<CCharacter> pCharacter = Weak_StaticCast<CCharacter>(m_pOwner);

	if (pCharacter.lock()->Is_Edit())
	{
		return;
	}

	weak_ptr<CUI_ScriptQueue> pScriptQeuue = GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL_STATIC).front();
	pScriptQeuue.lock()->Call_SetScript_Tutorial_Varg();

	Weak_StaticCast<CBossMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(true);

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkF>(0.05f);
}
 
void CVargBossState_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Start::Call_AnimationEnd, this);
}

void CVargBossState_Start::Free()
{

}

_bool CVargBossState_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리


	switch (m_eBossStartType)
	{
	case Client::BOSSSTARTTYPE::BEGINSTART:
		if (fPToMDistance <= 10.f)
		{
			m_fSinematic = 4.f;
		}
		break;
	case Client::BOSSSTARTTYPE::NORMALSTART:
		if (fPToMDistance <= 10.f)
		{
			m_fSinematic = 4.f;
		}
		break;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 925)
	{
		weak_ptr<CUI_ScriptQueue> pScriptQeuue = GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL_STATIC).front();
		pScriptQeuue.lock()->Call_SetScript_Tutorial_Varg_Appear();
	}


	



	return false;
}

