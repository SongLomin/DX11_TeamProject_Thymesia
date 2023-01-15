#include "stdafx.h"
#include "Interaction_Prop.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "PhysXCollider.h"
#include "Collider.h"
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

    if (m_bNearPlayer && !m_bUsingInteraction)
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

HRESULT CInteraction_Prop::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(SetUp_ShaderResource(pDeviceContext)))
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
        else
        {
            BEGIN_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
            END_PERFROMANCE_CHECK(m_pModelCom.lock()->Get_ModelKey());
        }
        m_iPassIndex = Preset::ShaderPass::ModelShaderPass(BindTextureFlag, false, false, false);

        if ((_uint)-1 == m_iPassIndex)
        {
            continue;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return __super::Render(pDeviceContext);
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

    m_CollisionIndex.push_back(pMyCollider.lock()->Get_ColliderIndex());

    Callback_ActStart += bind(&CUI_Interaction::Call_ActionStart, pUI_Interaction.lock());
    Callback_ActEnd   += bind(&CUI_Interaction::Call_ActionEnd  , pUI_Interaction.lock());

    pUI_Interaction.lock()->Call_CollisionEnter(pMyCollider, (_uint)m_eInteractionType);
}

void CInteraction_Prop::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    if (Callback_ActUpdate.empty() && KEY_INPUT(KEY::E, KEY_STATE::TAP))
    {
        if (!CallBack_Requirement.empty())
        {
            _bool bCheckState = true;

            CallBack_Requirement(bCheckState);

            if (!bCheckState)
                return;
        }

        Callback_ActStart();

        Act_Interaction();
        m_bUsingInteraction = true;
    }
}

void CInteraction_Prop::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    m_bNearPlayer = false;

    weak_ptr<CUI_Interaction> pUI_Interaction = GAMEINSTANCE->Get_GameObjects<CUI_Interaction>(LEVEL_STATIC).front();

    if (!pUI_Interaction.lock())
        return;

    pUI_Interaction.lock()->Call_CollisionExit();

    Callback_ActStart.Clear();
    Callback_ActEnd.Clear();

    _uint iCheckIndex = pMyCollider.lock()->Get_ColliderIndex();

    auto iter_find = find_if(m_CollisionIndex.begin(), m_CollisionIndex.end(), [&](_uint _iIndex)->_bool 
    {
        return (iCheckIndex == _iIndex);
    });

    if (m_CollisionIndex.end() != iter_find)
        m_CollisionIndex.erase(iter_find);

    m_bUsingInteraction = false;
}

void CInteraction_Prop::Act_Interaction()
{
    GAMEINSTANCE->PlaySound2D("EVM_Fantasy_Game_Item_Wooden_Chest_Open_or_Close.ogg", 1.f);
}

HRESULT CInteraction_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
    return S_OK;
}

HRESULT CInteraction_Prop::SetUp_ShaderResource(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(CProp::SetUp_ShaderResource()))
        return E_FAIL;

    _vector	vShaderFlag = { 1.f, m_fOutLineBlurIntensity, 0.f, 0.f };
    if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector))))
    {
        DEBUG_ASSERT;
    }    

    return S_OK;
}

void CInteraction_Prop::Free()
{
    __super::Free();
}
