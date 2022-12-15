#include "stdafx.h"
#include "BossBat/BatBossState_HurtXL_F.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_HurtXL_F);
CLONE_C(CBatBossState_HurtXL_F, CComponent)

HRESULT CBatBossState_HurtXL_F::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_HurtXL_F::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_HurtXL_F::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_HurtXL_F");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_HurtXL_F::Call_AnimationEnd, this);
}

void CBatBossState_HurtXL_F::Tick(_float fTimeDelta)
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


void CBatBossState_HurtXL_F::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pModelCom.lock()->Set_AnimationSpeed(m_fSinematic);

	Check_AndChangeNextState();
}



void CBatBossState_HurtXL_F::OnStateStart(const _float& In_fAnimationBlendTime)
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


void CBatBossState_HurtXL_F::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

	if(m_fSinematic == 4.f)
	GET_SINGLE(CGameManager)->End_Cinematic();

}



void CBatBossState_HurtXL_F::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_HurtXL_F::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_HurtXL_F::Call_AnimationEnd, this);
}

void CBatBossState_HurtXL_F::Free()
{

}

_bool CBatBossState_HurtXL_F::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

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

