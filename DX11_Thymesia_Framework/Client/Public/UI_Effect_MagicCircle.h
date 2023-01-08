#pragma once
#include "UI_EffectBase.h"

BEGIN(Client)

class CUI_Effect_MagicCircle : public CUI_EffectBase
{
public:
	GAMECLASS_H(CUI_Effect_MagicCircle)
	CLONE_H(CUI_Effect_MagicCircle, CGameObject)


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual void Play(_bool bRepeat) override;
private:
	virtual void SetUp_Animaiton() override;


};

END

