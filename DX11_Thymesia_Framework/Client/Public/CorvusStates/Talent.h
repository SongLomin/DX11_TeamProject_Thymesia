#pragma once
#include "UI.h"
#include "UI_Button.h"


BEGIN(Engine)

END

BEGIN(Client)
class CTalent_Effect;
class CCustomUI;

class CTalent :
    public CUI_Button_Base
{
    GAMECLASS_H(CTalent)
        CLONE_H(CTalent, CGameObject)

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void    Tick(_float fTimeDelta) override;
    virtual void    LateTick(_float fTimeDelta) override;

public:
    TALENT_RESULT Check_Requiment(const int In_iPoint, int& Out_iCost, list<weak_ptr<CTalent>>& Out_pVisitNodes);
    void Find_ActiveParent_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveParent, int& out_iDepth);
    void Find_ActiveChild_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveChild, int& out_iDepth);
    _bool Check_ActiveAble_ThisNode();
    void Find_AllParent_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveParent);

    void    Set_TalentInfo(_bool In_bActive, 
        weak_ptr<CTalent_Effect> In_pEffet, weak_ptr<CTalent> In_pParent);
    void    Set_Parent(weak_ptr<CTalent> In_Parent);
    void Set_Active(_bool Active);
    weak_ptr<CTalent_Effect> Get_Effect() { return m_pEffect; }
    list<weak_ptr<CTalent>> CTalent::Get_Child()
    {
        return m_pChilds;
    }
    inline weak_ptr<CTalent> CTalent::Get_Parent()
    {
        return m_pParent;
    }
    _int                       Get_NodeLevel()
    {
        return m_iNodeLevel;
    }

    void TestTalentCheck();
    void Set_TALENT_NAME(TALENT_NAME TalentName);
private:
    void                        Add_TalentChild(weak_ptr<CTalent> In_pChild);
    



//For Button
private:
    void			Update_ChildUI();

private:
    virtual void	OnMouseOver()		override;
    virtual void	OnMouseOut()		override;
    virtual void	OnLButtonDown()		override;
    virtual void	OnLButtonClick()	override;

    virtual void	CheckMouseOver()      override;
    virtual void	CheckMouseOut()       override;
    virtual void	CheckLButtonClick()   override;
                                          
    virtual void	UnCheckMouseOver()    override;
    virtual void	UnCheckMouseOut()     override;
    virtual void	UnCheckLButtonClick() override;


public:
    _uint			GetActiveNodeCount(weak_ptr<CTalent> In_Talent);
    weak_ptr<CTalent> Get_RootNode();
private:
    _bool                         m_bActive = false;
    weak_ptr<CTalent_Effect>     m_pEffect;
    list<weak_ptr<CTalent>>      m_pChilds;
    weak_ptr<CTalent>            m_pParent;
    TALENT_NAME                  m_tTalentName = TALENT_NAME::TALENT_NAME_END;

    _int                           m_iNodeLevel;
    
private:
    weak_ptr<CCustomUI>	m_pButtonFrame;//프레임
    weak_ptr<CCustomUI>	m_pButtonActive;//버튼 눌렸을대
    weak_ptr<CCustomUI> m_pIcon;

private:
    //TalentInformation
    weak_ptr<CCustomUI>	m_pTalentBG;//프레임
    weak_ptr<CCustomUI>	m_pTalentTitle;//버튼 눌렸을대
    weak_ptr<CCustomUI> m_pTalentInformation;

private:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

    void Free();

};


END