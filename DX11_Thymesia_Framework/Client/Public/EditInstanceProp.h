#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_DynamicCube;
class CVIBuffer_Model_Instance;
class CShader;
class CRenderer;
END

BEGIN(Client)

class CProp;

class CEditInstanceProp final :
    public CGameObject
{
    GAMECLASS_H(CEditInstanceProp)
    CLONE_H(CEditInstanceProp, CGameObject)
    SHALLOW_COPY(CEditInstanceProp)

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual void   OnEventMessage(_uint iArg) override;

private:
    void    View_SelectModelComponent();
    void    View_PickingInfo();

    void    View_Picking_Prop();
    void    View_Picking_List();
    void    View_Picking_Option();
    void    View_Picking_Option_Y();
    void    View_SelectTransformInfo();

    void    View_SelectJson();

private:
    HRESULT SetUp_ShaderResource();

    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");

    void Save_Json(string _szName);
    void Load_Json(string _szName);

    virtual void Write_Json(json& Out_Json) override;

private:                                                                                                                                                                                                                
    typedef vector<string>               RESOURCE_LIST;
    typedef vector<INSTANCE_MESH_DESC>   PROP_INFO;

    PROP_INFO                           m_pPropInfos;
    weak_ptr<CVIBuffer_Model_Instance>  m_pInstanceModelCom;
    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;

    weak_ptr<CVIBuffer_DynamicCube>     m_pSelect_VIBufferCom;
    weak_ptr<CShader>                   m_pSelect_ShaderCom;
                                              
    _int                m_iPickingIndex       = -1;

    string              m_szSelectModelName   = "";
    _float4             m_vPickingPos         = _float4(0.f, 0.f, 0.f, 0.f);
    _float              m_fPosY               = 0.f;
    _uint               m_iPassIndex          = 0;

    RESOURCE_LIST       m_ModelList;
    RESOURCE_LIST       m_JsonList;

public:
    void Free();
};

END