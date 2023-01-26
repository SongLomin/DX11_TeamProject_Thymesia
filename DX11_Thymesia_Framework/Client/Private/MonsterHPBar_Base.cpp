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
#include "PipeLine.h"
#include "Monster.h"
#include "Status_Monster.h"
#include "MonsterParryingBar.h"

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

	if (pArg == nullptr)
	{
		UI_DESC tDesc;
		tDesc.fDepth = 0.f;
		tDesc.fX = g_iWinCX / 2.f;
		tDesc.fY = g_iWinCY / 2.f;
		tDesc.fSizeX = 150.f;
		tDesc.fSizeY = 15.f;
		memcpy(&m_tUIDesc, &tDesc, sizeof(UI_DESC));
	}	
	else
		memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));

//	if (nullptr != pArg)
//	else
//		MSG_BOX("CUI_MonsterHpBar_Base Error : Not Defined UI_Desc");

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
	m_pTrack.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

	
	tTrackDesc = m_tUIDesc;

	tTrackDesc.fSizeX = 2.f;
	tTrackDesc.fSizeY = m_tUIDesc.fSizeY - 2.f;
	tTrackDesc.fX = m_tUIDesc.fX - ((m_tUIDesc.fSizeX / 2.f));
	tTrackDesc.fY = m_tUIDesc.fY;
	tTrackDesc.fDepth = 0.f;

	m_pGreenTrack = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tTrackDesc);
	m_pGreenTrack.lock()->Set_Texture("Monster_HPBar_GreenTrack");
	m_pGreenTrack.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

	tTrackDesc = m_tUIDesc;

	UI_DESC tRecoveryDesc = m_tUIDesc;
	tRecoveryDesc.fSizeY = m_tUIDesc.fSizeY + 6.f;
	tRecoveryDesc.fSizeX = m_tUIDesc.fSizeX + 10.f;
	tRecoveryDesc.fDepth = 0.0f;

	m_pRecovery = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tRecoveryDesc);
	m_pRecovery.lock()->Set_Texture("Monster_HPBar_RecoveryShine");
	m_pRecovery.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);


	UI_DESC tStunDesc = m_tUIDesc;
	tStunDesc.fSizeY = m_tUIDesc.fSizeY + 8.f;
	tStunDesc.fSizeX = m_tUIDesc.fSizeX + 20.f;
	tStunDesc.fDepth = 0.1f;

	m_pStunned = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tRecoveryDesc);
	m_pStunned.lock()->Set_Texture("Monster_HPBar_StunnedShine");
	m_pStunned.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);


	m_bStun = false;

	m_vOffset = _float3(0.f, 0.f, 0.f);

	UI_DESC	tParryingDesc = m_tUIDesc;

	tParryingDesc.fSizeY = 10.f;
	tParryingDesc.fX = m_tUIDesc.fX;
	tParryingDesc.fY = m_tUIDesc.fY+ 18.f;
	tParryingDesc.fDepth = 0.f;

	m_pParryingBar = GAMEINSTANCE->Add_GameObject<CMonsterParryingBar>(LEVEL_STATIC, &tParryingDesc);


	m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

	Add_Child(m_pBorder);
	Add_Child(m_pWhite);
	Add_Child(m_pGreen);
	Add_Child(m_pTrack);
	Add_Child(m_pDecoration_Head);
	Add_Child(m_pParryingBar);
	Add_Child(m_pGreenTrack);
	Add_Child(m_pRecovery);
	Add_Child(m_pStunned);


	Set_Enable(false);

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


	Set_ChildPosFromThis();
	Check_Track();

	FollowTarget();


	for (auto& elem : m_vecChildUI)
	{
		elem.lock()->Set_Render(m_bRender);
	}

	if (!Get_Enable())
	{
		m_pTrack.lock()->Set_Enable(false);
		m_pRecovery.lock()->Set_Enable(false);
		m_pStunned.lock()->Set_Enable(false);
		m_pGreenTrack.lock()->Set_Enable(false);
	}
}

void CMonsterHPBar_Base::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CMonsterHPBar_Base::Render(ID3D11DeviceContext* pDeviceContext)
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

