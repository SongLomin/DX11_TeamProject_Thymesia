#include "stdafx.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_Containers.h"
#include "UI_Elements.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"
#include "EasingTransform.h"
#include "State_Player.h"
#include "Player.h"


GAMECLASS_C(CUI_EvolveMenu_Level)
CLONE_C(CUI_EvolveMenu_Level, CGameObject)


HRESULT CUI_EvolveMenu_Level::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY] = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::RED] = _float4(1.f, 0.f, 0.f, 1.f);
    m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN] = _float4(104.f / 255.f, 209.f / 255.f, 170.f / 255.f, 1.f);

    m_fFontSize = m_fFontOriginSize * m_fFontScale;
    
    GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);

    weak_ptr<CPlayer>   pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock();
    if (pPlayer.lock())
        pPlayer.lock()->Get_Status().lock()->Get_Desc(&m_tOriginStatus);
    else
    {
        m_tOriginStatus.m_iLevel = 1;
        m_tOriginStatus.m_fCurrentHP = 300.f;
        m_tOriginStatus.m_fMaxHP = 300.f;
        m_tOriginStatus.m_fMaxMP = 150.f;
        m_tOriginStatus.m_fCurrentMP = 150.f;
        m_tOriginStatus.m_fNormalAtk = 25.f;
        m_tOriginStatus.m_fPlagueAtk = 200.f;
        m_tOriginStatus.m_iWound = 75.f;
        m_tOriginStatus.m_iMemory = 5000;
        m_tOriginStatus.m_fParryingAtk = 25.f;
        m_tOriginStatus.m_iStr = 1;
        m_tOriginStatus.m_iVital = 1;
        m_tOriginStatus.m_iPlague = 1;

    }

    m_tChangeStatus = m_tOriginStatus;


    m_stackChangedPlayerDesc.push_back(m_tOriginStatus);

    
    m_iArrowArraySize = 10;


    m_fDecorationArrowPos[0] = { 608.f ,195.f };
    m_fDecorationArrowPos[1] = { 608.f ,252.f };
    m_fDecorationArrowPos[2] = { 617.f ,475.f };
    m_fDecorationArrowPos[3] = { 617.f ,552.f };
    m_fDecorationArrowPos[4] = { 617.f ,629.f };
    m_fDecorationArrowPos[5] = { 1300.f ,246.f };
    m_fDecorationArrowPos[6] = { 1300.f ,310.f };
    m_fDecorationArrowPos[7] = { 1300.f ,386.f };
    m_fDecorationArrowPos[8] = { 1300.f ,456.f };
    m_fDecorationArrowPos[9] = { 1300.f ,526.f };


    CalculateNeedMemory();

    Create_Background();
    Create_Memory();
    Create_Status();
    Create_State();
    Create_NoneGrouping();
    Create_ReconfirmWindow();
    Init_OriginFontInfo();
    Init_ChangeFontInfo();
    Update_FontInfo();

    m_bOpenableReconfirmWindow = true;
    m_iReconfirmWindowIndex = 0;
    m_pEasingTransformCom = Add_Component<CEasingTransform>();
    m_iReconfirmWindowIndex = 0;
    m_iSelectedIndex = 0;

    ChangeSelectedIndex();
    Set_Enable(false);
    return S_OK;
}

void CUI_EvolveMenu_Level::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


    m_pEasingTransformCom.lock()->Tick(fTimeDelta);

    if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
    {
        FaderDesc tFaderDesc;
        tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
        tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
        tFaderDesc.fFadeMaxTime = 0.2f;
        tFaderDesc.fDelayTime = 0.f;
        tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

        m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
        m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu_Level::Call_ReturnToEvolveMenu, this);
    }

    if (m_bOpenableReconfirmWindow)//화면이 켜지지 않은 경우.
    {
        if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
        {
            if (m_iSelectedIndex > 0)
            {
                m_iSelectedIndex--;
                ChangeSelectedIndex();
            }
        }
        if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
        {
            if (m_iSelectedIndex <= (_uint)EVOLVE_LEVEL_TYPE::PLA)
            {
                m_iSelectedIndex++;
                ChangeSelectedIndex();
            }
        }
        if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
        {
            if (m_stackChangedPlayerDesc.size() > 1)
                DecreaseStatus();
        }
        if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
        {
            if (m_stackChangedPlayerDesc.back().m_iMemory >= m_iNeedMemory)
                IncreaseStatus((CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE)m_iSelectedIndex);
        }

        if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP))
        {
            if (m_bOpenableReconfirmWindow == true && m_iSelectedIndex == (_uint)EVOLVE_LEVEL_TYPE::APPLY)
            {
                OpenReconfirmWindow();
            }
        }
    }

    TickReconfirmWindow();
   
    
    m_bOpenReconfirmWindowThisFrame = false;

}

