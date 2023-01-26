#include "stdafx.h"
#include "Preset_PhysXColliderDesc.h"
#include "PhysXCollider.h"
#include "GameInstance.h"
#include "Client_Components.h"
#include "Transform.h"
#include "SMath.h" 

//void Preset::PhysXColliderDesc::PlayerBodySetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
//{
//    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CYLINDER;
//    Out_Desc.eActorType = PHYSXACTOR_TYPE::DYNAMIC;
//    Out_Desc.iFilterType = (_uint)COLLISION_LAYER::PLAYER;
//    Out_Desc.fDensity = 1000.f;
//    PxConvexMesh* pCylinderMesh = nullptr;
//    GAMEINSTANCE->Create_CylinderMesh(0.6f, 0.6f, 3.f, &pCylinderMesh);
//    Out_Desc.pConvecMesh = pCylinderMesh;
//    Out_Desc.vAngles = { 0.f, 0.f, 0.f, 0.f };
//    Out_Desc.vPosition = pTransform.lock()->Get_Position();
//    Out_Desc.vScale = { 1.f, 1.f, 1.f };
//    PxMaterial* pMaterial = nullptr;
//    GAMEINSTANCE->Create_Material(1.f, 1.f, 0.f, &pMaterial);
//    Out_Desc.pMaterial = pMaterial;
//    Out_Desc.bTrigger = false;
//}
//
//void Preset::PhysXColliderDesc::PlayerBodyTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
//{
//    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CYLINDER;
//    Out_Desc.eActorType = PHYSXACTOR_TYPE::DYNAMIC;
//    Out_Desc.iFilterType = (_uint)COLLISION_LAYER::PLAYER;
//    Out_Desc.fDensity = 10.f;
//    PxConvexMesh* pCylinderMesh = nullptr;
//    GAMEINSTANCE->Create_CylinderMesh(0.5f, 0.5f, 1.f, &pCylinderMesh);
//    Out_Desc.pConvecMesh = pCylinderMesh;
//    Out_Desc.vAngles = { 0.f, 0.f, 0.f, 0.f };
//    Out_Desc.vPosition = pTransform.lock()->Get_Position();
//    Out_Desc.vScale = { 1.f, 1.f, 1.f };
//    PxMaterial* pMaterial = nullptr;
//    GAMEINSTANCE->Create_Material(1.f, 1.f, 0.f, &pMaterial);
//    Out_Desc.pMaterial = pMaterial;
//    Out_Desc.bTrigger = true;
//}

void Preset::PhysXColliderDesc::GroundSetting(PHYSXCOLLIDERDESC& Out_Desc, const _bool bTrigger)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::MESH;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::GROUND;
    Out_Desc.fDensity = 0.f;
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.f, 0.f, 0.f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.vAngles = { 0.f, 0.f, 0.f, 0.f };
    Out_Desc.vPosition = { 0.f, 0.f, 0.f, 1.f };
    Out_Desc.vScale = { 1.f, 1.f, 1.f };
    Out_Desc.bTrigger = bTrigger;
}

void Preset::PhysXColliderDesc::TestLightPropSetting(PHYSXCOLLIDERDESC& Out_Desc)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::MONSTER;
    Out_Desc.fDensity = 5.f;
    PxConvexMesh* pCylinderMesh = nullptr;
    //GAMEINSTANCE->Create_CylinderMesh(0.3f, 0.3f, 1.f, &pCylinderMesh);
    Out_Desc.pConvecMesh = nullptr;
    Out_Desc.vAngles = { 0.f, 0.f, 0.f, 0.f };
    Out_Desc.vPosition = { 4.f, 1.f, 4.f, 1.f };
    Out_Desc.vScale = { 1.f, 1.f, 1.f };
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}

void Preset::PhysXColliderDesc::TestLightPropTriggerSetting(PHYSXCOLLIDERDESC& Out_Desc)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CYLINDER;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::DYNAMIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::MONSTER;
    Out_Desc.fDensity = 5.f;
    PxConvexMesh* pCylinderMesh = nullptr;
    GAMEINSTANCE->Create_CylinderMesh(0.3f, 0.3f, 1.f, &pCylinderMesh);
    Out_Desc.pConvecMesh = pCylinderMesh;
    Out_Desc.vAngles = { 0.f, 0.f, 0.f, 0.f };
    Out_Desc.vPosition = { 3.f, 15.f, 3.f, 1.f };
    Out_Desc.vScale = { 1.f, 1.f, 1.f };
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = true;
}

