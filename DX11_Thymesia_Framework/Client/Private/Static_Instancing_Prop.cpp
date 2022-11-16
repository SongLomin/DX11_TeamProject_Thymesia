#include "stdafx.h"
#include "Static_Instancing_Prop.h"
#include "Model.h"
#include "VIBuffer_Model_Instance.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

GAMECLASS_C(CStatic_Instancing_Prop);
CLONE_C(CStatic_Instancing_Prop, CGameObject);

HRESULT CStatic_Instancing_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModelInstance"),
        VTXMODEL_INSTANCE_DECLARATION::Element,
        VTXMODEL_INSTANCE_DECLARATION::iNumElements
    );

	GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));


    m_pInstancingModel = Add_Component<CVIBuffer_Model_Instance>();
    m_pInstancingModel.lock()->Init_NoAnimInstance("Torch", 10);

    return S_OK;
}

HRESULT CStatic_Instancing_Prop::Start()
{
    __super::Start();

    return S_OK;
}

void CStatic_Instancing_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CStatic_Instancing_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CStatic_Instancing_Prop::Render()
{
	SetUp_ShaderResource();

	_uint iNumMeshContainers = m_pInstancingModel.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pInstancingModel.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			DEBUG_ASSERT;
			//cout << i << ". None Texture" << endl;
		}

		// 노말인데 5에 저장되어 있다..
		if (FAILED(m_pInstancingModel.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			// 노말 텍스쳐가 없는 경우
			m_iPassIndex = 0;
		}
		// 노말 텍스쳐가 있는 경우
		else
		{
			m_iPassIndex = 1;
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex);

		m_pInstancingModel.lock()->Render_Mesh(i);
	}


    return __super::Render();
}

HRESULT CStatic_Instancing_Prop::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

	_uint iNumMeshContainers = m_pInstancingModel.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		

		m_pShaderCom.lock()->Begin(2);

		m_pInstancingModel.lock()->Render_Mesh(i);
	}

	return S_OK;
}

void CStatic_Instancing_Prop::Free()
{
}
