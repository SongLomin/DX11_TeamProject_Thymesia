#pragma once
#include "Map.h"

BEGIN(Client)

class CStage2 :
    public CMap
{
    GAMECLASS_H(CStage2);
    CLONE_H(CStage2, CGameObject);

public:

    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    

protected:
    virtual void SetUp_ShaderResource() override;

private:
    

private:
    void Free();

};

END