void CUI_EvolveMenu_Level::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);


    //GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::HEIROLIGHT, m_tTextInfo_Memory);

    for (auto& elem : m_vecTextInfo)
    {
        GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::HEIROLIGHT, elem);
    }

}

void CUI_EvolveMenu_Level::Create_Background()
{
    m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");
    m_pPauseMenuBackground.lock()->Set_Depth(0.9f);

    m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");
    m_pPauseMenuBackground_Main.lock()->Set_Depth(0.8f);

    m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");
    m_pPauseMenuBackground_Top.lock()->Set_Depth(0.7f);

    m_vecChildUI.push_back(m_pPauseMenuBackground);
    m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
    m_vecChildUI.push_back(m_pPauseMenuBackground_Top);
}

void CUI_EvolveMenu_Level::Create_Memory()
{
    m_pTitle = ADD_STATIC_CUSTOMUI;
    m_pTitle.lock()->Set_UIPosition
    (
        54.f,
        38.f,
        474.f,
        64.f,
        ALIGN_LEFTTOP
    );
    m_pTitle.lock()->Set_Depth(0.f);
    m_pTitle.lock()->Set_Texture("EvolveMenu_LevelUp_Title");

    m_pLevel = ADD_STATIC_CUSTOMUI;
    m_pLevel.lock()->Set_UIPosition
    (
        144.f,
        184.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP

    );
    m_pLevel.lock()->Set_Texture("EvolveMenu_LevelUp_Level");

    m_pMemory = ADD_STATIC_CUSTOMUI;
    m_pMemory.lock()->Set_UIPosition
    (
        144.f,
        246.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pMemory.lock()->Set_Texture("EvolveMenu_LevelUp_Memory");


    m_pRequired_Memory = ADD_STATIC_CUSTOMUI;
    m_pRequired_Memory.lock()->Set_UIPosition
    (
        144.f,
        300.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pRequired_Memory.lock()->Set_Texture("EvolveMenu_LevelUp_Required_Memory");


    Add_Child(m_pTitle);
    Add_Child(m_pLevel);
    Add_Child(m_pMemory);
    Add_Child(m_pRequired_Memory);

}

void CUI_EvolveMenu_Level::Create_Status()
{
    m_pStatusBG = ADD_STATIC_CUSTOMUI;

    m_pStatusBG.lock()->Set_UIPosition
    (
        97.f,
        397.f,
        770.f,
        296.f,
        ALIGN_LEFTTOP
    );
    m_pStatusBG.lock()->Set_Texture("EvolveMenu_LevelUp_StatusBG");
    m_pStatusBG.lock()->Set_PassIndex(1);
    m_pStatusBG.lock()->Set_Depth(0.6f);

    m_pStatusTitle = ADD_STATIC_CUSTOMUI;

    m_pStatusTitle.lock()->Set_UIPosition
    (
        144.f,
        407.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pStatusTitle.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Title");

    m_pStatusStrength = ADD_STATIC_CUSTOMUI;

    m_pStatusStrength.lock()->Set_UIPosition
    (
        151.f,
        467.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pStatusStrength.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Strength");
    m_pStatusStrength.lock()->Set_AlphaColor(0.7f);


    m_pStatusVitality = ADD_STATIC_CUSTOMUI;

    m_pStatusVitality.lock()->Set_UIPosition
    (
        151.f,
        544.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pStatusVitality.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Vitality");
    m_pStatusVitality.lock()->Set_AlphaColor(0.7f);

    m_pStatusPlague = ADD_STATIC_CUSTOMUI;

    m_pStatusPlague.lock()->Set_UIPosition
    (
        151.f,
        621.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pStatusPlague.lock()->Set_Texture("EvolveMenu_LevelUp_Status_Plague");
    m_pStatusPlague.lock()->Set_AlphaColor(0.7f);


    Add_Child(m_pStatusBG);
    Add_Child(m_pStatusTitle);
    Add_Child(m_pStatusStrength);
    Add_Child(m_pStatusVitality);
    Add_Child(m_pStatusPlague);


}

void CUI_EvolveMenu_Level::Create_State()
{
    m_pState_Title = ADD_STATIC_CUSTOMUI;
    m_pState_Title.lock()->Set_UIPosition
    (
        913.f,
        178.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pState_Title.lock()->Set_Texture("EvolveMenu_LevelUp_State_Title");
    m_pState_Title.lock()->Set_Depth(0.5f);

    m_pState_AttackDamage = ADD_STATIC_CUSTOMUI;
    m_pState_AttackDamage.lock()->Set_UIPosition
    (
        949.f,
        238.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pState_AttackDamage.lock()->Set_Texture("EvolveMenu_LevelUp_State_AttackDamgage");
    m_pState_AttackDamage.lock()->Set_AlphaColor(0.7f);
    m_pState_AttackDamage.lock()->Set_Depth(0.5f);


    m_pState_Wound = ADD_STATIC_CUSTOMUI;
    m_pState_Wound.lock()->Set_UIPosition
    (
        949.f,
        308.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pState_Wound.lock()->Set_Texture("EvolveMenu_LevelUp_State_Wound");
    m_pState_Wound.lock()->Set_AlphaColor(0.7f);
    m_pState_Wound.lock()->Set_Depth(0.5f);


    m_pState_ClawDamage = ADD_STATIC_CUSTOMUI;
    m_pState_ClawDamage.lock()->Set_UIPosition
    (
        949.f,
        378.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pState_ClawDamage.lock()->Set_Texture("EvolveMenu_LevelUp_State_ClawDamage");
    m_pState_ClawDamage.lock()->Set_AlphaColor(0.7f);
    m_pState_ClawDamage.lock()->Set_Depth(0.5f);

    m_pState_HP = ADD_STATIC_CUSTOMUI;
    m_pState_HP.lock()->Set_UIPosition
    (
        949.f,
        448.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pState_HP.lock()->Set_Texture("EvolveMenu_LevelUp_State_HP");
    m_pState_HP.lock()->Set_AlphaColor(0.7f);
    m_pState_HP.lock()->Set_Depth(0.5f);


    m_pState_MP = ADD_STATIC_CUSTOMUI;
    m_pState_MP.lock()->Set_UIPosition
    (
        949.f,
        518.f,
        200.f,
        34.f,
        ALIGN_LEFTTOP
    );
    m_pState_MP.lock()->Set_Texture("EvolveMenu_LevelUp_State_MP");
    m_pState_MP.lock()->Set_AlphaColor(0.7f);
    m_pState_MP.lock()->Set_Depth(0.5f);


    Add_Child(m_pState_Title);
    Add_Child(m_pState_AttackDamage);
    Add_Child(m_pState_Wound);
    Add_Child(m_pState_ClawDamage);
    Add_Child(m_pState_HP);
    Add_Child(m_pState_MP);
}

void CUI_EvolveMenu_Level::Create_NoneGrouping()
{
    m_pApply = ADD_STATIC_CUSTOMUI;

    m_pApply.lock()->Set_UIPosition
    (
        459.f,
        726.f,
        86.f,
        36.f,
        ALIGN_LEFTTOP
    );
    m_pApply.lock()->Set_Texture("EvolveMenu_LevelUp_Apply");
    m_pApply.lock()->Set_Depth(0.5f);

    m_pHighlight = ADD_STATIC_CUSTOMUI;

    m_pHighlight.lock()->Set_UIPosition
    (
        121.f,
        725.f,
        723.f,
        39.f,
        ALIGN_LEFTTOP
    );
    m_pHighlight.lock()->Set_Texture("MainMenu_SelectableButton_2");
    m_pHighlight.lock()->Set_Depth(0.6f);



    weak_ptr<CCustomUI>   pArrow;

    for (_uint i = 0; i < m_iArrowArraySize; i++)
    {
        pArrow = ADD_STATIC_CUSTOMUI;
        pArrow.lock()->Set_UIPosition(
            m_fDecorationArrowPos[i].x,
            m_fDecorationArrowPos[i].y,
            52.f,
            23.f,
            ALIGN_LEFTTOP);
        pArrow.lock()->Set_Depth(0.4f);
        pArrow.lock()->Set_Texture("Decoration_Arrow_Right");
        pArrow.lock()->Set_AlphaColor(0.6f);
        Add_Child(pArrow);
    }

    Add_Child(m_pHighlight);
    Add_Child(m_pApply);

}

void CUI_EvolveMenu_Level::Create_ReconfirmWindow()
{
    m_pReconfirmWindowBG = ADD_STATIC_CUSTOMUI;
    m_pReconfirmWindowBG.lock()->Set_UIPosition
    (
        475.f,
        346.f,
        650.f,
        209.f,
        ALIGN_LEFTTOP
    );
    m_pReconfirmWindowBG.lock()->Set_Depth(0.3f);
    m_pReconfirmWindowBG.lock()->Set_Texture("LevelUp_ReconfirmWindowBG");

    m_pReconfirmWindowNotice = ADD_STATIC_CUSTOMUI;
    m_pReconfirmWindowNotice.lock()->Set_UIPosition
    (
        581.f,
        368.f,
        438.f,
        33.f,
        ALIGN_LEFTTOP);
    m_pReconfirmWindowNotice.lock()->Set_Depth(0.2f);
    m_pReconfirmWindowNotice.lock()->Set_Texture("LevelUp_ReconfirmWindow_ChangedStatusApply");
    m_pReconfirmWindowNotice.lock()->Set_AlphaColor(1.f);


    m_pReconfirmWindowYes = ADD_STATIC_CUSTOMUI;
    m_pReconfirmWindowYes.lock()->Set_UIPosition
    (
        782.f,
        458.f,
        36.f,
        33.f,
        ALIGN_LEFTTOP);
    m_pReconfirmWindowYes.lock()->Set_Depth(0.1f);
    m_pReconfirmWindowYes.lock()->Set_Texture("LevelUp_ReconfirmWindow_Yes");



    m_pReconfirmWindowNo = ADD_STATIC_CUSTOMUI;
    m_pReconfirmWindowNo.lock()->Set_UIPosition
    (
        753.f,
        504.f,
        95.f,
        33.f,
        ALIGN_LEFTTOP);
    m_pReconfirmWindowNo.lock()->Set_Depth(0.1f);
    m_pReconfirmWindowNo.lock()->Set_Texture("LevelUp_ReconfirmWindow_No");



    m_pReconfirmWindowHighlight = ADD_STATIC_CUSTOMUI;
    m_pReconfirmWindowHighlight.lock()->Set_UIPosition
    (
        475.f,
        504.f,
        650.f,
        34.f,
        ALIGN_LEFTTOP);
    m_pReconfirmWindowHighlight.lock()->Set_Depth(0.2f);
    m_pReconfirmWindowHighlight.lock()->Set_Texture("MainMenu_SelectableButton_2");




    m_vecEventChildUI.push_back(m_pReconfirmWindowBG);
    m_vecEventChildUI.push_back(m_pReconfirmWindowNotice);
    m_vecEventChildUI.push_back(m_pReconfirmWindowYes);
    m_vecEventChildUI.push_back(m_pReconfirmWindowNo);
    m_vecEventChildUI.push_back(m_pReconfirmWindowHighlight);

    Disable_AllEventChild();
}

void CUI_EvolveMenu_Level::Init_OriginFontInfo()
{

    m_tTextInfo_OriginLevel.bAlways = false;
    m_tTextInfo_OriginLevel.bCenterAlign = true;
    m_tTextInfo_OriginLevel.fRotation = 0;
    m_tTextInfo_OriginLevel.vColor = _float4(1.f,1.f,1.f,1.f);
    m_tTextInfo_OriginLevel.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginLevel.vPosition = _float2(513.f - m_fFontSize * 0.5f , 196.f - m_fFontSize * 0.5f);


    m_tTextInfo_OriginMemory.bAlways = false;
    m_tTextInfo_OriginMemory.bCenterAlign = true;
    m_tTextInfo_OriginMemory.fRotation = 0;
    m_tTextInfo_OriginMemory.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginMemory.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginMemory.vPosition = _float2(513.f - m_fFontSize * 0.5f, 254.f - m_fFontSize * 0.5f);



    m_tTextInfo_OriginStr.bAlways = false;
    m_tTextInfo_OriginStr.bCenterAlign = true;
    m_tTextInfo_OriginStr.fRotation = 0;
    m_tTextInfo_OriginStr.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginStr.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginStr.vPosition = _float2(513.f - m_fFontSize * 0.5f, 475.f - m_fFontSize * 0.5f);



    m_tTextInfo_OriginVit.bAlways = false;
    m_tTextInfo_OriginVit.bCenterAlign = true;
    m_tTextInfo_OriginVit.fRotation = 0;
    m_tTextInfo_OriginVit.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginVit.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginVit.vPosition = _float2(513.f - m_fFontSize * 0.5f, 552.f - m_fFontSize * 0.5f);



    m_tTextInfo_OriginPlague.bAlways = false;
    m_tTextInfo_OriginPlague.bCenterAlign = true;
    m_tTextInfo_OriginPlague.fRotation = 0;
    m_tTextInfo_OriginPlague.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginPlague.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginPlague.vPosition = _float2(513.f - m_fFontSize * 0.5f, 628.f - m_fFontSize * 0.5f);


    //State

    m_tTextInfo_OriginAttackDamage.bAlways = false;
    m_tTextInfo_OriginAttackDamage.bCenterAlign = true;
    m_tTextInfo_OriginAttackDamage.fRotation = 0;
    m_tTextInfo_OriginAttackDamage.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginAttackDamage.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginAttackDamage.vPosition = _float2(1186.f - m_fFontSize * 0.5f, 245.f - m_fFontSize * 0.5f);



    m_tTextInfo_OriginWound.bAlways = false;
    m_tTextInfo_OriginWound.bCenterAlign = true;
    m_tTextInfo_OriginWound.fRotation = 0;
    m_tTextInfo_OriginWound.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginWound.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginWound.vPosition = _float2(1186.f - m_fFontSize * 0.5f, 310.f - m_fFontSize * 0.5f);



    m_tTextInfo_OriginClawDamage.bAlways = false;
    m_tTextInfo_OriginClawDamage.bCenterAlign = true;
    m_tTextInfo_OriginClawDamage.fRotation = 0;
    m_tTextInfo_OriginClawDamage.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginClawDamage.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginClawDamage.vPosition = _float2(1186.f - m_fFontSize * 0.5f, 387.f- m_fFontSize * 0.5f);

    



    m_tTextInfo_OriginHP.bAlways = false;
    m_tTextInfo_OriginHP.bCenterAlign = true;
    m_tTextInfo_OriginHP.fRotation = 0;
    m_tTextInfo_OriginHP.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginHP.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginHP.vPosition = _float2(1186.f - m_fFontSize * 0.5f, 455.f - m_fFontSize * 0.5f);
    
    

    m_tTextInfo_OriginMP.bAlways = false;
    m_tTextInfo_OriginMP.bCenterAlign = true;
    m_tTextInfo_OriginMP.fRotation = 0;
    m_tTextInfo_OriginMP.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTextInfo_OriginMP.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_OriginMP.vPosition = _float2(1186.f - m_fFontSize * 0.5f, 518.f - m_fFontSize * 0.5f);









}

void CUI_EvolveMenu_Level::Init_ChangeFontInfo()
{

    m_tTextInfo_ChangeLevel.bAlways = false;
    m_tTextInfo_ChangeLevel.bCenterAlign = true;
    m_tTextInfo_ChangeLevel.fRotation = 0;
    m_tTextInfo_ChangeLevel.vColor = _float4(0.7f,0.7f,0.7f,0.7f);
    m_tTextInfo_ChangeLevel.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeLevel.vPosition = _float2(733.f - m_fFontSize * 0.5f, 196.f - m_fFontSize * 0.5f);


    m_tTextInfo_ChangeMemory.bAlways = false;
    m_tTextInfo_ChangeMemory.bCenterAlign = true;
    m_tTextInfo_ChangeMemory.fRotation = 0;
    m_tTextInfo_ChangeMemory.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeMemory.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeMemory.vPosition = _float2(733.f - m_fFontSize * 0.5f, 254.f - m_fFontSize * 0.5f);
    


    m_tTextInfo_RequireMemory.bAlways = false;
    m_tTextInfo_RequireMemory.bCenterAlign = true;
    m_tTextInfo_RequireMemory.fRotation = 0;
    m_tTextInfo_RequireMemory.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_RequireMemory.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_RequireMemory.vPosition = _float2(733.f - m_fFontSize * 0.5f, 310.f - m_fFontSize * 0.5f);



    m_tTextInfo_ChangeStr.bAlways = false;
    m_tTextInfo_ChangeStr.bCenterAlign = true;
    m_tTextInfo_ChangeStr.fRotation = 0;
    m_tTextInfo_ChangeStr.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeStr.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeStr.vPosition = _float2(733.f - m_fFontSize * 0.5f, 475.f - m_fFontSize * 0.5f);



    m_tTextInfo_ChangeVit.bAlways = false;
    m_tTextInfo_ChangeVit.bCenterAlign = true;
    m_tTextInfo_ChangeVit.fRotation = 0;
    m_tTextInfo_ChangeVit.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeVit.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeVit.vPosition = _float2(733.f - m_fFontSize * 0.5f, 552.f - m_fFontSize * 0.5f);



    m_tTextInfo_ChangePlague.bAlways = false;
    m_tTextInfo_ChangePlague.bCenterAlign = true;
    m_tTextInfo_ChangePlague.fRotation = 0;
    m_tTextInfo_ChangePlague.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangePlague.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangePlague.vPosition = _float2(733.f - m_fFontSize * 0.5f, 628.f - m_fFontSize * 0.5f);


    //State

    m_tTextInfo_ChangeAttackDamage.bAlways = false;
    m_tTextInfo_ChangeAttackDamage.bCenterAlign = true;
    m_tTextInfo_ChangeAttackDamage.fRotation = 0;
    m_tTextInfo_ChangeAttackDamage.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeAttackDamage.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeAttackDamage.vPosition = _float2(1441.f - m_fFontSize * 0.5f, 245.f - m_fFontSize * 0.5f);

    

    m_tTextInfo_ChangeWound.bAlways = false;
    m_tTextInfo_ChangeWound.bCenterAlign = true;
    m_tTextInfo_ChangeWound.fRotation = 0;
    m_tTextInfo_ChangeWound.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeWound.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeWound.vPosition = _float2(1441.f - m_fFontSize * 0.5f, 310.f - m_fFontSize * 0.5f);


    m_tTextInfo_ChangeClawDamage.bAlways = false;
    m_tTextInfo_ChangeClawDamage.bCenterAlign = true;
    m_tTextInfo_ChangeClawDamage.fRotation = 0;
    m_tTextInfo_ChangeClawDamage.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeClawDamage.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeClawDamage.vPosition = _float2(1441.f - m_fFontSize * 0.5f, 387.f - m_fFontSize * 0.5f);


    m_tTextInfo_ChangeHP.bAlways = false;
    m_tTextInfo_ChangeHP.bCenterAlign = true;
    m_tTextInfo_ChangeHP.fRotation = 0;
    m_tTextInfo_ChangeHP.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeHP.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeHP.vPosition = _float2(1441.f - m_fFontSize * 0.5f, 455.f - m_fFontSize * 0.5f);

    

    m_tTextInfo_ChangeMP.bAlways = false;
    m_tTextInfo_ChangeMP.bCenterAlign = true;
    m_tTextInfo_ChangeMP.fRotation = 0;
    m_tTextInfo_ChangeMP.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo_ChangeMP.vScale = _float2(m_fFontScale, m_fFontScale);
    m_tTextInfo_ChangeMP.vPosition = _float2(1441.f - m_fFontSize * 0.5f, 518.f - m_fFontSize * 0.5f);



    
}




void CUI_EvolveMenu_Level::Update_FontInfo()
{

    CStatus_Player::PLAYERDESC      tChangedStatus = m_tChangeStatus;
    
    m_tTextInfo_OriginLevel.szText = to_wstring(m_tOriginStatus.m_iLevel);
    m_tTextInfo_OriginMemory.szText = to_wstring(m_tOriginStatus.m_iMemory);
    m_tTextInfo_OriginStr.szText = to_wstring(m_tOriginStatus.m_iStr);
    m_tTextInfo_OriginVit.szText = to_wstring(m_tOriginStatus.m_iVital);
    m_tTextInfo_OriginPlague.szText = to_wstring(m_tOriginStatus.m_iPlague);
    m_tTextInfo_OriginAttackDamage.szText = to_wstring((_uint)m_tOriginStatus.m_fNormalAtk);
    m_tTextInfo_OriginWound.szText = to_wstring((_uint)m_tOriginStatus.m_iWound);
    m_tTextInfo_OriginClawDamage.szText = to_wstring((_uint)m_tOriginStatus.m_fPlagueAtk);
    m_tTextInfo_OriginHP.szText = to_wstring((_uint)m_tOriginStatus.m_fMaxHP);
    m_tTextInfo_OriginMP.szText = to_wstring((_uint)m_tOriginStatus.m_fMaxMP);

    m_tTextInfo_ChangeLevel.szText = to_wstring(tChangedStatus.m_iLevel);
    if (tChangedStatus.m_iLevel > m_tOriginStatus.m_iLevel)
        m_tTextInfo_ChangeLevel.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeLevel.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];


    m_tTextInfo_ChangeMemory.szText = to_wstring(tChangedStatus.m_iMemory);
    if (tChangedStatus.m_iMemory < m_tOriginStatus.m_iMemory)
        m_tTextInfo_ChangeMemory.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::RED];
    else
        m_tTextInfo_ChangeMemory.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];


    m_tTextInfo_RequireMemory.szText = to_wstring(m_iNeedMemory);
    m_tTextInfo_RequireMemory.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::RED];


    m_tTextInfo_ChangeStr.szText = to_wstring(tChangedStatus.m_iStr);
    if (tChangedStatus.m_iStr > m_tOriginStatus.m_iStr)
        m_tTextInfo_ChangeStr.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeStr.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];

    m_tTextInfo_ChangeVit.szText = to_wstring(tChangedStatus.m_iVital);
    if (tChangedStatus.m_iVital> m_tOriginStatus.m_iVital)
        m_tTextInfo_ChangeVit.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeVit.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];

    m_tTextInfo_ChangePlague.szText = to_wstring(tChangedStatus.m_iPlague);

    m_tTextInfo_ChangePlague.szText = to_wstring(tChangedStatus.m_iPlague);
    if (tChangedStatus.m_iPlague > m_tOriginStatus.m_iPlague)
        m_tTextInfo_ChangePlague.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangePlague.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];



    if (tChangedStatus.m_fNormalAtk > m_tOriginStatus.m_fNormalAtk)
        m_tTextInfo_ChangeAttackDamage.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeAttackDamage.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];
        m_tTextInfo_ChangeAttackDamage.szText = to_wstring((_uint)tChangedStatus.m_fNormalAtk);


    if (tChangedStatus.m_iWound > m_tOriginStatus.m_iWound)
        m_tTextInfo_ChangeWound.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeWound.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];
    m_tTextInfo_ChangeWound.szText = to_wstring((_uint)tChangedStatus.m_iWound);


    
    if (tChangedStatus.m_fPlagueAtk > m_tOriginStatus.m_fPlagueAtk)
        m_tTextInfo_ChangeClawDamage.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeClawDamage.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];
    m_tTextInfo_ChangeClawDamage.szText = to_wstring((_uint)tChangedStatus.m_fPlagueAtk);

    if (tChangedStatus.m_fMaxHP > m_tOriginStatus.m_fMaxHP)
        m_tTextInfo_ChangeHP.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeHP.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];
    m_tTextInfo_ChangeHP.szText = to_wstring((_uint)tChangedStatus.m_fMaxHP);

    if (tChangedStatus.m_fMaxMP > m_tOriginStatus.m_fMaxMP)
        m_tTextInfo_ChangeMP.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::LIGHT_GREEN];
    else
        m_tTextInfo_ChangeMP.vColor = m_fColorType[(_uint)EVOLOVE_TEXT_COLOR::GRAY];
    m_tTextInfo_ChangeMP.szText = to_wstring((_uint)tChangedStatus.m_fMaxMP);


    m_vecTextInfo.clear();



    m_vecTextInfo.push_back(m_tTextInfo_OriginLevel);
    m_vecTextInfo.push_back(m_tTextInfo_OriginMemory);
    m_vecTextInfo.push_back(m_tTextInfo_OriginStr);
    m_vecTextInfo.push_back(m_tTextInfo_OriginVit);
    m_vecTextInfo.push_back(m_tTextInfo_OriginPlague);
    m_vecTextInfo.push_back(m_tTextInfo_OriginAttackDamage);
    m_vecTextInfo.push_back(m_tTextInfo_OriginWound);
    m_vecTextInfo.push_back(m_tTextInfo_OriginClawDamage);
    m_vecTextInfo.push_back(m_tTextInfo_OriginHP);
    m_vecTextInfo.push_back(m_tTextInfo_OriginMP);

    m_vecTextInfo.push_back(m_tTextInfo_ChangeLevel);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeMemory);
    m_vecTextInfo.push_back(m_tTextInfo_RequireMemory);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeStr);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeVit);
    m_vecTextInfo.push_back(m_tTextInfo_ChangePlague);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeAttackDamage);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeWound);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeClawDamage);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeHP);
    m_vecTextInfo.push_back(m_tTextInfo_ChangeMP);

}

