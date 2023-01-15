#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillView.h"
#include "PlayerSkillHeader.h"
#include "UIManager.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButtonSlot.h"
#include "UI_EvolveMenu_PlagueWeapon_PlayerSkillSlot.h"
#include "GameManager.h"

GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_SkillView)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_SkillView, CGameObject)


HRESULT CUI_EvolveMenu_PlagueWeapon_SkillView::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillView::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;

	SetUp_UI();
	SetUp_SkillPos();
	SetUp_SkillButtons();
	SetUp_PlayerSkillSlot();
	



	return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillView::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();


	__super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillView::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_SkillPos()
{
	_float2 fStartPos;
	_float	fOffsetX = 175.f;
	fStartPos = { 221.f, 267.f };

	for (_uint i = 0; i < 3; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}

	fStartPos = { 150.f, 384.f};
	fOffsetX = 156.f;
	for (_uint i = 0; i < 4; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}

	fStartPos = { 221.f, 481.f};
	fOffsetX = 175.f;
	for (_uint i = 0; i < 3; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}
	fStartPos = { 306.f, 528.f };
	fOffsetX = 180.f;
	for (_uint i = 0; i < 2; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_UI()
{
	EvolveMenu_SkillView_Frame = ADD_STATIC_CUSTOMUI;

	EvolveMenu_SkillView_Frame.lock()->Set_UIPosition
	(
		84.f,
		150.f,
		624.f,
		600.f,
		CUI::ALIGN_LEFTTOP
	);
	EvolveMenu_SkillView_Frame.lock()->Set_Depth(0.3f);
	EvolveMenu_SkillView_Frame.lock()->Set_Texture("EvolveMenu_SkillView_Frame");

	Add_Child(EvolveMenu_SkillView_Frame);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_SkillButtons()
{
	weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>	pButton;

	weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot>	pButtonSlot;


	for(_uint i = 0 ; i < (_uint)SKILL_NAME::SKILL_BANKAI;i++)
	{
		pButtonSlot = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_SkillButtonSlot>(LEVEL_STATIC);

		pButton = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_SkillButton>(LEVEL_STATIC);
		pButton.lock()->Set_Skill((SKILL_NAME)i);
		pButton.lock()->Set_Slot(pButtonSlot);

		pButton.lock()->Set_UIPositionAllChilds(m_vecSkillPos[i].x, m_vecSkillPos[i].y);

		pButtonSlot.lock()->Set_UIPositionAllChilds(m_vecSkillPos[i].x, m_vecSkillPos[i].y);

		Bind_Callback(pButton);

		Add_Child(pButtonSlot);
		m_vecSkillButton.push_back(pButton);
	}
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_PlayerSkillSlot()
{

	_float2 fPlayerSkillSlotPos[2] = {{194.f, 672.f}, {576.f, 672.f}};

	_uint	iPlayerSkillSlotSize = 2;

	weak_ptr< CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot> pPlayerSkillSlot;
	for (_uint i = 0; i < iPlayerSkillSlotSize; i++)
	{
		pPlayerSkillSlot  = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot>(LEVEL_STATIC);
		pPlayerSkillSlot.lock()->Set_UIPositionAllChilds(fPlayerSkillSlotPos[i].x, fPlayerSkillSlotPos[i].y);
	
		m_vecPlayerSkillSlot.push_back(pPlayerSkillSlot);

		Add_Child(pPlayerSkillSlot);

	
	}
}
void CUI_EvolveMenu_PlagueWeapon_SkillView::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	//여기서 다 빼주고 넣어주는 이유.
	//플레이어가 어떤 스킬을 어디서 넣고 저장했는지를 알 수가 없음.
	//인게임 중에서 로드하는 상황도 나올 수 있기 때문에, 
	//일단 다 빼고 플레이어 스킬 상황에 맞게 넣어준다.


	/*
		비용이 많이 드는가? X : 스킬 개수도 적고, 많이 켜질일도없음
	*/
	SKILLMAP SkillMap;
	weak_ptr<CPlayerSkill_System> pPlayerSkillSystem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CPlayerSkill_System>();
	weak_ptr<CSkill_Base> pSkill;

	for (auto& elem : m_vecSkillButton)
	{
		elem.lock()->Set_Enable(true);//모든 스킬버튼의 렌더를 ON
		
		SkillMap.emplace(elem.lock()->Get_SkillName(), elem);
	}
	//창 열리면 장착한 스킬 모두 뺀다.
	for (_uint i = 0; i < m_vecPlayerSkillSlot.size(); i++)
	{
		m_vecPlayerSkillSlot[i].lock()->Clear_Slot();
	}
	//순회를 돌면서 활성화된 스킬을 찾아서 활성화시켜줌
	/*
		둘다 맵이라 비용은 크지 않을듯
	*/
	//Check To Unlocked Skills
	for(auto& pair : SkillMap)
	{
		pSkill = pPlayerSkillSystem.lock()->Find_Skill(pair.first);

		if (pSkill.lock())
		{
			if (pSkill.lock()->Get_Unlock())
			{
				pair.second.lock()->Unlock_Skill();
			}
		}
		pSkill = weak_ptr<CSkill_Base>();
	}
	//Check to Equip Skills
	_uint iSocketCount = (_uint)CPlayerSkill_System::SOCKET_TYPE::SOCKET_END;

	for (_uint i = 0; i < iSocketCount; i++)
	{
		pSkill = pPlayerSkillSystem.lock()->Get_EquipSkill((CPlayerSkill_System::SOCKET_TYPE)i);

		if (pSkill.lock())
		{
			m_vecPlayerSkillSlot[i].lock()->Equip_Skill(SkillMap[pSkill.lock()->Get_SkillName()], i);
		}
	}	
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_vecSkillButton)
	{
		elem.lock()->Set_Enable(false);
	}
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Bind_Callback(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pButton)
{
	pButton.lock()->Callback_MouseOver += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOver, this,
		placeholders::_1);

	pButton.lock()->Callback_MouseOut += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOut, this);

	pButton.lock()->Callback_UnLockSkill += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnUnlockSkill, this,
		placeholders::_1);
	
	pButton.lock()->Callback_ButtonUp += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnLButtonUp, this,
		placeholders::_1);

	pButton.lock()->Callback_ButtonDown += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnLButtonDown, this,
		placeholders::_1);

}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOver(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	Callback_OnMouseOver(pSkillButton);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOut()
{
	Callback_OnMouseOut();
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnUnlockSkill(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	Callback_OnUnlockSkill(pSkillButton);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnLButtonUp(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	Callback_OnLButtonUp(pSkillButton);

	for (_uint i = 0; i < m_vecPlayerSkillSlot.size(); i++)
	{
		if (m_vecPlayerSkillSlot[i].lock()->MousePtInUI())
		{
			GAMEINSTANCE->PlaySound2D("Fantasy_Game_UI_Fire_Select.ogg", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));

			m_vecPlayerSkillSlot[i].lock()->Equip_Skill(pSkillButton, i);
			return;
		}
	}
	pSkillButton.lock()->SetPosToMyOriginSlot();
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnLButtonDown(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	//플레이어 슬롯에 있는 것을 집은 경우
	for (_uint i = 0; i < m_vecPlayerSkillSlot.size(); i++)
	{
		if (pSkillButton.lock() == m_vecPlayerSkillSlot[i].lock()->Get_Equiped_Skill().lock())
		{
			m_vecPlayerSkillSlot[i].lock()->UnEquip_Skill(i);
		}
	}
}


void CUI_EvolveMenu_PlagueWeapon_SkillView::Free()
{

}
