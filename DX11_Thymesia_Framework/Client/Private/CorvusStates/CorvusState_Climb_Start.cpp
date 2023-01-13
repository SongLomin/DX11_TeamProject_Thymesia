#include "stdafx.h"
#include "CorvusStates/CorvusState_Climb_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXCharacterController.h"
#include "Weapon.h"

GAMECLASS_C(CCorvusState_Climb_Start);
CLONE_C(CCorvusState_Climb_Start, CComponent)

HRESULT CCorvusState_Climb_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Climb_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_Climb_Start::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Ladder_Climb_Start");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Climb_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Climb_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Climb_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Climb_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	//m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Z);
	Get_OwnerPlayer()->Change_State<CCorvusState_Climb_L_Idle>();

}

void CCorvusState_Climb_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CPlayer> pPlayer = Weak_Cast<CPlayer>(m_pOwner);
	list<weak_ptr<CWeapon>>	pWeapons = pPlayer.lock()->Get_Weapon();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_RenderOnOff(false);
	}

	m_pPhysXControllerCom.lock()->Set_EnableSimulation(false);
	m_pPhysXControllerCom.lock()->Set_EnableColliderSimulation(false);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_RootNode("root_$AssimpFbx$_Translation", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Y | (_byte)ROOTNODE_FLAG::Z);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: Stop -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_Climb_Start::OnStateEnd()
{
	__super::OnStateEnd();

	//m_pModelCom.lock()->Set_RootNode("root", (_byte)ROOTNODE_FLAG::X | (_byte)ROOTNODE_FLAG::Z);
}

void CCorvusState_Climb_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Climb_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Climb_Start::Free()
{
	
}

_bool CCorvusState_Climb_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

