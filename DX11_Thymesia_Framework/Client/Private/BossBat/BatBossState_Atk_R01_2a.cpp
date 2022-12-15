#include "stdafx.h"
#include "BossBat/BatBossState_Atk_R01_2a.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Atk_R01_2a);
CLONE_C(CBatBossState_Atk_R01_2a, CComponent)

HRESULT CBatBossState_Atk_R01_2a::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Atk_R01_2a::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Atk_R01_2a::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_AttackR_01_2a");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Atk_R01_2a::Call_AnimationEnd, this);
}

void CBatBossState_Atk_R01_2a::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));

	if (m_fSinematic == 4.f)
	{
		GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Atk_R01_2a::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pModelCom.lock()->Set_AnimationSpeed(m_fSinematic);

	Check_AndChangeNextState();
}



void CBatBossState_Atk_R01_2a::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	m_pModelCom.lock()->Set_AnimationSpeed(m_fSinematic);

}	


void CBatBossState_Atk_R01_2a::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

	if(m_fSinematic == 4.f)
	GET_SINGLE(CGameManager)->End_Cinematic();

}



void CBatBossState_Atk_R01_2a::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Atk_R01_2a::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Atk_R01_2a::Call_AnimationEnd, this);
}

void CBatBossState_Atk_R01_2a::Free()
{

}

_bool CBatBossState_Atk_R01_2a::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�

	//if (fPToMDistance <= 8.f)
	//{
	//	m_bNextState = true;
	//}
	if (fPToMDistance <= 10.f)
	{
		m_fSinematic = 4.f;
	}



	return false;
}

