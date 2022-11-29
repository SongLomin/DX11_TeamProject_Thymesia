#pragma once
#include "UI.h"

BEGIN(Engine)

END

BEGIN(Client)
class CTalent_Effect;

class CTalent :
    public CUI
{
    GAMECLASS_H(CTalent)
        CLONE_H(CTalent, CGameObject)

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    TALENT_RESULT Check_Requiment(const int In_iPoint, int& Out_iCost, list<weak_ptr<CTalent>>& Out_pVisitNodes);
    void Find_ActiveParent_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveParent, int& out_iDepth);
    void Find_ActiveChild_Recursive(weak_ptr<CTalent> In_pTalent, list<weak_ptr<CTalent>>& out_pActiveChild, int& out_iDepth);
    void    Set_TalentInfo(_bool In_bActive, 
        weak_ptr<CTalent_Effect> In_pEffet, weak_ptr<CTalent> In_pParent);
    void    Add_TalentChild(weak_ptr<CTalent> In_pChild) { m_pChilds.push_back(In_pChild); }
    void Set_Active(_bool Active) { m_bActive = Active; }
    weak_ptr<CTalent_Effect> Get_Effect() { return m_pEffect; }
protected:
    _bool                         m_bActive = false;
    weak_ptr<CTalent_Effect>     m_pEffect;
    list<weak_ptr<CTalent>>      m_pChilds;
    weak_ptr<CTalent>            m_pParent;


private:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

    void Free();

};


END