#include "stdafx.h"
#include "PreView_Prop.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"

GAMECLASS_C(CPreview_Prop);
CLONE_C(CPreview_Prop, CGameObject);

HRESULT CPreview_Prop::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPreview_Prop::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    return S_OK;
}

HRESULT CPreview_Prop::Start()
{
    __super::Start();
#ifdef _DEBUG
	m_fCullingOffsetRange = FLT_MAX;
#endif // _DEBUG
    return S_OK;
}

void CPreview_Prop::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

#ifdef _DEBUG
	/*if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		++m_iContainerIndex;
		cout << "m_iContainerIndex : " << m_iContainerIndex << endl;
	}*/
#endif // _DEBUG
}

void CPreview_Prop::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();
	m_pModelCom.lock()->Update_NvCloth(pDeferredContext, m_pTransformCom.lock()->Get_WorldMatrix());
	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);
}

HRESULT CPreview_Prop::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

	SetUp_ShaderResource();

	m_iPassIndex = 3;
	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();

#ifdef _DEBUG
	if (m_iContainerIndex >= iNumMeshContainers)
		m_iContainerIndex = 0;
#endif //_DEBUG
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
#ifdef _DEBUG
		/*if (i == m_iContainerIndex)
			continue;*/
#endif

		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			m_iPassIndex = 0;
		}
		else
		{
			if (m_bInvisibility && LEVEL::LEVEL_EDIT != Get_CreatedLevel())
			{
				if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
					m_iPassIndex = 6;
				else
					m_iPassIndex = 7;
			}
			else
			{
				m_iPassIndex = 3;
			}
		}

		m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);
		m_pModelCom.lock()->Render_Mesh(i, pDeviceContext);
	}

	return S_OK;
}

void CPreview_Prop::Free()
{
}
