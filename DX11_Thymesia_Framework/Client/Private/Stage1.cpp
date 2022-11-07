#include "stdafx.h"
#include "Stage1.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"

GAMECLASS_C(CStage1);
CLONE_C(CStage1, CGameObject);

HRESULT CStage1::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CStage1::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);
	m_pModelCom.lock()->Init_Model("Stage1");
	//m_pModelCom.lock()->Init_Model("Stage1", "../Bin/Resources/Textures/MyTexture2D/");

	CNavigation::NAVIDESC tNaviDesc;
	tNaviDesc.m_iCurrentIndex = -1;

	m_pNaviMeshCom.lock()->Init_Navi("Stage1_Navi", tNaviDesc);
	m_pTransformCom.lock()->Add_Position(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	m_iPassIndex = 3;

	return S_OK;
}

HRESULT CStage1::Start()
{
	__super::Start();

	return S_OK;
}

void CStage1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CStage1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	//GAMEINSTANCE->Add_DebugRenderGroup(m_pNaviMeshCom);
	//m_pNaviMeshCom.lock()->Render();
#endif // _DEBUG
}

HRESULT CStage1::Render()
{
	__super::Render();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			//cout << i << ". None Texture" << endl;
		}

		// 노말인데 5에 저장되어 있다..
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, (aiTextureType)5)))
		{
			// 노말 텍스쳐가 없는 경우
			m_iPassIndex = 0;
		}
		// 노말 텍스쳐가 있는 경우
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


void CStage1::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CStage1::Free()
{
}
