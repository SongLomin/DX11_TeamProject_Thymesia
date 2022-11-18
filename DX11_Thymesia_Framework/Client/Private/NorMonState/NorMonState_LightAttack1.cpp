#include "stdafx.h"
#include "NorMonState/NorMonState_LightAttack1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_LightAttack1);
CLONE_C(CNorMonState_LightAttack1, CComponent)

HRESULT CNorMonState_LightAttack1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_LightAttack1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_LightAttack1::Start()
{
	__super::Start();

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Attack01|BaseLayer|Arm");
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_LightAttack1::Call_AnimationEnd, this);
}

void CNorMonState_LightAttack1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_LightAttack1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	
	if(m_bAttackLookAtLimit)
	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}


void CNorMonState_LightAttack1::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	//TODO 야매
	yame = true;
	//Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
}





void CNorMonState_LightAttack1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: Attack1 -> OnStateStart" << endl;
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(2.f);

	m_bAttackLookAtLimit = true;  // 애니메이션시작할떄 룩엣시작
}

void CNorMonState_LightAttack1::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


void CNorMonState_LightAttack1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_LightAttack1::Call_AnimationEnd, this);
}

void CNorMonState_LightAttack1::Free()
{

}

_bool CNorMonState_LightAttack1::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.6f)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_LightAttack2>(0.05f);
		return true;
	}
	



	return false;
}