void CUI_EvolveMenu_Level::Update_ChangeStatus(CStatus_Player::PLAYERDESC& tChangedPlayerStatus)
{
    //str
    tChangedPlayerStatus.m_fNormalAtk = 25 + (((_float)tChangedPlayerStatus.m_iStr -1) * 3.f);
    tChangedPlayerStatus.m_iWound = 75 + ((tChangedPlayerStatus.m_iStr - 1) * 3);

    //Vital
    tChangedPlayerStatus.m_fMaxHP = 300.f + ((_float)(tChangedPlayerStatus.m_iVital - 1) * 20.f);

    //
    tChangedPlayerStatus.m_fPlagueAtk = 200.f + ((_float)(tChangedPlayerStatus.m_iPlague - 1)* 10.f);

    tChangedPlayerStatus.m_fMaxMP = 150.f + ((_float)(tChangedPlayerStatus.m_iPlague - 1) * 20.f);

  
    CalculateNeedMemory();
    Update_FontInfo();
}

void CUI_EvolveMenu_Level::CalculateNeedMemory()
{
    m_iNeedMemory = m_stackChangedPlayerDesc.back().m_iLevel * 100;
}

_bool CUI_EvolveMenu_Level::Check_Changed(_uint dest, _uint sour)
{
    return dest != sour ? true : false;
}


