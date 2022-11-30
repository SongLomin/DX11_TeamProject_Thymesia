#include "stdafx.h"
#include "UI.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transform.h"
#include "VIBuffer_Rect.h"
#include "Fader.h"
#include "GameManager.h"
#include "Player.h"
#include "Status_Player.h"

GAMECLASS_C(CUI)
CLONE_C(CUI, CGameObject)

_float CUI::Get_CamDistance()
{
	return m_tUIDesc.fDepth;
}

HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();

	m_pRendererCom = Add_Component<CRenderer>();
	m_pTextureCom = Add_Component<CTexture>();
	
	m_pVIBufferCom = Add_Component<CVIBuffer_Rect>();
	m_pFaderCom = Add_Component<CFader>();

	Set_OwnerForMyComponents();

	m_tUIDesc.fSizeX = g_iWinCX;
	m_tUIDesc.fSizeY = g_iWinCY;
	m_tUIDesc.fX = g_iWinCX >> 1;
	m_tUIDesc.fY = g_iWinCY >> 1;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));
	m_bShaking = false;
	return S_OK;
}

HRESULT CUI::Start()
{
	__super::Start();

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
	//__super::Tick(fTimeDelta);

	m_pFaderCom.lock()->Tick(fTimeDelta);

	if (m_fCurrentShakeTime > 0.f)
	{
		m_fCurrentShakeTime -= fTimeDelta;
		Update_Shaking(fTimeDelta);
	}
	else
	{
		m_fOffsetPosition = { 0.f, 0.f };
		if (m_bShaking)
		{
			CallBack_ShakingEnd();
			CallBack_ShakingEnd.Clear();
			m_bShaking = false;
		}
	}
	
}

void CUI::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));


	m_fShakedPos.x = m_tUIDesc.fX - (g_iWinCX * 0.5f) + m_fOffsetPosition.x;
	m_fShakedPos.y = -m_tUIDesc.fY + (g_iWinCY * 0.5f) - m_fOffsetPosition.y;

	m_pTransformCom.lock()->Set_Scaled(_float3(m_tUIDesc.fSizeX, m_tUIDesc.fSizeY, 0.f));
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_tUIDesc.fX - (g_iWinCX * 0.5f) + m_fOffsetPosition.x, 
		-m_tUIDesc.fY + (g_iWinCY * 0.5f) - m_fOffsetPosition.y, 0.f, 1.f));
}

HRESULT CUI::Render()
{
	//__super::Render();

	/* 셰이더 전역변수에 값을 던진다. */
	SetUp_ShaderResource();

	m_pShaderCom.lock()->Begin(m_iPassIndex);

	m_pVIBufferCom.lock()->Render();

	return S_OK;
}

void CUI::Set_Texture(const _char* sKey)
{
	m_pTextureCom.lock()->Use_Texture(sKey);

}

void CUI::Set_UIPosition(const _float fX, const _float fY, const _float fSizeX, const _float fSizeY,
	UI_ALIGN_TYPE eType)
{
	switch (eType)
	{
	case Client::CUI::ALIGN_LEFTTOP:

		m_tUIDesc.fSizeX = fSizeX;
		m_tUIDesc.fSizeY = fSizeY;
		m_tUIDesc.fX = fX + (fSizeX * 0.5f);
		m_tUIDesc.fY = fY +(fSizeY * 0.5f);
		break;
	case Client::CUI::ALIGN_CENTER:
		m_tUIDesc.fX = fX;
		m_tUIDesc.fY = fY;
		m_tUIDesc.fSizeX = fSizeX;
		m_tUIDesc.fSizeY = fSizeY;

		break;
	case Client::CUI::ALIGN_END:
		break;
	default:
		break;
	}
}

void CUI::Set_UIPosition(const _float fX, const _float fY, UI_ALIGN_TYPE eType)
{
	switch (eType)
	{
	case Client::CUI::ALIGN_LEFTTOP:

		m_tUIDesc.fX = fX + (m_tUIDesc.fSizeX * 0.5f);
		m_tUIDesc.fY = fY + (m_tUIDesc.fSizeY * 0.5f);
		break;
	case Client::CUI::ALIGN_CENTER:
		m_tUIDesc.fX = fX;
		m_tUIDesc.fY = fY;
		break;
	case Client::CUI::ALIGN_END:
		break;
	default:
		break;
	}
}

void CUI::Set_UIPosition(const _float fX, const _float fY)
{
	m_tUIDesc.fX = fX;
	m_tUIDesc.fY = fY;
}

void CUI::Set_UIDesc(UI_DESC _tUIDesc)
{
	m_tUIDesc = _tUIDesc;
}

void CUI::Set_Size(const _float& fSizeX, const _float& fSizeY)
{
	m_tUIDesc.fSizeX = fSizeX;
	m_tUIDesc.fSizeY = fSizeY;

}

void CUI::Set_Y(const _float& fY)
{
	m_tUIDesc.fY = fY;
}

void CUI::Set_Depth(_float _fDepth)
{
	m_tUIDesc.fDepth = _fDepth;
}

void CUI::Set_SizeX(const _float In_fSize)
{
	m_tUIDesc.fSizeX = In_fSize;
}

void CUI::Add_Shaking(const _float& In_ShakeTime, const _float& _fShakePower)
{
	m_fCurrentShakeTime = In_ShakeTime;
	m_fShakingTime = In_ShakeTime;
	m_fCurrentFreq = 0.f;
	m_fPower = _fShakePower;

	m_fShakedPos.x = m_tUIDesc.fX;
	m_fShakedPos.y = m_tUIDesc.fY;

	m_bShaking = true;

	CallBack_ShakingStart();
	CallBack_ShakingStart.Clear();

}

