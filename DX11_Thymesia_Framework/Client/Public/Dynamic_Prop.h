#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CDynamic_Prop  :
    public CProp
{
    GAMECLASS_H(CDynamic_Prop);
    CLONE_H(CDynamic_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    weak_ptr<CCollider>    m_pCColliderCom;

protected:
    void Free();

};

END