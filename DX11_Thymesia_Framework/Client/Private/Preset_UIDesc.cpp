#include "stdafx.h"
#include "Preset_UIDesc.h"
#include "UI_MonsterFocus.h"

void CPreset_UIDesc::Set_CUI_MonsterFocus(weak_ptr<class CUI_MonsterFocus> pUI)
{
	pUI.lock()->Set_UIPosition
	(
		0.f,0.f,100.f,100.f
	);
	pUI.lock()->Set_Depth(0.f);
	pUI.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);
	
	pUI.lock()->Set_Texture("Target_Icon");

}
