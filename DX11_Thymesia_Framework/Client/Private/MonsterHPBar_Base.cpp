#include "stdafx.h"
#include "MonsterHPBar_Base.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "HUD_Hover.h"
#include "Fader.h"


GAMECLASS_C(CMonsterHPBar_Base)
CLONE_C(CMonsterHPBar_Base, CGameObject)

HRESULT CMonsterHPBar_Base::Initialize_Prototype()
{
	__super::Initialize_Prototype();


	return S_OK;
}

HRESULT CMonsterHPBar_Base::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (nullptr != pArg)
		memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));
	else
		MSG_BOX("CUI_MonsterHpBar_Base Error : Not Defined UI_Desc");

	UI_DESC tBorderDesc = m_tUIDesc;
	tBorderDesc.fSizeY = m_tUIDesc.fSizeY - 6.f;
	tBorderDesc.fDepth = 0.6f;;


	m_pBorder = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tBorderDesc);
	m_pBorder.lock()->Set_Texture("Monster_HPBar_Border");

	UI_DESC tWhiteDesc = tBorderDesc;
	tWhiteDesc.fDepth = 0.1f;
	tWhiteDesc.fSizeX -= 5.f;
	tWhiteDesc.fSizeY -= 4.f;

	m_pWhite = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &tWhiteDesc);
	m_pWhite.lock()->Set_Texture("Monster_HPBar_White");
	m_pWhite.lock()->Set_Ratio(1.f);


	UI_DESC tGreenDesc = tWhiteDesc;
	tGreenDesc.fDepth = 0.2f;

	m_pGreen = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &tGreenDesc);
	m_pGreen.lock()->Set_Texture("Monster_HPBar_Green");


	m_pDecoration_Head = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pDecoration_Head.lock()->Set_Texture("Monster_HPBar_Decoration_Head");
	Set_DecorationHead();


	UI_DESC tTrackDesc = m_tUIDesc;

	tTrackDesc .fSizeX = 2.f;
	tTrackDesc .fSizeY = m_tUIDesc.fSizeY - 2.f;
	tTrackDesc .fX = m_tUIDesc.fX - ((m_tUIDesc.fSizeX / 2.f));
	tTrackDesc .fY = m_tUIDesc.fY;
	tTrackDesc .fDepth = 0.f;

	m_pTrack = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &tTrackDesc);
	m_pTrack.lock()->Set_Texture("Monster_HPBar_Track");

	
	tTrackDesc = m_tUIDesc;

	tTrackDesc.fSizeX = 2.f;
	tTrackDesc.fSizeY = m_tUIDesc.fSizeY - 2.f;
	tTrackDesc.fX = m_tUIDesc.fX - ((m_tUIDesc.fSizeX / 2.f));
	tTrackDesc.fY = m_tUIDesc.fY;
	tTrackDesc.fDepth = 0.f;

	m_pGreenTrack = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tTrackDesc);
	m_pGreenTrack.lock()->Set_Texture("Monster_HPBar_GreenTrack");

	tTrackDesc = m_tUIDesc;



	UI_DESC tRecoveryDesc = m_tUIDesc;
	tRecoveryDesc.fSizeY = m_tUIDesc.fSizeY + 6.f;
	tRecoveryDesc.fSizeX = m_tUIDesc.fSizeX + 10.f;
	tRecoveryDesc.fDepth = 0.0f;

	m_pRecovery = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tRecoveryDesc);
	m_pRecovery.lock()->Set_Texture("Monster_HPBar_RecoveryShine");


	UI_DESC tStunDesc = m_tUIDesc;
	tStunDesc.fSizeY = m_tUIDesc.fSizeY + 8.f;
	tStunDesc.fSizeX = m_tUIDesc.fSizeX + 20.f;
	tStunDesc.fDepth = 0.1f;

	m_pStunned = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tRecoveryDesc);
	m_pStunned.lock()->Set_Texture("Monster_HPBar_StunnedShine");


	m_bStun = false;


	Add_Child(m_pBorder);
	Add_Child(m_pWhite);
	Add_Child(m_pGreen);
	Add_Child(m_pDecoration_Head);
	Add_Child(m_pTrack);

	return S_OK;
}

