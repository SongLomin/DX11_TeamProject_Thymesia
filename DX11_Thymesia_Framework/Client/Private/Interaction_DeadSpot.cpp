#include "stdafx.h"
#include "Interaction_DeadSpot.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"

#include "UI_Landing.h"
#include "UI_Interaction.h"


#include "GameInstance.h"
#include "ClientLevel.h"
#include "GameManager.h"
#include "imgui.h"

GAMECLASS_C(CInteraction_DeadSpot);
CLONE_C(CInteraction_DeadSpot, CGameObject);

HRESULT CInteraction_DeadSpot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_DeadSpot::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("P_DropTombstone01");

    m_eInteractionType = INTERACTIONTYPE::INTERACTION_DEADSPOT;

    SetUpColliderDesc();

    return S_OK;
}

HRESULT CInteraction_DeadSpot::Start()
{
    __super::Start();

    ZeroMemory(&m_tLightDesc, sizeof(LIGHTDESC));
	m_tLightDesc.eActorType = LIGHTDESC::TYPE::TYPE_POINT;
	m_tLightDesc.bEnable    = false;

    XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 0.5f, 0.f, 0.f));
	m_tLightDesc.vDiffuse   = { 0.f, 1.f, 0.486f, 0.3f };
	m_tLightDesc.vAmbient   = { 1.f, 1.f,    1.f, 0.f };
	m_tLightDesc.vSpecular  = { 0.f, 1.f, 0.486f, 0.6f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f,    1.f, 1.f };
    m_tLightDesc.fIntensity = 1.f;
	m_tLightDesc.fRange     = 1.5f;

	m_tLightDesc = GAMEINSTANCE->Add_Light(m_tLightDesc);  
    m_pColliderCom.lock()->Set_Enable(false);
    Set_Enable(false);
#ifdef _INTERACTION_EFFECT_
    m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("DeadSpotEffect", m_pTransformCom.lock(), (_uint)TIMESCALE_LAYER::NONE);
#endif // _INTERACTION_EFFECT_
    return S_OK;
}

void CInteraction_DeadSpot::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_DeadSpot::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_DeadSpot::Render(ID3D11DeviceContext* pDeviceContext)
{

    return __super::Render(pDeviceContext);
}

void CInteraction_DeadSpot::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_RESET_OBJ:
        {
            Set_Enable(false);
            m_pColliderCom.lock()->Set_Enable(false);
            m_iMemory = 0;

            m_tLightDesc.bEnable = false;
            GAMEINSTANCE->Set_LightDesc(m_tLightDesc);
#ifdef _INTERACTION_EFFECT_
            GET_SINGLE(CGameManager)->UnUse_EffectGroup("DeadSpotEffect", m_iEffectIndex);
#endif // _INTERACTION_EFFECT_
        }
        break;
    }
}

void CInteraction_DeadSpot::Init_DeadSpot(_vector In_vPos, _uint In_iMemory)
{
    m_pTransformCom.lock()->Set_Position(In_vPos);

    m_tLightDesc.bEnable = true;
    XMStoreFloat4(&m_tLightDesc.vPosition, In_vPos + XMVectorSet(0.f, 0.5f, 0.f, 0.f));

    GAMEINSTANCE->Set_LightDesc(m_tLightDesc);

    Set_Enable(true);
    m_pColliderCom.lock()->Set_Enable(true);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    m_iMemory = In_iMemory;
}

void CInteraction_DeadSpot::Act_Interaction()
{
    //GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL::LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_BECONFOUND);

    GAMEINSTANCE->PlaySound2D("EVM_ItemPickUp.ogg", 1.f);
    GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->Add_Memory(m_iMemory);
    
    GAMEINSTANCE->Get_GameObjects<CUI_Interaction>(LEVEL_STATIC).front().lock()->Call_CollisionExit();

    OnEventMessage((_uint)EVENT_TYPE::ON_RESET_OBJ);
}

void CInteraction_DeadSpot::SetUpColliderDesc()
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(3.f, 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(0.f, 1.5f, 0.f);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_DeadSpot::OnDestroy()
{
    GAMEINSTANCE->Remove_Light(m_tLightDesc.Get_LightIndex());
}

void CInteraction_DeadSpot::Free()
{
}