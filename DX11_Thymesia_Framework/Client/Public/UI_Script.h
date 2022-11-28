#pragma once
#include "UI_Container.h"
#include <queue>

BEGIN(Client)

class CCustomUI;
class CEasingTransform;


class CUI_Script :
    public CUI_Container
{
public:
	typedef struct tagScriptDesc 
	{
		wstring	strScriptKey;

		_float2	fSize;
		_float2	fPos;
		_float	fLifeTime;

		tagScriptDesc() { *this = ZeroMem(); }
		tagScriptDesc(_float2 _fSize, _float2 _fPos, _float _fLifeTime, wstring& _strKeyName)
			: fSize(_fSize), fPos(_fPos), fLifeTime(_fLifeTime), strScriptKey(_strKeyName)
		{

		}
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
			tScriptDesc.strScriptKey = L"";

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
	void			Excute();

	void			Add_Script(SCRIPTDESC script);


private:
	queue<SCRIPTDESC>				m_ScriptQueue;
	weak_ptr<CCustomUI>				m_pScript;
	weak_ptr<CEasingTransform>		m_pEasingTrnaformCom;

	SCRIPTDESC	m_tPlayScript;
};

END
