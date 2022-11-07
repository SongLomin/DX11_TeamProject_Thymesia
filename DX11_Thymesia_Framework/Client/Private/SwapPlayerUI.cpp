#include "stdafx.h"
#include "SwapPlayerUI.h"
#include "Texture.h"
#include "Shader.h"
#include "Status.h"
#include "VIBuffer_Rect.h"
#include "GameManager.h"
#include "Player.h"

GAMECLASS_C(CSwapPlayerUI)
CLONE_C(CSwapPlayerUI, CGameObject)

HRESULT CSwapPlayerUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSwapPlayerUI::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	Set_UIPosition(g_iWinCX / 2.f, 50.f, 100, 100.f);
	m_vOffsetPosition = { 2.7f, 2.5f };

	// 프레임
	m_pBackTextureCom = Add_Component<CTexture>();
	m_pBackVIBufferCom = Add_Component<CVIBuffer_Rect>();

	m_pBackTextureCom.lock()->Use_Texture("SwapFrame");

	
	// 링
	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);
	m_pTextureCom.lock()->Use_Texture(("SwapRing"));
	m_iPassIndex = 0;

	// 아이콘
	m_pIconTextureCom = Add_Component<CTexture>();
	m_pIconVIBufferCom = Add_Component<CVIBuffer_Rect>();

	m_pIconTextureCom.lock()->Use_Texture("SwapIcon");

	// SP
	m_pBarShaderCom = Add_Component<CShader>();
	m_pSPTextureCom = Add_Component<CTexture>();
	m_pSPVIBufferCom = Add_Component<CVIBuffer_Rect>();

	m_pBarShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_HPBar"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);
	m_pSPTextureCom.lock()->Use_Texture("SwapSP");

	// HP
	m_pHPTextureCom = Add_Component<CTexture>();
	m_pHPVIBufferCom = Add_Component<CVIBuffer_Rect>();

	m_pHPTextureCom.lock()->Use_Texture("SwapHP");

	
	GET_SINGLE(CGameManager)->Register_SwapPlayerUI(Weak_StaticCast<CSwapPlayerUI>(m_this));

	//5

	//GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Weak_StaticCast<CGameObject>(m_this));

	//GET_SINGLE(CGameManager)->CallBack_ChangePlayer += bind(&CSwapPlayerUI::Call_ChangePlayer, this);

	Set_Enable(false);
	
	return S_OK;
}

HRESULT CSwapPlayerUI::Start()
{
	__super::Start();

	return S_OK;
}

void CSwapPlayerUI::Tick(_float fTimeDelta)
{
	if (!m_pCurrentPlayerStatusCom.lock())
		return;

	__super::Tick(fTimeDelta);

}

void CSwapPlayerUI::LateTick(_float fTimeDelta)
{
	if (!m_pCurrentPlayerStatusCom.lock())
		return;

	__super::LateTick(fTimeDelta);

	m_pCurrentPlayerStatusCom.lock()->Add_SwapTime(-fTimeDelta);
	
	CStatus::STATUS_DESC StatusDesc = m_pCurrentPlayerStatusCom.lock()->Get_Desc();

	m_fSwapTimeRatio = StatusDesc.fSwapTime / StatusDesc.fMaxSwapTime;
	m_fHPViewRatio = StatusDesc.fCurrentHP / StatusDesc.fMaxHP;
	m_fSPViewRatio = StatusDesc.fSPRatio;
	
	//m_pTransformCom.lock()->Set_Scaled(_float3(m_tUIDesc.fSizeX, m_tUIDesc.fSizeY, 0.f));
	_matrix IconWorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();

	IconWorldMatrix = SMath::Set_ScaleMatrix(IconWorldMatrix, XMVectorSet(m_tUIDesc.fSizeX * m_fScale, m_tUIDesc.fSizeY * m_fScale, 0.f, 0.f));
	IconWorldMatrix.r[3].m128_f32[0] += m_vOffsetPosition.x;
	IconWorldMatrix.r[3].m128_f32[1] += m_vOffsetPosition.y;
	
	XMStoreFloat4x4(&m_IconWorldMatrix, IconWorldMatrix);
}

