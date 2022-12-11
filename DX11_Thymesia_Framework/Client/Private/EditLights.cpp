#include "stdafx.h"

#include "EditLights.h"
#include "SMath.h"

#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_DynamicCube.h"
#include "Shader.h"
#include "Renderer.h"
#include "Model.h"
#include "Collider.h"

#include "Client_GameObjects.h"

#include "Window_HierarchyView.h"
#include "ImGui_Window.h"

GAMECLASS_C(CEditLights)
CLONE_C(CEditLights, CGameObject)

HRESULT CEditLights::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEditLights::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pShaderCom.lock()->Set_ShaderInfo
	(
		TEXT("Shader_VtxCubeTex"),
		VTXCUBETEX_DECLARATION::Element,
		VTXCUBETEX_DECLARATION::iNumElements
	);

	m_pVIBufferCom = Add_Component<CVIBuffer_DynamicCube>();
	m_ModelList    = GET_SINGLE(CGameInstance)->Get_AllNoneAnimModelKeys();

	XMStoreFloat4x4(&m_PickingMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CEditLights::Start()
{
	return S_OK;
}

void CEditLights::Tick(_float fTimeDelta)
{
	//__super::Tick(fTimeDelta);
}

void CEditLights::LateTick(_float fTimeDelta)
{
	//m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Cast<CGameObject>(m_this));
}

HRESULT CEditLights::Render()
{
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEditLights::SetUp_ShaderResource()
{
	
	return S_OK;
}

void CEditLights::Write_Json(json& Out_Json)
{
}

_bool CEditLights::IsPicking(const RAY& In_Ray, _float& Out_fRange)
{
	auto iter_prop = GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.find(typeid(CEditLights).hash_code());

	if (iter_prop == GET_SINGLE(CWindow_HierarchyView)->m_pObjGroup.end())
		return false;

	_float fPickedDist;
	_bool bPicked = false;
	_uint iIndex  = 0;

	for (auto& iter : iter_prop->second)
	{
		weak_ptr<CModel>		pModelCom     = iter.pInstance.lock()->Get_Component<CModel>();
		weak_ptr<CTransform>	pTransformCom = iter.pInstance.lock()->Get_Component<CTransform>();

		if (!pModelCom.lock().get() || !pTransformCom.lock().get())
		{
			++iIndex;
			continue;
		}

		MESH_VTX_INFO Info = pModelCom.lock()->Get_ModelData().lock()->VertexInfo;

		if (SMath::Is_Picked_AbstractCube(In_Ray, Info, pTransformCom.lock()->Get_WorldMatrix(), &fPickedDist))
		{
			if (Out_fRange > fPickedDist)
			{
				Out_fRange      = fPickedDist;
				m_iPickingIndex = iIndex;
				bPicked = true;
			}
		}

		++iIndex;
	}

	return bPicked;
}

void CEditLights::OnEventMessage(_uint iArg)
{
	switch (iArg)
	{
		case (_uint)EVENT_TYPE::ON_EDITDRAW_ACCEPT:
		{
			m_bSubDraw = true;
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW_NONE:
		{
			m_bSubDraw = false;
		}
		break;

		case (_uint)EVENT_TYPE::ON_EDITDRAW:
		{
			if (!m_bSubDraw)
				return;

			if (ImGui::BeginTabBar("Edit"))
			{
				if (ImGui::BeginTabItem("Light"))
				{
					View_AddLight();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Frog"))
				{
					View_SettingFrog();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		break;
	}
}

void CEditLights::View_AddLight()
{
	_bool bChange = false;

	bChange |= ImGui::InputFloat4("Fog Color  : ", &m_vFogColor.x);
	bChange |= ImGui::InputFloat ("Fog Range  : ", &m_fFogRange);

	if (bChange)
		GAMEINSTANCE->Set_FogDesc(m_vFogColor, m_fFogRange);
}

void CEditLights::View_SettingFrog()
{

}

void CEditLights::Free()
{
}