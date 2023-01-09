#include "stdafx.h"
#include "BossUrd/UrdBossState_Equip_R.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "Weapon.h"
#include "MobWeapon.h"
#include "UrdWeapon.h"

GAMECLASS_C(CUrdBossState_Equip_R);
CLONE_C(CUrdBossState_Equip_R, CComponent)

HRESULT CUrdBossState_Equip_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Equip_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Equip_R::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Equip_R|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Equip_R::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Equip_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Equip_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Equip_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bDisableWeaponCheck = false;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Equip_R::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_Equip_R::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Equip_R::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Equip_R::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Equip_R::Free()
{

}

_bool CUrdBossState_Equip_R::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 13 && !m_bDisableWeaponCheck)
	{

		weak_ptr<CMonster> pMonster = Weak_Cast<CMonster>(m_pOwner);
		list<weak_ptr<CMobWeapon>>	pWeapons = pMonster.lock()->Get_Weapons();
		weak_ptr<CUrd> pUrd = Weak_StaticCast<CUrd>(pMonster).lock();
		list<weak_ptr<CMobWeapon>>	pDecoWeapons = pUrd.lock()->Get_DecoWeapons();

		pWeapons.front().lock()->Set_RenderOnOff(true);

		for (auto& elem : pDecoWeapons)
		{
			if (!Weak_StaticCast<CUrdWeapon>(elem).lock()->Get_UsingCheck())
			{
				Weak_StaticCast<CUrdWeapon>(elem).lock()->Set_RenderOnOff(false);
				Weak_StaticCast<CUrdWeapon>(elem).lock()->Set_UsingCheck(true);
				m_bDisableWeaponCheck = true;
				break;
			}
			
		}

	}

	return false;
}

