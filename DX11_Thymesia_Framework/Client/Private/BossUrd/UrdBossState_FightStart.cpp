#include "stdafx.h"
#include "BossUrd/UrdBossState_FightStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "Effect_Decal.h"

GAMECLASS_C(CUrdBossState_FightStart);
CLONE_C(CUrdBossState_FightStart, CComponent)


void CUrdBossState_FightStart::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 23:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		XMStoreFloat4x4(&m_DecalDesc.WorldMatrix, OwnerWorldMatrix);

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);

		break;
	}
	}
}

HRESULT CUrdBossState_FightStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_FightStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_FightStart::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Seq_BossFightStart_1|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_FightStart::Call_AnimationEnd, this, placeholders::_1);

	m_DecalDesc.vScale = { 10.f,10.f, 5.f };
	m_DecalDesc.vPosition = { -0.027f,0.f,2.017f, 1.f };
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 1.f;
	//1Æä µ¥Ä® emissive color
	m_DecalDesc.vColor = _float3(1.f, 1.f, 1.f);
	m_DecalDesc.strTextureTag = "DecalUrd";
}

void CUrdBossState_FightStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_FightStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_FightStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CUrdBossState_FightStart::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_FightStart::OnStateEnd()
{
	__super::OnStateEnd();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CUrdBossState_FightStart::Call_NextKeyFrame, this, placeholders::_1);

}



void CUrdBossState_FightStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_FightStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_FightStart::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_FightStart::Free()
{

}

_bool CUrdBossState_FightStart::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

