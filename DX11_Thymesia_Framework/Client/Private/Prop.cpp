#include "stdafx.h"
#include "Prop.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameManager.h"
#include "Texture.h"

GAMECLASS_C(CProp);
CLONE_C(CProp, CGameObject);

HRESULT CProp::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CProp::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pModelCom     = Add_Component<CModel>();
    m_pShaderCom    = Add_Component<CShader>();
    m_pRendererCom  = Add_Component<CRenderer>();
	m_pMaskingTextureCom = Add_Component<CTexture>();
	m_pMaskingTextureCom.lock()->Use_Texture("UVMask");

#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::CUSTOM_THREAD1);
#endif

    return S_OK;
}

HRESULT CProp::Start()
{
    return S_OK;
}

void CProp::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CProp::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

#ifdef _USE_THREAD_
	if (m_bRendering)
	{
		m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
	}
#else
	/*if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), 0.f))
	{
		if (RENDERGROUP::RENDER_END != m_eRenderGroup)
			m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));
	}*/

#endif // !_USE_THREAD_

}

void CProp::Custom_Thread1(_float fTimeDelta)
{
	__super::Custom_Thread1(fTimeDelta);

	/*if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(m_pTransformCom.lock()->Get_Position(), m_fCullingOffsetRange))
	{
		m_bRendering = true;
	}
	else
	{
		m_bRendering = false;
	}*/
}

HRESULT CProp::Render()
{
    SetUp_ShaderResource();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			return E_FAIL;
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
			m_iPassIndex = 4;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex);
		m_pModelCom.lock()->Render_Mesh(i);
	}

    __super::Render();

    return S_OK;
}

void CProp::SetUp_ShaderResource()
{
    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));


	_float4 vCamDesc;
	XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &vCamDesc, sizeof(_float4));

	XMStoreFloat4(&vCamDesc, GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamLook", &vCamDesc, sizeof(_float4));
	
	_float4 vPlayerPos;
	XMStoreFloat4(&vPlayerPos,GET_SINGLE(CGameManager)->Get_PlayerPos());
	m_pShaderCom.lock()->Set_RawValue("g_vPlayerPosition", &vPlayerPos, sizeof(_float4));

	m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 92);

	_vector vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));
}

void CProp::Free()
{
	__super::Free();
}
