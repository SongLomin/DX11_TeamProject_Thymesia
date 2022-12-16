#include "stdafx.h"
#include "BossBat/BatBossState_Bite_2.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Bite_2);
CLONE_C(CBatBossState_Bite_2, CComponent)

HRESULT CBatBossState_Bite_2::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Bite_2::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Bite_2::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_Bite_2");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Bite_2::Call_AnimationEnd, this);
}

void CBatBossState_Bite_2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	TurnAttack(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Bite_2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Bite_2::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif


}	


void CBatBossState_Bite_2::OnStateEnd()
{
	__super::OnStateEnd();


}



void CBatBossState_Bite_2::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CBatBossState_Idle>().lock()->Set_AttackCount(1);
	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Bite_2::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Bite_2::Call_AnimationEnd, this);
}

void CBatBossState_Bite_2::Free()
{

}

_bool CBatBossState_Bite_2::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	return false;
}

