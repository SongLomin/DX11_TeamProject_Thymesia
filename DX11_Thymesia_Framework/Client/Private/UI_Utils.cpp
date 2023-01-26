#include "stdafx.h"
#include "UI_Utils.h"
#include "GameInstance.h"
#include "GameManager.h"
#include "UI.h"


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
		둘의 중점을 구한다.
		둘 사이의 거리의 절반을 오프셋으로 사용한다. 이 값이 더해지면서 커브가 된다.
		가중치를 곱해 커브의 방향을 정한다.

		500,300,  1000,300

		->중점 = 750,300(a+b/2)

		->거리 ->500.f,0.f(abs(a-b))

		그러면 최대 비율에서 가져야할 가중치를 계산함.

		1 - (원소의 값을 두 원소를 더한 값으로 나눔)
		그렇게 하면
		 거리의  가중치는 
		 x = 0 (1 - (500 / (500 + 0))
	     y = 1 (1 - (0 / (500 + 0))

		y +  (반대쪽 원소의 값) * 가중치 * 커스텀 가중치 = 커브의 높이나 너비.

		왜 계산을 저렇게 함?
		커브를 굳이 생각하면 튀어나온 선분이라는건데,
		상대적으로 평평한 쪽을 기준으로 값에 변화를 줘야한다고 생각함.
		
		같은 원소의 값을 줘봤자, 직선 선분은 더 길어지고, 곡선은 생기지 않음

		커스텀 가중치가 0.5라면
		x = 0(거리.y) * 0(가중치.x) * 0.5 = 0
		y = 500(거리.x) * 1(가중치.y) * 0.5 = 250.

		(750, 300) + (0,250) = (750,550)

		가중치를 반대로주면 -가 나옴.


	*/
	_float2 fCurve = { 0.f, 0.f };

	_vector	vStart = XMVectorSet(fStart.x, fStart.y, 0.f, 1.f);
	_vector	vTarget = XMVectorSet(fTarget.x, fTarget.y, 0.f, 1.f);

	_vector vCenter = (vStart + vTarget) / 2.f;
	_vector vDistance = XMVectorAbs(vTarget - vStart);

	_float2	fWeight;

	fWeight.x = 1.f - (vDistance.m128_f32[0] / (vDistance.m128_f32[0] + vDistance.m128_f32[1]));
	fWeight.y = 1.f - (vDistance.m128_f32[1] / (vDistance.m128_f32[0] + vDistance.m128_f32[1]));

	_vector vCurve;
	_vector vWeight;

	vWeight.m128_f32[0] = vDistance.m128_f32[1] * fWeight.x * fCustomWeight;
	vWeight.m128_f32[1] = vDistance.m128_f32[0] * fWeight.y * fCustomWeight;

	vCurve = vCenter + vWeight;

	fCurve = { vCurve.m128_f32[0],vCurve.m128_f32[1] };

	return fCurve;
}

void CUI_Utils::Set_SkillIcon(weak_ptr<CUI> pUI, SKILL_NAME eSkillName)
{
	switch (eSkillName)
	{
	case Client::SKILL_NAME::SKILL_VARGSWORD:
		pUI.lock()->Set_Texture("SkillIcon_VargSword");
		break;
	case Client::SKILL_NAME::SKILL_AXE:
		pUI.lock()->Set_Texture("SkillIcon_Axe");
		break;
	case Client::SKILL_NAME::SKILL_KNIFE:
		pUI.lock()->Set_Texture("SkillIcon_Knife");
		break;
	case Client::SKILL_NAME::SKILL_HAMMER:
		pUI.lock()->Set_Texture("SkillIcon_Hammer");
		break;
	case Client::SKILL_NAME::SKILL_SCYTHE:
		pUI.lock()->Set_Texture("SkillIcon_Scythe");
		break;
	case Client::SKILL_NAME::SKILL_BLOODSTORM:
		pUI.lock()->Set_Texture("SkillIcon_BloodStorm");
		break;
	case Client::SKILL_NAME::SKILL_HALBERDS:
		pUI.lock()->Set_Texture("SkillIcon_Halberd");
		break;
	case Client::SKILL_NAME::SKILL_BIGHAND:
		pUI.lock()->Set_Texture("SkillIcon_BigHand");
		break;
	case Client::SKILL_NAME::SKILL_BANKAI:
		pUI.lock()->Set_Texture("SkillIcon_Bankai");
		break;
	case Client::SKILL_NAME::SKILL_END:
		pUI.lock()->Set_Texture("None");
		break;
	default:
		pUI.lock()->Set_Texture("None");
		break;
	}
}

ITEM_NAME CUI_Utils::ConvertSkillNameToSkillPiece(SKILL_NAME eSkillName)
{
	ITEM_NAME eItemName = ITEM_NAME::ITEM_NAME_END;

	switch (eSkillName)
	{
	case Client::SKILL_NAME::SKILL_AXE:
		eItemName = ITEM_NAME::SKILLPIECE_AXE;
		break;
	case Client::SKILL_NAME::SKILL_KNIFE:
		eItemName = ITEM_NAME::SKILLPIECE_KNIFE;
		break;
	case Client::SKILL_NAME::SKILL_HAMMER:
		eItemName = ITEM_NAME::SKILLPIECE_HAMMER;
		break;
	case Client::SKILL_NAME::SKILL_SCYTHE:
		eItemName = ITEM_NAME::SKILLPIECE_SCYTHE;
		break;
	case Client::SKILL_NAME::SKILL_VARGSWORD:
		eItemName = ITEM_NAME::SKILLPIECE_VARGSWORD;
		break;
	case Client::SKILL_NAME::SKILL_BLOODSTORM:
		eItemName = ITEM_NAME::SKILLPIECE_BLOODSTORM;
		break;
	case Client::SKILL_NAME::SKILL_HALBERDS:
		eItemName = ITEM_NAME::SKILLPIECE_HALBERDS;
		break;
	case Client::SKILL_NAME::SKILL_BIGHAND:
		eItemName = ITEM_NAME::SKILLPIECE_BIGHAND;
		break;
	case Client::SKILL_NAME::SKILL_END:
		break;
	default:
		break;
	}
	return eItemName;
}

ITEM_NAME CUI_Utils::ConvertMonsterTypeToSkillPiece(MONSTERTYPE eMonsterType)
{
	ITEM_NAME	 eItemName = ITEM_NAME::ITEM_NAME_END;

	switch (eMonsterType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		eItemName = ITEM_NAME::SKILLPIECE_AXE;
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		eItemName = ITEM_NAME::SKILLPIECE_KNIFE;
		break;
	case Client::MONSTERTYPE::SKULL:
		eItemName = ITEM_NAME::SKILLPIECE_KNIFE;
		break;
	case Client::MONSTERTYPE::GARDENER:
		eItemName = ITEM_NAME::SKILLPIECE_SCYTHE;
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		eItemName = ITEM_NAME::SKILLPIECE_AXE;
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		eItemName = ITEM_NAME::SKILLPIECE_SCYTHE;		
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		eItemName = ITEM_NAME::SKILLPIECE_KNIFE;
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		eItemName = ITEM_NAME::SKILLPIECE_HALBERDS;
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		eItemName = ITEM_NAME::SKILLPIECE_KNIFE;
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		eItemName = ITEM_NAME::SKILLPIECE_HALBERDS;
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		eItemName = ITEM_NAME::SKILLPIECE_KNIFE;
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		eItemName = ITEM_NAME::SKILLPIECE_HALBERDS;
		break;
	case Client::MONSTERTYPE::BIGHANDMAN:
		eItemName = ITEM_NAME::SKILLPIECE_BIGHAND;
		break;
	case Client::MONSTERTYPE::JOKER:
		eItemName = ITEM_NAME::SKILLPIECE_HAMMER;
		break;
	default:
		break;
	}


	return eItemName;
}

