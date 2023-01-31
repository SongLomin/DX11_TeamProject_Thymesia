#include "stdafx.h"
#include "Interaction_CastleGate.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"
#include "PhysXCollider.h"
#include "Inventory.h"
#include "Item.h"


#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"
#include "SMath.h"
#include "UI_ItemRequirement.h"


GAMECLASS_C(CInteraction_CastleGate);
CLONE_C(CInteraction_CastleGate, CGameObject);

HRESULT CInteraction_CastleGate::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_CastleGate::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pDoorRightModelCom      = Add_Component<CModel>();
    m_pDoorLeftModelCom       = Add_Component<CModel>();
    m_pGearModelCom           = Add_Component<CModel>();
    m_pColliderCom            = Add_Component<CCollider>();
    m_pRightPhysXColliderCom  = Add_Component<CPhysXCollider>();
    m_pLeftPhysXColliderCom   = Add_Component<CPhysXCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));
    m_eInteractionType = INTERACTION_DOOR;

    m_pModelCom.lock()->Init_Model("P_CityGateMachine01_01_Body");
    m_pDoorRightModelCom.lock()->Init_Model("P_MainChurchDoorRight01");
    m_pDoorLeftModelCom.lock()->Init_Model("P_MainChurchDoorLeft01");
    m_pGearModelCom.lock()->Init_Model("P_CityGateMachine01_01_Roll");

    _float fDefaultDesc[4] = { 3.5f, -1.f, 1.5f, 0.f };
    SetUpColliderDesc(m_pColliderCom, fDefaultDesc, COLLISION_LAYER::TRIGGER);

    XMStoreFloat4x4(&m_RightDoorMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_LeftDoorMatrix , XMMatrixIdentity());

    return S_OK;
}

HRESULT CInteraction_CastleGate::Start()
{
    __super::Start();

    weak_ptr<MODEL_DATA> pModelData = m_pModelCom.lock()->Get_ModelData();

    if (pModelData.lock())
    {
        m_vCenterOffset       = pModelData.lock()->VertexInfo.vCenter;
        m_fCullingOffsetRange = pModelData.lock()->Get_MaxOffsetRange() * 15.f;
    }

    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    if (LEVEL::LEVEL_EDIT != m_CreatedLevel)
    {
        _matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

        _matrix RightWorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
        RightWorldMatrix.r[0]  = XMVector3Normalize(RightWorldMatrix.r[0]) * m_fDoorSize.x;
        RightWorldMatrix.r[1]  = XMVector3Normalize(RightWorldMatrix.r[1]) * m_fDoorSize.y;
        RightWorldMatrix.r[2]  = XMVector3Normalize(RightWorldMatrix.r[2]) * m_fDoorSize.z;
        RightWorldMatrix.r[3] += XMLoadFloat3(&m_vOffset);

        _matrix LeftWorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
        LeftWorldMatrix.r[0]  = XMVector3Normalize(LeftWorldMatrix.r[0]) * m_fDoorSize.x;
        LeftWorldMatrix.r[1]  = XMVector3Normalize(LeftWorldMatrix.r[1]) * m_fDoorSize.y;
        LeftWorldMatrix.r[2]  = XMVector3Normalize(LeftWorldMatrix.r[2]) * m_fDoorSize.z;
        LeftWorldMatrix.r[3] += XMLoadFloat3(&m_vOffset) + XMVectorSet(m_fDoorOffset, 0.f, 0.f, 0.f);

        XMStoreFloat4x4(&m_RightDoorMatrix, RightWorldMatrix);
        XMStoreFloat4x4(&m_LeftDoorMatrix , LeftWorldMatrix);

#ifdef _GENERATE_PROP_COLLIDER_
        PhysXColliderDesc tDesc;
        m_pRightPhysXColliderCom.lock()->Init_ModelCollider(m_pDoorRightModelCom.lock()->Get_ModelData(), true);
        m_pTransformCom.lock()->Set_WorldMatrix(XMLoadFloat4x4(&m_RightDoorMatrix));
        Preset::PhysXColliderDesc::ConvexStaticPropSetting(tDesc, m_pTransformCom);
        m_pRightPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
        m_pRightPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();
        m_pRightPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);

        m_pLeftPhysXColliderCom.lock()->Init_ModelCollider(m_pDoorLeftModelCom.lock()->Get_ModelData(), true);
        m_pTransformCom.lock()->Set_WorldMatrix(XMLoadFloat4x4(&m_LeftDoorMatrix));
        Preset::PhysXColliderDesc::ConvexStaticPropSetting(tDesc, m_pTransformCom);
        m_pLeftPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
        m_pLeftPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();
        m_pLeftPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
#endif

        m_pTransformCom.lock()->Set_WorldMatrix(WorldMatrix);
    }

    CallBack_Requirement     += bind(&CInteraction_CastleGate::Requirement_Key, this, placeholders::_1);
    Callback_UpdateComponent += bind(&CInteraction_CastleGate::Update_PhysX, this);

    return S_OK;
}

