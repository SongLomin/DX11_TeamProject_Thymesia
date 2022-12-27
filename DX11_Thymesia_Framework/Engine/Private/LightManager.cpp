#include "LightManager.h"

IMPLEMENT_SINGLETON(CLight_Manager)

const LIGHTDESC& CLight_Manager::Get_LightDesc(_uint iIndex) const
{
	const LIGHTDESC* elem_LightDesc;

	for (auto& elem : m_pLights)
	{
		elem_LightDesc = &elem->Get_LightDesc();

		if (iIndex == elem_LightDesc->Get_LightIndex())
			return *elem_LightDesc;
	}

	return LIGHTDESC();
}

void CLight_Manager::Set_LightDesc(const LIGHTDESC& LightDesc)
{
	for (auto& elem : m_pLights)
	{
		if (LightDesc.Get_LightIndex() == elem->Get_LightDesc().Get_LightIndex())
		{
			elem->Set_LightDesc(LightDesc);
			return;
		}
	}
}

const LIGHTDESC& CLight_Manager::Add_Light(const LIGHTDESC& LightDesc)
{
	m_pLights.push_back(CCustomLight::Create(LightDesc));

	return m_pLights.back()->Get_LightDesc();
}

_bool CLight_Manager::Remove_Light(const _uint& iIndex)
{
	for (auto iter = m_pLights.begin(); iter != m_pLights.end();)
	{
		if (iIndex == (*iter)->Get_LightDesc().Get_LightIndex())
		{
			m_pLights.erase(iter);
			return true;
		}

		else
		{
			++iter;
		}
	}

	return false;
}

void CLight_Manager::Clear_Lights()
{
	m_pLights.clear();
}

HRESULT CLight_Manager::Render_Lights(weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer, ID3D11DeviceContext* pDeviceContext)
{
	for (auto& pLight : m_pLights)
	{
		pLight->Render(pShader, pVIBuffer, pDeviceContext);
	}

	return S_OK;
}

void CLight_Manager::Free()
{
}
