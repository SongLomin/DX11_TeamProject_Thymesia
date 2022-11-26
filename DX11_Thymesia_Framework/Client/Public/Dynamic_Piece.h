#pragma once
#include "Prop.h"

BEGIN(Engine)
class CCollider;
class CPhysXCollider;
END

BEGIN(Client)

class CDynamic_Piece :
    public CProp
{
    GAMECLASS_H(CDynamic_Piece);
    CLONE_H(CDynamic_Piece, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Set_WorldMatrixWithPhysX(_fmatrix In_WorldMatrix);

protected:
    weak_ptr<CPhysXCollider>    m_pPhysXColliderCom;


private:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

protected:
    void Free();

};

END