HRESULT CSwapPlayerUI::Render()
{
	_matrix IdentityMatrix = XMMatrixIdentity();

	//프레임
	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", &IdentityMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pBackTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0);

	_float FaderAhlpaScale = 1.f;
	m_pShaderCom.lock()->Set_RawValue("g_fAhlpaScale", &FaderAhlpaScale, sizeof(_float));
	m_pShaderCom.lock()->Begin(0);

	m_pBackVIBufferCom.lock()->Render();

	//// 링
	//__super::Render();

	// SP
	m_pTransformCom.lock()->Set_ShaderResource(m_pBarShaderCom, "g_WorldMatrix");
	m_pBarShaderCom.lock()->Set_RawValue("g_ViewMatrix", &IdentityMatrix, sizeof(_float4x4));
	m_pBarShaderCom.lock()->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	//_float FaderAhlpaScale = 1.f;
	FaderAhlpaScale = 1.f;
	m_pBarShaderCom.lock()->Set_RawValue("g_AhlpaScale", &FaderAhlpaScale, sizeof(_float));

	m_pSPTextureCom.lock()->Set_ShaderResourceView(m_pBarShaderCom, "g_DiffuseTexture", 0);
	m_pBarShaderCom.lock()->Set_RawValue("g_Ratio", &m_fSPViewRatio, sizeof(_float));
	m_pBarShaderCom.lock()->Begin(6);
	m_pSPVIBufferCom.lock()->Render();

	// HP
	m_pHPTextureCom.lock()->Set_ShaderResourceView(m_pBarShaderCom, "g_DiffuseTexture", 0);
	m_pBarShaderCom.lock()->Set_RawValue("g_Ratio", &m_fHPViewRatio, sizeof(_float));
	m_pBarShaderCom.lock()->Begin(7);
	m_pHPVIBufferCom.lock()->Render();

	// 플레이어 아이콘
	_matrix IconWorldMatrix = XMLoadFloat4x4(&m_IconWorldMatrix);
	IconWorldMatrix = XMMatrixTranspose(IconWorldMatrix);

	m_pBarShaderCom.lock()->Set_RawValue("g_WorldMatrix", &IconWorldMatrix, sizeof(_float4x4));
	m_pBarShaderCom.lock()->Set_RawValue("g_Ratio", &m_fSwapTimeRatio, sizeof(_float));
	m_pIconTextureCom.lock()->Set_ShaderResourceView(m_pBarShaderCom, "g_DiffuseTexture", m_iTextureIndex);
	m_pBarShaderCom.lock()->Begin(8);

	m_pIconVIBufferCom.lock()->Render();

	



	return S_OK;
}

void CSwapPlayerUI::Init_SwapPlayerUI(weak_ptr<CPlayer> pTargetPlayer, const _uint& In_iIndex)
{
	m_pTargetPlayer = pTargetPlayer;
	m_pCurrentPlayerStatusCom = pTargetPlayer.lock()->Get_Component<CStatus>();
	m_iTextureIndex = In_iIndex;
}

void CSwapPlayerUI::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Swap Player UI GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::DragFloat("Scale", &m_fScale, 0.1f);
			ImGui::DragFloat2("OffsetPosition", &m_vOffsetPosition.x, 0.1f);
			m_fScale = max(0.01f, m_fScale);
		}
	}

}

void CSwapPlayerUI::OnEnable(void* pArg)
{
	/*if (m_pCurrentPlayerStatusCom.lock())
	{
		m_pCurrentPlayerStatusCom.lock()->On_Swap();
	}*/

}


HRESULT CSwapPlayerUI::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
	{
		DEBUG_ASSERT;
	}

	//m.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 2);

	//m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fViewRatio, sizeof(_float));

	return S_OK;
}

void CSwapPlayerUI::OnDestroy()
{
	//GET_SINGLE(CGameManager)->CallBack_ChangePlayer -= bind(&CSwapPlayerUI::Call_ChangePlayer, this);
}

void CSwapPlayerUI::Free()
{
}