void CUI_EvolveMenu_Level::OnDisable()
{
    __super::OnDisable();

    for (auto& elem : m_vecEventChildUI)
    {
        elem.lock()->Set_Enable(false);
    }
    m_stackChangedPlayerDesc.clear();
}

void CUI_EvolveMenu_Level::ChangeSelectedIndex()
{
    EVOLVE_LEVEL_TYPE eType = (EVOLVE_LEVEL_TYPE)m_iSelectedIndex;

    switch (eType)
    {
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::STR:
        m_pHighlight.lock()->Set_UIPosition(231.f, 465.f, ALIGN_LEFTTOP);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::VIT:
        m_pHighlight.lock()->Set_UIPosition(231.f, 542.f, ALIGN_LEFTTOP);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::PLA:
        m_pHighlight.lock()->Set_UIPosition(231.f, 619.f, ALIGN_LEFTTOP);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::APPLY:
        m_pHighlight.lock()->Set_UIPosition(121.f, 725.f, ALIGN_LEFTTOP);
        break;
    default:
        break;
    }


    LEVEL_RECONFIRM_TYPE eReconfirmType = (LEVEL_RECONFIRM_TYPE)m_iReconfirmWindowIndex;

    switch (eReconfirmType)
    {
    case Client::CUI_EvolveMenu_Level::LEVEL_RECONFIRM_TYPE::YES:
        m_pReconfirmWindowHighlight.lock()->Set_UIPosition
        (
            475.f,
            458.f,
            ALIGN_LEFTTOP);
        break;
    case Client::CUI_EvolveMenu_Level::LEVEL_RECONFIRM_TYPE::NO:
        m_pReconfirmWindowHighlight.lock()->Set_UIPosition
        (
            475.f,
            504.f,
            ALIGN_LEFTTOP);
        break;
    }

}

