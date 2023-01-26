#include "stdafx.h"
#include "BossUrd/UrdBossState_Attacks.h"
#include "GameManager.h"
#include "BossUrd/Urd.h"
#include "Status_Boss.h"

GAMECLASS_C(CUrdBossState_Attacks);

HRESULT CUrdBossState_Attacks::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attacks::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CUrdBossState_Attacks::Start()
{
	__super::Start();
}

void CUrdBossState_Attacks::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta * m_fTurnAtkSpeedRatio);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CUrdBossState_Attacks::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CUrdBossState_Attacks::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
}

void CUrdBossState_Attacks::OnStateEnd()
{
	__super::OnStateEnd();
}

void CUrdBossState_Attacks::Set_MoveScale(const _float fRatio)
{
	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(fRatio, fRatio, fRatio));
}

_bool CUrdBossState_Attacks::Is_Urd_Phase1()
{
	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();
	_uint iLifeCount(pStatus.lock()->Get_Desc().m_iLifeCount);
	return ((2 == iLifeCount) ? true : false);
}

void CUrdBossState_Attacks::TurnOn_Effect(const std::string& szEffectKey)
{
	GET_SINGLE(CGameManager)->Store_EffectIndex(szEffectKey.c_str(), GET_SINGLE(CGameManager)->Use_EffectGroup(szEffectKey, m_pTransformCom, _uint(TIMESCALE_LAYER::MONSTER)));
}

void CUrdBossState_Attacks::TurnOff_Effect(const std::string& szEffectKey)
{
	GET_SINGLE(CGameManager)->UnUse_EffectGroup(szEffectKey, GET_SINGLE(CGameManager)->Get_StoredEffectIndex(szEffectKey.c_str()));
}

void CUrdBossState_Attacks::OnDestroy()
{
}

void CUrdBossState_Attacks::Free()
{
}