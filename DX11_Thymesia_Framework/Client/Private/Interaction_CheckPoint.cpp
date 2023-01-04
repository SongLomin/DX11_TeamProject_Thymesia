#include "stdafx.h"
#include "Interaction_CheckPoint.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
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

    m_pColliderCom = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pModelCom.lock()->Init_Model("P_ArchiveChair01", "");
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

    XMStoreFloat4(&m_tLightDesc.vPosition, m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 0.5f, 0.f, 0.f));
	m_tLightDesc.vDiffuse   = { 0.f, 1.f, 0.486f, 0.3f };
	m_tLightDesc.vAmbient   = { 1.f, 1.f,    1.f, 0.f };
	m_tLightDesc.vSpecular  = { 0.f, 1.f, 0.486f, 0.6f };
	m_tLightDesc.vLightFlag = { 1.f, 1.f,    1.f, 1.f };
    m_tLightDesc.fIntensity = 0.8f;
	m_tLightDesc.fRange     = 1.5f;

	m_tLightDesc   = GAMEINSTANCE->Add_Light(m_tLightDesc);
    m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("CheckPointChair_Loop", m_pTransformCom.lock(), (_uint)TIMESCALE_LAYER::NONE);

    return S_OK;
}

void CInteraction_CheckPoint::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_CheckPoint::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_CheckPoint::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
}

void CInteraction_CheckPoint::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
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
            GET_SINGLE(CGameManager)->Registration_CheckPoint(Weak_Cast<CInteraction_CheckPoint>(m_this));
    }

    SetUpColliderDesc();
}

void CInteraction_CheckPoint::Act_Interaction()
{
    if (GET_SINGLE(CUIManager)->Is_OpenedMenu())
        return;

    GET_SINGLE(CUIManager)->Set_OpenedMenu(true);

    GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front().lock()->Call_Landing(CUI_Landing::LANDING_BECONFOUND);

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

void CInteraction_CheckPoint::Free()
{
    GET_SINGLE(CGameManager)->UnUse_EffectGroup("CheckPointChair_Loop", m_iEffectIndex);
}