void CInteraction_CastleGate::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_CastleGate::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    Callback_UpdateComponent();
}

HRESULT CInteraction_CastleGate::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource_Default(pDeviceContext)))
        return E_FAIL;

    DrawShader_Body(pDeviceContext);
    DrawShader_Door(pDeviceContext);
    DrawShader_Gear(pDeviceContext);

    CGameObject::Render(pDeviceContext);

    return S_OK;
}

void CInteraction_CastleGate::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            SetUp_Invisibility();

            ImGui::DragFloat3("Offset", &m_vOffset.x);

            static const char* szKeyTag[] =
            {
                "GARDEN_KEY",
                "VARG_KEY"
            };

            static _int iSelect_KeyID = 0;

            if (ImGui::Combo("Key ID", &iSelect_KeyID, szKeyTag, IM_ARRAYSIZE(szKeyTag)))
                m_iKeyID = (ITEM_NAME)((_uint)ITEM_NAME::GARDEN_KEY + iSelect_KeyID);

            ImGui::InputFloat("Door Offset" , &m_fDoorOffset);
            ImGui::InputFloat3("Gear Offset", &m_vGearOffset.x);
            ImGui::InputFloat3("Door Size"  , &m_fDoorSize.x);

            COLLIDERDESC ColliderDesc;
            ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

            ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();

            _bool bChage = false;

            bChage |= ImGui::DragFloat3("Coll Transform", &ColliderDesc.vTranslation.x);
            bChage |= ImGui::DragFloat ("Coll Size"     , &ColliderDesc.vScale.x);

            if (bChage)
            {
                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
            }
        }
        break;
    }
}

void CInteraction_CastleGate::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["KeyID"]         = m_iKeyID;
    Out_Json["DoorOffset"]    = m_fDoorOffset;
    CJson_Utility::Write_Float3(Out_Json["DoorSize"]  , m_fDoorSize);
    CJson_Utility::Write_Float3(Out_Json["Offset"]    , m_vOffset);
    CJson_Utility::Write_Float3(Out_Json["GearOffset"], m_vGearOffset);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_CastleGate::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_iKeyID        = In_Json["KeyID"];
    m_fDoorOffset   = In_Json["DoorOffset"];
    CJson_Utility::Load_Float3(In_Json["DoorSize"], m_fDoorSize);
    CJson_Utility::Load_Float3(In_Json["Offset"]  , m_vOffset);
}

void CInteraction_CastleGate::Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fGearRotationRadian += XMConvertToRadians(160.f) * fTimeDelta;
    m_fDoorRotationRadian += XMConvertToRadians(7.f)  * fTimeDelta;

    if (XMConvertToRadians(80.f) <= m_fDoorRotationRadian)
    {
        m_bActionFlag         = false;
        Out_IsEnd             = true;
        m_fDoorRotationRadian = XMConvertToRadians(80.f);

        Callback_ActEnd();
    }
}

void CInteraction_CastleGate::Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fGearRotationRadian -= XMConvertToRadians(160.f) * fTimeDelta;
    m_fDoorRotationRadian -= XMConvertToRadians(7.f)  * fTimeDelta;

    if (0.f >= m_fDoorRotationRadian)
    {
        m_bActionFlag         = true;
        Out_IsEnd             = true;
        m_fDoorRotationRadian = 0.f;

        Callback_ActEnd();
    }
}

void CInteraction_CastleGate::Act_Interaction()
{
    if (m_bActionFlag)
    {
        GAMEINSTANCE->Get_GameObjects<CUI_ItemRequirement>(LEVEL_STATIC).front().lock()->Call_UseItem(m_iKeyID);
        Callback_ActUpdate += bind(&CInteraction_CastleGate::Act_OpenDoor, this, placeholders::_1, placeholders::_2);

    }
    else
    {
        Callback_ActUpdate += bind(&CInteraction_CastleGate::Act_CloseDoor, this, placeholders::_1, placeholders::_2);
    }

    GAMEINSTANCE->PlaySound3D("EVM_Door_Industrial_Open_01.wav", 3.f, m_pTransformCom.lock()->Get_Position());
}

void CInteraction_CastleGate::Requirement_Key(_bool& Out_bRequirement)
{
    if (ITEM_NAME::ITEM_NAME_END == m_iKeyID)
    {
        Out_bRequirement = true;
    }
    else
    {
        weak_ptr<CInventory> pInventory = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock();
        weak_ptr<CItem>      pItem = pInventory.lock()->Find_Item(m_iKeyID);

        Out_bRequirement = (nullptr != pItem.lock());
    }

    if (!Out_bRequirement)
    {
        GAMEINSTANCE->Get_GameObjects<CUI_ItemRequirement>(LEVEL_STATIC).front().lock()->Call_ItemRequireMent(m_iKeyID);
        GAMEINSTANCE->PlaySound3D("EVM_Fantasy_Game_Item_Wooden_Chest_Open_or_Close.ogg", 3.f, m_pTransformCom.lock()->Get_Position());
    }
}

