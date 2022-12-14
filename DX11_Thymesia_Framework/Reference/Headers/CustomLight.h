#pragma once
#include "Base.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;

class CCustomLight :
    public CBase
{
    GAMECLASS_H(CCustomLight)

public:
	const LIGHTDESC& Get_LightDesc() const {
		return m_LightDesc;
	}

	void Set_LightDesc(const LIGHTDESC& LightDesc) {
		m_LightDesc = LightDesc;
	}

private:
	HRESULT Initialize(const LIGHTDESC& LightDesc);
	
public:
	HRESULT Render(weak_ptr<CShader> pShader, weak_ptr<CVIBuffer_Rect> pVIBuffer, ID3D11DeviceContext* pDeviceContext);


private:
	static _uint				g_iLightIndex;
	LIGHTDESC					m_LightDesc;

public:
	static shared_ptr<CCustomLight> Create(const LIGHTDESC& LightDesc);
	void Free();

};

END