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
	typedef struct tagHoverDesc
	{
		_bool		m_bSizeChange;//사이즈 변경
		_float		m_fSizeMag;//사이즈 변경 배율

	}HUDHOVERDESC;
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
	void		Init_Fader(const Engine::FaderDesc& _tFaderDesc, const HUDHOVERDESC& _tHoverDesc );
	
public:
	void Call_FadeEnd(FADER_TYPE In_eFaderType);

public:
	FDelegate<> CallBack_FadeEnd;
	

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	_float4						m_vFadeColor;
	UI_DESC						m_tBackUpDesc;
	HUDHOVERDESC				m_tHoverDesc;

public:
	void Free();
};
END

