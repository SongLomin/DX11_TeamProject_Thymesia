#pragma once
#include "CustomUI.h"


BEGIN(Client)

class CEasingComponent_Alpha;


class CUI_Script : public CCustomUI
{
public:
	typedef enum class tagScriptPlayingType
	{
		SCRIPT_FADIN, SCRIPT_PLAYING, SCRIPT_FADEOUT, SCRIPT_END
	}SCRIPTPLAYINGTYPE;

	typedef struct tagScriptDesc 
	{
		string	strScriptKey;

		_float2	fSize;
		_float2	fPos;
		_float	fLifeTime;
		_bool	bLerp;
		~tagScriptDesc()
		{
			strScriptKey.clear();
		}
		static tagScriptDesc& ZeroMem()
		{
			tagScriptDesc tScriptDesc;

			tScriptDesc.fSize = _float2(g_iWinCX >> 1, g_iWinCY >> 1);
			tScriptDesc.fPos = _float2(g_iWinCX >> 1, g_iWinCY >> 1);
			tScriptDesc.fLifeTime = 0.f;
			tScriptDesc.strScriptKey = "";

			return tScriptDesc;
		}
	}SCRIPTDESC;


public:
	GAMECLASS_H(CUI_Script)
	CLONE_H(CUI_Script, CGameObject)

public:
	// CUI_Container을(를) 통해 상속됨
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;

public:
	void				ReadyScript(SCRIPTDESC tScriptDesc);
	SCRIPTPLAYINGTYPE	GetScriptPlayingType() { return m_eScriptPlaying; }
private:
	_float				m_fLifeTime;
	_float				m_fLifeTick;
	SCRIPTPLAYINGTYPE	m_eScriptPlaying;
	_bool				m_bLerp;



public:
	void				Call_LerpEnd_FadeIn();
	void				Call_LerpEnd_FadeOut();

private:
	weak_ptr<CEasingComponent_Alpha>		m_pEasingAlphaCom;
	SCRIPTDESC						m_tPlayScript;


};

END
