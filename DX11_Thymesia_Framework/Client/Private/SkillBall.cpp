#include "stdafx.h"
#include "SkillBall.h"
#include "Texture.h"
#include "Shader.h"
#include "SkillChainEffect.h"
#include "Fader.h"
#include "GameManager.h"

GAMECLASS_C(CSkillBall)
CLONE_C(CSkillBall, CGameObject)

HRESULT CSkillBall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillBall::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxTex"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	m_pTextureCom.lock()->Use_Texture(("SkillIcon0"));
	//m_pTextureCom.lock()->Use_Texture(TEXT("Default"));

	Set_UIPosition(g_iWinCX / 2.f, g_iWinCY / 2.f, 100.f, 100.f);
	m_fAnchor = g_iWinCX / 2.f;
	m_iPassIndex = 0;
	m_vMaskUV = { 0.f, 0.f };

	m_pMaskTextureCom = Add_Component<CTexture>();
	m_pMaskTextureCom.lock()->Use_Texture("UVMask");


	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Weak_Cast<CGameObject>(m_this));

	return S_OK;
}

HRESULT CSkillBall::Start()
{
	__super::Start();

	return S_OK;
}

void CSkillBall::Tick(_float fTimeDelta)
{
	if (fabs(m_tUIDesc.fX - m_fAnchor) < 0.01f)
	{
		if (!m_bDropped)
		{
			GAMEINSTANCE->Add_GameObject<CSkillChainEffect>(m_CreatedLevel)
				.lock()->Init_DropEffect(Weak_StaticCast<CUI>(m_this));

			m_bDropped = true;
		}

		m_bMove = false;
	}

	else
	{
		m_bMove = true;
	}

	m_tUIDesc.fX -= fTimeDelta * 1000.f;
	m_tUIDesc.fX = max(m_tUIDesc.fX, m_fAnchor);

	__super::Tick(fTimeDelta);

}

void CSkillBall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CSkillBall::Render()
{
	__super::Render();

	return S_OK;
}

void CSkillBall::Use_SkillBall(const _float& In_fEffectDelayTime)
{
	GAMEINSTANCE->Add_GameObject<CSkillChainEffect>(m_CreatedLevel)
		.lock()->Init_UseEffect(m_tUIDesc, In_fEffectDelayTime);
}

void CSkillBall::Init_PositionX(const _float& In_fPositionX)
{
	Set_UIPosition(In_fPositionX, 840.f, 80.f, 80.f);
}

void CSkillBall::Set_Anchor(_float fAnchorX)
{
	m_fAnchor = fAnchorX;
}

void CSkillBall::Command_Skill(_uint iSkillChainCnt, _bool& Out_bActive)
{
	if (m_bMove)
	{
		Out_bActive = false;
		return;
	}

	CallBack_Skill(iSkillChainCnt, Out_bActive);
	

}

HRESULT CSkillBall::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();
	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	_float FaderAhlpaScale = 1.f;
	//_float FaderAhlpaScale = m_pFaderCom.lock()->Get_FadeColor().w;
	m_pShaderCom.lock()->Set_RawValue("g_fAhlpaScale", &FaderAhlpaScale, sizeof(_float));

	//m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_iMaskIndex);
	//m_pShaderCom.lock()->Set_RawValue("g_MaskUV", &m_vMaskUV, sizeof(_float2));

	return S_OK;
}

void CSkillBall::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("SkillBall GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::InputInt("Mask Index", &m_iMaskIndex);

			ImGui::DragFloat2("Mask UV", &m_vMaskUV.x, 0.01f);

		}
	}

}

void CSkillBall::Free()
{
}
