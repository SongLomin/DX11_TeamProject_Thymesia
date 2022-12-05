#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CVIBuffer_DynamicCube;
class CVIBuffer_Model_Instance;
class CShader;
class CRenderer;
class CTexture;

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
    virtual void Custom_Thread1(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual void   OnEventMessage(_uint iArg) override;

private:
    void    View_SelectModelComponent();
    void    View_PickingInfo();

    void    View_Picking_Prop();
    void    View_Picking_List();
    void    View_Picking_Option();
    void    View_SelectTransformInfo();

    void    View_SelectJson();

    void    View_PhysXOption();

private:
    HRESULT SetUp_ShaderResource();
    void    SetUp_ShaderResource_Select();

    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");
public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;

private:                                                                                                                                                                                                                
    typedef vector<string>                  RESOURCE_LIST;
    typedef vector<INSTANCE_MESH_DESC>      PROP_INFO;
    typedef map<string, weak_ptr<CTexture>> TEXTURE_GROUP;

    PROP_INFO                           m_pPropInfos;
    weak_ptr<CVIBuffer_Model_Instance>  m_pInstanceModelCom;
    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;
    TEXTURE_GROUP                       m_pTextureGroupCom;

    weak_ptr<CVIBuffer_DynamicCube>     m_pSelect_VIBufferCom;
    weak_ptr<CShader>                   m_pSelect_ShaderCom;
                                              
    static _bool        m_bDetailPicking;
    static _uint        m_iOption;

    _int                m_iPickingIndex       = -1;

    string              m_szSelectModelName   = "";
    INSTANCE_MESH_DESC  m_PickingDesc;
    _uint               m_iPassIndex          = 0;

    RESOURCE_LIST       m_ModelList;
    RESOURCE_LIST       m_JsonList;
    _bool               m_bSubDraw            = false;

    _int                m_iColliderType       = 0;
    _bool               m_bViewPhysXInfo      = false;
    _bool               m_bNonCulling         = false;
    _bool               m_bDissolve           = false;
    _float              m_fDissolveRatio      = 0.f;
    _float              m_fDissolveSpeed      = 1.f;

    _bool               m_bUpdateTick         = true;

public:
    void Free();
};

END