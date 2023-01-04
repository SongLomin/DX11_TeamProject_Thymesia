#pragma once
#include "UI.h"

BEGIN(Engine)
class CFader;
END

BEGIN(Client)

class CFadeMask final : public CUI
{
	GAMECLASS_H(CFadeMask);
	CLONE_H(CFadeMask, CGameObject);

public:
	enum class FADE_TYPE { SIMPLEX_LNIEAR, SIMPLEX_POLY, BOTH_LNIEAR, BOTH_POLY };

public:
	virtual void OnLevelEnter() override;
	virtual void OnLevelExit() override;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	void Init_Fader(void* In_Arg_Fader);
	virtual void OnEventMessage(_uint iArg) override;

public:
	void Call_FadeEnd(FADER_TYPE In_eFaderType);



public:
	FDelegate<> CallBack_FadeEnd;

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:

	_float4 m_vFadeColor;

public:
	void Free();
};

END