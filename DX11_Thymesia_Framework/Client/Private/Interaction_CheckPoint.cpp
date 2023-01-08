#include "stdafx.h"
#include "Interaction_CheckPoint.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "ActorDecor.h"
#include "UI_Landing.h"
#include "UIManager.h"

#include "GameInstance.h"
#include "ClientLevel.h"
#include "GameManager.h"
#include "Status_Player.h"
#include "imgui.h"

GAMECLASS_C(CInteraction_CheckPoint);
CLONE_C(CInteraction_CheckPoint, CGameObject);

HRESULT CInteraction_CheckPoint::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_CheckPoint::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom       = Add_Component<CCollider>();
    m_pAnimShader        = Add_Component<CShader>();
    m_pTextureCom        = Add_Component<CTexture>();
    m_pAnimModelCom      = Add_Component<CModel>();
    m_pChairTransfromCom = Add_Component<CTransform>();
    m_pDeco              = GAMEINSTANCE->Add_GameObject<CActorDecor>(m_CreatedLevel);
    
    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pAnimShader.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxAnimModel"),
        VTXANIM_DECLARATION::Element,
        VTXANIM_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("P_ArchiveChair01", "");
    m_pAnimModelCom.lock()->Init_Model("Aisemy_SavePoint", "");
    m_pTextureCom.lock()->Use_Texture("UVMask");
    m_pDeco.lock()->Init_Model("FuckingOilLamp", TIMESCALE_LAYER::NONE);
    m_pDeco.lock()->Init_ActorDecor(m_pAnimModelCom, m_pTransformCom, "weapon_l_end");
    
    m_pDeco.lock()->Set_Enable(false);

    _matrix DecoMatrix = XMMatrixScaling(0.15f, 0.15f, 0.15f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f));
    m_pDeco.lock()->Set_OffsetMatrix(DecoMatrix);
    m_pDeco.lock()->Set_Offset(XMVectorSet(0.05f, 0.f, 0.f, 0.f));

#ifdef _USE_THREAD_
    Use_Thread(THREAD_TYPE::PRE_TICK);
    Use_Thread(THREAD_TYPE::PRE_LATETICK);
#endif // _USE_THREAD_

    return S_OK;
}

HRESULT CInteraction_CheckPoint::Start()
{
    __super::Start();

    if (LEVEL::LEVEL_EDIT == m_CreatedLevel)
        m_pColliderCom.lock()->Set_Enable(false);

    ZeroMemory(&m_tLightDesc, sizeof(LIGHTDESC));
	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_POINT;
	m_tLightDesc.bEnable    = true;

    XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 1.5f, 0.f, 0.f));
	m_tLightDesc.vDiffuse   = { 0.f, 1.f, 0.486f, 0.3f };
	m_tLightDesc.vAmbient   = { 1.f, 1.f,    1.f, 0.f };
	m_tLightDesc.vSpecular  = { 0.f, 1.f, 0.486f, 0.6f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f,    1.f, 1.f };
    m_tLightDesc.fIntensity = 5.f;
	m_tLightDesc.fRange     = 1.f;
    
    _matrix OffsetWorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
    OffsetWorldMatrix.r[3] += XMVector3Normalize(OffsetWorldMatrix.r[0]) * -m_fAisemyOffset;
    m_pTransformCom.lock()->Set_WorldMatrix(OffsetWorldMatrix);
    m_pChairTransfromCom.lock()->Set_WorldMatrix(OffsetWorldMatrix);

	m_tLightDesc   = GAMEINSTANCE->Add_Light(m_tLightDesc);
#ifdef _INTERACTION_EFFECT_
    m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("CheckPointChair_Loop", m_pTransformCom.lock(), (_uint)TIMESCALE_LAYER::NONE);
#endif // _INTERACTION_EFFECT_

    return S_OK;
}

void CInteraction_CheckPoint::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_bAisemyRender)
        m_pAnimModelCom.lock()->Play_Animation(fTimeDelta);

    if (!CallBack_EquipEnd.empty())
    {
        _bool bCheckState = false;

         CallBack_EquipEnd(bCheckState);

         if (bCheckState)
             CallBack_EquipEnd.Clear();
    }

    Update_AnimIndex();
}

