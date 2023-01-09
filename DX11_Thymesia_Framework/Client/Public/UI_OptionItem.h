#pragma once
#include "UI_Button.h"

BEGIN(Client)

class CUI_OptionItem :  public CUI_Button_Base
{
public:
	GAMECLASS_H(CUI_OptionItem)
	CLONE_H(CUI_OptionItem , CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

public:
	virtual void	Set_OriginCenterPosFromThisPos();
	void			Set_ScrollOffsetY(_float fOffsetY);
	void			Add_ScrollOffsetY(_float fOffsetY);
	void			ResetPos();



protected:
	virtual void	OnMouseOver() override;
	virtual void	OnMouseOut() override;


private:
	_float					m_fScrollOffsetY = 0.f;
	_float2					m_fOriginCenterPos = { 0.f,0.f };
};

END
