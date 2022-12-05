#pragma once
#include "ClientComponent.h"


BEGIN(Client)


class CEasingTransform : public CClientComponent
{
    GAMECLASS_H(CEasingTransform)
    CLONE_H(CEasingTransform, CComponent)

public:

	enum LOOPING_TYPE
	{
		JUST_START = 0,
		GO_AND_BACK = 1
	};
	enum EASING_TRANSFORM_USING_TYPE
	{
		USING_ONLY_ALPHA,//Late틱에서 자동으로 알파값 넘겨줌
		USING_ONLY_OFFSET,//Late틱에서 자동으로 오프셋 세팅해줌
		USING_CUSTOM//사용자가 세팅해줘야함.
	};


public:
    // CClientComponent을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Start() override;

	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;


public:
	_bool		Is_Lerping() { return m_bLerp; }
	void		Set_Lerp_Alpha(_float In_fTime, EASING_TYPE eType = EASING_TYPE::QUAD_IN, _bool bLoop = false, LOOPING_TYPE eLoopingType = JUST_START, EASING_TRANSFORM_USING_TYPE eUsingType = USING_ONLY_ALPHA);
	void		Set_LerpFloat2(_float2 In_fStart, _float2 In_fEnd, _float In_fTime, EASING_TYPE eType = EASING_TYPE::QUAD_IN, _bool bLoop = false, LOOPING_TYPE eLoopingType = JUST_START);
	void		Set_LerpFloat2_StartFromZero(_float2 In_fEnd, _float In_fTime, EASING_TYPE eType = EASING_TYPE::QUAD_IN, _bool bLoop = false, LOOPING_TYPE eLoopingType = JUST_START);
	void		Set_LerpFloat(_float In_fStart, _float In_fEnd, _float In_fTime, EASING_TYPE eType = EASING_TYPE::QUAD_IN, _bool bLoop = false, LOOPING_TYPE eLoopingType = JUST_START, EASING_TRANSFORM_USING_TYPE eUsingType = USING_CUSTOM);

	_float4		Get_Lerp();
	_float		Get_PlayingRatio() { return m_fLerpTick / m_fLerpTime; }

private:
	


public:
	FDelegate<>		Callback_LerpEnd;
	FDelegate<>		Callback_LerpStart;


private:
	//Lerp Variagbles
	_bool			m_bLoop;
	_bool			m_bLerp;

	_float			m_fLerpAcc;
	_float			m_fLerpTick;
	_float			m_fLerpTime;

	_float4			m_vStart;
	_float4			m_vTarget;
	_float4			m_vLerped;

	EASING_TYPE		m_eLerpType;
	LOOPING_TYPE	m_eLooping_Type;
	EASING_TRANSFORM_USING_TYPE	m_eUsingType;
};


END

