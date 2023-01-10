#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CCustomUI;
class CProgressBar;


class CUI_Slider : public CUI_Button
{
public:
	GAMECLASS_H(CUI_Slider)
	CLONE_H(CUI_Slider, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	void			OnLButtonDown() override;
	void			OnMouseOut() override;


public:
	FDelegate<_int>	Callback_ChangeSliderAmount;
	
public:
	virtual void			Set_Y(_float fY) override;

public:
	void			IncreaseAmount(_int	iAmount);
	void			DecreaseAmount(_int	iAmount);


private:
	_bool                Check_Drag();
	void				CalcAmount();
	void				CheckAmount();
	void				SetToAmount();

private:
	weak_ptr<CProgressBar>	m_pBG;
	weak_ptr<CProgressBar>	m_pFill;
	weak_ptr<CCustomUI>	m_pTrack;

private:
	_bool			m_bDragAble = false;

	_int			m_iAmount = 50;
	_int			m_iMaxAmount = 100;

private:
	void			Free();

};

END
