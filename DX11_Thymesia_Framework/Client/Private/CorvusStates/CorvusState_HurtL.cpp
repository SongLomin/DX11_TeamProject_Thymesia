#include "stdafx.h"
#include "CorvusStates/CorvusState_HurtL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXController.h"
#include "Weapon.h"
#include "MobWeapon.h"

GAMECLASS_C(CCorvusState_HurtL);
CLONE_C(CCorvusState_HurtL, CComponent)

HRESULT CCorvusState_HurtL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_HurtL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_HurtL::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_HurtMFL");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_HurtL::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_HurtL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_HurtL::OnDisable()
{

}

void CCorvusState_HurtL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


	weak_ptr<CPlayer> pPlayer = Weak_Cast<CPlayer>(m_pOwner);
	list<weak_ptr<CWeapon>>	pWeapons = pPlayer.lock()->Get_Weapon();

	pWeapons.front().lock()->Set_RenderOnOff(true);


	m_pPhysXControllerCom.lock()->Set_EnableSimulation(true);
	m_pPhysXControllerCom.lock()->Enable_Gravity(true);
	m_pPhysXControllerCom.lock()->Set_EnableColliderSimulation(true);


	_float3 vRandDir = SMath::vRandom(_float3(-1.f, -1.f, 0.f), _float3(1.f, 1.f, 0.f));

	_matrix WorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
	GET_SINGLE(CGameManager)->Add_Shaking(XMVector3TransformNormal(XMLoadFloat3(&vRandDir), WorldMatrix), 0.2f, 1.f, 9.f, 0.4f);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif
}

void CCorvusState_HurtL::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_HurtL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_HurtL::OnDestroy()
{

	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_HurtL::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtL::Free()
{
	
}

_bool CCorvusState_HurtL::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.15f)
	{
		if (Check_RequirementParryState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
			return true;
		}

		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}

	return false;
}


