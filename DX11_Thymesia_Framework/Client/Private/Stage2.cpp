#include "stdafx.h"
#include "Stage2.h"
#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"

GAMECLASS_C(CStage2);
CLONE_C(CStage2, CGameObject);

HRESULT CStage2::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CStage2::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxModel"),
		VTXMODEL_DECLARATION::Element,
		VTXMODEL_DECLARATION::iNumElements);
	m_pModelCom.lock()->Init_Model("Stage2");

	CNavigation::NAVIDESC tNaviDesc;
	tNaviDesc.m_iCurrentIndex = -1;

	m_pNaviMeshCom.lock()->Init_Navi("Stage2_Navi", tNaviDesc);
	m_pTransformCom.lock()->Add_Position(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	m_iPassIndex = 0;

	return S_OK;
}

HRESULT CStage2::Start()
{
	__super::Start();

	return S_OK;
}

void CStage2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CStage2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	//GAMEINSTANCE->Add_DebugRenderGroup(m_pNaviMeshCom);
	//m_pNaviMeshCom.lock()->Render();
#endif // _DEBUG
}

HRESULT CStage2::Render()
{
	__super::Render();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{

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

	return S_OK;
}


void CStage2::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
}

void CStage2::Free()
{
}
