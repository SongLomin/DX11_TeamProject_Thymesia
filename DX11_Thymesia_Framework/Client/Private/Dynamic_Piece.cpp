#include "stdafx.h"
#include "Dynamic_Piece.h"
#include "Easing_Utillity.h"
#include "Client_Components.h"
#include "Client_Presets.h"

GAMECLASS_C(CDynamic_Piece);
CLONE_C(CDynamic_Piece, CGameObject);

HRESULT CDynamic_Piece::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CDynamic_Piece::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (nullptr == pArg)
        return E_FAIL;

    const char* szModelKey = (const char*)pArg;

    m_pModelCom.lock()->Init_Model(szModelKey);
    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

    m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);

#ifdef _USE_THREAD_
    Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
#endif // _USE_THREAD_


    return S_OK;
}

HRESULT CDynamic_Piece::Start()
{   

    PhysXColliderDesc tPhysXColliderDesc;
    Preset::PhysXColliderDesc::DynamicPieceSetting(tPhysXColliderDesc, m_pTransformCom);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tPhysXColliderDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtScene();
    Set_Enable(false);

    __super::Start();

    return S_OK;
}

void CDynamic_Piece::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

#ifdef _LIFEGUARD_FOR_FALL_
    _vector vPos = m_pTransformCom.lock()->Get_Position();
    if (vPos.m128_f32[1] < -50.f)
    {
        vPos.m128_f32[1] = 50.f;
        m_pTransformCom.lock()->Set_Position(vPos);
    }
#endif

    m_fCurrentLifeTime += fTimeDelta;

    if (m_fCurrentLifeTime >= m_fMaxLifeTime)
    {
        m_fCurrentLifeTime = min(m_fMaxLifeTime, m_fCurrentLifeTime);
        Set_Dead();
    }

    
}

void CDynamic_Piece::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CDynamic_Piece::Thread_PreBeforeRender(_float fTimeDelta)
{
    __super::Thread_PreBeforeRender(fTimeDelta);
    m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom);
}

void CDynamic_Piece::Before_Render(_float fTimeDelta)
{
    __super::Before_Render(fTimeDelta);
} 

HRESULT CDynamic_Piece::Render(ID3D11DeviceContext* pDeviceContext)
{
    SetUp_ShaderResource();

    __super::Render(pDeviceContext);

    _vector vStart = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    _vector vEnd = XMVectorSet(0.f, 0.f, 0.f, 0.f);

    _float fMaskRatio = XMVectorGetX(CEasing_Utillity::QuartIn(vStart, vEnd, m_fCurrentLifeTime, m_fMaxLifeTime));

    m_pShaderCom.lock()->Set_RawValue("g_fMaskingScalar", &fMaskRatio, sizeof(_float));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
        {
            // Do Nothing.
        }

        // 노말인데 5에 저장되어 있다..

        if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        {
            // 노말 텍스쳐가 없는 경우
            m_iPassIndex = 0;
        }
        // 노말 텍스쳐가 있는 경우
        else
        {
            if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
                m_iPassIndex = 8;
            else
                m_iPassIndex = 9;
        }

        m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
        m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
    }

    return S_OK;
}

void CDynamic_Piece::Set_WorldMatrixWithPhysX(_fmatrix In_WorldMatrix)
{
    m_pTransformCom.lock()->Set_WorldMatrix(In_WorldMatrix);
    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

void CDynamic_Piece::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    m_pPhysXColliderCom.lock()->Set_Enable(true);
    m_pPhysXColliderCom.lock()->WakeUp();
}

void CDynamic_Piece::OnDisable()
{
    __super::OnDisable();

    m_pPhysXColliderCom.lock()->Set_Enable(false);
}

void CDynamic_Piece::Free()
{
}
