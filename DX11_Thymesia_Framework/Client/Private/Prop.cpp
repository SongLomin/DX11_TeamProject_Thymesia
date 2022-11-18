#include "stdafx.h"
#include "Prop.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

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

    if(RENDERGROUP::RENDER_END != m_eRenderGroup)
        m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));
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
			m_iPassIndex = 3;
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

	_vector vLightFlag = { 1.f, 1.f, 1.f, 1.f };
	m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));
}

void CProp::Free()
{
	__super::Free();
}
