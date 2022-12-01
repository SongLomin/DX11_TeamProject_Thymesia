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

private:
    enum LADDER_COL_TYPE
    {
        UP_USE,
        UP_ANIM,
        DOWN_USE,
        DOWN_ANIM,

        TYPE_END
    };

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override; 

public:
    void OnEventMessage(_uint iArg) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

private:
    HRESULT SetUp_ShaderResource_Up();
    HRESULT SetUp_ShaderResource_Mid();
    HRESULT SetUp_ShaderResource_Down();

    void    SetUpColliderDesc();

private:
    weak_ptr<CCollider>                 m_pColliderCom[LADDER_COL_TYPE::TYPE_END];
    weak_ptr<CModel>                    m_pUpModelCom;
    weak_ptr<CVIBuffer_Model_Instance>  m_pInstanceModelCom;
    weak_ptr<CShader>                   m_pInstanceShaderCom;
    weak_ptr<CTexture>                  m_pMaskingTextureCom;

    _float                              m_fOffset   = 0.4f;
    _int                                m_iMidSize  = 5;
};

END