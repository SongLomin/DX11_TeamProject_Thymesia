#include "stdafx.h"
#include "UI_PlagueWeapon.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "CustomUI.h"
#include "UI_Utils.h"


GAMECLASS_C(CUI_PlagueWeapon)
CLONE_C(CUI_PlagueWeapon, CGameObject)

HRESULT CUI_PlagueWeapon::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_PlagueWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	UI_DESC tIconDesc = m_tUIDesc;
	
	tIconDesc.fDepth -= 0.1f;

	tIconDesc.fSizeX *= 2.f;
	tIconDesc.fSizeY *= 2.f;

	m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tIconDesc);
	
	UI_DESC tDecorationDesc = tIconDesc;


	tDecorationDesc.fSizeX = 16.f;
	tDecorationDesc.fSizeY = 16.f;
	tDecorationDesc.fX = m_tUIDesc.fX + 130.f;
	tDecorationDesc.fY = m_tUIDesc.fY;

	m_pDecoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tDecorationDesc);
	m_pDecoration.lock()->Set_Texture("Font_Diamond");

	m_MainTextInfo.bAlways = false;
	m_MainTextInfo.bCenterAlign= false;
	m_MainTextInfo.fRotation = 0.f;
	m_MainTextInfo.szText = L"";
	m_MainTextInfo.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_MainTextInfo.vPosition.x = m_tUIDesc.fX + 150.f;
	m_MainTextInfo.vPosition.y = m_tUIDesc.fY - 10;
	m_MainTextInfo.vScale = _float2(0.7f, 0.7f);
	

	m_WeaponTextInfo.bAlways = false;
	m_WeaponTextInfo.bCenterAlign = false;
	m_WeaponTextInfo.fRotation = 0.f;
	m_WeaponTextInfo.szText = L"";
	m_WeaponTextInfo.vColor = _float4(28.f / 225.f, 227.f / 255.f, 151.f / 255.f, 1.f);
	m_WeaponTextInfo.vPosition.x = m_tUIDesc.fX + 150.f;
	m_WeaponTextInfo.vPosition.y = m_tUIDesc.fY + 30.f;
	m_WeaponTextInfo.vScale = _float2(0.7f, 0.7f);


	Add_Child(m_pIcon);
	Add_Child(m_pDecoration);

	return S_OK;
}

HRESULT CUI_PlagueWeapon::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_PlagueWeapon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_PlagueWeapon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_MainTextInfo);
	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_WeaponTextInfo);


}

HRESULT CUI_PlagueWeapon::Render(ID3D11DeviceContext* pDeviceContext)
{
	__super::Render(pDeviceContext);

	return S_OK;
}

void CUI_PlagueWeapon::Set_MainText(const _tchar* pKey)
{
	m_MainTextInfo.szText = pKey;

}

void CUI_PlagueWeapon::Set_SkillInfo(const char* pKey, const _tchar* pSkillName)
{
	m_pIcon.lock()->Set_Texture(pKey);

	m_WeaponTextInfo.szText = pSkillName;
	
}

void CUI_PlagueWeapon::Set_SkillIcon(SKILL_NAME eSkillName)
{
	CUI_Utils::Set_SkillIcon(m_pIcon, eSkillName);
}

void CUI_PlagueWeapon::Set_IconSize(_float fSizeX, _float fSizeY)
{
	m_pIcon.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, fSizeX, fSizeY);
}