void CInteraction_CheckPoint::LateTick(_float fTimeDelta)
{
    if (!m_bRendering)
        return;

    __super::LateTick(fTimeDelta);

    if (m_bAisemyRender)
    {
        m_vAddUVPos.x += fTimeDelta * m_vAddSpeed.x;
        m_vAddUVPos.y += fTimeDelta * m_vAddSpeed.y;
    }
}

void CInteraction_CheckPoint::Thread_PreTick(_float fTimeDelta)
{
    if (!Get_Enable())
        return;

    if (m_bRendering && m_bAisemyRender)
        m_pAnimModelCom.lock()->Update_BoneMatrices();
}

void CInteraction_CheckPoint::Thread_PreLateTick(_float fTimeDelta)
{
    __super::Thread_PreLateTick(fTimeDelta);

#ifdef _Actor_Culling_
    // TODO : 과연 수정을 할 것인가?

    //m_bRendering = true;

    if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), 30.f))
    {
        m_bRendering = true;

#ifdef _PROP_EFFECT_
        if (0 > m_iEffectIndex)
        {
            m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("CheckPointChair_Loop", m_pChairTransfromCom.lock(), (_uint)TIMESCALE_LAYER::NONE);
        }
#endif
    }
    else
    {
        m_bRendering = false;

#ifdef _PROP_EFFECT_
        if (0 <= m_iEffectIndex)
        {
            GET_SINGLE(CGameManager)->UnUse_EffectGroup("CheckPointChair_Loop", m_iEffectIndex);
            m_iEffectIndex = -1;
        }
#endif
    }
#else
    m_bRendering = true;
#endif // _Actor_Culling_
}

HRESULT CInteraction_CheckPoint::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(Draw_Chair(pDeviceContext)))
        return E_FAIL;

    if (FAILED(Draw_Aisemy(pDeviceContext)))
        return E_FAIL;

    CGameObject::Render(pDeviceContext);

    return S_OK;
}

void CInteraction_CheckPoint::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EXIT_SECTION:
        {
            if (ANIM_EVENT::EQUIP_LOOP == m_eAnimEvent || ANIM_EVENT::EQUIP_BEGINE == m_eAnimEvent || ANIM_EVENT::BEGINE == m_eAnimEvent)
            {
                CallBack_EquipEnd += bind(&CInteraction_CheckPoint::Call_CheckEquipEnd, this, placeholders::_1);
            }
        }
        break;

        case EVENT_TYPE::ON_EDITINIT:
        {
            if (!m_pColliderCom.lock())
                SetUpColliderDesc();
        }
        break;

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            _float OffsetY = m_pColliderCom.lock()->Get_ColliderDesc().vTranslation.y;

            if (ImGui::InputFloat("Offset Y", &OffsetY))
            {
                COLLIDERDESC ColliderDesc;
                ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

                ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();
                ColliderDesc.vTranslation.y = OffsetY;

                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            }

            ImGui::InputInt("CheckIndex", &m_iCheckIndex);

            ImGui::Separator();
            ImGui::Text("");

            ImGui::InputInt("Tex", &m_iTexPass);
            ImGui::InputFloat2("Speed", &m_vAddSpeed.x);
            ImGui::InputFloat("AisemyOffset", &m_fAisemyOffset);

            ImGui::Separator();
            ImGui::Text("");

            if (ImGui::InputInt("AnimIndex", &m_iAnimIndex))
                m_pAnimModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
            if (ImGui::InputFloat("AnimSpeed", &m_fAnimSpeed))
                m_pAnimModelCom.lock()->Set_AnimationSpeed(m_fAnimSpeed);
            ImGui::Checkbox("AisemyRender", &m_bAisemyRender);

            ImGui::Separator();
            ImGui::Text("");

            static _float3 vDecoOffset = { 0.f, 0.f, 0.f };
            static _float3 vDecoRot    = { 0.f, 0.f, 0.f };

            if (ImGui::InputFloat3("vDecoOffset", &vDecoOffset.x))
            {
                m_pDeco.lock()->Set_Offset(XMLoadFloat3(&vDecoOffset));
            }

            if (ImGui::InputFloat3("vDecoRot", &vDecoRot.x))
            {
                _matrix DecoMatrix 
                    = XMMatrixScaling(0.15f, 0.15f, 0.15f)
                    * XMMatrixRotationX(XMConvertToRadians(vDecoRot.x))
                    * XMMatrixRotationY(XMConvertToRadians(vDecoRot.y))
                    * XMMatrixRotationZ(XMConvertToRadians(vDecoRot.z));

                m_pDeco.lock()->Set_OffsetMatrix(DecoMatrix);
            }

            ImGui::Separator();
            ImGui::Text("");
        }
        break;
    }
}