void CUI_EvolveMenu_Level::SelectButton()
{
    

}

void CUI_EvolveMenu_Level::OpenReconfirmWindow()
{
    m_bOpenableReconfirmWindow = false;
    m_bOpenReconfirmWindowThisFrame = true;

    m_iReconfirmWindowIndex = 0;
    m_pEasingTransformCom.lock()->Set_LerpFloat
    (
        0.f,1.f,0.3f,   EASING_TYPE::QUAD_IN
    );
    Enable_AllEventChild();
    TickReconfirmWindow();
}

void CUI_EvolveMenu_Level::TickReconfirmWindow()
{
    //창이 열 수 있는 상황(안열려있는상황)일시는 리턴
    if (m_bOpenableReconfirmWindow)
        return;
    
    if (m_pEasingTransformCom.lock()->Is_Lerping())
    {
        for (auto& elem : m_vecEventChildUI)
        {
            elem.lock()->Set_AlphaColor(m_pEasingTransformCom.lock()->Get_Lerp().x);
        }
    }

    if (m_bOpenableReconfirmWindow == false)
    {
        if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
        {
            if (m_iReconfirmWindowIndex > 0)
            {
                m_iReconfirmWindowIndex--;
                ChangeSelectedIndex();
            }
        }
        if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
        {
            if (m_iReconfirmWindowIndex <= (_uint)LEVEL_RECONFIRM_TYPE::NO)
            {
                m_iReconfirmWindowIndex++;
                ChangeSelectedIndex();
            }
        }
        if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP) && m_bOpenReconfirmWindowThisFrame == false)
        {
            if (m_iReconfirmWindowIndex == (_uint)LEVEL_RECONFIRM_TYPE::YES)
            {
                m_tOriginStatus = m_stackChangedPlayerDesc.back();
                m_stackChangedPlayerDesc.clear();
                m_stackChangedPlayerDesc.push_back(m_tOriginStatus);
            }
            m_iReconfirmWindowIndex = 0;
            m_bOpenableReconfirmWindow = true;
            ChangeSelectedIndex();
            Disable_AllEventChild();
            Update_ChangeStatus(m_tOriginStatus);
        }
    }
    
}
void CUI_EvolveMenu_Level::IncreaseStatus(EVOLVE_LEVEL_TYPE eEvolveType)
{
    m_tChangeStatus;

    switch (eEvolveType)
    {
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::STR:
        ++m_tChangeStatus.m_iStr;
        ++m_tChangeStatus.m_iLevel;
        m_tChangeStatus.m_iMemory -= m_iNeedMemory;

        Update_ChangeStatus(m_tChangeStatus);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::VIT:
        ++m_tChangeStatus.m_iVital;
        ++m_tChangeStatus.m_iLevel;
        Update_ChangeStatus(m_tChangeStatus);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::PLA:
        ++m_tChangeStatus.m_iPlague;
        ++m_tChangeStatus.m_iLevel;
        Update_ChangeStatus(m_tChangeStatus);
        break;
    default:
        break;
    }
}

