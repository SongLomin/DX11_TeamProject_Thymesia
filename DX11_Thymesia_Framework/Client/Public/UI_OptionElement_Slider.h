#pragma once
#include "UI_OptionElementBase.h"

BEGIN(Client)

class CCustomUI;
class CUI_Button;
class CUI_Slider;

class CUI_OptionElement_Slider : public CUI_OptionElementBase
{
public:
	GAMECLASS_H(CUI_OptionElement_Slider);
	CLONE_H(CUI_OptionElement_Slider, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual void				Call_OnClickLeftArrow();
	virtual void				Call_OnClickRightArrow();

	virtual void				Set_Y(_float fY);

public:
	void						Call_ChangeSliderAmount(_int iAmount);

protected:
	virtual void				SetUp_ArrowPos();

private:
	void						ClickArrow();

private:
	weak_ptr<CCustomUI>			m_pOptionState;
	TEXTINFO					m_tAmountInfo;

private: //Slider Child
	weak_ptr<CUI_Slider>		m_pSlider;

private:
	void						Free();






};

END