void CMonsterHPBar_Base::Set_RecoveryAlram(_bool _bRecovery)
{
	if (!_bRecovery)
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

void CMonsterHPBar_Base::Reset()
{
	Reset_UI();
	Set_Enable(false);
	m_pTrack.lock()->Set_Enable(false);
	m_pGreenTrack.lock()->Set_Enable(false);
	if (m_pStunned.lock())
		m_pStunned.lock()->Set_Enable(false);
	m_pRecovery.lock()->Set_Enable(false);

}

void CMonsterHPBar_Base::Call_UpdateHP(CStatus_Monster::MONSTERDESC tDesc)
{
	_float fWhiteRatio = 0.f;
	_float fGreenRatio = 0.f;

	fWhiteRatio = tDesc.m_fCurrentHP_white / tDesc.m_fMaxHP_white;
	fGreenRatio = tDesc.m_fCurrentHP_Green / tDesc.m_fMaxHP_Green;

	m_pWhite.lock()->Set_Ratio(fWhiteRatio);
	m_pGreen.lock()->Set_Ratio(fGreenRatio);
}

void CMonsterHPBar_Base::Call_Update_ParryGauge(_float _fRatio, _bool bLerp)
{
	Set_Enable(true);
	m_pParryingBar.lock()->Set_Ratio(_fRatio, bLerp);
}

void CMonsterHPBar_Base::Call_Damaged_White(_float _fRatio)
{
	Set_Enable(true);
	m_pWhite.lock()->Set_Ratio(_fRatio);
	Set_RecoveryAlram(false);

}

void CMonsterHPBar_Base::Call_Damaged_Green(_float _fRatio)
{
	Set_Enable(true);
	_float fDamgedRatio;//

	fDamgedRatio = m_pGreen.lock()->Get_Ratio() - _fRatio;//데미지 들어오기전 비율 - 데미지 들어오고 난 후 비율.->데미지.

	Green_Damaged(fDamgedRatio);//그만큼만.

	Set_RecoveryAlram(false);
}

void CMonsterHPBar_Base::Call_Damaged_Parry(_float _fRatio)
{
	Set_Enable(true);
}

void CMonsterHPBar_Base::Call_RecoveryAlram()
{
	if (!m_pRecovery.lock()->Get_Enable() && Get_Enable())
		Set_RecoveryAlram(true);
}

void CMonsterHPBar_Base::Call_Recovery()
{
	if (m_pRecovery.lock()->Get_Enable())
		Set_RecoveryAlram(false);

	weak_ptr<CStatus_Monster> pStatus_Monster;
	pStatus_Monster = Weak_Cast< CStatus_Monster>(m_pTarget.lock()->Get_Status());

	_float fRatio = pStatus_Monster.lock()->Get_WhiteRatio();

	m_pWhite.lock()->Set_Ratio(fRatio);
}

void CMonsterHPBar_Base::Call_Disable()
{
	Set_Enable(false);

}

void CMonsterHPBar_Base::Call_Stun()
{
	Set_Stun(true);
}

void CMonsterHPBar_Base::Call_Restart()
{
	Set_Stun(false);

	weak_ptr<CStatus_Monster> pStatus_Monster;
	pStatus_Monster = Weak_Cast<CStatus_Monster>(m_pTarget.lock()->Get_Status());

	m_pWhite.lock()->Set_Ratio(pStatus_Monster.lock()->Get_WhiteRatio());
	m_pGreen.lock()->Set_Ratio(pStatus_Monster.lock()->Get_GreenRatio());
	m_pParryingBar.lock()->Set_Ratio(0.f, false);
}

void CMonsterHPBar_Base::Call_Full_Recovery()
{
	Reset();
}

void CMonsterHPBar_Base::OnEnable(void* pArg)
{
	if (!m_pTarget.lock())
		return;

	if (false == m_pTarget.lock()->Get_Enable())
		return;

	__super::OnEnable(pArg);

	m_pTrack.lock()->Set_Enable(false);
	m_pRecovery.lock()->Set_Enable(false);
	m_pStunned.lock()->Set_Enable(false);
}

void CMonsterHPBar_Base::OnDisable()
{
	__super::OnDisable();

	m_pTrack.lock()->Set_Enable(false);

	m_pGreenTrack.lock()->Set_Enable(false);

	if (m_pStunned.lock())
		m_pStunned.lock()->Set_Enable(false);

	m_pRecovery.lock()->Set_Enable(false);
	m_pBorder.lock()->Set_Enable(false);

	Set_RecoveryAlram(false);
}

void CMonsterHPBar_Base::FollowTarget()
{
	if (!m_pTarget.lock())
		return;

	_vector vViewPosition;
	_matrix ViewProjMatrix;

	vViewPosition = m_pTarget.lock()->Get_WorldPosition();
	if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vViewPosition))
	{
		m_bRender   = true;
	}
	else
	{
		m_bRender = false;
	}
	vViewPosition += XMVectorSet(0.f,2.f, 0.f, 1.f);
	ViewProjMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW) * GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);
	vViewPosition = XMVector3TransformCoord(vViewPosition, ViewProjMatrix);
	
	/* -1 ~ 1 to 0 ~ ViewPort */
	vViewPosition.m128_f32[0] = (vViewPosition.m128_f32[0] + 1.f) * (_float)g_iWinCX * 0.5f;
	vViewPosition.m128_f32[1] = (-1.f * vViewPosition.m128_f32[1] + 1.f) * (_float)g_iWinCY * 0.5f;

	//_float fHeight = vViewPosition.m128_f32[1] - 200.f;
