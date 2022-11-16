#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CProp;

class CEditGroupProp final :
    public CGameObject
{
    GAMECLASS_H(CEditGroupProp)
    CLONE_H(CEditGroupProp, CGameObject)
    SHALLOW_COPY(CEditGroupProp)

private:
    enum class EDIT_MODE
    {
        MOVE,
        PICKING,
        NON,

        EDIT_END
    };

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
    virtual void   OnEventMessage(_uint iArg) override;

private:
    void    View_SelectPropObjectType();
    void    View_EditMode();
    void    View_SelectModelComponent();
    void    View_PickingInfo();

private:
    void    Pick_Prop();
    _bool   Check_Click(RAY _Ray, MESH_VTX_INFO _VtxInfo, _matrix _WorldMatrix);

private:
    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");

    void Load_MeshList();
    void Load_MashInfo(string _szName);

    void Save_Json(string _szName);
    void Load_Json(string _szName);

    virtual void Write_Json(json& Out_Json) override;

private:
    typedef vector<string>                      RESOURCE_LIST;
    typedef vector<PROPS_DESC>                  PROPS_INFOS;

    PROPS_INFOS         m_PropList;
    _bool               m_bSubDraw          = false;

    EDIT_MODE           m_eEditMode         = EDIT_MODE::NON;
    _int                m_iPickingIndex     = -1;

    string              m_szSelectPropType  = "CStatic_Prop";
    string              m_szSelectModelName = "";
    _float4             m_vPickingPos       = _float4(0.f, 0.f, 0.f, 0.f);
    _float              m_fPosY             = 0.f;
    RESOURCE_LIST       m_ModelList;
    RESOURCE_LIST       m_JsonList;

public:
    void Free();
};

END