#include "..\Public\Component.h"

CComponent::CComponent()
	: m_isCloned(false)
	//: m_pDevice(pDevice), m_pContext(pContext) 매개변수 ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{
	//Safe_AddRef(m_pDevice);
	//Safe_AddRef(m_pContext);

}

//: m_pDevice(Prototype.m_pDevice)
//, m_pContext(Prototype.m_pContext)
//아래 붙어 있었음.

CComponent::CComponent(const CComponent & Prototype)
	: m_isCloned(true)
{
	//Safe_AddRef(m_pDevice);
	//Safe_AddRef(m_pContext);
}

HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CComponent::Set_Owner(weak_ptr<CGameObject> _pOwner)
{
	m_pOwner = _pOwner;

	return S_OK;
}

weak_ptr<CGameObject> CComponent::Get_Owner()
{
	return m_pOwner;
}

void CComponent::Free()
{
	//Safe_Release(m_pContext);
	//Safe_Release(m_pDevice);
}