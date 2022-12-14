#include "stdafx.h"
#include "EffectObject.h"
#include "Client_Components.h"

GAMECLASS_C(CEffectObject)

HRESULT CEffectObject::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CEffectObject::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom = Add_Component<CShader>();
	m_pRendererCom = Add_Component<CRenderer>();
	m_pTextureCom = Add_Component<CTexture>();

	return S_OK;
}

void CEffectObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEffectObject::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));

}

HRESULT CEffectObject::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}

void CEffectObject::SetUp_ShaderResource()
{
	CallBack_Bind_SRV(m_pShaderCom, "");

	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
	m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", 0);
}

void CEffectObject::Free()
{
}
