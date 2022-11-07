#include "stdafx.h"
#include "SkillChain.h"
#include "GameInstance.h"
#include "SkillBall.h"
#include "Skill.h"
#include "FadeMask.h"

#define MAX_SKILLBALL	16
#define MAX_VIEWSKILLBALL 8
#define MAX_SKILL		3

GAMECLASS_C(CSkillChain)
CLONE_C(CSkillChain, CComponent)

HRESULT CSkillChain::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSkillChain::Initialize(void* pArg)
{
	USE_START(CSkillChain);

	m_pSkills.reserve(MAX_SKILL);

	/*for (int i = 0; i < MAX_SKILLBALL; i++)
	{
		m_pSkillBalls.push_back({});
	}*/

	return S_OK;
}

void CSkillChain::Start()
{
	USE_TICK(CSkillChain)
}

void CSkillChain::Tick(_float fTimeDelta)
{
	if (Get_Enable() == false)
		return;

	/*m_fCurCreateBallTime -= fTimeDelta;

	if (m_fCurCreateBallTime < 0.f)
	{
		Add_RendomSkillBall();
		m_fCurCreateBallTime = m_fMaxCreateBallTime;
	}*/

	if (KEY_INPUT(KEY::E, KEY_STATE::TAP))
	{
		Add_RendomSkillBall();
	}

	for (_uint i = 0; i < 8; i++)
	{
		if (KEY_INPUT(KEY((_uint)KEY::NUM1 + i), KEY_STATE::TAP))
		{
			Command_Skill(i);
		}
	}


}

void CSkillChain::Set_EnableSkillBall(const _bool& In_Active)
{
	if (In_Active)
	{
		ReDraw_AnchorBalls();
	}

	else
	{
		for (auto& elem_SkillBall : m_pSkillBalls)
		{
			if (!elem_SkillBall.pInstance.lock().get())
				continue;

			elem_SkillBall.pInstance.lock()->Set_Enable(In_Active);
		}
	}

	

}

void CSkillChain::Add_SkillBall(_uint Index)
{
	_size_t iSkillBallCounts = m_pSkillBalls.size();

	if (m_pSkills.size() <= Index)
	{
		//등록된 스킬 개수보다 높은 인덱스 접근
		assert(false);
	}

	if (iSkillBallCounts >= MAX_SKILLBALL)
	{
		return;
	}

	_float FarSkillBallPositionX = 0.f;

	if (!m_pSkillBalls.empty())
	{
		FarSkillBallPositionX = m_pSkillBalls.back().pInstance.lock()->Get_PositionX();
	}

	//최소한 화면 밖 생성 보장
	FarSkillBallPositionX += 120.f;

	FarSkillBallPositionX = max((_float)g_iWinCX, FarSkillBallPositionX);


	m_pSkillBalls.push_back({ m_pSkills[Index].lock()->Add_SkillBall(FarSkillBallPositionX), Index });

	Set_AnchorBalls();
	

}

void CSkillChain::Add_RendomSkillBall()
{
	if (m_pSkills.empty())
		return;

	_int iRandomIndex = rand() % m_pSkills.size();

	Add_SkillBall(iRandomIndex);
}

void CSkillChain::Register_Skill(weak_ptr<CSkill> pSkill)
{
	m_pSkills.push_back(pSkill);
}

