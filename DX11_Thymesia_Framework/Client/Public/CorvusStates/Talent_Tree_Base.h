#pragma once
#include "Component.h"
#include "Client_Defines.h"
#include "ClientComponent.h"
BEGIN(Engine)

END

BEGIN(Client)

class CTalent;

class CTalent_Tree_Base abstract :
    public CClientComponent
{
    GAMECLASS_H(CTalent_Tree_Base)
    CLONE_H(CTalent_Tree_Base, CComponent)

public:
    virtual HRESULT Initialize_Prototype() PURE;
    virtual HRESULT Initialize(void* pArg) PURE;
    virtual void Start() PURE;


protected://여기에 트리 검색 함수 넣기.



protected:
    weak_ptr<CTalent> m_pTalentTreeRoot;
private:
    void Free();
};


END