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
        string              szName = "";
        _hashcode           hash;

        _float3             vPosition[8];
        _uint3			    vIndicse[12];
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
    void    View_EditMode();
    void    View_CrateProp();
    void    View_PickingInfo();
    void    View_ModelList();

private:
    void    Pick_Prop();

private:
    void Load_ResourceList(vector<string>& In_List, const filesystem::path& In_Path, string _szCutName = "");

    void Load_MeshList();
    void Load_MashInfo(string _szName);

    void Save_Json(string _szName);
    void Load_Json(string _szName);

private:
    typedef vector<string>        RESOURCE_LIST;
    typedef vector<PROPS_DESC>    PROPS_INFOS;

    PROPS_INFOS         m_PropList;

    EDIT_MODE           m_eEditMode         = EDIT_MODE::NON;
    _int                m_iPickingIndex     = -1;

    RESOURCE_LIST       m_NonAnimList;
    RESOURCE_LIST       m_AnimList;
    RESOURCE_LIST       m_JsonList;

public:
    void Free();
};

END