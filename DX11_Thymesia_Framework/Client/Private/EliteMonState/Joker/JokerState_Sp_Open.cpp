#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Sp_Open.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"

GAMECLASS_C(CJokerState_Sp_Open);
CLONE_C(CJokerState_Sp_Open, CComponent)

void CJokerState_Sp_Open::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 33:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.3f, 1.f, 9.f, 0.25f);
		break;
	}
}

HRESULT CJokerState_Sp_Open::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_Sp_Open::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CJokerState_Sp_Open::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_SP_Open");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_Sp_Open::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_Sp_Open::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if(m_bCloseStart)
		m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CJokerState_Sp_Open::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}



void CJokerState_Sp_Open::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	


	m_bCloseStart = false;
	m_bOnce = true;

#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CJokerState_Sp_Open::OnStateEnd()
{
	__super::OnStateEnd();
	GET_SINGLE(CGameManager)->Store_EffectIndex("Joker_Passive", GET_SINGLE(CGameManager)->Use_EffectGroup("Joker_Passive", m_pTransformCom, _uint(TIMESCALE_LAYER::MONSTER)));
}

void CJokerState_Sp_Open::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Idle>(0.05f);
}

void CJokerState_Sp_Open::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_Sp_Open::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_Sp_Open::Free()
{
}

_bool CJokerState_Sp_Open::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 10.f)
		m_bCloseStart = true;

	if (m_bCloseStart && m_bOnce)
	{
		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
		XMStoreFloat4x4(&m_vPlyerMatrix, pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix());
		m_bOnce = false;
	}

	return false;
}

