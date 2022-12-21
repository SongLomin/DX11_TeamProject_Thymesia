#pragma once
#include "UI_Container.h"

BEGIN(Client)

class CCustomUI;
class CEasingComponent_Transform;
class CUI_DamageFontPiece;

class CUI_DamageFont : public CUI_Container
{

public:
	enum class FONT_PLAY_TYPE{FONT_FADEIN, FONT_STAY, FONT_FADE_OUT, FONT_END};

public:
	GAMECLASS_H(CUI_DamageFont)
	CLONE_H(CUI_DamageFont, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta)override ;

public:
	void		SetUp_DamageFont(_uint iDmg, _float2 vPos, ATTACK_OPTION eAttackOption);

private:
	_float2		m_vPos;
	_float		m_fOffsetX;
	_float		m_fDefaultSize;
	FONT_PLAY_TYPE		m_ePlayingType = FONT_PLAY_TYPE::FONT_END;
	
	weak_ptr<CEasingComponent_Transform>	m_pEasingTransform;

	_float		m_fLifeTime;
	_float		m_fLifeTick;

public:
	void				Call_LerpEnd_FadeIn();
	void				Call_LerpEnd_FadeOut();
	void				Call_Shaking_End();

private:
	virtual void Free();

};

END

