#pragma once

#include "CustomUI.h"

BEGIN(Client)

class CEasingComponent_Float;
class CEasingComponent_Alpha;
class CEasingComponent_Transform;


typedef struct tagUIEffectDesc
{
	
	FDelegate<_uint>		Callback_AnimaitonClipEnd;

}UIEFFECTDESC;


class CUI_EffectBase : public CCustomUI
{
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

protected:
	virtual void SetUp_Component() override;
	virtual void SetUp_Animaiton();

protected:
	_bool		m_bLoop;

protected:
	weak_ptr<CEasingComponent_Alpha> m_pEasingAlpha;
	weak_ptr<CEasingComponent_Float> m_pEasingFloat;
	weak_ptr<CEasingComponent_Transform> m_pEasingTransform;
	weak_ptr<CEasingComponent_Transform> m_pEasingSizing;
	weak_ptr<CEasingComponent_Float> m_pEasingRotation;
};

END

