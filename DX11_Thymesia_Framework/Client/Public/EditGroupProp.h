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

        _float3             vPosition[8];
        _uint3			    vIndicse[12];

        tag_PropInfo() {}
    } PROPS_DESC;

    typedef struct tag_Prop_Prototype
    {
        _hashcode           hash    = 0;
        weak_ptr<CProp>     pProp;

        tag_Prop_Prototype() {}
        tag_Prop_Prototype(_hashcode _hash, weak_ptr<CProp> _pProp)
            : hash      (_hash)
            , pProp     (_pProp)
        {}

    } PROPS_PROTOTYPE;

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

private:
    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");

    void Load_MeshList();
    void Load_MashInfo(string _szName);

    void Save_Json(string _szName);
    void Load_Json(string _szName);

private:
    typedef map<string, PROPS_PROTOTYPE>        PROP_PROTOTYPE;
    typedef vector<string>                      RESOURCE_LIST;
    typedef vector<PROPS_DESC>                  PROPS_INFOS;

    PROPS_INFOS         m_PropList;

    EDIT_MODE           m_eEditMode         = EDIT_MODE::NON;
    _int                m_iPickingIndex     = -1;

    string              m_szSelectPropType  = "CStatic_Prop";
    string              m_szSelectModelName = "";
    _float4             m_vPickingPos       = _float4(0.f, 0.f, 0.f, 0.f);

    PROP_PROTOTYPE      m_PropPrototype;
    RESOURCE_LIST       m_NonAnimList;
    RESOURCE_LIST       m_AnimList;
    RESOURCE_LIST       m_JsonList;

public:
    void Free();
};

END