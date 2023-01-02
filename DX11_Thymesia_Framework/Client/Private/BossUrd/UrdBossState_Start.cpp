#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "MobWeapon.h"
#include "UrdWeapon.h"

GAMECLASS_C(CUrdBossState_Start);
CLONE_C(CUrdBossState_Start, CComponent)

HRESULT CUrdBossState_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Start::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Seq_BossFightStart_1|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Start::Call_AnimationEnd, this);
}

void CUrdBossState_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_bSinematicStart)
	{
		_matrix LocalMat = XMMatrixIdentity();
		LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
		LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));
		GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera", LocalMat, CINEMATIC_TYPE::CINEMATIC);

		m_pModelCom.lock()->Play_Animation(fTimeDelta);
	}

}


void CUrdBossState_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CUrdBossState_Start::OnStateEnd()
{
	__super::OnStateEnd();
	GET_SINGLE(CGameManager)->End_Cinematic();
}



void CUrdBossState_Start::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Start::Call_AnimationEnd, this);
}

void CUrdBossState_Start::Free()
{

}

_bool CUrdBossState_Start::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	
	if (fPToMDistance <= 15.f)
	{
		m_bSinematicStart = true;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 500)
	{

		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Wepons();
		pWeapons.front().lock()->Weapon_BoneChange(m_pModelCom, "weapon_r");


	}
	
	GET_SINGLE(CGameManager)->Activate_Section(200, EVENT_TYPE::ON_LOCK_SECTION);
	GET_SINGLE(CGameManager)->Activate_Fog(1);
	
	return false;
}