void CSkillChain::Command_Skill(_int Index)
{
	if (Index < 0)
	{
		DEBUG_ASSERT;
	}

	if (Index >= m_pSkillBalls.size())
	{
		return;
	}

	_int iSpace = Index;

	_bool bIsActive = true;
	CHAIN_TYPE eChainType = CHAIN_END;
	_uint iMax = 0;

	list<SKILLBALLDESC>::iterator iter = m_pSkillBalls.begin();

	for (iter; iter != m_pSkillBalls.end();)
	{
		if (0 < iSpace)
		{
			--iSpace;
			++iter;
		}
		else
		{
			break;
		}
	}

	//해당 인덱스에 도착했음.

	_uint iSkillChainCounts[CHAIN_END];

	iSkillChainCounts[CHAIN_LEFT] = Search_SameSkillBall_Recursive(iter->iSkillIndex, iter, 2, false);
	iSkillChainCounts[CHAIN_RIGHT] = Search_SameSkillBall_Recursive(iter->iSkillIndex, iter, 2, true);
	iSkillChainCounts[CHAIN_ADJ] = Search_SameSkillBall_Recursive(iter->iSkillIndex, iter, 1, false)
		+ Search_SameSkillBall_Recursive(iter->iSkillIndex, iter, 1, true) - 1;



	for (_uint i = 0; i < (_uint)CHAIN_END; ++i)
	{
		if (iSkillChainCounts[i] > iMax)
		{
			iMax = iSkillChainCounts[i];
			eChainType = (CHAIN_TYPE)i;
		}
	}

	//같은 스킬볼 서치는 최대 3개만 나와야한다.
	if (iMax > 3)
	{
		DEBUG_ASSERT;
	}

	iter->pInstance.lock()->Command_Skill(iMax, bIsActive);
	CallBack_CommandSkill();

	//같은 스킬볼이 0개보다 큰게 없을 경우
	if (CHAIN_END == eChainType)
	{
		DEBUG_ASSERT;
	}

	//사용 허가됐다. 스킬볼을 지운다.
	if (bIsActive)
	{
		switch (eChainType)
		{
		case Client::CSkillChain::CHAIN_LEFT:
			for (_uint i = 0; i < iMax; ++i)
			{
				iter->pInstance.lock()->Use_SkillBall(i*0.05f);
				iter->pInstance.lock()->Set_Dead();
				iter = m_pSkillBalls.erase(iter);

				if(m_pSkillBalls.begin() != iter)
					iter--;
			}
			break;

		case Client::CSkillChain::CHAIN_RIGHT:
			for (_uint i = 0; i < iMax; ++i)
			{
				iter->pInstance.lock()->Use_SkillBall(i * 0.05f);
				iter->pInstance.lock()->Set_Dead();
				iter = m_pSkillBalls.erase(iter);
			}
			break;

		case Client::CSkillChain::CHAIN_ADJ:
			iter->pInstance.lock()->Use_SkillBall(0.f);
			iter->pInstance.lock()->Set_Dead();
			iter = m_pSkillBalls.erase(iter);

			iter->pInstance.lock()->Use_SkillBall(0.05f);
			iter->pInstance.lock()->Set_Dead();
			iter = m_pSkillBalls.erase(iter);

			iter--;
			iter->pInstance.lock()->Use_SkillBall(0.1f);
			iter->pInstance.lock()->Set_Dead();
			iter = m_pSkillBalls.erase(iter);
			break;

		case Client::CSkillChain::CHAIN_END:

			break;

		default:
			break;
		}
	}

	Set_AnchorBalls();

	//for (auto iter = m_pSkillBalls.begin(); iter != m_pSkillBalls.end();)
	//{
	//	if (Index < 0)
	//	{
	//		iter->pInstance.lock()->Command_Skill((_uint)iter->size(), bIsActive);

	//		//요구사항 통과 스킬 발동됨.
	//		if (bIsActive)
	//		{
	//			m_iCurBallCnt -= (_int)iter->size();

	//			for (auto& elem : *iter)
	//			{
	//				elem.pInstance.lock()->Set_Dead();
	//			}

	//			iter = m_pSkillBalls.erase(iter);

	//			if (m_pSkillBalls.size() == 0)
	//			{
	//				m_pSkillBalls.push_back({});
	//			}

	//			m_iter_SkillBalls = --m_pSkillBalls.end();
	//			
	//			Set_AnchorBalls();
	//			Sort_SkillBallGroup();
	//			break;
	//		}
	//	}

	//	iter++;
	//}

}

void CSkillChain::Set_AnchorBalls()
{
	_uint index = 0;
	for (auto& elem : m_pSkillBalls)
	{
		if (index < MAX_VIEWSKILLBALL)
		{
			elem.pInstance.lock()->Set_Enable(true);
			elem.pInstance.lock()->Set_Anchor(((_float)index * 100) + 100);
		}

		else
		{
			elem.pInstance.lock()->Set_Enable(false);
		}

		index++;
	}



}

void CSkillChain::ReDraw_AnchorBalls()
{
	_float FarSkillBallPositionX = 0.f;

	FarSkillBallPositionX = max((_float)g_iWinCX, FarSkillBallPositionX);

	_uint index = 0;
	for (auto& elem : m_pSkillBalls)
	{
		elem.pInstance.lock()->Init_PositionX(FarSkillBallPositionX);

		if (index < MAX_VIEWSKILLBALL)
		{
			elem.pInstance.lock()->Set_Enable(true);
			elem.pInstance.lock()->Set_Anchor(((_float)index * 100) + 100);
		}

		else
		{
			elem.pInstance.lock()->Set_Enable(false);
		}

		index++;

		FarSkillBallPositionX += 120.f;
	}


	


}

_uint CSkillChain::Search_SameSkillBall_Recursive(const _uint& iSkillIndex, list<SKILLBALLDESC>::iterator iter, _uint iDepth, const _bool& isRight)
{
	_uint iSame = (_uint)(iSkillIndex == iter->iSkillIndex);

	if (0 == iSame)
	{
		return iSame;
	}

	if (isRight)
	{
		++iter;
		if (m_pSkillBalls.end() == iter)
			return 0;
	}
	else
	{
		if (m_pSkillBalls.begin() == iter)
		{
			return iSame;
		}

		--iter;
	}

	if (0 == iDepth)
	{
		return iSame;
	}

	return iSame + Search_SameSkillBall_Recursive(iSkillIndex, iter, --iDepth, isRight);
}

void CSkillChain::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITINIT == iArg)
	{
		Set_Enable(false);
	}

}

void CSkillChain::OnDestroy()
{
	UNUSE_TICK(CSkillChain)
}

void CSkillChain::Free()
{	
}
