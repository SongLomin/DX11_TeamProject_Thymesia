#include "UI_Utils.h"
#include "GameInstance.h"


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
