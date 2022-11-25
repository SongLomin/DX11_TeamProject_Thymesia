#pragma once
#include "UI.h"

/*
UI���� �����ϴ� ��ɸ� �ϴ� UI.

*/
BEGIN(Client)

class CUI_Container abstract : public CUI
{
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) PURE;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    virtual void Free(){};
};

END