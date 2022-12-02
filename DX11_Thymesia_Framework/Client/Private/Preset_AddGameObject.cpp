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
	pSword1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::NORSWORDLV1);
	pSword1Talent.lock()->Set_Active(true);

	// Sword2
	weak_ptr<CTalent> pSword2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSword2Talent.lock()->Add_Component<CTalent_Effect_NorSwordLV2>();
	pSword2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::NORSWORDLV2);
	pSword2Talent.lock()->Set_Parent(pSword1Talent);

	// Slash1
	weak_ptr<CTalent> pSlash1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSlash1Talent.lock()->Add_Component<CTalent_Effect_AvoidSlashLV1>();
	pSlash1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::AVOIDSLASHLV1);
	pSlash1Talent.lock()->Set_Parent(pSword2Talent);

	// Slash2
	weak_ptr<CTalent> pSlash2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSlash2Talent.lock()->Add_Component<CTalent_Effect_AvoidSlashLV2>();
	pSlash2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::AVOIDSLASHLV2);
	pSlash2Talent.lock()->Set_Parent(pSlash1Talent);

	// Thrust1
	weak_ptr<CTalent> pThrust1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pThrust1Talent.lock()->Add_Component<CTalent_Effect_AvoidThrustLV1>();
	pThrust1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::AVOIDTHRUSTLV1);
	pThrust1Talent.lock()->Set_Parent(pSword2Talent);

	// Thrust2
	weak_ptr<CTalent> pThrust2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pThrust2Talent.lock()->Add_Component<CTalent_Effect_AvoidThrustLV2>();
	pThrust2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::AVOIDTHRUSTLV2);
	pThrust2Talent.lock()->Set_Parent(pThrust1Talent);
	// 검파트 첫번째부분 끝  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	pEvolveMenu_Talent.lock()->SetRootTalent(pSword1Talent, CUI_EveolveMenu_Talent::TALENT_TAP::TALENT_SWORD);



	// 검파트  공중공격@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 	
	// 검공중 부모
	weak_ptr<CTalent> pJumpSword1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pJumpSword1Talent.lock()->Add_Component<CTalent_Effect_JumpAttackLV1>();
	pJumpSword1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::JUMPATTACKLV1);
	pJumpSword1Talent.lock()->Set_Active(true);

	// 검공중 공격2
	weak_ptr<CTalent> pJumpSword2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pJumpSword2Talent.lock()->Add_Component<CTalent_Effect_JumpAttackLV2>();
	pJumpSword2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::JUMPATTACKLV2);
	pJumpSword2Talent.lock()->Set_Parent(pJumpSword1Talent);

	// 검공중 공격3
	weak_ptr<CTalent> pJumpSword3Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pJumpSword3Talent.lock()->Add_Component<CTalent_Effect_JumpAttackLV3>();
	pJumpSword3Talent.lock()->Set_TALENT_NAME(TALENT_NAME::JUMPATTACKLV3);
	pJumpSword3Talent.lock()->Set_Parent(pJumpSword2Talent);

	// 검파트  공중공격@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// 검파트 처형

	//처형부모
	weak_ptr<CTalent> pExecutionTalent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pExecutionTalent.lock()->Add_Component<CTalent_Effect_Execution>();
	pExecutionTalent.lock()->Set_TALENT_NAME(TALENT_NAME::EXECUTION);
	pExecutionTalent.lock()->Set_Active(true);

	// 치유의처형1
	weak_ptr<CTalent> pHealingExecution1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pHealingExecution1Talent.lock()->Add_Component<CTalent_Effect_HealingExecutionLV1>();
	pHealingExecution1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::HEALINGEXECUTIONLV1);
	pHealingExecution1Talent.lock()->Set_Parent(pExecutionTalent);

	// 치유의처형2
	weak_ptr<CTalent> pHealingExecution2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pHealingExecution2Talent.lock()->Add_Component<CTalent_Effect_HealingExecutionLV2>();
	pHealingExecution2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::HEALINGEXECUTIONLV2);
	pHealingExecution2Talent.lock()->Set_Parent(pHealingExecution1Talent);

	// 검파트 처형

	// 검파트 날카로운 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// 날카로운 공격1  부모
	weak_ptr<CTalent> pSharpWeapon1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSharpWeapon1Talent.lock()->Add_Component<CTalent_Effect_SharpWeaponLV1>();
	pSharpWeapon1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::SHARPWEAPONLV1);

	// 날카로운 공격 2 
	weak_ptr<CTalent> pSharpWeapon2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pSharpWeapon2Talent.lock()->Add_Component<CTalent_Effect_SharpWeaponLV2>();
	pSharpWeapon2Talent.lock()->Set_Parent(pSharpWeapon1Talent);
	pSharpWeapon2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::SHARPWEAPONLV2);
 
	// 검파트 날카로운 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// 검파트 에너지가깃든 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
	//에너지가깃든공격1  부모
	weak_ptr<CTalent> pEnergisedWeapon1Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pEnergisedWeapon1Talent.lock()->Add_Component<CTalent_Effect_EnergisedWeaponLV1>();
	pEnergisedWeapon1Talent.lock()->Set_TALENT_NAME(TALENT_NAME::ENERGISEDWEAPONLV1);

	//에너지가깃든공격2 
	weak_ptr<CTalent> pEnergisedWeapon2Talent = GAMEINSTANCE->Add_GameObject<CTalent>(LEVEL_STATIC);
	pEnergisedWeapon2Talent.lock()->Add_Component<CTalent_Effect_EnergisedWeaponLV2>();
	pEnergisedWeapon2Talent.lock()->Set_TALENT_NAME(TALENT_NAME::ENERGISEDWEAPONLV2);
	pEnergisedWeapon2Talent.lock()->Set_Parent(pEnergisedWeapon1Talent);
	// 검파트 에너지가깃든 공격 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



	//pSlash2Talent.lock()->TestTalentCheck();
}
