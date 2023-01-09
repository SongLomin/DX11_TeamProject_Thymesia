#include "stdafx.h"
#include "Actor.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "RigidBody.h"
#include "GameManager.h"
#include "RequirementChecker.h"

GAMECLASS_C(CActor)
CLONE_C(CActor, CGameObject)

HRESULT CActor::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CActor::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pModelCom = Add_Component<CModel>();
    m_pShaderCom = Add_Component<CShader>();
    m_pRendererCom = Add_Component<CRenderer>();


#ifdef _USE_THREAD_
    Use_Thread(THREAD_TYPE::PRE_LATETICK);
    Use_Thread(THREAD_TYPE::PRE_RENDER);
#endif // _USE_THREAD_


    return S_OK;
}

void CActor::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


}

void CActor::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

#ifdef _USE_THREAD_
    if (m_bRendering)
    {
        m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
    }

#else
    if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), 0.f))
    {
        m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
        m_pModelCom.lock()->Update_BoneMatrices();
    }

#endif // !_USE_THREAD_

    //m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));
}

void CActor::Before_Render(_float fTimeDelta)
{
    //m_pModelCom.lock()->Update_BoneMatrices();

}

void CActor::Thread_PreRender(_float fTimeDelta)
{
    if (!Get_Enable())
        return;

    if (m_bRendering)
        m_pModelCom.lock()->Update_BoneMatrices();
}

void CActor::Thread_PreLateTick(_float fTimeDelta)
{
    __super::Thread_PreLateTick(fTimeDelta);

#ifdef _Actor_Culling_
    if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), m_fCullingRange))
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

HRESULT CActor::Render(ID3D11DeviceContext* pDeviceContext)
{
    this->SetUp_ShaderResource();
    __super::Render(pDeviceContext);
    return S_OK;
}

_vector CActor::Get_WorldPosition()
{
    return m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);
}

void CActor::Bind_KeyEvent(const char* szKeyEventName)
{
    m_strKeyEventName = szKeyEventName;
	GET_SINGLE(CGameManager)->Bind_KeyEvent(szKeyEventName, m_pModelCom, bind(&CActor::Call_NextAnimationKey, this, placeholders::_1));
}

void CActor::Unbind_KeyEvent(const char* szKeyEventName)
{
	GET_SINGLE(CGameManager)->Unbind_KeyEvent(szKeyEventName, m_pModelCom, bind(&CActor::Call_NextAnimationKey, this, placeholders::_1));
    m_strKeyEventName.clear();
}

void CActor::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
    // Unbind_KeyEvent가 제대로 작동 하지 않을 시
    if (m_strKeyEventName.empty())
        DEBUG_ASSERT;

    GET_SINGLE(CGameManager)->Active_KeyEvent(m_strKeyEventName, m_pModelCom, m_pTransformCom, In_iKeyIndex);
}

weak_ptr<CRequirementChecker> CActor::Get_Requirement(const string& In_szCheckerKey)
{
    return m_pRequirementChecker[hash<string>()(In_szCheckerKey)];
}


void CActor::SetUp_ShaderResource()
{
    //CallBack_Bind_SRV(m_pShaderCom, "");
    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

    _float fCamFar = GAMEINSTANCE->Get_CameraFar();
    m_pShaderCom.lock()->Set_RawValue("g_fFar", &fCamFar, sizeof(_float));
}

void CActor::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    for (auto& elem : m_EffectIndexList)
    {
        elem.second = GET_SINGLE(CGameManager)->Use_EffectGroup(elem.first, m_pTransformCom);
    }

}

void CActor::OnDisable()
{
    __super::OnDisable();

    for (auto& elem : m_EffectIndexList)
    {
        GET_SINGLE(CGameManager)->UnUse_EffectGroup(elem.first, elem.second);
    }

    m_EffectIndexList.clear();
}

void CActor::OnDestroy()
{
    __super::OnDestroy();

    for (auto& elem : m_EffectIndexList)
    {
        GET_SINGLE(CGameManager)->UnUse_EffectGroup(elem.first, elem.second);
    }

    m_EffectIndexList.clear();
}

void CActor::Free()
{
}
