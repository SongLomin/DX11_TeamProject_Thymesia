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
#include "UIManager.h"
#include "UI_FadeMask.h"


GAMECLASS_C(CVargBossState_Start);
CLONE_C(CVargBossState_Start, CComponent)

void CVargBossState_Start::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 1090:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 1251:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		break;
	case 1486:
	{
		weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
		pFadeMask.lock()->Set_Fade_Delay(0.f, 1.f, 0.5f, 0.5f, EASING_TYPE::LINEAR);
		break;
	}
	}
}

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

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));
	GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

}

void CVargBossState_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CVargBossState_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	XMStoreFloat4x4(&m_vPlyerMatrix, pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix());

	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 1.f, EASING_TYPE::LINEAR);



	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_Start::OnStateEnd()
{
	__super::OnStateEnd();
	GET_SINGLE(CGameManager)->End_Cinematic();

	weak_ptr<CUI_FadeMask> pFadeMask = GAMEINSTANCE->Add_GameObject<CUI_FadeMask>(m_pOwner.lock()->Get_CreatedLevel());
	pFadeMask.lock()->Set_Fade(1.f, 0.f, 0.5f, EASING_TYPE::LINEAR);
}

void CVargBossState_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
	weak_ptr<CCharacter> pCharacter = Weak_StaticCast<CCharacter>(m_pOwner);

	if (pCharacter.lock()->Is_Edit())
		return;

	weak_ptr<CUI_ScriptQueue> pScriptQeuue = GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL_STATIC).front();
	pScriptQeuue.lock()->Call_SetScript_Tutorial_Varg();
	Weak_StaticCast<CBossMonster>(m_pOwner).lock()->Get_HPBar().lock()->Set_Enable(true);
	Get_OwnerCharacter().lock()->Change_State<CVargBossState_WalkF>(1.f);

	GAMEINSTANCE->PlayBGM("Varg_Boss_Phase1.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM), 1.f);
}
 
void CVargBossState_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Start::Free()
{
}

_bool CVargBossState_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	_uint CurAnimationKey = m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex();

	if (CurAnimationKey == 1210)
	{
		weak_ptr<CUI_ScriptQueue> pScriptQeuue = GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL_STATIC).front();
		//pScriptQeuue.lock()->Call_SetScript_Tutorial_Varg_Appear();
	}

	return false;
}