void CUI_EvolveMenu_Level::DecreaseStatus(EVOLVE_LEVEL_TYPE eEvolveType)
{
    switch (eEvolveType)
    {
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::STR:
        --m_tChangeStatus.m_iStr;
        --m_tChangeStatus.m_iLevel;
        m_tChangeStatus.m_iMemory -= m_iNeedMemory;

        Update_ChangeStatus(m_tChangeStatus);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::VIT:
        --m_tChangeStatus.m_iVital;
        --m_tChangeStatus.m_iLevel;
        Update_ChangeStatus(m_tChangeStatus);
        break;
    case Client::CUI_EvolveMenu_Level::EVOLVE_LEVEL_TYPE::PLA:
        --m_tChangeStatus.m_iPlague;
        --m_tChangeStatus.m_iLevel;
        Update_ChangeStatus(m_tChangeStatus);
        break;
    default:
        break;
    }

}


void CUI_EvolveMenu_Level::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    if (!m_pFadeMask.lock())
        m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

    GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);

    weak_ptr<CPlayer>   pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock();
    if (pPlayer.lock())
        pPlayer.lock()->Get_Status().lock()->Get_Desc(&m_tOriginStatus);
    else
    {
        m_tOriginStatus.m_iLevel = 1;
        m_tOriginStatus.m_fCurrentHP = 300.f;
        m_tOriginStatus.m_fMaxHP = 300.f;
        m_tOriginStatus.m_fMaxMP = 150.f;
        m_tOriginStatus.m_fCurrentMP = 150.f;
        m_tOriginStatus.m_fNormalAtk = 25.f;
        m_tOriginStatus.m_fPlagueAtk = 200.f;
        m_tOriginStatus.m_iWound = 75.f;
        m_tOriginStatus.m_iMemory = 5000;
        m_tOriginStatus.m_fParryingAtk = 25.f;
        m_tOriginStatus.m_iStr = 1;
        m_tOriginStatus.m_iVital = 1;
        m_tOriginStatus.m_iPlague = 1;

    }

    m_stackChangedPlayerDesc.push_back(m_tOriginStatus);
    
}

void CUI_EvolveMenu_Level::Call_ReturnToEvolveMenu()
{
    Set_Enable(false);
    GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front().lock()->Set_Enable(true);

}