void CUI::Add_Child(weak_ptr<CUI> pChild)
{
	m_vecChildUI.push_back(pChild);

	pChild.lock()->Set_Owner(m_this);

}

void CUI::Set_Owner(weak_ptr<CBase> pOwner)
{
	m_pOwner = pOwner;
}

void CUI::Enable_AllEventChild()
{
	for (auto& elem : m_vecEventChildUI)
	{
		elem.lock()->Set_Enable(true);
	}

}

void CUI::Disable_AllEventChild()
{
	for (auto& elem : m_vecEventChildUI)
	{
		elem.lock()->Set_Enable(false);
	}
}

_float2 CUI::Get_Point(UI_POINT eType)
{
	_float2 vPos;


	_float2 fHalfSize;
	fHalfSize.x = m_tUIDesc.fSizeX * 0.5f;
	fHalfSize.y = m_tUIDesc.fSizeY * 0.5f;

	

	switch (eType)
	{
	case Client::CUI::UI_POINT::LEFT:
		vPos.x = m_tUIDesc.fX - fHalfSize.x;
		vPos.y = m_tUIDesc.fY;
		break;
	case Client::CUI::UI_POINT::RIGHT:
		vPos.x = m_tUIDesc.fX + fHalfSize.x;
		vPos.y = m_tUIDesc.fY;
		break;
	case Client::CUI::UI_POINT::TOP:
		vPos.x = m_tUIDesc.fX;
		vPos.y = m_tUIDesc.fY - fHalfSize.y;
		break;
	case Client::CUI::UI_POINT::BOTTOM:
		vPos.x = m_tUIDesc.fX;
		vPos.y = m_tUIDesc.fY + fHalfSize.y;
		break;
	case Client::CUI::UI_POINT::LEFT_TOP:
		vPos.x = m_tUIDesc.fX - fHalfSize.x;
		vPos.y = m_tUIDesc.fY - fHalfSize.y;
		break;
	case Client::CUI::UI_POINT::LEFT_BOTTOM:
		vPos.x = m_tUIDesc.fX - fHalfSize.x;
		vPos.y = m_tUIDesc.fY + fHalfSize.y;
		break;
	case Client::CUI::UI_POINT::RIGHT_TOP:
		vPos.x = m_tUIDesc.fX + fHalfSize.x;
		vPos.y = m_tUIDesc.fY - fHalfSize.y;
		break;
	case Client::CUI::UI_POINT::RIGHT_BOTTOM:
		vPos.x = m_tUIDesc.fX + fHalfSize.x;
		vPos.y = m_tUIDesc.fY + fHalfSize.y;
		break;
	case Client::CUI::UI_POINT::UI_POINT_END:
		break;
	default:
		break;
	}
	return vPos;
}

void CUI::OnEnable(void* _Arg)
{
	Set_Enable(true);
	for (auto i = 0; i < m_vecChildUI.size(); i++)
		m_vecChildUI[i].lock()->Set_Enable(true);
}

void CUI::OnDisable()
{
	__super::OnDisable();
	for (auto i = 0; i < m_vecChildUI.size(); i++)
		m_vecChildUI[i].lock()->Set_Enable(false);
}

HRESULT CUI::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");

	_matrix IdentityMatrix = XMMatrixIdentity();

	m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", &IdentityMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	return S_OK;
}

void CUI::Bind_Player()
{
#ifndef _ONLY_UI_
	m_pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	m_pPlayerStatus = m_pPlayer.lock()->Get_ComponentByType<CStatus_Player>();
#endif // !_ONLY_UI

}

void CUI::Update_Shaking(_float fTimeDelta)
{
	if (m_fCurrentFreq > 0.f)
	{
		m_fCurrentFreq -= fTimeDelta;

		return;
	}

	m_fPower -= (fTimeDelta * m_fPower) * 30.f;

	m_fOffsetPosition.x = SMath::fRandom(-m_fPower, m_fPower);
	m_fOffsetPosition.y = SMath::fRandom(-m_fPower, m_fPower);

	m_fCurrentFreq = m_fShakeFreq;

}



void CUI::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);

	Out_Json.emplace("fX", m_tUIDesc.fX);
	Out_Json.emplace("fY", m_tUIDesc.fY);
	Out_Json.emplace("SizeX", m_tUIDesc.fSizeX);
	Out_Json.emplace("sizeY", m_tUIDesc.fSizeY);

}

void CUI::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

	if(In_Json.find("fX") != In_Json.end())
		m_tUIDesc.fX = In_Json["fX"];

	if (In_Json.find("fY") != In_Json.end())
		m_tUIDesc.fY = In_Json["fY"];

	if (In_Json.find("SizeX") != In_Json.end())
		m_tUIDesc.fSizeX = In_Json["SizeX"];

	if (In_Json.find("sizeY") != In_Json.end())
		m_tUIDesc.fSizeY = In_Json["sizeY"];

}

void CUI::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("UI GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::DragFloat("fX", &m_tUIDesc.fX, 1.f);
			ImGui::DragFloat("fY", &m_tUIDesc.fY, 1.f);
			ImGui::DragFloat("SizeX", &m_tUIDesc.fSizeX, 1.f, 1.f);
			ImGui::DragFloat("sizeY", &m_tUIDesc.fSizeY, 1.f, 1.f);

			m_tUIDesc.fSizeX = max(1.f, m_tUIDesc.fSizeX);
			m_tUIDesc.fSizeY = max(1.f, m_tUIDesc.fSizeY);

		}
	}

}



void CUI::Free()
{
	__super::Free();
	m_vecChildUI.clear();

}
