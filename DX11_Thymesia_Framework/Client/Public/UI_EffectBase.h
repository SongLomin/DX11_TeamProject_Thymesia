#pragma once

#include "CustomUI.h"
#include "UI_Enums.h"

BEGIN(Client)

class CEasingComponent_Float;
class CEasingComponent_Alpha;
class CEasingComponent_Transform;

typedef struct tagUIClipDesc
{
	_float		_fClipTime;

	_float		_fStartAlpha;
	_float		_fTargetAlpha;


	_float2		_fStartPos;
	_float2		_fTargetPos;

	_float2		_fStartSize;
	_float2		_fTargetSize;
	
	EASING_TYPE	_eAlphaEasingType = EASING_TYPE::QUAD_IN;
	_ulong		_eEffectCondition = (LONGLONG)UI_EFFECT_CONDITION::UI_NONE;

	tagUIClipDesc()
	{
		ZeroMemory(this, sizeof(tagUIClipDesc));
	}

}UICLIPDESC;

class CUI_EffectBase : public CCustomUI
{

public:
	GAMECLASS_H(CUI_EffectBase)
	CLONE_H(CUI_EffectBase, CGameObject)


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	void		Init_UIEffect();
	void		Add_Clip(UICLIPDESC tUIClipDesc);

	void		Play();
	void		Stop();
protected:
	virtual void SetUp_Component() override;
	virtual void SetUp_Animaiton();


protected:
	

	virtual void	ExecuteCurrentClip(UICLIPDESC tagClipDesc);

protected:
	void					NextClip();
	

	void					Call_AnimaitonClipEnd(_uint	iCallbackClipIndex);
	void					Call_AnimationEnd();
public:
	FDelegate<>				Callback_AnimaitonEnd;
	FDelegate<_uint>		Callback_AnimaitonClipEnd;

protected:
	UI_EFFECT_ANIM_STATE	m_eAnimState = UI_EFFECT_ANIM_STATE::STOP;

	_float					m_fCurrentClipTime = 0;
	int						m_iIndex = 0;
	
	vector<UICLIPDESC>		m_vecUIClipDesc;
protected:
	weak_ptr<CEasingComponent_Alpha> m_pEasingAlpha;
	weak_ptr<CEasingComponent_Float> m_pEasingFloat;
	weak_ptr<CEasingComponent_Transform> m_pEasingTransform;
	weak_ptr<CEasingComponent_Transform> m_pEasingSizing;
	weak_ptr<CEasingComponent_Float> m_pEasingRotation;

};

END

