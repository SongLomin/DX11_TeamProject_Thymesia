#include "stdafx.h"
#include "WorldUI.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Fader.h"


GAMECLASS_C(CWorldUI)
CLONE_C(CWorldUI, CGameObject)

HRESULT CWorldUI::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CWorldUI::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();

	m_pRendererCom = Add_Component<CRenderer>();
	m_pTextureCom = Add_Component<CTexture>();
	m_pVIBufferCom = Add_Component<CVIBuffer_Rect>();
	m_pFaderCom = Add_Component<CFader>();

	Set_OwnerForMyComponents();

	

	return S_OK;
}

HRESULT CWorldUI::Start()
{
	__super::Start();

	return S_OK;
}

void CWorldUI::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CWorldUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Weak_StaticCast<CGameObject>(m_this));

}

HRESULT CWorldUI::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	_matrix CamWorldMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD);
	CamWorldMatrix = SMath::Get_MatrixNormalize(CamWorldMatrix);

	m_pTransformCom.lock()->Set_WorldMatrix(CamWorldMatrix);
	m_pTransformCom.lock()->Go_Straight(5.f);

	if (nullptr == m_pShaderCom.lock().get() ||
		nullptr == m_pVIBufferCom.lock().get())
		return E_FAIL;

	/* 셰이더 전역변수에 값을 던진다. */
	if (FAILED(SetUp_ShaderResource()))
		return E_FAIL;

	m_pShaderCom.lock()->Begin(m_iPassIndex, pDeviceContext);

	m_pVIBufferCom.lock()->Render(pDeviceContext);

	return S_OK;
}

void CWorldUI::Set_Texture(const _char* sKey)
{
	m_pTextureCom.lock()->Use_Texture(sKey);

}

HRESULT CWorldUI::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");

	_matrix		WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	WorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_OffsetMatrix) * WorldMatrix);
	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", (void*)&WorldMatrix, sizeof(_float4x4));

	/*if (FAILED(m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	


	return S_OK;
}

void CWorldUI::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json.emplace("WorldUI", m_OffsetMatrix.m);

}

void CWorldUI::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	if (In_Json.find("WorldUI") == In_Json.end())
		return;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_OffsetMatrix.m[i][j] = In_Json["WorldUI"][i][j];
		}
	}
}

void CWorldUI::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("WorldUI GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			_matrix matOffset = XMLoadFloat4x4(&m_OffsetMatrix);

			//Position
			_vector vPositionVector = matOffset.r[3];

			ImGui::Text("Offset_Position");
			ImGui::DragFloat3("##Offset_Position", &vPositionVector.m128_f32[0], 1.f);
			ImGui::Separator();

			matOffset.r[3] = vPositionVector;

			_float3 vPitchYawRoll = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(matOffset));

			ImGui::Text("Offset Pitch Yaw Roll");
			ImGui::DragFloat3("##Offset Pitch Yaw Roll", &vPitchYawRoll.x, 0.01f);
			ImGui::Separator();

			_matrix matQuaternion = XMMatrixRotationRollPitchYaw(vPitchYawRoll.x, vPitchYawRoll.y, vPitchYawRoll.z);

			matOffset = SMath::Get_ScaleMatrix(matOffset) * matQuaternion * SMath::Get_PositionMatrix(matOffset);
			

			//Scale
			_vector vScaleVector = SMath::Get_Scale(matOffset);

			ImGui::Text("Offset_Scale");
			ImGui::DragFloat3("##Offset_Scale", &vScaleVector.m128_f32[0], 0.1f);
			ImGui::Separator();
			
			vScaleVector.m128_f32[0] = max(0.00001f, vScaleVector.m128_f32[0]);
			vScaleVector.m128_f32[1] = max(0.00001f, vScaleVector.m128_f32[1]);
			vScaleVector.m128_f32[2] = max(0.00001f, vScaleVector.m128_f32[2]);

			matOffset = SMath::Set_ScaleMatrix(matOffset, vScaleVector);
			XMStoreFloat4x4(&m_OffsetMatrix, matOffset);

		}
	}

}


void CWorldUI::Free()
{
	__super::Free();

}
