#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "Json/Json_Utility.h"
#include "GameManager.h"
#include "Player_HPBar.h"
#include "Player_MPBar.h"
#include "Player_Memory.h"
#include "Player.h"
#include "Player_PotionUI.h"
#include "Player_FeatherUI.h"
#include "UI_Landing.h"
#include "CustomUI.h"
#include "UI_Logo.h"
#include "UI_Loading.h"

CLoader::CLoader()
	//: m_pDevice(pDevice), m_pContext(pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{
	
}

CLoader::~CLoader()
{
	Free();
}

unsigned int APIENTRY LoadingMain(void* pArg)
//void LoadingMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;
	CRITICAL_SECTION pCS = pLoader->Get_CS();
	EnterCriticalSection(&pCS);
	HRESULT hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		hr = pLoader->Loading_ForLogoLevel();
		break;

	case LEVEL_LOBBY:
		hr = pLoader->Loading_ForLobby();
		break;

	case LEVEL_GAMEPLAY:
		hr = pLoader->Loading_ForGamePlayLevel();
		break;

	case LEVEL_STAGE2:
		hr = pLoader->Loading_ForStage2Level();
		break;

	case LEVEL_STAGE3:
		hr = pLoader->Loading_ForStage3Level();
		break;

	case LEVEL_EDIT:
		hr = pLoader->Loading_ForEditLevel();
		break;
	}	

	if (FAILED(hr))
		MSG_BOX("Failed to Loading");

	CRITICAL_SECTION pCSLeave = pLoader->Get_CS();
	LeaveCriticalSection(&pCSLeave);

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	m_eNextLevel = eNextLevel;

	//std::future<void> a = std::async(std::launch::async, LoadingMain, this);

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	GAMEINSTANCE->Add_Font((_uint)FONT_INDEX::DREAM, TEXT("../Bin/Resources/Fonts/128.spriteFont"));
	GAMEINSTANCE->Add_Font((_uint)FONT_INDEX::PRETENDARD, TEXT("../Bin/Resources/Fonts/Pretendard.spriteFont"));

	GAMEINSTANCE->Add_Prototype_GameObject<CFadeMask>();
	GAMEINSTANCE->Add_Prototype_GameObject<CCamera_Target>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStage1>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStage2>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStage3>();
	GAMEINSTANCE->Add_Prototype_GameObject<CCorvus>();
	GAMEINSTANCE->Add_Prototype_GameObject<CNorMonster>();
	GAMEINSTANCE->Add_Prototype_GameObject<CTerrain>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStatic_Prop>();
	GAMEINSTANCE->Add_Prototype_GameObject<CLight_Prop>();
	GAMEINSTANCE->Add_Prototype_GameObject<CGround>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStatic_Instancing_Prop>();

#pragma endregion

