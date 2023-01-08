#pragma once
#include "ClientComponent.h"

BEGIN(Engine)
class CBoneNode;
END

BEGIN(Client)

class CNvClothCollider :
    public CClientComponent
{
    GAMECLASS_H(CNvClothCollider)
    CLONE_H(CNvClothCollider, CComponent)
    SHALLOW_COPY(CNvClothCollider)

public: /* Member Struct */
    struct NVCLOTH_COLLIDER_DESC
    {
        ::string    szBoneName;
        _float3     vOffset;
        _float      fScale;
    };

private:
    struct NVCLOTH_COLLIDER_INTERNAL_DESC
    {
        weak_ptr<CBoneNode> pBoneNode;
        _float3     vOffset;
        _float      fScale;
    };

public: /* Getter Setter */

private:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;

public:
    HRESULT Init_NvClothColliders(const weak_ptr<CModel> pModelCom, const NVCLOTH_COLLIDER_DESC* In_Descs, const _uint In_iCount);
    HRESULT Update_Colliders(_fmatrix In_WorldMatrix);

    HRESULT Set_Spheres(weak_ptr<CMeshContainer> pMeshContainerCom, const _uint In_BeginIndex = 0, _uint In_EndIndex = -1);

private:
    vector<NVCLOTH_COLLIDER_INTERNAL_DESC>  m_ColliderInfos;
    vector<PxVec4>                          m_ColliderResults;
    weak_ptr<CModel>                        m_pModelCom;

private:
    void Free();
};

END