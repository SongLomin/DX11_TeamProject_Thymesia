#include "stdafx.h"
#include "Prop_Fence.h"

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

GAMECLASS_C(CProp_Fence);
CLONE_C(CProp_Fence, CGameObject);

HRESULT CProp_Fence::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CProp_Fence::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pPhysXColliderCom   = Add_Component<CPhysXCollider>();
    m_pDissolveTextureCom = Add_Component<CTexture>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pDissolveTextureCom.lock()->Use_Texture("T_Fire_Tile_BW_03");
    m_pModelCom.lock()->Init_Model("SM_fence_16");

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));
    
    m_iPassIndex = 3;

    return S_OK;
}

HRESULT CProp_Fence::Start()
{
    __super::Start();

    XMStoreFloat4(&m_vFirstPosition, m_pTransformCom.lock()->Get_Position());
    
    if (LEVEL::LEVEL_EDIT != m_CreatedLevel)
    {
        m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), false);
        PhysXColliderDesc tDesc;
        Preset::PhysXColliderDesc::StaticPropSetting(tDesc, m_pTransformCom);
        m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
        m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();

        m_pTransformCom.lock()->Set_Position(XMVectorSet(0.f, 0.f, 0.f, 1.f));
        m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);

        m_pPhysXColliderCom.lock()->Set_Enable(false);
        Set_Enable(false);
    }

    return S_OK;
}

void CProp_Fence::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CProp_Fence::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (!Callback_ActEvent.empty())
    {
        _bool bEnd = false;
        Callback_ActEvent(fTimeDelta, bEnd);

        if (bEnd)
            Callback_ActEvent.Clear();
    }
}

HRESULT CProp_Fence::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource(pDeviceContext)))
        return E_FAIL;

    return __super::Render(pDeviceContext);
}

HRESULT CProp_Fence::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::SetUp_ShaderResource()))
        return E_FAIL;

    if (12 == m_iPassIndex)
    {
        m_pShaderCom.lock()->Set_RawValue("g_fDissolveRatio", &m_fDissolveRatio, sizeof(_float));
        m_pDissolveTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", 0);
    }

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return __super::Render(pDeviceContext);;
}

void CProp_Fence::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_ENTER_SECTION:
        {
            Callback_ActEvent.Clear();
            
            /*if (0.f < m_fDelayTime)
                Callback_ActEvent += bind(&CProp_Fence::Act_Delay, this, placeholders::_1, placeholders::_2);*/
            Callback_ActEvent += bind(&CProp_Fence::Act_Event, this, placeholders::_1, placeholders::_2);

            m_pTransformCom.lock()->Set_Position(XMLoadFloat4(&m_vFirstPosition));
            m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);

            //_bDelayDone     = false;
            m_bDelayDone      = true;
            m_fDissolveRatio = 0.f;
            m_iPassIndex     = 12;

            m_pPhysXColliderCom.lock()->Set_Enable(true);
            Set_Enable(true);
        }
        break;

        case EVENT_TYPE::ON_EXIT_SECTION:
        {
            m_pTransformCom.lock()->Set_Position(XMVectorSet(0.f, 0.f, 0.f, 1.f));
            m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);

            m_iPassIndex = 3;

            m_pPhysXColliderCom.lock()->Set_Enable(false);
            Set_Enable(false);
        }
        break;

        case EVENT_TYPE::ON_RESET_OBJ:
        {
            m_pTransformCom.lock()->Set_Position(XMVectorSet(0.f, 0.f, 0.f, 1.f));
            m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
            m_pPhysXColliderCom.lock()->Set_Enable(false);
            Set_Enable(false);

            m_fDelayTime = 0.f;
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            SetUp_Invisibility();

            ImGui::DragFloat("Delay Time"    , &m_fDelayTime);
            ImGui::DragFloat("Dissolve Speed", &m_fDissolveSpeed);
            ImGui::DragInt("Section Index"   , &m_iSectionIndex);
        }
        break;
    }
}

void CProp_Fence::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["DelayTime"]     = m_fDelayTime;
    Out_Json["DissolveSpeed"] = m_fDissolveSpeed;
    Out_Json["SectionIndex"]  = m_iSectionIndex;
}

void CProp_Fence::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_fDelayTime     = In_Json["DelayTime"];
    m_fDissolveSpeed = In_Json["DissolveSpeed"];
    m_iSectionIndex  = In_Json["SectionIndex"];

    if (0 <= m_iSectionIndex)
        GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void   CProp_Fence::Act_Delay(_float _fTimeDelta, _bool& _bEnd)
{
    if (m_bDelayDone)
        return;

    m_fAccTime += _fTimeDelta;

    if (m_fAccTime >= m_fDelayTime)
    {
        m_fAccTime   = 0.f;
        m_bDelayDone = true;
    }
}

void CProp_Fence::Act_Event(_float _fTimeDelta, _bool& _bEnd)
{
    if (!m_bDelayDone)
        return;

    m_fDissolveRatio += _fTimeDelta * m_fDissolveSpeed;

    if (1.f <= m_fDissolveRatio)
    {
        m_fDissolveRatio = 1.f;
        m_bDelayDone     = false;
        _bEnd            = true;

        return;
    }

    _bEnd = false;
}

void CProp_Fence::OnDestroy()
{
    __super::OnDestroy();

    GET_SINGLE(CGameManager)->Remove_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void CProp_Fence::Free()
{
}
