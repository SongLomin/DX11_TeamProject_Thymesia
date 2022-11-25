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

private:
    typedef struct tag_PropInfo
    {
        weak_ptr<CProp>     pProp;
        _hashcode           hash;
        string              szName;
    } PROPS_DESC;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual _bool IsPicking(const RAY& In_Ray, _float& Out_fRange) override;
    virtual void   OnEventMessage(_uint iArg) override;

private:
    void    View_SelectPropObjectType();
    void    View_SelectModelComponent();
    void    View_PickingInfo();

    void    View_Picking_Prop();
    void    View_Picking_List();
    void    View_Picking_Option();
    void    View_Picking_Option_Y();
    void    View_ModelCopy();
    void    View_SelectTransformInfo();

    void    View_SelectJson();

private:
    void    Pick_Prop();

private:
    HRESULT SetUp_ShaderResource();

    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");

    void Save_Json(string _szName);
    void Load_Json(string _szName);

    virtual void Write_Json(json& Out_Json) override;

private:
    typedef vector<string>                      RESOURCE_LIST;
    typedef vector<PROPS_DESC>                  PROPS_INFOS;

    PROPS_INFOS         m_PropList;
    _bool               m_bSubDraw            = false;
                                              
    _int                m_iPickingIndex       = -1;
    _uint               m_iOption             = 0;
    _bool               m_bSelect_ShowGroup   = false;

    string              m_szSelectPropType    = "CStatic_Prop";
    string              m_szSelectModelName   = "";
    _float4             m_vPickingPos         = _float4(0.f, 0.f, 0.f, 0.f);
    _float              m_fPosY               = 0.f;
    RESOURCE_LIST       m_ModelList;
    RESOURCE_LIST       m_JsonList;

    weak_ptr<CVIBuffer_DynamicCube>     m_pVIBufferCom;
    weak_ptr<CShader>                   m_pShaderCom;
    weak_ptr<CRenderer>                 m_pRendererCom;

public:
    void Free();
};

END