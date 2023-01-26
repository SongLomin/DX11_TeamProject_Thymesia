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
#include "MobWeapon.h"
#include "PhysXController.h"

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

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_AttackR_01_2a");

	m_pLeftHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_l");
	m_pRightHandBoneNode = m_pModelCom.lock()->Find_BoneNode("hand_r");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Atk_R01_2a::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Atk_R01_2a::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bRootStop)
	{
		_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("root_$AssimpFbx$_Translation");

		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation(vMoveDir, 0.f, 1.f, Filters, nullptr, m_pTransformCom);
	}


	if (m_bTurnTuning)
	{
		_float fTurnValue = 1.77f / 2.4;

		m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue);
	}
	
	if (m_bWaterWave)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc >= 0.03f)
		{
			_matrix CombinedMatrix = Get_RightHandCombinedWorldMatrix();

			_vector vPosition = CombinedMatrix.r[3];
			GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);

			CombinedMatrix = Get_LeftHandCombinedWorldMatrix();

			vPosition = CombinedMatrix.r[3];
			GET_SINGLE(CGameManager)->Add_WaterWave(vPosition, 0.1f, 9.f, 3.f);
			m_fTimeAcc = 0.f;

		}
	}
		
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Atk_R01_2a::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CBatBossState_Atk_R01_2a::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
	//172~195
	if(In_iKeyIndex == 172 || In_iKeyIndex == 195)
	{
		m_bWaterWave = !m_bWaterWave;
		return;
	}

}

void CBatBossState_Atk_R01_2a::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bRootStop = true;

	m_bOne = true;

	m_bTurnTuning = false;

	weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);

	list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_WeaponDesc(HIT_TYPE::NORMAL_HIT, 1.8f);
	}

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBatBossState_Atk_R01_2a::Call_NextAnimationKey, this, placeholders::_1);

	m_bWaterWave = false;
	m_fTimeAcc = 0.f;

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif

}	


void CBatBossState_Atk_R01_2a::OnStateEnd()
{
	__super::OnStateEnd();

	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBatBossState_Atk_R01_2a::Call_NextAnimationKey, this, placeholders::_1);

}


void CBatBossState_Atk_R01_2a::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
	Get_Owner().lock()->Get_Component<CBatBossState_HellIdle>().lock()->Set_AttackCount(1);
	Get_Owner().lock()->Get_Component<CBatBossState_AttackIdle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Atk_R01_2a::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Atk_R01_2a::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Atk_R01_2a::Free()
{

}

_bool CBatBossState_Atk_R01_2a::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 6.5f && m_bOne)
	{
		m_bRootStop = false;
		m_bOne = false;
	}



	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 144 &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() < 288)
	{
		m_bTurnTuning = true;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 288)
	{
		m_bTurnTuning = false;
	}



	
	return false;
}

