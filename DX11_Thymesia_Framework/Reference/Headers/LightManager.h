#pragma once
#include "Base.h"
#include "CustomLight.h"
BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)

public:
	const LIGHTDESC& Get_LightDesc(_uint iIndex) const;
	void Set_LightDesc(const LIGHTDESC& LightDesc);

public:
	const LIGHTDESC& Add_Light(const LIGHTDESC& LightDesc);
	_bool Remove_Light(const _uint& iIndex);
	void Clear_Lights();
	HRESULT Render_Lights(weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer, ID3D11DeviceContext* pDeviceContext);

private:
	list<shared_ptr<CCustomLight>> m_pLights;

public:
	void Free();
};

END