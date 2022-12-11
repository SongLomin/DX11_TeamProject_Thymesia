#include "UI_Utils.h"
#include "GameInstance.h"
#include "GameManager.h"

_float2 CUI_Utils::ConvertWorldPosToUIPos(_fvector vWorldPos)
{
	_vector vViewPosition = vWorldPos;
	_matrix ViewProjMatrix;

	vViewPosition += XMVectorSet(0.f, 1.f, 0.f, 1.f);

	ViewProjMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW) * GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);

	vViewPosition = XMVector3TransformCoord(vViewPosition, ViewProjMatrix);

	/* -1 ~ 1 to 0 ~ ViewPort */
	vViewPosition.m128_f32[0] = (vViewPosition.m128_f32[0] + 1.f) * (_float)g_iWinCX * 0.5f;
	vViewPosition.m128_f32[1] = (-1.f * vViewPosition.m128_f32[1] + 1.f) * (_float)g_iWinCY * 0.5f;

	
	_float2 vUIPos;

	vUIPos.x = vViewPosition.m128_f32[0];
	vUIPos.y = vViewPosition.m128_f32[1];


    return vUIPos;
}

_float CUI_Utils::UI_TimeDelta(_float fTimeDelta)
{
	return fTimeDelta * GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);
}

_float4& CUI_Utils::GET_COLOR(COLOR_PALETTE eColor)
{
	_float4		fColor;

	ZeroMemory(&fColor, sizeof(_float4));
	
	switch (eColor)
	{
	case Client::COLOR_PALETTE::COLOR_WHITE:
		fColor = { 1.f,1.f,1.f,1.f };
		break;
	case Client::COLOR_PALETTE::COLOR_BLACK:
		fColor = { 0.f,0.f,0.f,1.f };
		break;
	case Client::COLOR_PALETTE::COLOR_RED:
		fColor = { 1.f,0.f,0.f,1.f };
		break;
	case Client::COLOR_PALETTE::COLOR_GREEN:
		fColor = { 0.f,1.f,0.f,1.f };
		break;
	case Client::COLOR_PALETTE::COLOR_BLUE:
		fColor = { 0.f,0.f,1.f,1.f };
		break;
	default:
		break;
	}
	return fColor;
}