void CInteraction_CastleGate::Update_PhysX()
{
    _matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

    _matrix	LeftRotationMatrix  = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f),  m_fDoorRotationRadian);
    _matrix	RightRotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_fDoorRotationRadian);

    _matrix RightWorldMatrix = WorldMatrix;
    RightWorldMatrix.r[0]  = XMVector3TransformNormal(XMVector3Normalize(RightWorldMatrix.r[0]) * m_fDoorSize.x, LeftRotationMatrix);
    RightWorldMatrix.r[1]  = XMVector3TransformNormal(XMVector3Normalize(RightWorldMatrix.r[1]) * m_fDoorSize.y, LeftRotationMatrix);
    RightWorldMatrix.r[2]  = XMVector3TransformNormal(XMVector3Normalize(RightWorldMatrix.r[2]) * m_fDoorSize.z, LeftRotationMatrix);
    RightWorldMatrix.r[3] += XMLoadFloat3(&m_vOffset);

    _matrix LeftWorldMatrix = WorldMatrix;
    LeftWorldMatrix.r[0]  = XMVector3TransformNormal(XMVector3Normalize(LeftWorldMatrix.r[0]) * m_fDoorSize.x, RightRotationMatrix);
    LeftWorldMatrix.r[1]  = XMVector3TransformNormal(XMVector3Normalize(LeftWorldMatrix.r[1]) * m_fDoorSize.y, RightRotationMatrix);
    LeftWorldMatrix.r[2]  = XMVector3TransformNormal(XMVector3Normalize(LeftWorldMatrix.r[2]) * m_fDoorSize.z, RightRotationMatrix);
    LeftWorldMatrix.r[3] += XMLoadFloat3(&m_vOffset) + XMVectorSet(m_fDoorOffset, 0.f, 0.f, 0.f);

    XMStoreFloat4x4(&m_RightDoorMatrix, RightWorldMatrix);
    XMStoreFloat4x4(&m_LeftDoorMatrix , LeftWorldMatrix);

    m_pTransformCom.lock()->Set_WorldMatrix(XMLoadFloat4x4(&m_RightDoorMatrix));
    m_pRightPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);

    m_pTransformCom.lock()->Set_WorldMatrix(XMLoadFloat4x4(&m_LeftDoorMatrix));
    m_pLeftPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);

    m_pTransformCom.lock()->Set_WorldMatrix(WorldMatrix);
}

HRESULT CInteraction_CastleGate::SetUp_ShaderResource_Default(ID3D11DeviceContext* pDeviceContext)
{
    _float4 vCamDesc;
    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92)))
        return E_FAIL;

    _vector vShaderFlag = { 1.f, m_fOutLineBlurIntensity, 0.f, 0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    return S_OK;
}

HRESULT CInteraction_CastleGate::DrawShader_Body(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }

        if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
            m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);
        else      
            m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, true, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

HRESULT CInteraction_CastleGate::DrawShader_Door(ID3D11DeviceContext* pDeviceContext)
{
    _float4x4 WorldMatrix;
    XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_RightDoorMatrix)));

    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pDoorRightModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pDoorRightModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pDoorRightModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pDoorRightModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }

        m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }   

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pDoorRightModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_LeftDoorMatrix)));

    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
        return E_FAIL;

    iNumMeshContainers = m_pDoorLeftModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pDoorLeftModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pDoorLeftModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pDoorLeftModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }

        m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pDoorLeftModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

HRESULT CInteraction_CastleGate::DrawShader_Gear(ID3D11DeviceContext* pDeviceContext)
{
    _matrix	RotationMatrix = XMMatrixRotationAxis(XMVectorSet(1.f, 0.f, 0.f, 0.f), m_fGearRotationRadian);

    _matrix GearWorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
    GearWorldMatrix.r[0]  = XMVector3TransformNormal(GearWorldMatrix.r[0], RotationMatrix);
    GearWorldMatrix.r[1]  = XMVector3TransformNormal(GearWorldMatrix.r[1], RotationMatrix);
    GearWorldMatrix.r[2]  = XMVector3TransformNormal(GearWorldMatrix.r[2], RotationMatrix);
    GearWorldMatrix.r[3] += XMLoadFloat3(&m_vGearOffset);

    _float4x4 WorldMatrix;
    XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(GearWorldMatrix));

    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _uint iNumMeshContainers = m_pGearModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        _flag BindTextureFlag = 0;

        if (SUCCEEDED(m_pGearModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            BindTextureFlag |= (1 << aiTextureType_DIFFUSE);
        }

        if (SUCCEEDED(m_pGearModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            BindTextureFlag |= (1 << aiTextureType_NORMALS);
        }

        if (SUCCEEDED(m_pGearModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
        {
            BindTextureFlag |= (1 << aiTextureType_SPECULAR);
        }

        if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
            m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);
        else
            m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, true, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pGearModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

void CInteraction_CastleGate::SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)_eCollLayer; // COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f); 
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    In_pColldierCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    In_pColldierCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_CastleGate::Free()
{
}
