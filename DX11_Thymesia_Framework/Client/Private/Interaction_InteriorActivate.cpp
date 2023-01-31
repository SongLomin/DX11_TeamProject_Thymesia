#include "stdafx.h"
#include "Interaction_InteriorActivate.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"

#include "ActorDecor.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"
#include "SMath.h"
#include "UI_ItemRequirement.h"


GAMECLASS_C(CInteraction_InteriorActivate);
CLONE_C(CInteraction_InteriorActivate, CGameObject);

HRESULT CInteraction_InteriorActivate::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_InteriorActivate::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();
    m_pTextureCom  = Add_Component<CTexture>();
    m_pModelCom    = Add_Component<CModel>();
    m_pDeco        = GAMEINSTANCE->Add_GameObject<CActorDecor>(m_CreatedLevel);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxAnimModel"),
        VTXANIM_DECLARATION::Element,
        VTXANIM_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("Aisemy_SavePoint", "");
    m_pTextureCom.lock()->Use_Texture("UVMask");
    m_pDeco.lock()->Init_Model("FuckingOilLamp", TIMESCALE_LAYER::NONE);
    m_pDeco.lock()->Init_ActorDecor(m_pModelCom, m_pTransformCom, "weapon_l_end");

    _matrix DecoMatrix = XMMatrixScaling(0.15f, 0.15f, 0.15f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f));
    m_pDeco.lock()->Set_OffsetMatrix(DecoMatrix);
    m_pDeco.lock()->Set_Offset(XMVectorSet(0.05f, 0.f, 0.f, 0.f));

    SetAnimState(ANIM_EVENT::STAY);

    SetUpColliderDesc();

    //m_eRenderGroup =

#ifdef _USE_THREAD_
    Use_Thread(THREAD_TYPE::PRE_TICK);
#endif // _USE_THREAD_

    return S_OK;
}

HRESULT CInteraction_InteriorActivate::Start()
{
    __super::Start();

    if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
        m_pColliderCom.lock()->Set_Enable(false);

    weak_ptr<MODEL_DATA> pModelData = m_pModelCom.lock()->Get_ModelData();

    if (pModelData.lock())
    {
        m_vCenterOffset       = pModelData.lock()->VertexInfo.vCenter;
        m_fCullingOffsetRange = pModelData.lock()->Get_MaxOffsetRange() * 2.f;
    }

    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    return S_OK;
}

void CInteraction_InteriorActivate::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    m_pModelCom.lock()->Play_Animation(fTimeDelta);

    m_vAddUVPos.x += fTimeDelta * m_vAddSpeed.x;
    m_vAddUVPos.y += fTimeDelta * m_vAddSpeed.y;
}

void CInteraction_InteriorActivate::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    CallBack_AnimPlay();

    if (m_bClear)
    {
        CallBack_AnimPlay.Clear();
        m_bClear = false;
    }
}

void CInteraction_InteriorActivate::Thread_PreTick(_float fTimeDelta)
{
    if (!Get_Enable())
        return;

    if (m_bRendering)
        m_pModelCom.lock()->Update_BoneMatrices();
}

void CInteraction_InteriorActivate::Thread_PreLateTick(_float fTimeDelta)
{
    __super::Thread_PreLateTick(fTimeDelta);

#ifdef _Actor_Culling_
    if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), m_fCullingOffsetRange))
    {
        m_bRendering = true;
    }
    else
    {
        m_bRendering = false;
    }
#else
    m_bRendering = true;
#endif // _Actor_Culling_
}

HRESULT CInteraction_InteriorActivate::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource(pDeviceContext)))
        return E_FAIL;

    CGameObject::Render(pDeviceContext);

    return S_OK;
}

void CInteraction_InteriorActivate::OnEventMessage(_uint iArg)
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
            static _int   iAnimIndex = 0;
            static _float fAnimSpeed = 1.f;

            if (ImGui::InputInt("AnimIndex", &iAnimIndex))
                m_pModelCom.lock()->Set_CurrentAnimation(iAnimIndex);
            if (ImGui::InputFloat("AnimSpeed", &fAnimSpeed))
                m_pModelCom.lock()->Set_AnimationSpeed(fAnimSpeed);
        }
        break;

        case EVENT_TYPE::ON_EDIT_DELETE:
        {
            if (m_pDeco.lock())
                m_pDeco.lock()->Set_Dead();
        }
        break;

    }
}

void CInteraction_InteriorActivate::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CInteraction_InteriorActivate::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);
}


