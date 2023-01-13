#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_PlayerSkillSlot.h"
#include "CustomUI.h"
#include "UI_Utils.h"
#include "UI_EvolveTalent_Active.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"
#include "PlayerSkillHeader.h"
#include "GameManager.h"


GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot, CGameObject)



HRESULT CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Init_UI();

    return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::Equip_Skill(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkill, _uint iSkillSocketType)
{
    if (m_pEquipSkill.lock())
    {
        m_pEquipSkill.lock()->Set_Slot(m_pEquipSkill.lock()->Get_OriginSlot());
        m_pEquipSkill.lock()->SetPosToMyOriginSlot();
    }
    m_pEquipSkill = pSkill;
    m_pEquipSkill.lock()->Set_Slot(CUI::Get_This());
    m_pEquipSkill.lock()->SetPosToMyCurrentSlot();
    
    weak_ptr<CPlayerSkill_System> pSkillSystem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CPlayerSkill_System>();

    if (pSkillSystem.lock())
    {
        pSkillSystem.lock()->OnChangeSkill(m_pEquipSkill.lock()->Get_SkillName(), (CPlayerSkill_System::SOCKET_TYPE)iSkillSocketType);
    }
    m_pActive.lock()->Set_Click(true);

}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::Clear_Slot()
{
    if (m_pEquipSkill.lock())
    {
        m_pEquipSkill.lock()->Set_Slot(m_pEquipSkill.lock()->Get_OriginSlot());
    }
    m_pEquipSkill = weak_ptr< CUI_EvolveMenu_PlagueWeapon_SkillButton>();
}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::UnEquip_Skill(_uint iSkillSocketType)
{
    Clear_Slot();

    weak_ptr<CPlayerSkill_System> pSkillSystem = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CPlayerSkill_System>();

    if (pSkillSystem.lock())
    {
        pSkillSystem.lock()->UnBindSkill((CPlayerSkill_System::SOCKET_TYPE)iSkillSocketType);
    }


    m_pActive.lock()->Set_Click(false);
}

_bool CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::IsEquipedSkill()
{
    if (m_pEquipSkill.lock())
    {
        return true;
    }
    return false;
}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::Init_UI()
{
    Set_Size(125.f, 125.f);

    m_pFrame = ADD_STATIC_CUSTOMUI;

    m_pActive = GAMEINSTANCE->Add_GameObject<CUI_EvolveTalent_Active>(LEVEL_STATIC);

    m_pFrame.lock()->Set_Size(125.f, 125.f);
    m_pActive.lock()->Set_Size(256.f, 256.f);

    m_pFrame.lock()->Set_Texture("HUD_Frame");

    m_pActive.lock()->Set_Depth(0.6f);
    m_pFrame.lock()->Set_Depth(0.59f);

    Add_Child(m_pFrame);
    Add_Child(m_pActive);


}

void CUI_EvolveMenu_PlagueWeapon_PlayerSkillSlot::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (m_pEquipSkill.lock())
    {
        m_pActive.lock()->Set_Click(true);
    }

}
