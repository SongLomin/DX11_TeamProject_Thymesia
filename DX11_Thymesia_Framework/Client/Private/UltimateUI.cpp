#include "stdafx.h"
#include "UltimateUI.h"
#include "Texture.h"
#include "Shader.h"
#include "Status.h"
#include "VIBuffer_Rect.h"
#include "GameManager.h"
#include "Player.h"
#include "Fader.h"

GAMECLASS_C(CUltimateUI)
CLONE_C(CUltimateUI, CGameObject)

HRESULT CUltimateUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUltimateUI::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	m_pTextureCom.lock()->Use_Texture(("LuxiyaSkillIcon"));
	m_iTextureIndex = 3;
	//m_pTextureCom.lock()->Use_Texture(TEXT("Default"));

	Set_UIPosition(1310.f, 820.f, 140, 140.f);

	m_iPassIndex = 0;

	m_pBackShaderCom = Add_Component<CShader>();
	m_pBackTextureCom = Add_Component<CTexture>();
	m_pBackVIBufferCom = Add_Component<CVIBuffer_Rect>();

	m_pBarShaderCom = Add_Component<CShader>();
	m_pBarTextureCom = Add_Component<CTexture>();
	m_pBarVIBufferCom = Add_Component<CVIBuffer_Rect>();

	m_pBackShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);
	m_pBackTextureCom.lock()->Use_Texture("UIButtonBackFace");

	m_pBarShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_HPBar"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);
	m_pBarTextureCom.lock()->Use_Texture("DodgeBar");
	//5

	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Weak_StaticCast<CGameObject>(m_this));

	GET_SINGLE(CGameManager)->CallBack_ChangePlayer += bind(&CUltimateUI::Call_ChangePlayer, this);

	return S_OK;
}

HRESULT CUltimateUI::Start()
{
	__super::Start();

	return S_OK;
}

void CUltimateUI::Tick(_float fTimeDelta)
{
	if (!m_pCurrentPlayerStatusCom.lock())
		return;

	__super::Tick(fTimeDelta);

	m_fViewRatio = 1.f;
	//m_fViewRatio = m_pCurrentPlayerStatusCom.lock()->Get_Desc().fDashRatio;

}

void CUltimateUI::LateTick(_float fTimeDelta)
{

	if (!m_pCurrentPlayerStatusCom.lock())
		return;

	__super::LateTick(fTimeDelta);


}

HRESULT CUltimateUI::Render()
{
	_matrix IdentityMatrix = XMMatrixIdentity();

	//후면 이미지 처리
	m_pTransformCom.lock()->Set_ShaderResource(m_pBackShaderCom, "g_WorldMatrix");
	m_pBackShaderCom.lock()->Set_RawValue("g_ViewMatrix", &IdentityMatrix, sizeof(_float4x4));
	m_pBackShaderCom.lock()->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pBackTextureCom.lock()->Set_ShaderResourceView(m_pBackShaderCom, "g_DiffuseTexture", 0);

	_float FaderAhlpaScale = 0.2f;
	m_pBackShaderCom.lock()->Set_RawValue("g_fAhlpaScale", &FaderAhlpaScale, sizeof(_float));

	m_pBackShaderCom.lock()->Begin(0);

	m_pBackVIBufferCom.lock()->Render();

	// 아이콘 이미지 처리
	__super::Render();

	// 원형 바 이미지 처리
	m_pTransformCom.lock()->Set_ShaderResource(m_pBarShaderCom, "g_WorldMatrix");
	m_pBarShaderCom.lock()->Set_RawValue("g_ViewMatrix", &IdentityMatrix, sizeof(_float4x4));
	m_pBarShaderCom.lock()->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	m_pBarTextureCom.lock()->Set_ShaderResourceView(m_pBarShaderCom, "g_DiffuseTexture", 0);

	FaderAhlpaScale = 1.f;
	m_pBarShaderCom.lock()->Set_RawValue("g_AhlpaScale", &FaderAhlpaScale, sizeof(_float));
	m_pBarShaderCom.lock()->Set_RawValue("g_Ratio", &m_fViewRatio, sizeof(_float));

	m_pBarShaderCom.lock()->Begin(5);

	m_pBarVIBufferCom.lock()->Render();


	return S_OK;
}

void CUltimateUI::Call_ChangePlayer()
{
	m_pCurrentPlayerStatusCom = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CStatus>();
	m_fViewRatio = m_pCurrentPlayerStatusCom.lock()->Get_Desc().fDashRatio;
}


HRESULT CUltimateUI::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
	{
		DEBUG_ASSERT;
	}

	//_float FaderAhlpaScale = 1.f;
	_float FaderAhlpaScale = 1.f;
	m_pShaderCom.lock()->Set_RawValue("g_fAhlpaScale", &FaderAhlpaScale, sizeof(_float));

	//m.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 2);

	//m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fViewRatio, sizeof(_float));

	return S_OK;
}

void CUltimateUI::OnDestroy()
{
	GET_SINGLE(CGameManager)->CallBack_ChangePlayer -= bind(&CUltimateUI::Call_ChangePlayer, this);
}

void CUltimateUI::Free()
{
}
