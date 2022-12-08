#include "stdafx.h"
#include "Interaction_Prop.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "PhysXCollider.h"

#include "GameInstance.h"
#include "ClientLevel.h"
#include "UI_Interaction.h"

GAMECLASS_C(CInteraction_Prop);

HRESULT CInteraction_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    return S_OK;
}

HRESULT CInteraction_Prop::Start()
{
    __super::Start();

    return S_OK;
}

void CInteraction_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    if (m_bNearPlayer)
    {
        m_fOutLineBlurIntensity += fTimeDelta*2.f;
        m_fOutLineBlurIntensity = min(1.f, m_fOutLineBlurIntensity);
    }
    else
    {
        m_fOutLineBlurIntensity -= fTimeDelta*2.f;
        m_fOutLineBlurIntensity = max(0.f, m_fOutLineBlurIntensity);
    }

    _bool isActEnd = false;
    Callback_ActUpdate(fTimeDelta, isActEnd);

    if (isActEnd)
        Callback_ActUpdate.Clear();
}

void CInteraction_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Prop::Render()
{
    if (FAILED(SetUp_ShaderResource()))
        return E_FAIL;

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            m_iPassIndex = 0;
        }
        else
        {
            m_iPassIndex = 3;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex);
        m_pModelCom.lock()->Render_Mesh(i);
    }

    return __super::Render();
}

_bool CInteraction_Prop::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
    if (!m_pModelCom.lock().get())
        return false;

    return m_pModelCom.lock()->IsModelPicking(In_Ray, Out_fRange);
}

void CInteraction_Prop::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_bNearPlayer = true;


    weak_ptr<CUI_Interaction> pUI_Interaction = GAMEINSTANCE->Get_GameObjects<CUI_Interaction>(LEVEL_STATIC).front();

    if (!pUI_Interaction.lock())
        return;

    pUI_Interaction.lock()->Call_CollisionEnter(pMyCollider, (_uint)m_eInteractionType);

}

    if (Callback_ActUpdate.empty())
    {

    }
}

void CInteraction_Prop::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (Callback_ActUpdate.empty() && KEY_INPUT(KEY::E, KEY_STATE::TAP))
    {
        Act_Interaction();
    }
}

void CInteraction_Prop::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_bNearPlayer = false;

    weak_ptr<CUI_Interaction> pUI_Interaction = GAMEINSTANCE->Get_GameObjects<CUI_Interaction>(LEVEL_STATIC).front();

    if (!pUI_Interaction.lock())
        return;

    pUI_Interaction.lock()->Call_CollisionExit();
}

void CInteraction_Prop::Act_Interaction()
{
}

HRESULT CInteraction_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    return S_OK;
}

HRESULT CInteraction_Prop::SetUp_ShaderResource()
{
    if (FAILED(CProp::SetUp_ShaderResource()))
        return E_FAIL;

    _vector	vShaderFlag = { 1.f,m_fOutLineBlurIntensity,0.f,0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
    {
        DEBUG_ASSERT;
    }    

    return S_OK;
}

void CInteraction_Prop::Free()
{
}
