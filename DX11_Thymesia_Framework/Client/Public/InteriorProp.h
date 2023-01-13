#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Ground;
class CTransform;
END

BEGIN(Client)

class CPreView_InteriorProp;

class CInteriorProp :
    public CGameObject
{
    GAMECLASS_H(CInteriorProp);
    CLONE_H(CInteriorProp, CGameObject);
     
private:
    enum class PROP_ID 
    {
        STATIC_FLOWERPOT,
        STATIC_TABLE_01,
        STATIC_STATIS_04,
        STATIC_BARREL_CLOSED,
        STATIC_BARREL_BIG,
        STATIC_SHELF,

        INTERACTION_CHAIR,

        ID_END
    };

    struct SAVE_PROP
    {
        weak_ptr<CGameObject>   pObj;
        json                    json;
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

private:
    virtual HRESULT SetUp_ShaderResource();

    void Select_Props();
    void Rotation_Prop();
    void Edit_Props();

    void Create_Prop(PROP_ID _eItemID, _fmatrix _WorldMatrix);
    void Delete_Prop(weak_ptr<CGameObject> _pCollisionObject);
    void SetUp_PreviewPropMesh(PROP_ID _eItemID);
    _bool Compute_IsInTerrain(_fvector _vPos);
    void LoadJson_PropS();
    void SaveJson_PropS();
    void SaveJson_SingleProp(weak_ptr<CGameObject> _pObj, string _szTypeName, _hashcode _hashcode);
    void RotationProp(weak_ptr<CTransform> _pTargetTransfomCom);

private:
    weak_ptr<CShader>               m_pShaderCom;
    weak_ptr<CRenderer>             m_pRendererCom;
    weak_ptr<CVIBuffer_Ground>      m_pVIBufferCom;

    _float                          m_vMaxRangeX    = 0.f;
    _float                          m_vMaxRangeZ    = 0.f;
    _float                          m_fRotationY    = 0.f;
    _float                          m_fOffsetLength = 2.f;
    PROP_ID                         m_ePropID       = PROP_ID::STATIC_FLOWERPOT;

    RENDERGROUP                     m_eRenderGroup  = RENDERGROUP::RENDER_NONALPHABLEND;

    list<SAVE_PROP>                 m_PropSaveInfo;
    weak_ptr<CPreView_InteriorProp> m_pPreviewProp;

private:
    void OnDestroy() override;
    void Free();
};

END