void CInteraction_CheckPoint::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);

    Out_Json["CheckIndex"] = m_iCheckIndex;
}

void CInteraction_CheckPoint::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    if (In_Json.end() != In_Json.find("CheckIndex"))
    {
        m_iCheckIndex = In_Json["CheckIndex"];

        // TODO : Save 기능이 구현될 경우 수정하기, 스테이지 시작시 처음 인덱스 의자를 체크포인트로 등록하는 기능임
        if (0 == m_iCheckIndex)
        {
            GET_SINGLE(CGameManager)->Registration_CheckPoint(Weak_Cast<CInteraction_CheckPoint>(m_this));
            Set_State(ANIM_EVENT::BEGINE);
        }
    }

    SetUpColliderDesc();
}

void CInteraction_CheckPoint::Act_Interaction()
{
    if (GET_SINGLE(CUIManager)->Is_OpenedMenu())
        return;

    GET_SINGLE(CUIManager)->Set_OpenedMenu(true);

    GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_BECONFOUND);
    Set_State((m_bAisemyRender) ? (ANIM_EVENT::EQUIP_BEGINE) : (ANIM_EVENT::BEGINE));
    m_bInteraction = true;

    weak_ptr<CStatus_Player> pPlayerStatus = GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status();

    if (pPlayerStatus.lock())
        pPlayerStatus.lock()->Full_Recovery();

    GET_SINGLE(CGameManager)->Registration_CheckPoint(Weak_Cast<CInteraction_CheckPoint>(m_this));
    GET_SINGLE(CGameManager)->ResetWorld();
}

void CInteraction_CheckPoint::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::CHECKPOINT;
    ColliderDesc.vScale = _float3(3.f, 0.f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

HRESULT CInteraction_CheckPoint::Draw_Chair(ID3D11DeviceContext* pDeviceContext)
{
    _matrix Mat = m_pTransformCom.lock()->Get_WorldMatrix();
    Mat.r[3] += XMVector3Normalize(Mat.r[0]) * -m_fAisemyOffset;
    _float4x4 WorldMatrix;
    XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(Mat));

    if (FAILED(m_pChairTransfromCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _float4 vCamDesc;
    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
    m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4));

    XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
    m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4));

    _float4 vPlayerPos;
    XMStoreFloat4(&vPlayerPos, GET_SINGLE(CGameManager)->Get_PlayerPos());
    m_pShaderCom.lock()->Set_RawValue("g_vPlayerPosition", &vPlayerPos, sizeof(_float4));

    if (FAILED(m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92)))
        return E_FAIL;

    _vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _float fCamFar = GAMEINSTANCE->Get_CameraFar();
    m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            m_iPassIndex = 0;
        else
            m_iPassIndex = 7;

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