void CInteraction_InteriorActivate::Act_Interaction()
{
    if (ANIM_EVENT::STAY == m_eAnimEvent)
    {    
        SetAnimState(ANIM_EVENT::EQUIP_BEGINE);
        GAMEINSTANCE->PlaySound2D("EVM_prop_summoningstone_activate.wav", 1.f);

        CallBack_AnimPlay += bind(&CInteraction_InteriorActivate::Call_PlayAnimState, this);
        CallBack_Activate += bind(&CInteraction_InteriorActivate::Call_ActivateInterior, this);
    }
    else
    {
        SetAnimState(ANIM_EVENT::EQUIP_END);
        GAMEINSTANCE->PlaySound2D("EVM_Aisemy_EquipOn.wav", 1.f);

        GET_SINGLE(CGameManager)->Activate_Section(3000, EVENT_TYPE::ON_EXIT_SECTION);
    }
}

HRESULT CInteraction_InteriorActivate::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _float fCamFar = GAMEINSTANCE->Get_CameraFar();
    m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

    m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_iTexPass);
    m_pShaderCom.lock()->Set_RawValue("g_vAddUVPos", &m_vAddUVPos, sizeof(_float2));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i(0); i < iNumMeshContainers; ++i)
    {
        m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
        m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture" , i, aiTextureType_NORMALS);

        m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 10, "g_Bones", pDeviceContext);
    }

    return S_OK;
}

void CInteraction_InteriorActivate::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(3.f, 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(0.f, 0.f, 2.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_InteriorActivate::SetAnimState(const ANIM_EVENT _eAnimIndex)
{
    m_eAnimEvent = _eAnimIndex;

    switch (m_eAnimEvent)
    {
        case Client::CInteraction_InteriorActivate::STAY:
        {
            m_pModelCom.lock()->Set_CurrentAnimation(STAY);
            m_pModelCom.lock()->Set_AnimationSpeed(1.f);
            m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_InteriorActivate::Call_CheckAnimEnd, this);
            m_pColliderCom.lock()->Set_Enable(true);

            m_bAnimEnd = false;
        }
        break;

        case Client::CInteraction_InteriorActivate::EQUIP_END:
        {
            m_pModelCom.lock()->Set_CurrentAnimation(EQUIP_END);
            m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
            m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_InteriorActivate::Call_CheckAnimEnd, this);
            m_pColliderCom.lock()->Set_Enable(false);
        
            m_bAnimEnd = false;
        }
        break;

        case Client::CInteraction_InteriorActivate::EQUIP_BEGINE:
        {
            m_pModelCom.lock()->Set_CurrentAnimation(EQUIP_BEGINE);
            m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
            m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_InteriorActivate::Call_CheckAnimEnd, this);
            m_pColliderCom.lock()->Set_Enable(false);

            m_bAnimEnd = false;
        }
        break;

        case Client::CInteraction_InteriorActivate::EQUIP_LOOP:
        {
            m_pModelCom.lock()->Set_CurrentAnimation(EQUIP_LOOP);
            m_pModelCom.lock()->Set_AnimationSpeed(1.f);     

            m_bAnimEnd = false;
        }
        break;
    }
}

void CInteraction_InteriorActivate::Call_CheckAnimEnd()
{
    m_bAnimEnd = true;
}

void CInteraction_InteriorActivate::Call_PlayAnimState()
{
    switch (m_eAnimEvent)
    {
        case Client::CInteraction_InteriorActivate::EQUIP_END:
        {
            if (m_bAnimEnd)
            {
                SetAnimState(ANIM_EVENT::STAY);

                m_bClear = true;
            }
        }
        break;

        case Client::CInteraction_InteriorActivate::EQUIP_LOOP:
        {
            CallBack_Activate();
            CallBack_Activate.Clear();
        }
        break;


        case Client::CInteraction_InteriorActivate::EQUIP_BEGINE:
        {
            if (m_bAnimEnd)
            {
                SetAnimState(ANIM_EVENT::EQUIP_LOOP);
            }
        }
        break;
    }
}

void CInteraction_InteriorActivate::Call_ActivateInterior()
{
    GET_SINGLE(CGameManager)->Activate_Section(3000, EVENT_TYPE::ON_ENTER_SECTION);
    m_pColliderCom.lock()->Set_Enable(true);
}

void CInteraction_InteriorActivate::OnDestroy()
{
}

void CInteraction_InteriorActivate::Free()
{
}
