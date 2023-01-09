#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_DynamicCube;
class CShader;
class CRenderer;
END

BEGIN(Client)

class CProp;

class CEditGroupProp final :
    public CGameObject
{
    GAMECLASS_H(CEditGroupProp)
    CLONE_H(CEditGroupProp, CGameObject)
    SHALLOW_COPY(CEditGroupProp)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    virtual void  OnEventMessage(_uint iArg) override;

private:
    void    View_CreateProp();
    void    View_OnlyTranformEdit();
    void    View_PropOption();

    void    View_PickProp();
    void    View_EditProp();

    void    View_GroupSetting();

private:
    _bool    Pick_Prop(RAY& _pMouseRayInWorldSpace);
    _bool    RenderView_SelectModelComponent();
    void     RenderView_Transform_Info(weak_ptr<CGameObject> In_Obj);
    void     RenderView_Transform_Edit(weak_ptr<CGameObject> In_Obj);
    void     Update_VtxInfo(weak_ptr<CGameObject> In_pObj, _hashcode In_HashCode);

private:
    HRESULT SetUp_ShaderResource();

    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");

    virtual void Write_Json(json& Out_Json) override;

private:
    typedef vector<const string*>                      RESOURCE_LIST;

    _bool               m_bSubDraw            = false;
                                              
    _int                m_iPickingIndex       = 0;
    _uint               m_iOption             = 0;
    _int                m_iSelect_Find        = 0;
    _uint               m_iSize               = 0;
    _float4x4           m_PickingMatrix;
    MESH_VTX_INFO	    m_tPickingVtxInfo;

    string              m_szSelectModelName   = "";
    _bool               m_bChangModel         = false;

    RESOURCE_LIST       m_ModelList;

    weak_ptr<CVIBuffer_DynamicCube>     m_pVIBufferCom;
    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;

public:
    void Free();
};

END