#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;

class CEasingComponent_Float;
class CEasingComponent_Alpha;
class CEasingComponent_Transform;

class CUI_AppearEventVarg : public CUI_Container
{
public:
	GAMECLASS_H(CUI_AppearEventVarg)
	CLONE_H(CUI_AppearEventVarg, CGameObject)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void                    Start_Event();//1

private:


    void                    Call_SliceEnd();//2



    void                    CreateUI();

    void                    SetUp_Component();

    void                    Reset();




private:
    weak_ptr<CCustomUI>     m_pSliceTop;
    weak_ptr<CCustomUI>     m_pSliceBottom;

    weak_ptr<CCustomUI>     m_pSliceTopBG;
    weak_ptr<CCustomUI>     m_pSliceBottomBG;


};

END

