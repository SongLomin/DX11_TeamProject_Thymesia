#include "stdafx.h"
#include "NorMonState/NorMonState_Awake.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateBase.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_Awake);
CLONE_C(CNorMonState_Awake, CComponent)

HRESULT CNorMonState_Awake::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Awake::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Awake::Start()
{
	__super::Start();

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_SP_Idle1_End|BaseLayer");
	}
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Awake::Call_AnimationEnd, this);

}

void CNorMonState_Awake::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Awake::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_Awake::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_Awake::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_Awake::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Awake::Call_AnimationEnd, this);
}


void CNorMonState_Awake::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	
	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_AnimIndex(0);
	Get_OwnerCharacter().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_MonIdleType(NORMONSTERIDLETYPE::NORIDLE);
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



_bool CNorMonState_Awake::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

void CNorMonState_Awake::Free()
{

}