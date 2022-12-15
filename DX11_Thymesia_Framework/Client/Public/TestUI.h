#pragma once
#include "CustomUI.h"


BEGIN(Engine)
class CTexture;
END


BEGIN(Client)

class CEasingComponent_Alpha;
class CEasingComponent_Transform;


class CTestUI : public CCustomUI
{
public:
	GAMECLASS_H(CTestUI)
	CLONE_H(CTestUI, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);
	void		Set_Selected(_bool bSelected);
protected:
	virtual HRESULT SetUp_ShaderResource() override;


private:
	weak_ptr<CTexture>	m_pMaskingTextureCom;

	weak_ptr<CEasingComponent_Alpha>	m_pEasingTransformCom;
	_float2						m_vUXOffset;
	_bool						m_bSelected;
	TEXTINFO					m_tTest;
	_float						m_fRange;

};


END