HRESULT CInteraction_CheckPoint::Draw_Aisemy(ID3D11DeviceContext* pDeviceContext)
{
    if (!m_bAisemyRender)
        return S_OK;

    if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pAnimShader, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pAnimShader.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
        return E_FAIL;
    if (FAILED(m_pAnimShader.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
        return E_FAIL;

    _vector	vShaderFlag = { 0.f,0.f,0.f,0.f };
    if (FAILED(m_pAnimShader.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
        return E_FAIL;

    _float fCamFar = GAMEINSTANCE->Get_CameraFar();
    m_pAnimShader.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));

    m_pTextureCom.lock()->Set_ShaderResourceView(m_pAnimShader, "g_NoiseTexture", m_iTexPass);
    m_pAnimShader.lock()->Set_RawValue("g_vAddUVPos", &m_vAddUVPos, sizeof(_float2));

    _uint iNumMeshContainers = m_pAnimModelCom.lock()->Get_NumMeshContainers();
    for (_uint i(0); i < iNumMeshContainers; ++i)
    {
        m_pAnimModelCom.lock()->Bind_SRV(m_pAnimShader, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
        m_pAnimModelCom.lock()->Bind_SRV(m_pAnimShader, "g_NormalTexture" , i, aiTextureType_NORMALS);

        m_pAnimModelCom.lock()->Render_AnimModel(i, m_pAnimShader, 10, "g_Bones", pDeviceContext);
    }

    return S_OK;
}   

void CInteraction_CheckPoint::Enter_AnimIndex()
{
    switch (m_eAnimEvent)
    {
        case Client::CInteraction_CheckPoint::BEGINE:
        {
            m_pAnimModelCom.lock()->Set_CurrentAnimation(BEGINE);
            m_pAnimModelCom.lock()->Set_AnimationSpeed(2.f);
            m_pAnimModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_CheckPoint::Call_CheckAnimEnd, this);
            m_pDeco.lock()->Set_Enable(true);

            m_bAisemyRender = true;
        }
        break;

        case Client::CInteraction_CheckPoint::STAY:
        {
            m_pAnimModelCom.lock()->Set_CurrentAnimation(STAY);
            m_pAnimModelCom.lock()->Set_AnimationSpeed(1.f);
            m_pAnimModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_CheckPoint::Call_CheckAnimEnd, this);
        }
        break;

        case Client::CInteraction_CheckPoint::EQUIP_END:
        {
            m_pAnimModelCom.lock()->Set_CurrentAnimation(EQUIP_END);
            m_pAnimModelCom.lock()->Set_AnimationSpeed(1.5f);
            m_pAnimModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_CheckPoint::Call_CheckAnimEnd, this);
        }
        break;

        case Client::CInteraction_CheckPoint::EQUIP_LOOP:
        {
            m_pAnimModelCom.lock()->Set_CurrentAnimation(EQUIP_LOOP);
            m_pAnimModelCom.lock()->Set_AnimationSpeed(1.f);
            m_pAnimModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_CheckPoint::Call_CheckAnimEnd, this);
        }
        break;

        case Client::CInteraction_CheckPoint::EQUIP_BEGINE:
        {
            m_pAnimModelCom.lock()->Set_CurrentAnimation(EQUIP_BEGINE);
            m_pAnimModelCom.lock()->Set_AnimationSpeed(1.5f);
            m_pAnimModelCom.lock()->CallBack_AnimationEnd += bind(&CInteraction_CheckPoint::Call_CheckAnimEnd, this);
        }
        break;

    }
}

void CInteraction_CheckPoint::Update_AnimIndex()
{
    switch (m_eAnimEvent)
    {
        case Client::CInteraction_CheckPoint::BEGINE:
        {
            if (m_bAnimEnd)
            {
                Set_State(ANIM_EVENT::STAY);
            }
        }
        break;

        case Client::CInteraction_CheckPoint::STAY:
        {
            if (m_bInteraction)
            {
                Set_State(ANIM_EVENT::EQUIP_BEGINE);
            }
        }
        break;

        case Client::CInteraction_CheckPoint::EQUIP_END:
        {
            if (m_bAnimEnd)
            {
                Set_State(ANIM_EVENT::STAY);
                m_bInteraction = false;
            }
        }
        break;

        case Client::CInteraction_CheckPoint::EQUIP_LOOP:
        {
        }
        break;

        case Client::CInteraction_CheckPoint::EQUIP_BEGINE:
        {
            if (m_bAnimEnd)
            {
                Set_State(ANIM_EVENT::EQUIP_LOOP);
            } 
        }
        break;
    }
}

void CInteraction_CheckPoint::Exit_AnimIndex()
{
    m_pAnimModelCom.lock()->CallBack_AnimationEnd.Clear();
}

void CInteraction_CheckPoint::Set_State(const ANIM_EVENT In_eEvent)
{
    Exit_AnimIndex();
    
    m_eAnimEvent = In_eEvent;

    Enter_AnimIndex();

    m_bAnimEnd = false;
}

void CInteraction_CheckPoint::Call_CheckEquipEnd(_bool& bState)
{
    if (m_bAnimEnd)
    {
        Set_State(ANIM_EVENT::EQUIP_END);
        bState = true;
    }
}

void CInteraction_CheckPoint::Call_CheckAnimEnd()
{
    m_bAnimEnd = true;
}

void CInteraction_CheckPoint::OnDestroy()
{
    if (0 <= m_iEffectIndex)
        GET_SINGLE(CGameManager)->UnUse_EffectGroup("CheckPointChair_Loop", m_iEffectIndex);
}

void CInteraction_CheckPoint::Free()
{
}
