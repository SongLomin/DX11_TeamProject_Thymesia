#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CEasingComponent_Float;

class CUI_CustomEffect;

class CUI_EffectGroup_SkillOpen :public CUI_Container
{
public:
    GAMECLASS_H(CUI_EffectGroup_SkillOpen)
    CLONE_H(CUI_EffectGroup_SkillOpen, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void                Init_EffectUI();
    void                Play(SKILL_NAME eSkillName);

public:
    void                Call_OnEasingFloatConditionEvent(_uint iEventNumber);
             
private:
    weak_ptr< CEasingComponent_Float> m_pEasingFloat;


    weak_ptr<CUI_CustomEffect>        m_pBackGround;
    weak_ptr<CUI_CustomEffect>        m_pTitleBG;
    weak_ptr<CUI_CustomEffect>        m_pTitleText;
    weak_ptr<CUI_CustomEffect>        m_pSkillName;

    weak_ptr<CUI_CustomEffect>        m_pStartCircleEffect;
private:
    void            Free();


};

END