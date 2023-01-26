#include "stdafx.h"
#include "FadeMask.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Fader.h"

GAMECLASS_C(CFadeMask)
CLONE_C(CFadeMask, CGameObject);

HRESULT CFadeMask::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFadeMask::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_VtxColor"),
		VTXPOS_DECLARATION::Element,
		VTXPOS_DECLARATION::iNumElements);

	m_eRenderGroup = RENDERGROUP::RENDER_FINAL;

	m_vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);
	m_pFaderCom.lock()->CallBack_FadeEnd += bind(&CFadeMask::Call_FadeEnd, this, placeholders::_1);

	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::FADEMASK, Cast<CGameObject>(m_this));

	m_tUIDesc.fDepth = 0.f;

	Set_Enable(false);

	return S_OK;
}

HRESULT CFadeMask::Start()
{
	__super::Start();

	return S_OK;
}

void CFadeMask::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
}

void CFadeMask::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CFadeMask::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}

void CFadeMask::Init_Fader(void* In_Arg_Fader)
{
	if (In_Arg_Fader == nullptr)
		DEBUG_ASSERT;

	FaderDesc tFaderDesc = *(FaderDesc*)In_Arg_Fader;
	m_pFaderCom.lock()->Init_Fader(tFaderDesc);
	
	Set_Enable(true);
}

HRESULT CFadeMask::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	m_vFadeColor.w = 1.f - m_pFaderCom.lock()->Get_FadeColor().w;

	if (FAILED(m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_vFadeColor, sizeof(_float4))))
		return E_FAIL;


	return S_OK;
}

void CFadeMask::OnLevelEnter()
{



}

void CFadeMask::OnLevelExit()
{
	m_pFaderCom.lock()->Exit_Fader();
	//Set_Enable(false);
}


void CFadeMask::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if (EVENT_INIT_EDITER == iArg)
	{

	}

	else if (EVENT_DRAW_EDITER == iArg)
	{
		if (ImGui::CollapsingHeader("FadeMask GameObject"), ImGuiTreeNodeFlags_DefaultOpen)
		{


			ImGui::Separator();
		}
	}
}

void CFadeMask::Call_FadeEnd(FADER_TYPE In_eFaderType)
{
	CallBack_FadeEnd();
	CallBack_FadeEnd.Clear();
}


void CFadeMask::Free()
{
	__super::Free();


}