HRESULT CMonsterHPBar_Base::Start()
{
	__super::Start();

	return S_OK;
}

void CMonsterHPBar_Base::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::Z, KEY_STATE::TAP))
	{
		m_pWhite.lock()->Decrease_Ratio(0.1f);
	}
	if (KEY_INPUT(KEY::X, KEY_STATE::TAP))
	{
		m_pWhite.lock()->Increase_Ratio(0.1f);
	}

	if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
	{
		Green_Damaged(0.3f);
	}
	if (KEY_INPUT(KEY::E, KEY_STATE::TAP))
	{
		m_pGreen.lock()->Increase_Ratio(0.3f);
		Set_Stun(false);
	}

	if (KEY_INPUT(KEY::R, KEY_STATE::TAP))
	{
		Toggle_Recovery();
	}

	if (KEY_INPUT(KEY::UP, KEY_STATE::TAP))
	{
		m_tUIDesc.fY -= 10.f;
	}
	if (KEY_INPUT(KEY::DOWN, KEY_STATE::TAP))
	{
		m_tUIDesc.fY += 10.f;

	}

	if (KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
	{
		m_tUIDesc.fX -= 10.f;

	}
	if (KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
	{
		m_tUIDesc.fX += 10.f;
	}

	Set_ChildPosFromThis();
	Check_Track();


}

void CMonsterHPBar_Base::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CMonsterHPBar_Base::Render()
{
	//

	return S_OK;
}

void CMonsterHPBar_Base::Green_Damaged(_float _fDmgRatio)
{
	_float fPrevGreenRatio = m_pGreen.lock()->Get_Ratio();

	if (fPrevGreenRatio <= 0.f)
		return;

	UI_DESC GreenDesc;

	_float fDmgRatio =  _fDmgRatio;

	if (m_pGreen.lock()->Get_Ratio() < fDmgRatio)
		fDmgRatio = m_pGreen.lock()->Get_Ratio();
	
	m_pGreen.lock()->Decrease_Ratio(fDmgRatio);

	GreenDesc = m_pGreen.lock()->Get_UIDESC();
	/*
		데미지 입은 위치에서, 얼만큼 입은 데미지 크기만큼 박스를 생성.
	*/
	_float	fGreenTrackSizeX = fDmgRatio * GreenDesc.fSizeX;
	m_pGreenTrack.lock()->Set_UIPosition(
		(GreenDesc.fX - (GreenDesc.fSizeX * 0.5f)) + (GreenDesc.fSizeX * m_pGreen.lock()->Get_Ratio()) + (fGreenTrackSizeX * 0.5f),
		GreenDesc.fY,
		fGreenTrackSizeX, 
		GreenDesc.fSizeY
	);

	CHUD_Hover::HUDHOVERDESC hoverDesc;

	hoverDesc.bSizeChange = true;
	hoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA_HEIGHT;
	hoverDesc.fSizeMag = 2.5f;

	FaderDesc faderDesc;

	faderDesc.eFaderType = FADER_OUT;
	faderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	faderDesc.fDelayTime = 0.f;
	faderDesc.fFadeMaxTime = 0.5f;
	faderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.7f);
	
	m_pGreenTrack.lock()->Init_Fader(faderDesc, hoverDesc);
	m_pGreenTrack.lock()->CallBack_FadeEnd += bind(&CMonsterHPBar_Base::Call_GreenTrackFadeEnd, this, placeholders::_1);

	if (m_pGreen.lock()->Get_Ratio() <= 0.f)
	{
		Set_Stun(true);
	}


}

void CMonsterHPBar_Base::Toggle_Recovery()
{
	if (m_pRecovery.lock()->Get_Enable())
	{
		m_pRecovery.lock()->Get_Component<CFader>().lock()->Exit_Fader();
		m_pRecovery.lock()->Set_Enable(false);
	}
	else
	{
		CHUD_Hover::HUDHOVERDESC hoverDesc;

		ZeroMemory(&hoverDesc, sizeof(CHUD_Hover::HUDHOVERDESC));

		FaderDesc faderDesc;
		faderDesc.eFaderType = FADER_INOUTLOOPING;
		faderDesc.fDelayTime = 0.f;
		faderDesc.fFadeMaxTime = 0.5f;
		faderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);
		faderDesc.eLinearType = LINEAR_TYPE::LNIEAR;

		m_pRecovery.lock()->Set_Enable(true);
		m_pRecovery.lock()->Init_Fader(faderDesc, hoverDesc);
	}
}

