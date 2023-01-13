#include "stdafx.h"
#include "Preset_AddGameObject.h"
#include "CorvusStates/Talent.h"
#include "Talent_Effects.h"
#include "UI_EvolveMenu_Talent.h"


void Preset::AddGameObject::TalentSetting()
{

	weak_ptr<CUI_EveolveMenu_Talent> pEvolveMenu_Talent = GAMEINSTANCE->Add_GameObject<CUI_EveolveMenu_Talent>(LEVEL_STATIC);

	// 검파트 첫번째부분  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	weak_ptr<CTalent> pSword1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSword1Talent.lock()->Add_Component<CTalent_Effect_NorSwordLV1>();
	pSword1Talent.lock()->Set_TalentName(TALENT_NAME::NORSWORDLV1);
	pSword1Talent.lock()->Set_Active(true);
	pSword1Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pSword1Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);


	// Sword2
	weak_ptr<CTalent> pSword2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSword2Talent.lock()->Add_Component<CTalent_Effect_NorSwordLV2>();
	pSword2Talent.lock()->Set_TalentName(TALENT_NAME::NORSWORDLV2);
	pSword2Talent.lock()->Set_Parent(pSword1Talent);
	
	pSword2Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pSword2Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);


	// Slash1
	weak_ptr<CTalent> pSlash1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSlash1Talent.lock()->Add_Component<CTalent_Effect_AvoidSlashLV1>();
	pSlash1Talent.lock()->Set_TalentName(TALENT_NAME::AVOIDSLASHLV1);
	pSlash1Talent.lock()->Set_Parent(pSword2Talent);

	pSlash1Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pSlash1Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);


	// Slash2
	weak_ptr<CTalent> pSlash2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSlash2Talent.lock()->Add_Component<CTalent_Effect_AvoidSlashLV2>();
	pSlash2Talent.lock()->Set_TalentName(TALENT_NAME::AVOIDSLASHLV2);
	pSlash2Talent.lock()->Set_Parent(pSlash1Talent);


	pSlash2Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pSlash2Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);


	// Thrust1
	weak_ptr<CTalent> pThrust1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pThrust1Talent.lock()->Add_Component<CTalent_Effect_AvoidThrustLV1>();
	pThrust1Talent.lock()->Set_TalentName(TALENT_NAME::AVOIDTHRUSTLV1);
	pThrust1Talent.lock()->Set_Parent(pSword2Talent);


	pThrust1Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pThrust1Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);


	// Thrust2
	weak_ptr<CTalent> pThrust2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pThrust2Talent.lock()->Add_Component<CTalent_Effect_AvoidThrustLV2>();
	pThrust2Talent.lock()->Set_TalentName(TALENT_NAME::AVOIDTHRUSTLV2);
	pThrust2Talent.lock()->Set_Parent(pThrust1Talent);
	// 검파트 첫번째부분 끝  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	pThrust2Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pThrust2Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);

	
	
	pEvolveMenu_Talent.lock()->SetRootTalent(pSword1Talent, CUI_EveolveMenu_Talent::TALENT_TAP::TALENT_SWORD);

	// 검파트  공중공격@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 	
	// 검공중 부모
	weak_ptr<CTalent> pJumpSword1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pJumpSword1Talent.lock()->Add_Component<CTalent_Effect_JumpAttackLV1>();
	pJumpSword1Talent.lock()->Set_TalentName(TALENT_NAME::JUMPATTACKLV1);
	pJumpSword1Talent.lock()->Set_Active(true);

	// 검공중 공격2
	weak_ptr<CTalent> pJumpSword2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pJumpSword2Talent.lock()->Add_Component<CTalent_Effect_JumpAttackLV2>();
	pJumpSword2Talent.lock()->Set_TalentName(TALENT_NAME::JUMPATTACKLV2);
	pJumpSword2Talent.lock()->Set_Parent(pJumpSword1Talent);

	// 검공중 공격3
	weak_ptr<CTalent> pJumpSword3Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pJumpSword3Talent.lock()->Add_Component<CTalent_Effect_JumpAttackLV3>();
	pJumpSword3Talent.lock()->Set_TalentName(TALENT_NAME::JUMPATTACKLV3);
	pJumpSword3Talent.lock()->Set_Parent(pJumpSword2Talent);

	// 검파트  공중공격@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// 검파트 처형

	//처형부모
	weak_ptr<CTalent> pExecutionTalent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pExecutionTalent.lock()->Add_Component<CTalent_Effect_Execution>();
	pExecutionTalent.lock()->Set_TalentName(TALENT_NAME::EXECUTION);
	pExecutionTalent.lock()->Set_Active(true);
	pExecutionTalent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pExecutionTalent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);

	// 치유의처형1
	weak_ptr<CTalent> pHealingExecution1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pHealingExecution1Talent.lock()->Add_Component<CTalent_Effect_HealingExecutionLV1>();
	pHealingExecution1Talent.lock()->Set_TalentName(TALENT_NAME::HEALINGEXECUTIONLV1);
	pHealingExecution1Talent.lock()->Set_Parent(pExecutionTalent);
	pHealingExecution1Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pHealingExecution1Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);

	// 치유의처형2
	weak_ptr<CTalent> pHealingExecution2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pHealingExecution2Talent.lock()->Add_Component<CTalent_Effect_HealingExecutionLV2>();
	pHealingExecution2Talent.lock()->Set_TalentName(TALENT_NAME::HEALINGEXECUTIONLV2);
	pHealingExecution2Talent.lock()->Set_Parent(pHealingExecution1Talent);
	pHealingExecution2Talent.lock()->Callback_OnMouseOver += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOver, pEvolveMenu_Talent.lock(), placeholders::_1);
	pHealingExecution2Talent.lock()->Callback_OnMouseOut += bind(&CUI_EveolveMenu_Talent::Call_TalentMouseOut, pEvolveMenu_Talent.lock(), placeholders::_1);

	pEvolveMenu_Talent.lock()->SetRootTalent(pExecutionTalent, CUI_EveolveMenu_Talent::TALENT_TAP::TALENT_SWORD);


	// 검파트 처형 끝


	// 검파트 날카로운 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// 날카로운 공격1  부모
	weak_ptr<CTalent> pSharpWeapon1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSharpWeapon1Talent.lock()->Add_Component<CTalent_Effect_SharpWeaponLV1>();
	pSharpWeapon1Talent.lock()->Set_TalentName(TALENT_NAME::SHARPWEAPONLV1);

	// 날카로운 공격 2 
	weak_ptr<CTalent> pSharpWeapon2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSharpWeapon2Talent.lock()->Add_Component<CTalent_Effect_SharpWeaponLV2>();
	pSharpWeapon2Talent.lock()->Set_Parent(pSharpWeapon1Talent);
	pSharpWeapon2Talent.lock()->Set_TalentName(TALENT_NAME::SHARPWEAPONLV2);
 
	// 검파트 날카로운 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// 검파트 에너지가깃든 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	//에너지가깃든공격1  부모
	weak_ptr<CTalent> pEnergisedWeapon1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pEnergisedWeapon1Talent.lock()->Add_Component<CTalent_Effect_EnergisedWeaponLV1>();
	pEnergisedWeapon1Talent.lock()->Set_TalentName(TALENT_NAME::ENERGISEDWEAPONLV1);

	//에너지가깃든공격2 
	weak_ptr<CTalent> pEnergisedWeapon2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pEnergisedWeapon2Talent.lock()->Add_Component<CTalent_Effect_EnergisedWeaponLV2>();
	pEnergisedWeapon2Talent.lock()->Set_TalentName(TALENT_NAME::ENERGISEDWEAPONLV2);
	pEnergisedWeapon2Talent.lock()->Set_Parent(pEnergisedWeapon1Talent);
	// 검파트 에너지가깃든 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


	//패링

	//패링부모
	weak_ptr<CTalent> pParring1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pParring1Talent.lock()->Add_Component<CTalent_Effect_ParringLV1>();
	pParring1Talent.lock()->Set_TalentName(TALENT_NAME::PARRING_LV1);

	//패링자식1
	weak_ptr<CTalent> pParring2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pParring2Talent.lock()->Add_Component<CTalent_Effect_ParringLV2>();
	pParring2Talent.lock()->Set_TalentName(TALENT_NAME::PARRING_LV2);
	pParring2Talent.lock()->Set_Parent(pParring1Talent);

	//패링자식2
	weak_ptr<CTalent> pParring3Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pParring3Talent.lock()->Add_Component<CTalent_Effect_Reckless_ParringLV1>();
	pParring3Talent.lock()->Set_TalentName(TALENT_NAME::RECKLESS_PARRING_LV1);
	pParring3Talent.lock()->Set_Parent(pParring2Talent);
	 
	//패링자식3
	weak_ptr<CTalent> pParring4Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pParring4Talent.lock()->Add_Component<CTalent_Effect_Reckless_ParringLV2>();
	pParring4Talent.lock()->Set_TalentName(TALENT_NAME::RECKLESS_PARRING_LV2);
	pParring4Talent.lock()->Set_Parent(pParring3Talent);
	//패링


	//회피
	// 
	//회피부모
	weak_ptr<CTalent> pAvoid1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAvoid1Talent.lock()->Add_Component<CTalent_Effect_AvoidLv1>();
	pAvoid1Talent.lock()->Set_TalentName(TALENT_NAME::AVOID_LV1);

	//롱회피자식1
	weak_ptr<CTalent> pAvoidLong1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAvoidLong1Talent.lock()->Add_Component<CTalent_Effect_Long_AvoidLv1>();
	pAvoidLong1Talent.lock()->Set_TalentName(TALENT_NAME::LONG_AVOID_LV1);
	pAvoidLong1Talent.lock()->Set_Parent(pAvoid1Talent);

	//롱회피자식2
	weak_ptr<CTalent> pAvoidLong2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAvoidLong2Talent.lock()->Add_Component<CTalent_Effect_Long_AvoidLv2>();
	pAvoidLong2Talent.lock()->Set_TalentName(TALENT_NAME::LONG_AVOID_LV2);
	pAvoidLong2Talent.lock()->Set_Parent(pAvoidLong1Talent);

	//숏회피자식1
	weak_ptr<CTalent> pAvoidShort1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAvoidShort1Talent.lock()->Add_Component<CTalent_Effect_Short_AvoidLv1>();
	pAvoidShort1Talent.lock()->Set_TalentName(TALENT_NAME::SHORT_AVOID_LV1);
	pAvoidShort1Talent.lock()->Set_Parent(pAvoid1Talent);

	//숏회피자식2
	weak_ptr<CTalent> pAvoidShort2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAvoidShort2Talent.lock()->Add_Component<CTalent_Effect_Short_AvoidLv2>();
	pAvoidShort2Talent.lock()->Set_TalentName(TALENT_NAME::SHORT_AVOID_LV2);
	pAvoidShort2Talent.lock()->Set_Parent(pAvoidShort1Talent);

	//회피

	//클로우

	//클로우부모
	weak_ptr<CTalent> pClawTalent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pClawTalent.lock()->Add_Component<CTalent_Effect_Long_ClawLv1>();
	pClawTalent.lock()->Set_TalentName(TALENT_NAME::CLAW_LV1);

	//롱클로우자식1
	weak_ptr<CTalent> pLongClaw1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pLongClaw1Talent.lock()->Add_Component<CTalent_Effect_Long_ClawLv2>();
	pLongClaw1Talent.lock()->Set_TalentName(TALENT_NAME::LONG_CLAW_LV1);
	pLongClaw1Talent.lock()->Set_Parent(pClawTalent);

	//롱클로우 자식2
	weak_ptr<CTalent> pLongClaw2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pLongClaw2Talent.lock()->Add_Component<CTalent_Effect_Long_ClawLv3>();
	pLongClaw2Talent.lock()->Set_TalentName(TALENT_NAME::LONG_CLAW_LV2);
	pLongClaw2Talent.lock()->Set_Parent(pLongClaw1Talent);
	//숏클로우 자식1
	weak_ptr<CTalent> pShortClaw1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pShortClaw1Talent.lock()->Add_Component<CTalent_Effect_Short_ClawLv1>();
	pShortClaw1Talent.lock()->Set_TalentName(TALENT_NAME::SHORT_CLAW_LV1);
	pShortClaw1Talent.lock()->Set_Parent(pClawTalent);

	//숏클로우 자식2
	weak_ptr<CTalent> pShortClaw2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pShortClaw2Talent.lock()->Add_Component<CTalent_Effect_Short_ClawLv2>();
	pShortClaw2Talent.lock()->Set_TalentName(TALENT_NAME::SHORT_CLAW_LV2);
	pShortClaw2Talent.lock()->Set_Parent(pShortClaw1Talent);

	//클로우


	// 포식자의발톱

	//포식자의발톱부모
	weak_ptr<CTalent> pPreadator1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pPreadator1Talent.lock()->Add_Component<CTalent_Effect_PredatorLv1>();
	pPreadator1Talent.lock()->Set_TalentName(TALENT_NAME::PREADAOTR_LV1);

	//포식자의발톱자식
	weak_ptr<CTalent> pPreadator2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pPreadator2Talent.lock()->Add_Component<CTalent_Effect_PredatorLv2>();
	pPreadator2Talent.lock()->Set_TalentName(TALENT_NAME::PREADAOTR_LV2);
	pPreadator2Talent.lock()->Set_Parent(pPreadator1Talent);

	// 포식자의발톱


	//깃털날리기

	//깃털날리기부모
	weak_ptr<CTalent> pFeatherFlying1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pFeatherFlying1Talent.lock()->Add_Component<CTalent_Effect_Feadther_FlyingLV1>();
	pFeatherFlying1Talent.lock()->Set_TalentName(TALENT_NAME::FEATHER_FLYING_LV1);


	//깃털날라기자식1
	weak_ptr<CTalent> pFeatherFlying2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pFeatherFlying2Talent.lock()->Add_Component<CTalent_Effect_Feadther_FlyingLV2>();
	pFeatherFlying2Talent.lock()->Set_TalentName(TALENT_NAME::FEATHER_FLYING_LV2);
	pFeatherFlying2Talent.lock()->Set_Parent(pFeatherFlying1Talent);
	//깃털날리기자식2
	weak_ptr<CTalent> pFeatherFlying3Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pFeatherFlying3Talent.lock()->Add_Component<CTalent_Effect_Feadther_FlyingLV3>();
	pFeatherFlying3Talent.lock()->Set_TalentName(TALENT_NAME::FEATHER_FLYING_LV3);
	pFeatherFlying3Talent.lock()->Set_Parent(pFeatherFlying2Talent);
	//깃털날리기자식3
	weak_ptr<CTalent> pFeatherFlying4Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pFeatherFlying4Talent.lock()->Add_Component<CTalent_Effect_Feadther_FlyingLV4>();
	pFeatherFlying4Talent.lock()->Set_TalentName(TALENT_NAME::FEATHER_FLYING_LV4);
	pFeatherFlying4Talent.lock()->Set_Parent(pFeatherFlying3Talent);
	//깃털날리기


	//풍성한깃털

	//풍성한깃털부모
	weak_ptr<CTalent> pAbundant_Feather1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAbundant_Feather1Talent.lock()->Add_Component<CTalent_Effect_Abundant_FlyingLV1>();
	pAbundant_Feather1Talent.lock()->Set_TalentName(TALENT_NAME::ABUNDANT_FEATHER_LV1);

	//풍성한깃털자식1
	weak_ptr<CTalent> pAbundant_Feather2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAbundant_Feather2Talent.lock()->Add_Component<CTalent_Effect_Abundant_FlyingLV2>();
	pAbundant_Feather2Talent.lock()->Set_TalentName(TALENT_NAME::ABUNDANT_FEATHER_LV2);
	pAbundant_Feather2Talent.lock()->Set_Parent(pAbundant_Feather1Talent);
	//풍성한깃털자식2
	weak_ptr<CTalent> pAbundant_Feather3Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pAbundant_Feather3Talent.lock()->Add_Component<CTalent_Effect_Abundant_FlyingLV3>();
	pAbundant_Feather3Talent.lock()->Set_TalentName(TALENT_NAME::ABUNDANT_FEATHER_LV3);
	pAbundant_Feather3Talent.lock()->Set_Parent(pAbundant_Feather2Talent);

	//풍성한깃털


	//자라나는깃털

	//자라나는깃털부모
	weak_ptr<CTalent> pGrowing_Feather1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pGrowing_Feather1Talent.lock()->Add_Component<CTalent_Effect_Growing_FlyingLV1>();
	pGrowing_Feather1Talent.lock()->Set_TalentName(TALENT_NAME::GROWING_FEATHER_LV1);

	//자라나는깃털자식1
	weak_ptr<CTalent> pGrowing_Feather2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pGrowing_Feather2Talent.lock()->Add_Component<CTalent_Effect_Growing_FlyingLV2>();
	pGrowing_Feather2Talent.lock()->Set_TalentName(TALENT_NAME::GROWING_FEATHER_LV2);
	pGrowing_Feather2Talent.lock()->Set_Parent(pGrowing_Feather1Talent);
	//자라나는깃털자식2
	weak_ptr<CTalent> pGrowing_Feather3Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pGrowing_Feather3Talent.lock()->Add_Component<CTalent_Effect_Growing_FlyingLV3>();
	pGrowing_Feather3Talent.lock()->Set_TalentName(TALENT_NAME::GROWING_FEATHER_LV3);
	pGrowing_Feather3Talent.lock()->Set_Parent(pGrowing_Feather2Talent);
	//자라나는깃털

	//pSlash2Talent.lock()->TestTalentCheck();
}
