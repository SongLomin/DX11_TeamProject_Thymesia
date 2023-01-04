#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Magician.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_Magician);
CLONE_C(CCorvusState_PS_Magician, CComponent)

void CCorvusState_PS_Magician::Call_AnimationEnd()
{
	__super::Call_AnimationEnd();
}

void CCorvusState_PS_Magician::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{

	}
}

HRESULT CCorvusState_PS_Magician::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Magician::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_Magician::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Magician_A");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Magician::Call_AnimationEnd, this);
}

void CCorvusState_PS_Magician::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_Magician::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCorvusState_PS_Magician::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
}

void CCorvusState_PS_Magician::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_PS_Magician::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_Magician::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Magician::Call_AnimationEnd, this);
}
