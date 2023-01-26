#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"
#include "UrdWeapon.h"
#include "UI_ScriptQueue.h"
#include "UIManager.h"

GAMECLASS_C(CUrdBossState_Start);
CLONE_C(CUrdBossState_Start, CComponent)

HRESULT CUrdBossState_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Start::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Seq_BossFightStart_1|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_bSinematicStart)
	{
		_matrix LocalMat = XMMatrixIdentity();
		LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
		LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));
		GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);

		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}

}


void CUrdBossState_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CUrdBossState_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	m_bSinematicStart = false;

	XMStoreFloat4x4(&m_vPlyerMatrix, pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix());


	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);

	

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);

	

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CUrdBossState_Start::OnStateEnd()
{
	__super::OnStateEnd();

	GAMEINSTANCE->PlayBGM("STAGE3_BOSS_0.mp3", 3.f, 1.f);

	if(m_bSinematicStart)
	GET_SINGLE(CGameManager)->End_Cinematic();

	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
	m_bSinematicStart = false;
}



void CUrdBossState_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_WalkF>(0.05f);

	GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL::LEVEL_STATIC).front().lock()->Call_SetScript_Urd_Appear();
}

void CUrdBossState_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Start::Free()
{

}

_bool CUrdBossState_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	
	if (fPToMDistance <= 20.f && !m_bSinematicStart)
	{
		m_bSinematicStart = true;
		_float fSound = GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_BGM);

		GET_SINGLE(CGameManager)->Activate_Section(200, EVENT_TYPE::ON_ENTER_SECTION);
		GET_SINGLE(CGameManager)->Activate_Fog(1);
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 500)
	{
		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
		pWeapons.front().lock()->Weapon_BoneChange(m_pModelCom, "weapon_r");
	}
	
	return false;
}

void CUrdBossState_Start::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);


	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_bSinematicStart = false;
	}
}

