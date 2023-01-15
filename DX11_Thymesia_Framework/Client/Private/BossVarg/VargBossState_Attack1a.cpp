#include "stdafx.h"
#include "BossVarg/VargBossState_Attack1a.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "../Public/BossVarg/Varg.h"
#include "VargWeapon.h"
#include "Effect_Decal.h"
#include "MobWeapon.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_Attack1a);
CLONE_C(CVargBossState_Attack1a, CComponent)

void CVargBossState_Attack1a::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 41:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 45:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.5f, 1.f, 9.f, 0.7f);

		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		XMStoreFloat4x4(&m_DecalDesc.WorldMatrix, OwnerWorldMatrix);

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);
		break;
	case 46:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		break;
	}
}

HRESULT CVargBossState_Attack1a::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Attack1a::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = { 0.f, -1.f, 0.f };
	return S_OK;
}

void CVargBossState_Attack1a::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_ComboAttack1_1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_Attack1a::Call_AnimationEnd, this, placeholders::_1);

	m_DecalDesc.vScale = { 3.4f,3.4f, 0.1f};
	m_DecalDesc.vPosition = { -0.18f,0.f,2.540f, 1.f };
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 2.f;
	//1∆‰ µ•ƒÆ emissive color
	m_DecalDesc.vColor = _float3(0.1f, 0.1f, 0.1f);
	m_DecalDesc.strTextureTag = "DecalTexture";
}

void CVargBossState_Attack1a::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Attack1a::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_bAttackLookAtLimit)
		TurnAttack(fTimeDelta);

	Check_AndChangeNextState();
}

void CVargBossState_Attack1a::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	m_bAttackLookAtLimit = true;  // æ÷¥œ∏ﬁ¿Ãº«Ω√¿€«“ãö ∑ËøßΩ√¿€
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	for (auto& elem : pWeapons)
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.1f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CVargBossState_Attack1a::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit +=
		bind(&CVargBossState_Attack1a::Call_OtherControllerHit, this, placeholders::_1);
#ifdef _DEBUG_COUT_
	cout << "VargState: Attack1a -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_Attack1a::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CVargBossState_Attack1a::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit -=
		bind(&CVargBossState_Attack1a::Call_OtherControllerHit, this, placeholders::_1);
}



void CVargBossState_Attack1a::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
}

void CVargBossState_Attack1a::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_Attack1a::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_Attack1a::Free()
{

}

_bool CVargBossState_Attack1a::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		int iRand = rand() % 3;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack2a>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack3a>(0.05f);
			break;
		case 2:
			Get_OwnerCharacter().lock()->Change_State<CVargBossState_Attack1b>(0.05f);
			break;
		}
		return true;
	}


	return false;
}

