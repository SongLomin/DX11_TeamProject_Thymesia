#include "stdafx.h"
#include "UI_DamageFont.h"
#include "CustomUI.h"


GAMECLASS_C(CUI_DamageFont)
CLONE_C(CUI_DamageFont, CGameObject)

HRESULT CUI_DamageFont::Initialize(void* pArg)
{
	__super::Initialize(pArg);

    return S_OK;
}

void CUI_DamageFont::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bShaking)//내 오프셋만큼 다른 자식들에게도 오프셋을 먹여줘야함.
	{
		for(auto& elem : m_vecChildUI)
		{ 
			elem.lock()->Set_OffsetPosition(m_fOffsetPosition);
		}
	}
}

void CUI_DamageFont::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CUI_DamageFont::SetUp_DamageFont(_uint iDmg, _float2 vPos, ATTACK_OPTION eAttackOption)
{
	//m_fDefaultSize = iDmg > 150 ? 256.f : 128.f;
	
	m_fDefaultSize = 48.f;

	m_fOffsetX = m_fDefaultSize * 0.5f;

	string	strDamage = to_string(iDmg);
	string strDamageFontKey;
	int		strSize = strDamage.size();
	weak_ptr<CCustomUI> pDamageFont;
	for (int i = 0; i < (int)strDamage.size(); i++)
	{
		strDamageFontKey = "DamageFont_";
		switch (eAttackOption)
		{
		case Client::ATTACK_OPTION::NONE:
			break;
		case Client::ATTACK_OPTION::NORMAL:
			strDamageFontKey += "Normal_";
			break;
		case Client::ATTACK_OPTION::PLAGUE:
			strDamageFontKey += "Claw";
			break;
		case Client::ATTACK_OPTION::PARRY:
			strDamageFontKey += "Parry_";
			break;
		}
		//800.f 
		//450.f
		//3
		/*
		10 1
		5/2 = 2

		i - (size/2)

		21012


		*/
		//strDamageFontKey += strDamage[i];
		//setting Offset;
		_float fOffset = m_fOffsetX * (i - (strSize / 2));

		pDamageFont = ADD_STATIC_CUSTOMUI;
		pDamageFont.lock()->Set_UIPosition
		(
			vPos.x + fOffset,
			vPos.y,
			m_fDefaultSize,
			m_fDefaultSize
		);
		pDamageFont.lock()->Set_Texture(strDamageFontKey.c_str());
		pDamageFont.lock()->Set_DeffuseIndex((_uint)(strDamage[i] - '0'));
		m_vecChildUI.push_back(pDamageFont);
	}
	CallBack_ShakingEnd += bind(&CUI_DamageFont::Call_Shaking_End, this);

	Add_Shaking(2.f, 5.f);
}


void CUI_DamageFont::Call_LerpEnd_FadeIn()
{
}

void CUI_DamageFont::Call_LerpEnd_FadeOut()
{

}

void CUI_DamageFont::Call_Shaking_End()
{
	Set_Enable(false);
	m_vecChildUI.clear();
}

void CUI_DamageFont::Free()
{
}
