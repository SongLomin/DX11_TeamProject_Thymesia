#include "stdafx.h"
#include "NorMonState/NorMonState_Walk_B.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"



GAMECLASS_C(CNorMonState_Walk_B);
CLONE_C(CNorMonState_Walk_B, CComponent)

HRESULT CNorMonState_Walk_B::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Walk_B::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Walk_B::Start()
{
	__super::Start();

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock())
	{
		m_bPlayerToClose = true;
	}
	else
	{
		m_bPlayerToClose = false;
	}

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkB|BaseLayer|Armatu");
	}

}

void CNorMonState_Walk_B::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);
	_vector vDirLook = Get_CurMonToStartMonDir();

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	
	m_pTransformCom.lock()->Add_Position(vDirLook * m_fCurrentSpeed * fTimeDelta);
}

void CNorMonState_Walk_B::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Walk_B::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: BBBBBBBBBBBB -> BBBBBBBBBBBB" << endl;
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

	m_bAttackLookAtLimit = true;
}

void CNorMonState_Walk_B::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}




void CNorMonState_Walk_B::Free()
{

}

_bool CNorMonState_Walk_B::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리
	_float fMToMDistance = GetStartPositionToCurrentPositionDir(); // 몬스터스타트포지션과 몬스터현재 포지션 사이의 거리

	
	if(!m_bPlayerToClose && fPToMDistance <= 3.f)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_IdleType(2);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_ClosePlayerCheck(true);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_CloseToRun(true);
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}

	if (!m_bPlayerToClose && fMToMDistance <= 13.f)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(false);
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
		return true;
	}

	if (m_bPlayerToClose && m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.99f)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}


	

	return false;

}

