#include "stdafx.h"
#include "BossVarg/VargBossState_RaidAttack.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Effect_Decal.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"
#include "../Public/BossVarg/Varg.h"
#include "VargWeapon.h"
#include "MobWeapon.h"
#include "PhysXCharacterController.h"

GAMECLASS_C(CVargBossState_RaidAttack);
CLONE_C(CVargBossState_RaidAttack, CComponent)

void CVargBossState_RaidAttack::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 86:
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(true);
		break;
	case 90:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.4f, 1.f, 9.f, 0.1f);
		break;
	case 93:
	{
		Weak_Cast<CVarg>(m_pOwner).lock()->Set_TrailEnable(false);
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.6f, 1.f, 9.f, 0.7f);

		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		XMStoreFloat4x4(&m_DecalDesc.WorldMatrix, OwnerWorldMatrix);

		GAMEINSTANCE->Add_GameObject<CEffect_Decal>(m_CreatedLevel, &m_DecalDesc);
		break;
	}
	}
}

HRESULT CVargBossState_RaidAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_RaidAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_vShakingOffSet = { 0.f, 1.f, 0.f };
	return S_OK;
}

void CVargBossState_RaidAttack::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_RaidAttack2");
	m_bAttackLookAtLimit = true;
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVargBossState_RaidAttack::Call_AnimationEnd, this, placeholders::_1);

	m_DecalDesc.vScale = { 3.4f,3.4f, 0.1f };
	m_DecalDesc.vPosition = { -0.687,0.f,3.187f, 1.f };
	m_DecalDesc.fTime = 1.f;
	m_DecalDesc.fDisapearTime = 2.f;
	//1Æä µ¥Ä® emissive color
	m_DecalDesc.vColor = _float3(0.1f, 0.1f, 0.1f);
	m_DecalDesc.strTextureTag = "DecalTexture";
}

void CVargBossState_RaidAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (m_bAttackLookAtLimit)
		Turn_ToThePlayer(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_RaidAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if(m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}



void CVargBossState_RaidAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
	for (auto& elem : pWeapons)
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.f);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CVargBossState_RaidAttack::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CVargBossState_RaidAttack::Call_OtherControllerHit, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: RaidAttack -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CVargBossState_RaidAttack::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CVargBossState_RaidAttack::Call_NextKeyFrame, this, placeholders::_1);
	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CVargBossState_RaidAttack::Call_OtherControllerHit, this, placeholders::_1);
}



void CVargBossState_RaidAttack::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CVargBossState_Idle>(0.05f);
}

void CVargBossState_RaidAttack::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVargBossState_RaidAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CVargBossState_RaidAttack::Free() { }

_bool CVargBossState_RaidAttack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.2f)
		m_bAttackLookAtLimit = false;

	return false;
}

