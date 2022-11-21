#include "stdafx.h"
#include "Stage3.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"

GAMECLASS_C(CStage3);
CLONE_C(CStage3, CGameObject);

HRESULT CStage3::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CStage3::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);
	m_pModelCom.lock()->Init_Model("Stage3");

	CNavigation::NAVIDESC tNaviDesc;
	tNaviDesc.m_iCurrentIndex = -1;

	//m_pNaviMeshCom.lock()->Init_Navi("Stage3_Navi", tNaviDesc);
	m_pTransformCom.lock()->Add_Position(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	m_iPassIndex = 3;

	return S_OK;
}

HRESULT CStage3::Start()
{
	__super::Start();

	return S_OK;
}

void CStage3::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CStage3::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	//GAMEINSTANCE->Add_DebugRenderGroup(m_pNaviMeshCom);
	//m_pNaviMeshCom.lock()->Render();
#endif // _DEBUG
}

HRESULT CStage3::Render()
{
	__super::Render();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
		}

		// �븻�ε� 5�� ����Ǿ� �ִ�..
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
		{
			// �븻 �ؽ��İ� ���� ���
			m_iPassIndex = 0;
		}
		// �븻 �ؽ��İ� �ִ� ���
		else
		{
			m_iPassIndex = 3;
		}
			//return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;*/

		m_pShaderCom.lock()->Begin(m_iPassIndex);

		m_pModelCom.lock()->Render_Mesh(i);
	}

	return S_OK;
}


void CStage3::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CStage3::Free()
{
}
