#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_UrdSword.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CCorvusState_PS_UrdSword);
CLONE_C(CCorvusState_PS_UrdSword, CComponent)

void CCorvusState_PS_UrdSword::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_UrdSword::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	}
}

HRESULT CCorvusState_PS_UrdSword::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_UrdSword::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_UrdSword::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_UrdSword01");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_UrdSword::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_UrdSword::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_UrdSword::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCorvusState_PS_UrdSword::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
}

void CCorvusState_PS_UrdSword::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_PS_UrdSword::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_UrdSword::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_UrdSword::Call_AnimationEnd, this, placeholders::_1);
}
