#include "stdafx.h"
#include "BossBat/BatBossState_Bite_1.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"

GAMECLASS_C(CBatBossState_Bite_1);
CLONE_C(CBatBossState_Bite_1, CComponent)

HRESULT CBatBossState_Bite_1::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Bite_1::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Bite_1::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_Bite_1");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Bite_1::Call_AnimationEnd, this);
}

void CBatBossState_Bite_1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_Bite_1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBatBossState_Bite_1::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CBatBossState_Bite_1::OnStateEnd()
{
	__super::OnStateEnd();

}



void CBatBossState_Bite_1::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_Bite_1::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Bite_1::Call_AnimationEnd, this);
}

void CBatBossState_Bite_1::Free()
{

}

_bool CBatBossState_Bite_1::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;
}