#pragma region TEXTURE

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	GAMEINSTANCE->Load_Textures(("Default"), TEXT("../Bin/Resources/Textures/Default%d.jpg"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("Background"), TEXT("../Bin/Resources/Textures/Background/BgFightLoading%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("ButtonDefault"), TEXT("../Bin/Resources/Textures/UI/ButtonDefault.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("UI_White"), TEXT("../Bin/Resources/Textures/UI/UI_White.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures("Grass", TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures("TestTile", TEXT("../Bin/Resources/Textures/TestTexture/DefaultGrid.dds"), MEMORY_TYPE::MEMORY_STATIC);
	

	GAMEINSTANCE->Load_Textures(("Loading_SafeHouse"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Lobby_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_SeaOfTrees"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Circus_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_RoyalGarden"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Circus_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_HermesFortress"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Garden_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_OceanOfMemories"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_MemoryOcean_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_Tutorial"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Tutorial_01.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("Loading_Icon"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingIcon.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_Complete"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/Loading_Complete.png"), MEMORY_TYPE::MEMORY_STATIC);


	GAMEINSTANCE->Load_Textures(("Loading_Font_Fortress_Title"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/Fortress_Name.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Loading_Font_Fortress_Desc"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/Fortress_Desc.png"), MEMORY_TYPE::MEMORY_STATIC);



	//MainMenu(LogoLevel)
	GAMEINSTANCE->Load_Textures(("MainMenu_Background"), TEXT("../Bin/Resources/Textures/UI/MainMenuBackrgound.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("GameLogo"), TEXT("../Bin/Resources/Textures/UI/GameLogo2.png"), MEMORY_TYPE::MEMORY_DYNAMIC);

	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_1"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SelectableButtonBackground.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_2"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SelectableButtonHighlight.png"), MEMORY_TYPE::MEMORY_DYNAMIC);

	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_ContinueGame"), TEXT("../Bin/Resources/Textures/UI/UI_ContinueGame.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_StartGame"), TEXT("../Bin/Resources/Textures/UI/UI_StartGame.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_LoadGame"), TEXT("../Bin/Resources/Textures/UI/UI_LoadGame.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_Options"), TEXT("../Bin/Resources/Textures/UI/UI_Options.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_Credits"), TEXT("../Bin/Resources/Textures/UI/UI_Credits.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_Quit"), TEXT("../Bin/Resources/Textures/UI/UI_Quit.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Textures(("MainMenu_SelectableButton_Tool"), TEXT("../Bin/Resources/Textures/UI/UI_Tool.png"), MEMORY_TYPE::MEMORY_DYNAMIC);


	//Player HPBar Texture
	GAMEINSTANCE->Load_Textures(("Player_HPBar_Border_Left"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_1Border_Left.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_HPBar_Border_Right"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_1Border_Right.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("Player_HPBar_BG"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_2BG.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_HPBar_MainBar"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_3MainBar.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_HPBar_DelayBar"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_4DelayBar.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_HPBar_Track"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_5Track.png"), MEMORY_TYPE::MEMORY_STATIC);
	//GAMEINSTANCE->Load_Textures(("Player_HPBar_Track_Mana"), TEXT("../Bin/Resources/Textures/UI/Track_Test.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Player_MPBar Texture
	GAMEINSTANCE->Load_Textures(("Player_MPBar_Border_Left"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_1Border_Left.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_MPBar_Border_Right"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_1Border_Right.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_MPBar_BG"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_2BG.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_MPBar_MainBar"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_3MainBar.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Player_Memory
	GAMEINSTANCE->Load_Textures(("Player_Memory_Icon"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_MemoryShardsIcon.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_Memory_BG"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_ContentBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_Memory_BG2"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SquareFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Player_Memory_Decoration"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DecorationLine_05.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Player Skill & ItemUI
	GAMEINSTANCE->Load_Textures(("Player_Item_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);
	
	//Potion Image
	//mini : 40x40
	GAMEINSTANCE->Load_Textures(("Potion_Default_Mini"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);

	//HUD
	GAMEINSTANCE->Load_Textures(("HUD_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_Frame_Hover"), TEXT("../Bin/Resources/Textures/UI/Hover_Rombo_Texture.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("HUD_FrameBorder"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_FrameBorder.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_Potion_Default_Mini"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_DefaultType_Mini.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_Font_BG"), TEXT("../Bin/Resources/Textures/UI/HUD/TexUI_PlagueWeaponBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_Feather"), TEXT("../Bin/Resources/Textures/UI/HUD/TexUI_Feather.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Player HUD PlagueWeapon_Main & PlagueWeapon_sub & PlagueWeapon_Steal
	GAMEINSTANCE->Load_Textures(("HUD_PlagueWeapon_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_DefaultSlotFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_PlagueWeapon_Frame_Ready"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_DefaultSlotFrame_02.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_PlagueWeapon_Frame_Steal"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_PlunderSlotFrame_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("HUD_PlagueWeapon_Frame_Steal_Decoration"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_PlunderSlotFrame_02.png"), MEMORY_TYPE::MEMORY_STATIC);

	//skill Icon
	GAMEINSTANCE->Load_Textures(("SkillIcon_Axe"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_Axe.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Landing
	GAMEINSTANCE->Load_Textures(("Landing_MemoriesRetrived"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_MemoriesRetrived.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_MemoriesRetrivedBG"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_MemoriesRetrivedBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_MemoryInterrupted"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_MemoryInterruptedFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_RecallCompleted"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_RecallCompleted.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_BeconFound"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_SanctumDiscoveredFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_BG"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingScreenBackground.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Landing_Stage
	GAMEINSTANCE->Load_Textures(("Landing_SafeHouse"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheSafeHouse.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_SeaOfTrees"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheCircus.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_RoyalGarden"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheGarden.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_HermesFortress"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheFortress.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Landing_OceanOfMemories"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheMemorySea.png"), MEMORY_TYPE::MEMORY_STATIC);

	//UI Masking Texture
	GAMEINSTANCE->Load_Textures(("CloudNoise"), TEXT("../Bin/Resources/Textures/UI/GoodUIIconTrail/Noise/T_CloudNoise.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Dissolve_1"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test%d.png"), MEMORY_TYPE::MEMORY_STATIC);
//	GAMEINSTANCE->Load_Textures(("Dissolve_2"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test2.png"), MEMORY_TYPE::MEMORY_STATIC);
//	GAMEINSTANCE->Load_Textures(("Dissolve_3"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test3.png"), MEMORY_TYPE::MEMORY_STATIC);
//	GAMEINSTANCE->Load_Textures(("Dissolve_4"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test4.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Dissolve_1"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("MaskColor_Vertical"), TEXT("../Bin/Resources/Textures/UI/W_PlagueWeapon_MaskColor01.png"), MEMORY_TYPE::MEMORY_STATIC);

	//TODO : UI_Menu : ESC키를 눌러서 나오는 메뉴창
	//UI_Menu
	GAMEINSTANCE->Load_Textures(("PauseMenu_Background0"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_00.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PauseMenu_Background1"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PauseMenu_Background2"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_02.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PauseMenu_Background3"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_03.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageIndex_Indicator_Selected"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_PageIndexIndicator_Selected.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageIndex_Indicator_UnSelected"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_PageIndexIndicator_UnSelected.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageIndex_Indicator_Decoration"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_PageIndexIndicator_Decoration.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageIndex_UnderLine"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_UnderLine.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Page_Status
	GAMEINSTANCE->Load_Textures(("Tap_Status_BG"), TEXT("../Bin/Resources/Textures/UI/TexUI_CharacterStatusBackground.png"), MEMORY_TYPE::MEMORY_STATIC);

	//TestFontImage
	GAMEINSTANCE->Load_Textures(("PageFont_Status"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Status.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageFont_Talent"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Talent.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageFont_Collection"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Collection.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageFont_Item"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Item.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageFont_Option"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Option.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("PageFont_Quit"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Quit.png"), MEMORY_TYPE::MEMORY_STATIC);
	

	GAMEINSTANCE->Load_Textures(("Font_Diamond"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DiamondIcon_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("Font_Decoration2"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DecorationLine_02.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("Mask_Horizontal"), TEXT("../Bin/Resources/Textures/UI/Mask./Mask_Horizontal%d.png"), MEMORY_TYPE::MEMORY_STATIC);


	Load_AllDiffuseTexture();

	// TODO : For. Ground Texture : (AN) 임시 텍스쳐이므로 나중에 삭제하기, GroundTexture로 필요한곳에 사용할 예정임
	Load_AllTexture("../Bin/GroundInfo/Texture/", MEMORY_TYPE::MEMORY_STATIC);
	Load_AllTexture("../Bin/GroundInfo/Filter/" , MEMORY_TYPE::MEMORY_STATIC);
	
	
#pragma endregion
	
	
	Load_AllMaskMap();
	Load_AllNoiseTexture();
	Load_AllParticleTexture();

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	// TODO : Turn off temporarily for Light_Prop

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.8f, 0.8f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 0.2f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable = true;

	GAMEINSTANCE->Add_Light(LightDesc);

	// 지형 라이트
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 0.7f);
	LightDesc.vLightFlag = _float4(0.f, 0.f, 1.f, 0.f);
	LightDesc.bEnable = false;

	GAMEINSTANCE->Add_Light(LightDesc);

	// 몬스터 라이트
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 0.7f);
	LightDesc.vLightFlag = _float4(0.f, 1.f, 0.f, 0.f);
	LightDesc.bEnable = false;

	GAMEINSTANCE->Add_Light(LightDesc);
	lstrcpy(m_szLoadingText, TEXT("셰이더를 로딩중입니다. "));

	GAMEINSTANCE->Load_Shader(TEXT("Shader_UI"), TEXT("../Bin/ShaderFiles/Shader_UI.hlsl"));


	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxAnimModel"), TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_HPBar"), TEXT("../Bin/ShaderFiles/Shader_HPBar.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_HyperSpace"), TEXT("../Bin/ShaderFiles/Shader_HyperSpace.hlsl"));
	// 메인앱에서 로딩 중 FaderMask 때문에.
	//GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxColor"), TEXT("../Bin/ShaderFiles/Shader_VtxColor.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxCubeTex"), TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxInstance"), TEXT("../Bin/ShaderFiles/Shader_VtxInstance.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxModel"), TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxNorTex"), TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxPointInstance"), TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxTex"), TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"));

	GAMEINSTANCE->Load_Shader(TEXT("Shader_EffectMesh"), TEXT("../Bin/ShaderFiles/Shader_EffectMesh.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_Cell"), TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_Luxiya"), TEXT("../Bin/ShaderFiles/Shader_Luxiya.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxGround"), TEXT("../Bin/ShaderFiles/Shader_VtxGround.hlsl"));
	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxModelInstance"), TEXT("../Bin/ShaderFiles/Shader_VtxModelInstance.hlsl"));

	lstrcpy(m_szLoadingText, TEXT("데이터를 로딩중입니다. "));
	
	GET_SINGLE(CGameManager)->Load_AllKeyEventFromJson();

	_matrix			TransformMatrix;
	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("Corvus", "../Bin/Resources/Meshes/Corvus/Corvus.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC,true);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("CorvusDefaultSaber", "../Bin/Resources/Meshes/Corvus/Weapon/Corvus_DefaultSaber/Corvus_DefaultSaber.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("CorvusDefaultDagger", "../Bin/Resources/Meshes/Corvus/Weapon/Corvus_DefaultDagger/Corvus_DefaultDagger.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	// 일반몬스터 관련모델
	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.0037f, 0.0037f, 0.0037f);
	GAMEINSTANCE->Load_Model("Mon_AxeMan", "../Bin/Resources/Meshes/NorMonster/AxeMan/SK_C_LV1Villager_M_Skeleton.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("Mon_KnifeWoMan", "../Bin/Resources/Meshes/NorMonster/KnifeWoMan/SK_C_LV1Villager_F_Skeleton.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC, true);
	// 일반몬스터 관련모델

	// 일반몬스터 무기모델
	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(270.0f)) * XMMatrixRotationY(XMConvertToRadians(90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("Mon_Weapon_Axe", "../Bin/Resources/Meshes/NorMonster/Weapon/Axe/Axe.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("Mon_Weapon_Knife", "../Bin/Resources/Meshes/NorMonster/Weapon/Knife/Knife.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
	// 일반몬스터 무기모델
	
	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	GAMEINSTANCE->Load_Model("Torch", "../Bin/Resources/Meshes/LightProp/Torch/Torch.FBX", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
	
	lstrcpy(m_szLoadingText, TEXT("객체 생성 중입니다. "));
	
	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));	

	m_isFinished = true;


	// TODO : 메쉬 생성용 나중에 삭제하시오 ( An )
	Load_AllMeshes("../Bin/Resources/Meshes/Temp/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);

	return S_OK;
}

HRESULT CLoader::Loading_ForLobby()
{
	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	


	//GAMEINSTANCE->Load_Model("Luxiya", "../Bin/Resources/Meshes/Luxiya_Hair/Luxiya_Hair_Cheek_Sword_Normal2.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);




	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{

#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));
	//Loading_ForEffectGroup("../Bin/EffectData/");
	Load_AllEffectMesh();
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_GAMEPLAY);

	Load_AllMeshes("../Bin/Resources/Meshes/ForTest_Mesh/"         , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Else/Binary/"      , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv1_Circus/Binary/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/GroundInfo/Mesh/"                       , MODEL_TYPE::GROUND , MEMORY_TYPE::MEMORY_DYNAMIC);

#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));
	
	
	//GAMEINSTANCE->Load_Textures("Grass", TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	
	
	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	CEditGround::Load_AllMeshInfo();

	lstrcpy(m_szLoadingText, TEXT("객체를 생성 중입니다."));

	//Create_GameObjectFromJson("../Bin/LevelData/Stage1.json", LEVEL_GAMEPLAY);
	

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));



	m_isFinished = true;


	return S_OK;
}

HRESULT CLoader::Loading_ForStage2Level()
{
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));
	Load_AllEffectMesh();
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_STAGE2);
#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));
	_matrix			TransformMatrix;

	lstrcpy(m_szLoadingText, TEXT("객체를 생성 중입니다."));

	//Create_GameObjectFromJson("../Bin/LevelData/Stage1.json", LEVEL_GAMEPLAY);

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForStage3Level()
{
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));
	Load_AllEffectMesh();
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_STAGE3);
#pragma endregion


	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));


	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));
	_matrix			TransformMatrix;

	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	GAMEINSTANCE->Load_Model("Stage1", "../Bin/Resources/Meshes/Stage1/Stage1.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_DYNAMIC);
	TransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
	GAMEINSTANCE->Load_Model("Stage2_Navi", "../Bin/NaviMeshData/Stage2_Navi.bin", MODEL_TYPE::NAVI, TransformMatrix, MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Load_Model("NaviMesh", "../Bin/NaviMeshData/Stage1.bin", MODEL_TYPE::NAVI, TransformMatrix, MEMORY_TYPE::MEMORY_DYNAMIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	GAMEINSTANCE->Load_Model("Monster1", "../Bin/Resources/Meshes/Monster1/Monster1.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	GAMEINSTANCE->Load_Model("FxR3LuxiyaAttack01", "../Bin/Resources/Meshes/EffectMesh/FxR3LuxiyaAttack01/FxR3LuxiyaAttack01.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);


	lstrcpy(m_szLoadingText, TEXT("객체를 생성 중입니다."));

	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForEditLevel()
{
#pragma region PROTOTYPE_GAMEOBJECT

	lstrcpy(m_szLoadingText, TEXT("객체를 생성중입니다."));


#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다. "));

	lstrcpy(m_szLoadingText, TEXT("모델을 로딩중입니다. "));

	Load_AllMeshes("../Bin/Resources/Meshes/ForTest_Mesh/"           , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Else/Binary/"        , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv1_Circus/Binary/"  , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv2_Fortress/Binary/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv3_Garden/Binary/"  , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/GroundInfo/Mesh/"						 , MODEL_TYPE::GROUND , MEMORY_TYPE::MEMORY_DYNAMIC);


	lstrcpy(m_szLoadingText, TEXT("로딩 끝 "));


	m_isFinished = true;


	return S_OK;
}

void CLoader::Loading_ForEffectGroup(const char* In_Path, const _uint& In_LevelIndex)
{

	weak_ptr<CEffectGroup> EffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(In_LevelIndex);
	EffectGroup.lock()->Load_EffectJson(In_Path, (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);
}

void CLoader::Loading_AllEffectGroup(const char* In_FolderPath, const _uint& In_LevelIndex)
{
	fs::directory_iterator itr(In_FolderPath);

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		weak_ptr<CEffectGroup> EffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(In_LevelIndex);
		EffectGroup.lock()->Load_EffectJson(entry.path().string(), (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);

		std::cout << entry.path().filename() << std::endl;


		itr++;
	}
}

void CLoader::Load_AllDiffuseTexture()
{
	GAMEINSTANCE->Load_Textures(("UVColorDiffuse"), TEXT("../Bin/Resources/Textures/Diffuse/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllMaskMap()
{
	GAMEINSTANCE->Load_Textures(("UVMask"), TEXT("../Bin/Resources/Textures/Mask/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllNoiseTexture()
{
	GAMEINSTANCE->Load_Textures(("UVNoise"), TEXT("../Bin/Resources/Textures/Noise/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllParticleTexture()
{
	fs::directory_iterator itr("..\\Bin\\Resources\\Textures\\Effect");
	tstring szPath;
	tstring szFileName;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;
		szFileName = entry.path().filename().wstring();

		std::cout << entry.path().filename() << std::endl;

		Load_AllParticleInPath_Recursive(entry.path());

		itr++;
	}
}

void CLoader::Load_AllParticleInPath_Recursive(const filesystem::path& In_Path)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	tstring szPath;
	string szFileName;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string();

		std::cout << entry.path().filename() << std::endl;

		Load_AllParticleInPath_Recursive(entry.path());

		szPath = entry.path().wstring();
		GAMEINSTANCE->Load_Textures(szFileName.c_str(), szPath.c_str());

		itr++;
	}
}

void CLoader::Load_AllEffectMesh()
{
	fs::directory_iterator itr("..\\Bin\\Resources\\Meshes\\EffectMesh");

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		Load_AllEffectMeshInPath_Recursive(entry.path());

		itr++;
	}
}

void CLoader::Load_AllEffectMeshInPath_Recursive(const filesystem::path& In_Path)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	string szFileName;
	string szExtension;
	fs::directory_iterator itr(In_Path);
	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		Load_AllEffectMeshInPath_Recursive(entry.path());

		szFileName = entry.path().filename().string().c_str();
		szFileName = szFileName.substr(0, szFileName.size() - 4);

		/*if (strcmp(entry.path().extension().string().c_str(), ".bin") == 0)
		{
			cout << szFileName << endl;
			GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::NONANIM, XMMatrixRotationY(XMConvertToRadians(180.0f)));
		}

		if (strcmp(entry.path().extension().string().c_str(), ".fbx") == 0)
		{
			cout << szFileName << endl;
			GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::NONANIM, XMMatrixRotationY(XMConvertToRadians(180.0f)));
		}*/

		if (strcmp(entry.path().extension().string().c_str(), ".FBX") == 0)
		{
			cout << szFileName << endl;
			GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::NONANIM, XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f));
		}


		itr++;
	}

}

void CLoader::Load_AllNaviMesh()
{
	
}

void CLoader::Load_AllMeshes(const filesystem::path& In_Path, MODEL_TYPE In_eModelType, const MEMORY_TYPE& In_eMemoryType)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	string szFileName;
	string szExtension;
	fs::directory_iterator itr(In_Path);

	while (itr != fs::end(itr)) 
	{
		const fs::directory_entry& entry = *itr;

		//string szPath
		Load_AllEffectMeshInPath_Recursive(entry.path());

		szFileName = entry.path().filename().string().c_str();
		szFileName = szFileName.substr(0, szFileName.size() - 4);

		if (strcmp(entry.path().extension().string().c_str(), ".bin") == 0)
		{
			cout << "Load_AllMeshes() : " << szFileName << endl;
			GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), In_eModelType, XMMatrixScaling(0.01f, 0.01f, 0.01f), In_eMemoryType);
		}

		else if (strcmp(entry.path().extension().string().c_str(), ".fbx") == 0 || strcmp(entry.path().extension().string().c_str(), ".FBX") == 0)
		{
			cout << "Load_AllMeshes() : " << szFileName << endl;
			GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), In_eModelType, XMMatrixScaling(0.01f, 0.01f, 0.01f), In_eMemoryType);
		}

		itr++;
	}
}

void CLoader::Load_AllTexture(const filesystem::path& In_Path, const MEMORY_TYPE& In_eMemoryType)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	tstring szPath;
	string szFileName;

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string();
		szFileName = szFileName.substr(0, szFileName.find("."));

		std::cout << "Load_AllTexture() : " << szFileName << std::endl;
		Load_AllTexture(entry.path(), In_eMemoryType);

		szPath = entry.path().wstring();
		GAMEINSTANCE->Load_Textures(szFileName.c_str(), szPath.c_str(), In_eMemoryType);

		itr++;
	}
}

void CLoader::Create_GameObjectFromJson(const string& In_szJsonPath, const LEVEL& In_eLevel)
{
	json LoadedJson;

	if (FAILED(CJson_Utility::Load_Json(In_szJsonPath.c_str(), LoadedJson)))
		DEBUG_ASSERT;

	for (auto& Elem_GameObjects : LoadedJson["GameObject"])
	{
		weak_ptr<CGameObject> pGameObjectInstance = GAMEINSTANCE->Add_GameObject(Elem_GameObjects["Hash"], (_uint)In_eLevel);

		//객체가 생성되지 않음. Loader에서 프로토타입이 생성됐는지 확인 필요함.
#ifdef _DEBUG
		if (!pGameObjectInstance.lock().get())
			cout << "객체가 생성되지 않음. 프로토타입 확인" << (string)Elem_GameObjects["Name"] << endl;
#endif // _DEBUG
		if (pGameObjectInstance.lock().get())
		{
			pGameObjectInstance.lock()->Set_Enable(Elem_GameObjects["Setting"]["Enable"]);
			pGameObjectInstance.lock()->Load_FromJson(Elem_GameObjects);
		}
	}
}

shared_ptr<CLoader> CLoader::Create(LEVEL eNextLevel)
{
	shared_ptr<CLoader>		pInstance = make_shared<CLoader>();
	pInstance->Initialize(eNextLevel);
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_hThread);
}
