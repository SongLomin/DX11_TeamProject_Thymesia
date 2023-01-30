#pragma once
#include "Interaction_Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CModel;
class CVIBuffer_Model_Instance;
class CShader;
END

BEGIN(Client)

class CInteraction_Ladder final :
    public CInteraction_Prop
{
    GAMECLASS_H(CInteraction_Ladder);
    CLONE_H(CInteraction_Ladder, CGameObject);

public:
    _float Get_UpLadderHeight() const {
        return m_fUpLadderHeight;
    }

private:
    enum LADDER_COL_TYPE
    {
        UP_USE,
        DOWN_USE,

        TYPE_END
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

    void Set_RenderOutLine(_bool bState);

private:
    virtual HRESULT SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext) override;

    HRESULT SetUp_ShaderResource_Up(ID3D11DeviceContext* pDeviceContext);
    HRESULT SetUp_ShaderResource_Mid(ID3D11DeviceContext* pDeviceContext);
    HRESULT SetUp_ShaderResource_Down(ID3D11DeviceContext* pDeviceContext);

    void    SetUpColliderDesc();
    void    UpdateLadder();

private:
    weak_ptr<CCollider>                 m_pColliderCom[LADDER_COL_TYPE::TYPE_END];
    weak_ptr<CModel>                    m_pUpModelCom;
    weak_ptr<CVIBuffer_Model_Instance>  m_pInstanceModelCom;
    weak_ptr<CShader>                   m_pInstanceShaderCom;
    weak_ptr<CTexture>                  m_pMaskingTextureCom;

    _float                              m_fOffset         = 0.4f;
    _int                                m_iMidSize        = 4.5;
    _float                              m_fUpLadderHeight = 0.f;
    _bool                               m_bRenderOutLine  = false;
    LADDER_COL_TYPE                     m_eEnterLadder    = LADDER_COL_TYPE::DOWN_USE;

private:
    void Free();
};

END