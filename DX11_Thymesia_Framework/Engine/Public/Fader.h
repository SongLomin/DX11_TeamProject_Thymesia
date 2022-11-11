#pragma once
#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CFader : public CComponent
{
	GAMECLASS_H(CFader)
	CLONE_H(CFader, CComponent)

public:
	CFader(const CFader& rhs);

public:
	_float4 Get_FadeColor() const { return m_tFaderDesc.vFadeColor; }
	_float	Get_FadeTime() const { return m_fFadeTime; }
	FaderDesc Get_FaderDesc() const { return m_tFaderDesc; }


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

	virtual void Tick(_float fTimeDelta) override;
public:
	void Init_Fader(const FaderDesc& In_tFaderDesc);
	void Reset_Fader();
	void Exit_Fader();

private:
	void Fade_In(_float fTimeDelta);
	void Fade_Out(_float fTimeDelta);
	void Fade_Looping(_float fTimeDelta);
	void Bake_RatioByLinearType(_float& Out_fRatio);
	
	

private:
	FaderDesc m_tFaderDesc;

	_bool	m_isFadeOut = false;

	_float	m_fFadeTime;
	_float	m_fCurrentDelayTime;
	_bool	m_bEnable = false;





public:
	FDelegate<FADER_TYPE> CallBack_FadeEnd;

public:
	virtual void OnDestroy() override;

public:
	void Free();
};

END