void CMonsterHPBar_Base::Set_Stun(bool _bStun)
{
	m_bStun = _bStun;


	if (m_bStun)
	{
		m_pGreen.lock()->Set_Enable(false);
		m_pWhite.lock()->Set_Enable(false);
		m_pTrack.lock()->Set_Enable(false);
		m_pDecoration_Head.lock()->Set_Enable(false);


		CHUD_Hover::HUDHOVERDESC hoverDesc;

		ZeroMemory(&hoverDesc, sizeof(CHUD_Hover::HUDHOVERDESC));

		FaderDesc faderDesc;
		faderDesc.eFaderType = FADER_INOUTLOOPING;
		faderDesc.fDelayTime = 0.f;
		faderDesc.fFadeMaxTime = 0.3f;
		faderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);
		faderDesc.eLinearType = LINEAR_TYPE::LNIEAR;

		m_pStunned.lock()->Set_Enable(true);
		m_pStunned.lock()->Init_Fader(faderDesc, hoverDesc);

	}
	else
	{
		m_pGreen.lock()->Set_Enable(true);
		m_pWhite.lock()->Set_Enable(true);
		m_pTrack.lock()->Set_Enable(true);
		m_pDecoration_Head.lock()->Set_Enable(true);
	
		m_pStunned.lock()->Set_Enable(false);
		m_pStunned.lock()->Get_Component<CFader>().lock()->Exit_Fader();
	}

}

void CMonsterHPBar_Base::Add_Child(weak_ptr<CUI> pChild)
{
	__super::Add_Child(pChild);

	//어차피 몬스터 체력바는 무죄~~~~~~~~~~~~~~~~~껀 배틀 UI임. 아님 말고.
	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Weak_StaticCast<CGameObject>(pChild));

}

void CMonsterHPBar_Base::Check_Track()
{
	_float fRatio = m_pWhite.lock()->Get_Ratio();

	UI_DESC WhiteDesc = m_pWhite.lock()->Get_UIDESC();

	if (fRatio  < 1.f)
	{
		m_pTrack.lock()->Set_Enable(true);
		m_pTrack.lock()->Set_UIPosition((WhiteDesc.fX - (WhiteDesc.fSizeX * 0.5f)) + (WhiteDesc.fSizeX * fRatio), WhiteDesc.fY);
	}
	else
	{
		m_pTrack.lock()->Set_Enable(false);
	}

}

void CMonsterHPBar_Base::Set_ChildPosFromThis()
{
	m_pBorder.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
	m_pWhite.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
	m_pGreen.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
	m_pRecovery.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);


	m_pStunned.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);

	//m_pRecovery.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
	
	UI_DESC GreenTrackDesc = m_pGreenTrack.lock()->Get_UIDESC();
	UI_DESC GreenDesc = m_pGreen.lock()->Get_UIDESC();


	m_pGreenTrack.lock()->Set_UIPosition
	((GreenDesc.fX - (GreenDesc.fSizeX * 0.5f)) + (GreenDesc.fSizeX * m_pGreen.lock()->Get_Ratio()) + (GreenTrackDesc.fSizeX * 0.5f),
		
	m_tUIDesc.fY);
	
	Set_DecorationHead();


}

void CMonsterHPBar_Base::Set_DecorationHead()
{
	UI_DESC tDecorationHeadDesc = m_tUIDesc;

	tDecorationHeadDesc.fSizeX = 2.f;
	tDecorationHeadDesc.fSizeY = m_tUIDesc.fSizeY;
	tDecorationHeadDesc.fX = m_tUIDesc.fX - ((m_tUIDesc.fSizeX / 2.f));
	tDecorationHeadDesc.fY = m_tUIDesc.fY;
	tDecorationHeadDesc.fDepth = 0.f;

	m_pDecoration_Head.lock()->Set_UIDesc(tDecorationHeadDesc);
}


void CMonsterHPBar_Base::Call_GreenTrackFadeEnd(FADER_TYPE eType)
{
	m_pGreenTrack.lock()->Set_Enable(false);
}
