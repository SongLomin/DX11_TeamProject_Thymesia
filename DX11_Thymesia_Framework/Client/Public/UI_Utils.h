#pragma once
#include "Client_Defines.h"
#include "Status_Player.h"
#include "Base.h"
#include "UI_EffectBase.h"

BEGIN(Client)

class CUI_Utils : public CBase
{
public:

	static _float				Get_BGMSoundAmount() { return 0.3f; }
	static _float				Get_UISoundAmount() { return 0.4f; }

	static _float2				ConvertWorldPosToUIPos(_fvector	vWorldPos, _fvector vOffset);
	static _float				UI_TimeDelta();
	static _float4&				GET_COLOR(COLOR_PALETTE eColor);

	static weak_ptr<class CUI>	Get_BaseToUI(weak_ptr<CBase> pBase);

	static _float2				Get_BezierCurve(_float2 fStart, _float2 fTarget, _float fCustomWeight);

	static void					Set_SkillIcon(weak_ptr<CUI> pUI, SKILL_NAME eSkillName);

	static ITEM_NAME			ConvertSkillNameToSkillPiece(SKILL_NAME eSkillName);

	static ITEM_NAME			ConvertMonsterTypeToSkillPiece(MONSTERTYPE eMonsterType);
};

END
