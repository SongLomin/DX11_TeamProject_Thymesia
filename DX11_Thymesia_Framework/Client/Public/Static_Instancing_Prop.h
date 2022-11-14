#pragma once
#include "Prop.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CVIBuffer_Model_Instance;
END

BEGIN(Client)

class CStatic_Instancing_Prop final :
    public CProp
{
    GAMECLASS_H(CStatic_Instancing_Prop);
    CLONE_H(CStatic_Instancing_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    weak_ptr<CVIBuffer_Model_Instance> m_pInstancingModel;

public:
    void Free();
};

END