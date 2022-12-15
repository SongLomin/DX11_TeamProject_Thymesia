#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CSkyBox :
    public CGameObject
{
    GAMECLASS_H(CSkyBox)
    CLONE_H(CSkyBox, CGameObject)
    SHALLOW_COPY(CSkyBox)

public:

private:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;


private:
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CTexture> m_pTextureCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CVIBuffer_Cube> m_pVIBufferCom;

private:
    _float          m_fUVFlow = 0.f;

private:
    void Free();


    

};

END