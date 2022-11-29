#include "stdafx.h"
#include "CorvusStates/Talent_Sword.h"
#include "GameManager.h"
#include "GameInstance.h"
#include "CorvusStates/Talent.h"
#include "CorvusStates/Talent_Effect_NorSwordLV1.h"
#include "CorvusStates/Talent_Effect_AvoidSlashLV1.h"
#include "CorvusStates/Talent_Effect_AvoidSlashLV2.h"
#include "CorvusStates/Talent_Effect_AvoidThrustLV1.h"
#include "CorvusStates/Talent_Effect_AvoidThrustLV2.h"
#include "Player.h"

GAMECLASS_C(CTalent_Sword)
CLONE_C(CTalent_Sword, CGameObject)

HRESULT CTalent_Sword::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}
HRESULT CTalent_Sword::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	//1에대한 정보넣어줘야함.
	//m_bActive = true;
	//m_pChilds.push_back(pSwordAttack2);
	m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	m_bActive = true;
	Add_TalentChild(pSwordLV2);
	//컴포넌트의 오너는, 게임오브젝트여야 해요.
	//기본적으로, 컴포넌트가 컴포넌트를 소유하는건 X임.

	

	pSwordLV2.lock()->Set_TalentInfo(true , Add_Component<CTalent_Effect_NorSwordLV1>(), Weak_Cast<CTalent>(m_this));
	pSwordLV2.lock()->Add_TalentChild(pAvoidSlashLV1);
	pSwordLV2.lock()->Add_TalentChild(pAvoidThrustLV1);
		
	pAvoidSlashLV1.lock()->Set_TalentInfo(true, Add_Component<CTalent_Effect_AvoidSlashLV1>(), pSwordLV2);
	pAvoidSlashLV1.lock()->Add_TalentChild(pAvoidSlashLV2);
	
	pAvoidSlashLV2.lock()->Set_TalentInfo(true, Add_Component<CTalent_Effect_AvoidSlashLV2>(), pAvoidSlashLV1);
		
	pAvoidThrustLV1.lock()->Set_TalentInfo(false, Add_Component<CTalent_Effect_AvoidThrustLV1>(), pSwordLV2);
	pAvoidThrustLV1.lock()->Add_TalentChild(pAvoidThrustLV2);
		
	pAvoidThrustLV2.lock()->Set_TalentInfo(false, Add_Component<CTalent_Effect_AvoidThrustLV2>(), pAvoidThrustLV1);

	return S_OK;
}

void  CTalent_Sword::TestTalentCheck()
{
	int iCost = 0;
	int iMyTalentPoint = 5;


	TALENT_RESULT eResult;

	m_pPlayer.lock()->Bind_TalentEffects(pSwordLV2.lock()->Get_Effect());
	list<weak_ptr<CTalent>> pVisitNodes;
	eResult = pSwordLV2.lock()->Check_Requiment(iMyTalentPoint, iCost, pVisitNodes);

	switch (eResult)
	{
	case Client::TALENT_RESULT::FAILED:
		break;
	case Client::TALENT_RESULT::NOT_ENOUGHTPOINT:
		break;
	case Client::TALENT_RESULT::USING_ATHORTREE:
		break;
	case Client::TALENT_RESULT::SUCCESS:
		iMyTalentPoint -= iCost;
		for (auto& elem : pVisitNodes)
		{
			elem.lock()->Set_Active(true);
		}
		m_pPlayer.lock()->Bind_TalentEffects(pSwordLV2.lock()->Get_Effect());
		break;
	case Client::TALENT_RESULT::SUBSTARICTPOINT:
		iMyTalentPoint += iCost;
		for (auto& elem : pVisitNodes)
		{
			elem.lock()->Set_Active(false);
		}
		m_pPlayer.lock()->UnBind_TalentEffects(pSwordLV2.lock()->Get_Effect());
		break;
	}
	cout << "iTalent Point : " << iMyTalentPoint<< endl;

}

void CTalent_Sword::Free()
{

}

