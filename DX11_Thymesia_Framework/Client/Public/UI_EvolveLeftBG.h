#pragma once
#include "CustomUI.h"

BEGIN(Engine)

class CTexture;
class CShader;
END

BEGIN(Client)

class CUI_EvolveLeftBG: public CCustomUI
{
public:
	GAMECLASS_H(CUI_EvolveLeftBG)
		CLONE_H(CUI_EvolveLeftBG, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

private:
	virtual void            Set_Texture(const _char* sKey) override;


private:
	weak_ptr<CTexture>	m_pUVAnimMask;


	_float2				m_fUVOffset;
	_float				m_fUVOffsetAmount;

	_float2				m_fNoneAnimRatio;

protected:
	virtual HRESULT SetUp_ShaderResource() override;

};

END
