#pragma once
#include "UI.h"

BEGIN(Client)
class CCustomUI;

class CPlayer_PotionUI :
	public CUI
{
	GAMECLASS_H(CPlayer_PotionUI);
	CLONE_H(CPlayer_PotionUI, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);


public:
	void			Bind_Player() override;

public:
	void			Call_ChangePotion(_uint iCurrentPotion, _uint iMaxPotion);



public:
	_uint			Get_CrrrentPotion();

public:
	void			Set_MaxPotion(_uint _iMaxPotion);
	void			Set_CurrentPotion(_uint _iCurrentPotion);
	

private:
	_uint			m_iMaxPotion;
	_uint			m_iCurrentPotion;

private:
	weak_ptr<CCustomUI> m_pFrame;
	weak_ptr<CCustomUI>	m_pFrameBorder;

	weak_ptr<CCustomUI> m_pIcon;
	weak_ptr<CCustomUI> m_pFontBG;

private:
	TEXTINFO		m_tCurrentPotionTextInfo;
	TEXTINFO		m_tMaxPotionTextInfo;


};

END
