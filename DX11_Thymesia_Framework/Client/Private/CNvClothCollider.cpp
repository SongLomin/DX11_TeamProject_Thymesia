#include "stdafx.h"
#include "CNvClothCollider.h"
#include "BoneNode.h"
#include "MeshContainer.h"

GAMECLASS_C(CNvClothCollider)
CLONE_C(CNvClothCollider, CComponent)

HRESULT CNvClothCollider::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CNvClothCollider::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CNvClothCollider::Init_NvClothColliders(const weak_ptr<CModel> pModelCom, const NVCLOTH_COLLIDER_DESC* In_Descs, const _uint In_iCount)
{
    HRESULT hr;

    // 'iCount' is over than the number of colliders in NvCloth.
    if (In_iCount > 32)
    {
        return E_INVALIDARG;
    }

    m_ColliderInfos.assign(In_iCount, {});
    m_ColliderResults.assign(In_iCount, {});

    for (_size_t i = 0; i < In_iCount; ++i)
    {
        m_ColliderInfos[i].pBoneNode = pModelCom.lock()->Find_BoneNode(In_Descs[i].szBoneName);

        if (!m_ColliderInfos[i].pBoneNode.lock())
        {
            DEBUG_ASSERT;
            return E_INVALIDARG;
        }

        m_ColliderInfos[i].vOffset = In_Descs[i].vOffset;
        m_ColliderInfos[i].fScale = In_Descs[i].fScale;
    }


    m_pModelCom = pModelCom;

    return S_OK;
}

HRESULT CNvClothCollider::Update_Colliders(_fmatrix In_WorldMatrix)
{
    _int iIndex = 0;

    _float4x4 ModelTransform = m_pModelCom.lock()->Get_TransformationMatrix();
    _matrix ModelTransformToMatrix = XMLoadFloat4x4(&ModelTransform);


    // 30000 nanoseconds
    /*std::for_each(
        std::execution::par,
        m_ColliderInfos.begin(),
        m_ColliderInfos.end(),
        [this, &iIndex, ModelTransformToMatrix, In_WorldMatrix](NVCLOTH_COLLIDER_INTERNAL_DESC& elem)
        {
            _matrix ResultMatrix = elem.pBoneNode.lock()->Get_CombinedMatrix() * ModelTransformToMatrix * In_WorldMatrix;
            _vector vPos = XMLoadFloat3(&elem.vOffset);
            vPos = XMVector3TransformCoord(vPos, ResultMatrix);
            m_ColliderResults[iIndex] = PxVec4(SMath::Convert_PxVec3(vPos), elem.fScale);
        });*/

    // 15000 nanoseconds
    std::for_each(m_ColliderInfos.begin(),
        m_ColliderInfos.end(),
        [this, &iIndex, ModelTransformToMatrix, In_WorldMatrix](NVCLOTH_COLLIDER_INTERNAL_DESC& elem)
        {
            _matrix ResultMatrix = elem.pBoneNode.lock()->Get_CombinedMatrix() * ModelTransformToMatrix * In_WorldMatrix;
            _vector vPos = XMLoadFloat3(&elem.vOffset);
            vPos = XMVector3TransformCoord(vPos, ResultMatrix);
            m_ColliderResults[iIndex++] = PxVec4(SMath::Convert_PxVec3(vPos), elem.fScale);
        });
   

    return S_OK;
}

HRESULT CNvClothCollider::Set_Spheres(weak_ptr<CMeshContainer> pMeshContainerCom, const _uint In_BeginIndex, _uint In_EndIndex)
{
    nv::cloth::Range<const physx::PxVec4> sphereRange(&m_ColliderResults[0], &m_ColliderResults[0] + m_ColliderResults.size());

    Cloth* pCloth = pMeshContainerCom.lock()->Get_NvCloth();

    // MeshContainer have not NvCloth.
    if (!pCloth)
    {
        DEBUG_ASSERT;
        return E_FAIL;
    }

    if ((_uint)-1 == In_EndIndex)
    {
        In_EndIndex = pCloth->getNumSpheres();
    }

    pMeshContainerCom.lock()->Get_NvCloth()->setSpheres(sphereRange, In_BeginIndex, In_EndIndex);

    return S_OK;
}

void CNvClothCollider::Free()
{
}
