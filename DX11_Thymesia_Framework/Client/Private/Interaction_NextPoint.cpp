#include "stdafx.h"
#include "Interaction_NextPoint.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "UI_Landing.h"

#include "GameInstance.h"
#include "ClientLevel.h"
#include "GameManager.h"
#include "imgui.h"

GAMECLASS_C(CInteraction_NextPoint);
CLONE_C(CInteraction_NextPoint, CGameObject);

HRESULT CInteraction_NextPoint::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_NextPoint::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();
    m_pTextureCom  = Add_Component<CTexture>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("SM_Aisemy_SavePoint");
    m_pTextureCom.lock()->Use_Texture("UVMask");

    m_eInteractionType = INTERACTIONTYPE::INTERACTION_NEXTPOINT;

    SetUpColliderDesc();

    return S_OK;
}

HRESULT CInteraction_NextPoint::Start()
{
    __super::Start();

    ZeroMemory(&m_tLightDesc, sizeof(LIGHTDESC));
	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_POINT;

    XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 1.5f, 0.f, 0.f));
	m_tLightDesc.vDiffuse   = { 0.f, 1.f, 0.486f, 0.3f };
	m_tLightDesc.vAmbient   = { 1.f, 1.f,    1.f, 0.f };
	m_tLightDesc.vSpecular  = { 0.f, 1.f, 0.486f, 0.6f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f,    1.f, 1.f };
    m_tLightDesc.fIntensity = 5.f;
	m_tLightDesc.fRange     = 1.f;
    m_tLightDesc.bEnable    = (LEVEL::LEVEL_EDIT == m_CreatedLevel);

    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    if (LEVEL::LEVEL_EDIT == m_CreatedLevel || LEVEL::LEVEL_TEST == m_CreatedLevel)
    {
        m_tLightDesc.bEnable = true;
        Set_Enable(true);
        m_pColliderCom.lock()->Set_Enable(true);
    }
    else
    {
        m_tLightDesc.bEnable = false;
        Set_Enable(false);
        m_pColliderCom.lock()->Set_Enable(false);
    }

    m_tLightDesc = GAMEINSTANCE->Add_Light(m_tLightDesc);

    return S_OK;
}

void CInteraction_NextPoint::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_vAddUVPos.x += fTimeDelta * m_vAddSpeed.x;
    m_vAddUVPos.y += fTimeDelta * m_vAddSpeed.y;
}

void CInteraction_NextPoint::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_NextPoint::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource(pDeviceContext)))
        return E_FAIL;

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
        m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

        m_pShaderCom.lock()->Begin(10, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    CGameObject::Render(pDeviceContext);

    return S_OK;
}

void CInteraction_NextPoint::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_ENTER_SECTION:
        {
            Set_Enable(true);
            m_pColliderCom.lock()->Set_Enable(true);

            m_tLightDesc.bEnable = true;
            GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
        }
        break;

        case EVENT_TYPE::ON_EXIT_SECTION:
        {
            Set_Enable(false);
            m_pColliderCom.lock()->Set_Enable(false);

            m_tLightDesc.bEnable = false;
            GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
        }
        break;

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            static const char* item_LevelTag[] =
            {
                "LEVEL_STATIC",
                "LEVEL_LOADING",
                "LEVEL_LOGO",
                "LEVEL_LOBBY",
                "LEVEL_GAMEPLAY",
                "LEVEL_STAGE1",
                "LEVEL_STAGE2",
                "LEVEL_STAGE3",
                "LEVEL_STAGE4",
                "LEVEL_EDIT",
                "LEVEL_TEST",
                "LEVEL_HOME"
            };

            _int iSelect_Level = (_int)m_eNextLevel;

            if (ImGui::Combo("Level", &iSelect_Level, item_LevelTag, IM_ARRAYSIZE(item_LevelTag)))
                m_eNextLevel = (LEVEL)iSelect_Level;

            ImGui::InputInt("SectionIndex", &m_iSectionIndex);

            ImGui::Separator();

            ImGui::InputInt("Tex", &m_iTexPass);
            ImGui::InputFloat2("Speed", &m_vAddSpeed.x);

            ImGui::Separator();
        }
        break;

        case EVENT_TYPE::ON_EDIT_RENDER_ACTIVATE:
        {
            m_tLightDesc.bEnable = true;

            GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
        }
        break;

        case EVENT_TYPE::ON_EDIT_RENDER_DISABLE:
        {
            m_tLightDesc.bEnable = false;

            GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
        }
        break;
    }
}

void CInteraction_NextPoint::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["NextLevel"]    = (_int)m_eNextLevel;
    Out_Json["SectionIndex"] = m_iSectionIndex;

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_NextPoint::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    _int iLevel = In_Json["NextLevel"];

    m_eNextLevel    = (LEVEL)iLevel;
    m_iSectionIndex = In_Json["SectionIndex"];

    if (0 <= m_iSectionIndex)
    {
        m_tLightDesc.bEnable = false;
        GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
    }
}

void CInteraction_NextPoint::Act_Interaction()
{
    if (LEVEL::LEVEL_END == m_eNextLevel)
        return;

    GAMEINSTANCE->PlaySound2D("EVM_prop_falling_platform_fade_01.ogg", 1.f);

    Weak_Cast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->ExitLevel(m_eNextLevel);  
}

HRESULT CInteraction_NextPoint::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(CProp::SetUp_ShaderResource()))
        return E_FAIL;

    m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_iTexPass);
    m_pShaderCom.lock()->Set_RawValue("g_vAddUVPos", &m_vAddUVPos, sizeof(_float2));

    return S_OK;
}

void CInteraction_NextPoint::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(3.f, 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(0.f, 1.5f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_NextPoint::OnDestroy()
{
    __super::OnDestroy();

    GAMEINSTANCE->Remove_Light(m_tLightDesc.Get_LightIndex());
    GET_SINGLE(CGameManager)->Remove_SectionLight(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void CInteraction_NextPoint::Free()
{
}