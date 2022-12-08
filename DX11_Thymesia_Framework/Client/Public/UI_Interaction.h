#pragma once
#include "CustomUI.h"

BEGIN(Client)

class CEasingComponent_Alpha;
class CInteraction_Prop;

class CUI_Interaction : public CCustomUI
{
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

private:
	weak_ptr< CEasingComponent_Alpha> m_pEasingAlphaCom;

public:
	void			Call_CollisionEnter(weak_ptr<CCollider> pMyCollider, _uint	_iInteractionType);
	void			Call_CollisionExit();
	void			Call_ActionStart();
	void			Call_ActionEnd();
};

END
