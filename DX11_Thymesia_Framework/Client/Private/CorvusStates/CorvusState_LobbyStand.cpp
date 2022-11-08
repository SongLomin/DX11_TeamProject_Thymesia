#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_LobbyStand.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"

GAMECLASS_C(CLuxiyaState_LobbyStand);
CLONE_C(CLuxiyaState_LobbyStand, CComponent)

HRESULT CLuxiyaState_LobbyStand::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CLuxiyaState_LobbyStand::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 35;
	return S_OK;
}

void CLuxiyaState_LobbyStand::Start()
{
	__super::Start();
}

void CLuxiyaState_LobbyStand::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
}

void CLuxiyaState_LobbyStand::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CLuxiyaState_LobbyStand::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: LobbyStand -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_LobbyStand::OnStateEnd()
{
	__super::OnStateEnd();
}

void CLuxiyaState_LobbyStand::Free()
{
}

_bool CLuxiyaState_LobbyStand::Check_AndChangeNextState()
{
	return false;
}

