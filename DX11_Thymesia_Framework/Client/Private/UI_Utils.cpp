#include "UI_Utils.h"
#include "GameInstance.h"
#include "GameManager.h"

_float2 CUI_Utils::ConvertWorldPosToUIPos(_fvector vWorldPos, _fvector vOffset)
{
	_vector vViewPosition = vWorldPos + vOffset;
	_matrix ViewProjMatrix;

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

_float CUI_Utils::UI_TimeDelta()
{

	return GAMEINSTANCE->Get_DeltaTime() * GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

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

weak_ptr<CUI> CUI_Utils::Get_BaseToUI(weak_ptr<CBase> pBase)
{
	return Weak_StaticCast<CUI>(pBase);
}

_float2 CUI_Utils::Get_BezierCurve(_float2 fStart, _float2 fTarget, _float fCustomWeight)
{
	/*
		���� ������ ���Ѵ�.
		�� ������ �Ÿ��� ������ ���������� ����Ѵ�. �� ���� �������鼭 Ŀ�갡 �ȴ�.
		����ġ�� ���� Ŀ���� ������ ���Ѵ�.

		500,300,  1000,300

		->���� = 750,300(a+b/2)

		->�Ÿ� ->500.f,0.f(abs(a-b))

		�׷��� �ִ� �������� �������� ����ġ�� �����.

		1 - (������ ���� �� ���Ҹ� ���� ������ ����)
		�׷��� �ϸ�
		 �Ÿ���  ����ġ�� 
		 x = 0 (1 - (500 / (500 + 0))
	     y = 1 (1 - (0 / (500 + 0))

		y +  (�ݴ��� ������ ��) * ����ġ * Ŀ���� ����ġ = Ŀ���� ���̳� �ʺ�.

		�� ����� ������ ��?
		Ŀ�긦 ���� �����ϸ� Ƣ��� �����̶�°ǵ�,
		��������� ������ ���� �������� ���� ��ȭ�� ����Ѵٰ� ������.

		Ŀ���� ����ġ�� 0.5���
		x = 0
		y = 500 * 1 * 0.5 = 250.

		(750, 300) + (0,250) = (750,550)

		����ġ�� �ݴ���ָ� -�� ����.


	*/
	_float2 fCurve = { 0.f, 0.f };

	_vector	vStart = XMVectorSet(fStart.x, fStart.y, 0.f, 1.f);
	_vector	vTarget = XMVectorSet(fTarget.x, fTarget.y, 0.f, 1.f);

	_vector vCenter = (vStart + vTarget) / 2.f;
	_vector vDistance = XMVectorAbs(vTarget - vStart);





	return _float2();
}
