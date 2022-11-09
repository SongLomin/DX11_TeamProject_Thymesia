#pragma once
#include "CustomUI.h"

BEGIN(Engine)
class CShader;
class CFader;

END



BEGIN(Client)
class CHUD_Hover final : public CCustomUI
{
public:
	GAMECLASS_H(CHUD_Hover);
	CLONE_H(CHUD_Hover, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();


public:
	void		Init_Fade();

public:
	void Call_FadeEnd(FADER_TYPE In_eFaderType);

public:
	FDelegate<> CallBack_FadeEnd;
	

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	_float4						m_vFadeColor;
	FaderDesc					m_tFaderDesc;
	UI_DESC						m_tBackUpDesc;
public:
	void Free();
};
END

