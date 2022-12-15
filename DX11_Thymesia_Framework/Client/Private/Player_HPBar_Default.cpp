#include "stdafx.h"
#include "Player_HPBar_Default.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"

GAMECLASS_C(CPlayer_HPBar_Default)
CLONE_C(CPlayer_HPBar_Default, CGameObject);

HRESULT CPlayer_HPBar_Default::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_HPBar_Default::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_HPBar"),
		VTXTEX_DECLARATION::Element,
		VTXTEX_DECLARATION::iNumElements);

	m_eRenderGroup = RENDERGROUP::RENDER_FINAL;

	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::PLAYERHUD, Cast<CGameObject>(m_this));

	m_tUIDesc.fDepth = 0.f;

	m_iPassIndex = 0;

	
	return S_OK;
}

HRESULT CPlayer_HPBar_Default::Start()
{
	__super::Start();

	return S_OK;
}

void CPlayer_HPBar_Default::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CPlayer_HPBar_Default::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

HRESULT CPlayer_HPBar_Default::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;;
}

void CPlayer_HPBar_Default::OnEventMessage(_uint iArg)
{
}

HRESULT CPlayer_HPBar_Default::SetUp_ShaderResource()
{
	__super::SetUp_ShaderResource();

	if (FAILED(m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture")))
	{
		MSG_BOX("PlayerHPBar_Default_Error");
	}
	return S_OK;
}

void CPlayer_HPBar_Default::Free()
{
	__super::Free();

}