/*	if (fHeight >= ((_float)g_iWinCY) * 0.4f)
		fHeight = ((_float)g_iWinCY) * 0.4f;*/

	Set_UIPosition(vViewPosition.m128_f32[0], vViewPosition.m128_f32[1]);
	
}

void CMonsterHPBar_Base::Bind_EventFunction(weak_ptr<CStatus_Monster> pStatus_Monster)
{
	pStatus_Monster.lock()->CallBack_UpdateParryGauge += bind
	(
		&CMonsterHPBar_Base::Call_Update_ParryGauge, this,
		placeholders::_1, placeholders::_2
	);

	pStatus_Monster.lock()->CallBack_Damged_White += bind
	(
		&CMonsterHPBar_Base::Call_Damaged_White, this,
		placeholders::_1
	);

	pStatus_Monster.lock()->CallBack_Damged_Green += bind
	(
		&CMonsterHPBar_Base::Call_Damaged_Green, this,
		placeholders::_1
	);

	pStatus_Monster.lock()->CallBack_RecoeoryAlram += bind
	(
		&CMonsterHPBar_Base::Call_RecoveryAlram, this
	);
	pStatus_Monster.lock()->CallBack_RecoeoryStart += bind
	(
		&CMonsterHPBar_Base::Call_Recovery, this
	);
	pStatus_Monster.lock()->CallBack_UI_Disable += bind
	(
		&CMonsterHPBar_Base::Call_Disable, this
	);
	pStatus_Monster.lock()->CallBack_ReStart += bind
	(
		&CMonsterHPBar_Base::Call_Restart, this
	);
	pStatus_Monster.lock()->Callback_Full_Recovery += bind
	(
		&CMonsterHPBar_Base::Call_Full_Recovery, this
	);
	pStatus_Monster.lock()->Callback_UpdateHP += bind
	(
		&CMonsterHPBar_Base::Call_UpdateHP, this, placeholders::_1
	);
}
void	CMonsterHPBar_Base::Reset_UI()
{
	m_pWhite.lock()->Set_Ratio(1.f);
	m_pGreen.lock()->Set_Ratio(1.f);
	m_pParryingBar.lock()->Set_Ratio(0.f, false);

}

void CMonsterHPBar_Base::Set_Target(weak_ptr<CBase> pMonster)
{
	m_pTarget =  Weak_StaticCast<CMonster>(pMonster);

	weak_ptr<CStatus_Monster> pStatus_Monster;

	pStatus_Monster = Weak_StaticCast<CStatus_Monster>(m_pTarget.lock()->Get_Status());

	Bind_EventFunction(pStatus_Monster);
	Reset_UI();
	Set_Enable(false);

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
	if (!Get_Enable())
		return;

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

	m_pParryingBar.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY + 18.f);
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
