#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CPhysXCollider;
END

BEGIN(Client)

class CDynamic_Piece;

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
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    weak_ptr<CCollider>         m_pColliderCom;
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;
    list<weak_ptr<CDynamic_Piece>>    m_pPieces;
    

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

    void Free();

};

END