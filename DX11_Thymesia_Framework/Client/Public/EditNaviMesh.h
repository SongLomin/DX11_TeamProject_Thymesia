#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CVIBuffer_Cell;
class CShader;
END

BEGIN(Client)

class CEditNaviMesh :
    public CGameObject
{
    GAMECLASS_H(CEditNaviMesh)
    CLONE_H(CEditNaviMesh, CGameObject)
    SHALLOW_COPY(CEditNaviMesh)

    enum class EDIT_MODE
    {
        VERTICE,
        HEIGHT,
        EDIT_END
    };

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    void Init_Collider(const _float& In_iWidth, const _float& In_iHeight, const _float& In_fPitch, ifstream* is = nullptr);
    void Load_DebugNaviMesh(const _char* In_szModelName);

private:
    void Reset_Picking();

    void Update_Vertice(_float fTimeDelta);
    void Update_Height(_float fTimeDelta);

    void Select_MultiHeightPoints(_float fTimeDelta);
    void Select_RangeHeightPoints(_float fTimeDelta);
    void Select_SingleHeightPoint(_float fTimeDelta);
    // Return Selected Index. -1 is fail to selection.
    _int Select_PickPoint() const;

    void Add_Height(const _float& In_fHeight);

    void Bake_NaviMesh();
    void Load_NaviMesh();

protected: /* For Event Function */
    virtual void OnEventMessage(_uint iArg) override;

private:
    weak_ptr<CRenderer>             m_pRendererCom;
    weak_ptr<CShader>               m_pCellShaderCom;

    _bool                           m_bEdit = false;

private:
    vector<weak_ptr<CCollider>>     m_pPickPointColliderComs;
    list<weak_ptr<CVIBuffer_Cell>>  m_pCells;
    

    EDIT_MODE                       m_eEditType = EDIT_MODE::EDIT_END;

    _float2                         m_vSize;
    _float                          m_fPitch = 0.f;
    _uint                           m_iWidthCount = 0;
    _uint                           m_iHeightCount = 0;

    string                          m_szNaviMeshName;
    
private: /* For Vertice Editer */
    _int                            m_iPickingIndex[3];
    _uint                           m_iCurrentPickingIndex = 0;

private: /* For Height Editer */
    vector<_bool>                   m_bSelectedPickPoints;
    _uint                           m_iLastSelectedIndex = -1;

private: /* For Load NaviMesh */
    shared_ptr<MODEL_DATA>          m_pModelData;

public:
    virtual void Free() override;

};

END