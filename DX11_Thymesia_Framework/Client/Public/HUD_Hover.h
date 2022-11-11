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

	enum HUD_HOVER_ANIMAITON_TYPE {
		HUD_HOVER_ANIMATION_FROM_ALPHA, 
		HUD_HOVER_ANIMATION_JUSTADD,
		HUD_HOVER_ANIMATION_END};

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
	void		Init_Fader(const Engine::FaderDesc& _tFaderDesc, const HUDHOVERDESC& _tHoverDesc, HUD_HOVER_ANIMAITON_TYPE eHoverType);
	virtual		void Set_UIDesc(UI_DESC _tUIDesc);



public:
	void Call_FadeEnd(FADER_TYPE In_eFaderType);

public:
	FDelegate<FADER_TYPE> CallBack_FadeEnd;
	
public:
	void				Set_Alpha(_float4 _vAlpha);

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	_float4						m_vFadeColor;
	UI_DESC						m_tBackUpDesc;
	HUDHOVERDESC				m_tHoverDesc;
	HUD_HOVER_ANIMAITON_TYPE				m_eHoverType;
public:
	void Free();
};
END