void Preset::PhysXColliderDesc::StaticPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::MODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();

    _vector vPos = pTransform.lock()->Get_Position();
    // vPos.m128_f32[1] += fCenterY * vScale.y;
    Out_Desc.vPosition = vPos;

    Out_Desc.vScale = XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;

}

void Preset::PhysXColliderDesc::DynamicPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PROP;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();

    _vector vPos = pTransform.lock()->Get_Position();
    // vPos.m128_f32[1] += fCenterY * vScale.y;
    Out_Desc.vPosition = vPos;

    Out_Desc.vScale = XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}

void Preset::PhysXColliderDesc::DynamicBottleSetting(PHYSXCOLLIDERDESC& Out_Desc, _fmatrix In_WorldMatrix)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::DYNAMIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PROP;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(In_WorldMatrix));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);

    Out_Desc.vPosition = In_WorldMatrix.r[3];

    Out_Desc.vScale = SMath::Get_Scale(In_WorldMatrix);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;

}

void Preset::PhysXColliderDesc::PlayerWeaponSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::PLAYER_WEAPON;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();

    _vector vPos = pTransform.lock()->Get_Position();
    // vPos.m128_f32[1] += fCenterY * vScale.y;
    Out_Desc.vPosition = vPos;

    Out_Desc.vScale =  1.5f * XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;

}

void Preset::PhysXColliderDesc::ConvexStaticPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();

    _vector vPos = pTransform.lock()->Get_Position();
    // vPos.m128_f32[1] += fCenterY * vScale.y;
    Out_Desc.vPosition = vPos;

    Out_Desc.vScale = XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}

void Preset::PhysXColliderDesc::DynamicPieceSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::DYNAMIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PEICE;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();

    _vector vPos = pTransform.lock()->Get_Position();
    // vPos.m128_f32[1] += fCenterY * vScale.y;
    Out_Desc.vPosition = vPos;

    Out_Desc.vScale = XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}

void Preset::PhysXColliderDesc::StaticInstancingPropSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::CONVEXMODEL;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();
    _vector vPos = pTransform.lock()->Get_Position();
    Out_Desc.vPosition = vPos;
    Out_Desc.vScale = XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(0.5f, 0.5f, 0.5f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}

void Preset::PhysXColliderDesc::StaticBoxDefaultSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform)
{
    Out_Desc.eShape = PHYSXCOLLIDER_TYPE::BOX;
    Out_Desc.eActorType = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP;
    Out_Desc.fDensity = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles = XMLoadFloat3(&PitchYawRoll);
    _float3 vScale = pTransform.lock()->Get_Scaled();
    _vector vPos = pTransform.lock()->Get_Position();
    //vPos.m128_f32[1] += vScale.y * 0.5f;
    Out_Desc.vPosition = vPos;
    Out_Desc.vScale = XMLoadFloat3(&vScale);
    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(1.f, 1.f, 0.f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}

void Preset::PhysXColliderDesc::StaticInteriorBoxDefaultSetting(PHYSXCOLLIDERDESC& Out_Desc, weak_ptr<CTransform> pTransform, MESH_VTX_INFO& MeshData, _float* _pOut)
{
    Out_Desc.eShape      = PHYSXCOLLIDER_TYPE::BOX;
    Out_Desc.eActorType  = PHYSXACTOR_TYPE::STATIC;
    Out_Desc.iFilterType = (_uint)PHYSX_COLLISION_LAYER::INTERIOR;
    Out_Desc.fDensity    = 5.f;
    Out_Desc.pConvecMesh = nullptr;

    _float3 PitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(pTransform.lock()->Get_WorldMatrix()));
    Out_Desc.vAngles     = XMLoadFloat3(&PitchYawRoll);

    _float3 vScale       = pTransform.lock()->Get_Scaled();
    vScale.x *= (MeshData.vMax.x - MeshData.vMin.x);
    vScale.y *= (MeshData.vMax.y - MeshData.vMin.y);
    //vScale.y *= 10.f;
    vScale.z *= (MeshData.vMax.z - MeshData.vMin.z);

    (*_pOut) = (MeshData.vMax.y - MeshData.vMin.y);

    _vector vPos = pTransform.lock()->Get_Position();

    Out_Desc.vPosition = vPos;
    Out_Desc.vScale    = XMLoadFloat3(&vScale);

    PxMaterial* pMaterial = nullptr;
    GAMEINSTANCE->Create_Material(1.f, 1.f, 0.f, &pMaterial);
    Out_Desc.pMaterial = pMaterial;
    Out_Desc.bTrigger = false;
}
