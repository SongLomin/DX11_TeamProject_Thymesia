#include "..\Public\Component.h"

CComponent::CComponent()
	: m_isCloned(false)
{

}


CComponent::CComponent(const CComponent & Prototype)
	: m_isCloned(true)
{
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
}