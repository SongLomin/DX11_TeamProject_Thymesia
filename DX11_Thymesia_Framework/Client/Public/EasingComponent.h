#pragma once
#include "ClientComponent.h"


BEGIN(Client)

class CEasingComponent abstract: public CClientComponent
{
public:
	GAMECLASS_H(CEasingComponent);

	enum PLAY_TYPE
	{
		ONCE = 0,
		LOOP_JUST_START,
		LOOP_GO_AND_BACK
	};

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;

public:
	_bool		Is_Lerping() { return m_bLerp; }

	_float		Get_PlayingRatio() { return m_fLerpTick / m_fLerpTime; }


	_float4		Get_Start() { return m_vStart; }
	_float4		Get_Target() { return m_vStart; }
	void		Stop();
	void		Set_PlayType(PLAY_TYPE ePlayType) { m_ePlay_Type = ePlayType; }

protected:
	virtual void	Set_Lerp(_float4	vStart, _float4	vTarget, _float fTime,
		EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse);//public으로풀지말고, 자식 단에서 public함수로 내려서 필요한 플롯을 채워서 쓰도록

protected:
	//Lerp Variagbles
	_bool			m_bLerp;

	_float			m_fLerpTick;
	_float			m_fLerpTime;

	_float4			m_vStart;
	_float4			m_vTarget;
	_float4			m_vLerped;

	EASING_TYPE		m_eEasingType;
	PLAY_TYPE		m_ePlay_Type;
	_bool			m_bCustomUse;

public:
	FDelegate<>		Callback_LerpEnd;
	FDelegate<>		Callback_LerpStart;

};

END