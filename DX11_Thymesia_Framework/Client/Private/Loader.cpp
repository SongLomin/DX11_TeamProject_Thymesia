#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "Json/Json_Utility.h"
#include "GameManager.h"
#include "Player_HPBar.h"
#include "Player_MPBar.h"
#include "HUD_Player_Memory.h"
#include "Player.h"
#include "Player_PotionUI.h"
#include "Player_FeatherUI.h"
#include "UI_Landing.h"
#include "CustomUI.h"
#include "UI_Logo.h"
#include "UI_Loading.h"
#include "SubThread_Pool.h"
#include <cmath>

#define LOAD_TEXTURES_USE_THREAD(szKey, szPath, eMemoryType) \
	GET_SINGLE(CGameManager)->Get_ClientThread()->Enqueue_Job(bind(&CGameInstance::Load_Textures, CGameInstance::Get_Instance(), szKey, szPath, eMemoryType));

#define LOAD_SHADER_USE_THREAD(szKey, szPath) \
	GET_SINGLE(CGameManager)->Get_ClientThread()->Enqueue_Job(bind(&CGameInstance::Load_Shader, CGameInstance::Get_Instance(), szKey, szPath));
	//GAMEINSTANCE->Load_Textures(szKey, szPath, eMemoryType);

///TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
///GAMEINSTANCE->Load_Model("Corvus", "../Bin/Resources/Meshes/Corvus/Corvus.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC, true);
///
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

	case LEVEL_TEST:
		hr = pLoader->Loading_ForTestLevel();
		break;

	case LEVEL_HOME:
		hr = pLoader->Loading_ForHomeLevel();
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
	
	// TODO : Turn off temporarily for Light_Prop
	GAMEINSTANCE->Clear_Lights();
	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForLogoLevel()
{
	lstrcpy(m_szLoadingText, TEXT("Loading Fonts..."));
	GAMEINSTANCE->Add_Font((_uint)FONT_INDEX::DREAM, TEXT("../Bin/Resources/Fonts/128.spriteFont"));
	GAMEINSTANCE->Add_Font((_uint)FONT_INDEX::PRETENDARD, TEXT("../Bin/Resources/Fonts/Pretendard.spriteFont"));
	GAMEINSTANCE->Add_Font((_uint)FONT_INDEX::HEIROLIGHT, TEXT("../Bin/Resources/Fonts/HeiroLightRegular.spritefont"));

	CEditGround::Load_AllMeshInfo();

	lstrcpy(m_szLoadingText, TEXT("Loading Prototype Objects..."));
	GAMEINSTANCE->Add_Prototype_GameObject<CFadeMask>();
	GAMEINSTANCE->Add_Prototype_GameObject<CCamera_Target>();
	GAMEINSTANCE->Add_Prototype_GameObject<CCamera_Interior>();
	GAMEINSTANCE->Add_Prototype_GameObject<CCorvus>();
	GAMEINSTANCE->Add_Prototype_GameObject<CNorMonster>();
	GAMEINSTANCE->Add_Prototype_GameObject<CTerrain>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStatic_Prop>();
	GAMEINSTANCE->Add_Prototype_GameObject<CDynamic_Prop>();
	GAMEINSTANCE->Add_Prototype_GameObject<CLight_Prop>();
	GAMEINSTANCE->Add_Prototype_GameObject<CGround>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStatic_Instancing_Prop>();
	GAMEINSTANCE->Add_Prototype_GameObject<CPhysXColliderObject>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Ladder>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_CheckPoint>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_NextPoint>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Elevator>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_DeadSpot>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Door>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_CastleGate>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Note>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Item>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Aisemy>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_MapTable>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_Chair>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteriorProp>();
	GAMEINSTANCE->Add_Prototype_GameObject<CInteraction_InteriorActivate>();
	GAMEINSTANCE->Add_Prototype_GameObject<CPhysXColliderObject_Interior>();
	GAMEINSTANCE->Add_Prototype_GameObject<CStatic_InteriorProp>();
	GAMEINSTANCE->Add_Prototype_GameObject<CFog>();
	GAMEINSTANCE->Add_Prototype_GameObject<CProp_Fence>();
	GAMEINSTANCE->Add_Prototype_GameObject<CSection_Eventer>();
	GAMEINSTANCE->Add_Prototype_GameObject<CWater>();
	GAMEINSTANCE->Add_Prototype_GameObject<CVarg>();
	GAMEINSTANCE->Add_Prototype_GameObject<CJoker>();
	GAMEINSTANCE->Add_Prototype_GameObject<CUrd>();
	GAMEINSTANCE->Add_Prototype_GameObject<CBat>();
	GAMEINSTANCE->Add_Prototype_GameObject<CBigHandMan>();


#ifdef _LOAD_CAPTURED_RESOURCE_

	lstrcpy(m_szLoadingText, TEXT("Loading Captured Resources..."));
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/Logo.json");
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/Loading.json");
	Load_UIResource();

#else // _LOAD_CAPTURED_RESOURCE_

	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Textures..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();

	lstrcpy(m_szLoadingText, TEXT("Loading Default Resources..."));
	LOAD_TEXTURES_USE_THREAD(("Default"), TEXT("../Bin/Resources/Textures/Default%d.jpg"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("Background"), TEXT("../Bin/Resources/Textures/Background/BgFightLoading%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("ButtonDefault"), TEXT("../Bin/Resources/Textures/UI/ButtonDefault.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("UI_White"), TEXT("../Bin/Resources/Textures/UI/UI_White.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD("Grass", TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD("TestTile", TEXT("../Bin/Resources/Textures/TestTexture/DefaultGrid.dds"), MEMORY_TYPE::MEMORY_STATIC);

	lstrcpy(m_szLoadingText, TEXT("Loading UI Resources..."));
	Load_UIResource();

	GAMEINSTANCE->Load_Textures(("DecalTexture"), TEXT("../Bin/Resources/Textures/Decal/Crack%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("DecalLinear"), TEXT("../Bin/Resources/Textures/DecalLinear/Crack%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("DecalUrd"), TEXT("../Bin/Resources/Textures/DecalUrd/Crack%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	lstrcpy(m_szLoadingText, TEXT("Loading Sound..."));
	GAMEINSTANCE->Load_SoundFileFromFolderPath("../bin/Sound");

#endif // _LOAD_CAPTURED_RESOURCE_


	lstrcpy(m_szLoadingText, TEXT("Loading Shaders..."));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_UI"), TEXT("../Bin/ShaderFiles/Shader_UI.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_UIEffect"), TEXT("../Bin/ShaderFiles/Shader_UIEffect.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxAnimModel"), TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxCubeTex"), TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxInstance"), TEXT("../Bin/ShaderFiles/Shader_VtxInstance.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxModel"), TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxNorTex"), TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxTex"), TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxDecal"), TEXT("../Bin/ShaderFiles/Shader_VtxDecal.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_EffectMesh"), TEXT("../Bin/ShaderFiles/Shader_EffectMesh.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxGround"), TEXT("../Bin/ShaderFiles/Shader_VtxGround.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxModelInstance"), TEXT("../Bin/ShaderFiles/Shader_VtxModelInstance.hlsl"));
	LOAD_SHADER_USE_THREAD(TEXT("Shader_VtxCurve"), TEXT("../Bin/ShaderFiles/Shader_VtxCurve.hlsl"));


	lstrcpy(m_szLoadingText, TEXT("Loading All Key Event from Json...."));
	GET_SINGLE(CGameManager)->Load_AllKeyEventFromJson();


	lstrcpy(m_szLoadingText, TEXT("Loading Damage Fonts..."));
	GAMEINSTANCE->Load_Textures(("DamageFont_Claw"), TEXT("../Bin/Resources/Textures/UI/DamageFont/Claw/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("DamageFont_Normal"), TEXT("../Bin/Resources/Textures/UI/DamageFont/Normal/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	GAMEINSTANCE->Load_Textures(("DamageFont_Parry"), TEXT("../Bin/Resources/Textures/UI/DamageFont/Parry/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	

	lstrcpy(m_szLoadingText, TEXT("Loading Irradiance Map..."));
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");

	lstrcpy(m_szLoadingText, TEXT("Loading PreFilter..."));
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	GAMEINSTANCE->Set_MaskingTexture("UVMask");

	

	lstrcpy(m_szLoadingText, TEXT("Wait for thread job done."));
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();


	lstrcpy(m_szLoadingText, TEXT("Logo : Loading Complete"));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForLobby()
{
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_ForTestLevel()
{
	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Textures..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();

	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Rare Mob..."));
	this->Load_RareMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();

#ifdef _SKYBOX_
	lstrcpy(m_szLoadingText, TEXT("Loading Skybox Texture..."));
	GAMEINSTANCE->Load_Textures("Sky", TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
#endif // _SKYBOX_

#ifdef _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading Captured Resources..."));
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/TestLevel.json");
#else // _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading All Map Model..."));
	Load_AllMapModel();
#endif // _LOAD_CAPTURED_RESOURCE_

	_matrix TransformMatrix(XMMatrixIdentity());
	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);
	lstrcpy(m_szLoadingText, TEXT("Loading All Meshes from : [ ../Bin/Resources/Meshes/Destructable/Fence_16a/ ]"));
	Load_AllMeshes("../Bin/Resources/Meshes/Destructable/Fence_16a/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_STATIC, TransformMatrix, ".fbx");

	lstrcpy(m_szLoadingText, TEXT("Loading Light..."));
	LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.2f, 0.19f, 0.18f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable = true;
	LightDesc.fIntensity = 1.f;
	GAMEINSTANCE->Add_Light(LightDesc);

	lstrcpy(m_szLoadingText, TEXT("Loading Irradiance Map..."));
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");

	GAMEINSTANCE->Set_Contrast(1.07f);
	GAMEINSTANCE->Set_Saturation(1.5f);
	GAMEINSTANCE->Set_Exposure(1.f);

	lstrcpy(m_szLoadingText, TEXT("Loading PreFilter..."));
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	GAMEINSTANCE->Set_MaskingTexture("UVMask");

	lstrcpy(m_szLoadingText, TEXT("Wait for thread job done."));
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();

	lstrcpy(m_szLoadingText, TEXT("Test : Loading Complete"));

	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Textures..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();

	lstrcpy(m_szLoadingText, TEXT("Loading Skybox Texture..."));
	GAMEINSTANCE->Load_Textures("Sky", TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);

#ifdef _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading Captured Resources from : [ ../Bin/LevelData/CapturedResource/GamePlay.json ]"));
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/GamePlay.json");
#else
	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Rare Mob..."));
	this->Load_RareMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();

	lstrcpy(m_szLoadingText, TEXT("Loading All Map Model..."));
	Load_AllMapModel();

#endif // _LOAD_CAPTURED_RESOURCE_
	_matrix TransformMatrix(XMMatrixIdentity());
	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);
	lstrcpy(m_szLoadingText, TEXT("Loading All Meshes from : [ ../Bin/Resources/Meshes/Destructable/Fence_16a/ ]"));
	Load_AllMeshes("../Bin/Resources/Meshes/Destructable/Fence_16a/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_STATIC, TransformMatrix, ".fbx");

	/*LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse   = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient   = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular  = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable    = true;
	LightDesc.fIntensity = 50.f;
	GAMEINSTANCE->Add_Light(LightDesc);*/

	lstrcpy(m_szLoadingText, TEXT("Loading Environmental Shader Effects..."));
	//GAMEINSTANCE->Set_FogDesc(_float4(0.2f, 0.15f, 0.03f, 0.5f), 20.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.0f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));
	GAMEINSTANCE->Set_Contrast(1.07f);
	GAMEINSTANCE->Set_Saturation(1.7f);
	GAMEINSTANCE->Set_Exposure(2.f);

	lstrcpy(m_szLoadingText, TEXT("Loading Irradiance Map..."));
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap1.dds"),MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");

	lstrcpy(m_szLoadingText, TEXT("Loading PreFilter..."));
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter1.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	GAMEINSTANCE->Set_IrradianceColorScale(_float3(1.f, 1.f, 1.f));

	lstrcpy(m_szLoadingText, TEXT("Wait for thread job done."));
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();

	lstrcpy(m_szLoadingText, TEXT("GamePlay : Loading Complete"));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForStage2Level()
{
#ifdef _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading Captured Resources from : [ ../Bin/LevelData/CapturedResource/Stage2.json ]"));
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/Stage2.json");
#else // _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading All Map Model..."));
	Load_AllMapModel();
#endif // _LOAD_CAPTURED_RESOURCE_

	
#ifdef _SKYBOX_
	lstrcpy(m_szLoadingText, TEXT("Loading Skybox Texture..."));
	GAMEINSTANCE->Load_Textures("Sky", TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
#endif // _SKYBOX_


	// TODO : Turn off temporarily for Light_Prop
	lstrcpy(m_szLoadingText, TEXT("Loading Light..."));
	LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

#ifdef _BRIGHT_LIGHT_
	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vSpecular = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable = true;
	LightDesc.fIntensity = 1.f;
#else // _BRIGHT_LIGHT_
	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.2f, 0.15f, 0.14f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.05f, 0.04f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.25f, 0.24f, 1.f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable = true;
	LightDesc.fIntensity = 1.f;
#endif // _BRIGHT_LIGHT_

	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Textures..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();
	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Rare Mob..."));
	this->Load_RareMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();

	Load_AllMapModel();


	GAMEINSTANCE->Add_Light(LightDesc);

	//GAMEINSTANCE->Set_FogDesc(_float4(0.45f, 0.26f, 0.28f, 0.92f), 160.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(1.f, 1.f, 1.f, 1.f), _float4(1.f, 1.f, 1.f, 1.f), _float4(1.f, 1.f, 1.f, 1.f));
	GAMEINSTANCE->Set_Contrast(1.2f);
	GAMEINSTANCE->Set_Saturation(1.5f);
	GAMEINSTANCE->Set_Exposure(2.f);

	lstrcpy(m_szLoadingText, TEXT("Loading IrradianceMap..."));
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");

	lstrcpy(m_szLoadingText, TEXT("Loading PreFilter..."));
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	lstrcpy(m_szLoadingText, TEXT("Wait for thread job done."));
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();

	GAMEINSTANCE->Set_IrradianceColorScale(_float3(0.53f, 0.43f, 0.43f));
	lstrcpy(m_szLoadingText, TEXT("Stage 2 Loading Complete"));
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_ForStage3Level()
{
#ifdef _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading Captured Resources from : [ ../Bin/LevelData/CapturedResource/Stage3.json ]"));
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/Stage3.json");
#else // _LOAD_CAPTURED_RESOURCE_

	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Textures..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();
	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Rare Mob..."));
	this->Load_RareMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();

	Load_AllMapModel();
#endif
	_matrix TransformMatrix(XMMatrixIdentity());
	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	lstrcpy(m_szLoadingText, TEXT("Loading All Meshes from : [ ../Bin/Resources/Meshes/Destructable/Column/ ]..."));
	Load_AllMeshes("../Bin/Resources/Meshes/Destructable/Column/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_STATIC, TransformMatrix, ".fbx");

#ifdef _SKYBOX_
	lstrcpy(m_szLoadingText, TEXT("Loading Skybox Texture..."));
	GAMEINSTANCE->Load_Textures("Sky", TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
#endif // _SKYBOX_

	//GAMEINSTANCE->Set_FogDesc(_float4(0.5f, 0.5f, 0.5f, 0.65f), 50.f);
	GAMEINSTANCE->Set_LiftGammaGain(_float4(0.95f, 0.95f, 0.97f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));
	GAMEINSTANCE->Set_Contrast(1.f);
	GAMEINSTANCE->Set_Saturation(1.f);
	GAMEINSTANCE->Set_Exposure(1.2f);

	lstrcpy(m_szLoadingText, TEXT("Loading IrradianceMap..."));
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");

	lstrcpy(m_szLoadingText, TEXT("Loading PreFilter..."));
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	GAMEINSTANCE->Set_IrradianceColorScale(_float3(1.f, 1.f, 1.f));

	lstrcpy(m_szLoadingText, TEXT("Wait for thread job done."));
	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();


	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForHomeLevel()
{
	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Textures..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();

#ifdef _LOAD_CAPTURED_RESOURCE_
	lstrcpy(m_szLoadingText, TEXT("Loading Captured Resources from : [ ../Bin/LevelData/CapturedResource/Home.json ]"));
	GAMEINSTANCE->Load_ResourcesFromJson("../Bin/LevelData/CapturedResource/Home.json");
#else
	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Rare Mob..."));
	this->Load_RareMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();
#endif // _LOAD_CAPTURED_RESOURCE_

	Load_AllMapModel();

	GAMEINSTANCE->Set_LiftGammaGain(_float4(0.95f, 0.95f, 0.97f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f), _float4(0.95f, 0.95f, 0.95f, 1.f));

	GAMEINSTANCE->Set_Contrast(1.f);
	GAMEINSTANCE->Set_Saturation(1.f);
	GAMEINSTANCE->Set_Exposure(1.2f);

	GAMEINSTANCE->Set_FogDesc(_float4(0.2f, 0.15f, 0.03f, 0.f), 10000.f);
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter%d.dds"));
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();
	m_isFinished = true;

	return S_OK;

}

HRESULT CLoader::Loading_ForEditLevel()
{
	//Load_AllMeshes("../Bin/Resources/Meshes/Temp/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);

	lstrcpy(m_szLoadingText, TEXT("Loading Diffuse Texturesww..."));
	Load_AllDiffuseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Mask Textures..."));
	Load_AllMaskMap();
	lstrcpy(m_szLoadingText, TEXT("Loading Noise Textures..."));
	Load_AllNoiseTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Particle Textures..."));
	Load_AllParticleTexture();
	lstrcpy(m_szLoadingText, TEXT("Loading Effect Meshes..."));
	Load_AllEffectMesh();


#ifdef _EFFECT_TOOL_
#ifdef _CORVUS_MODEL_
	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
#endif // _CORVUS_MODEL
#ifdef _BOSS_MODEL_
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
#endif // _BOSS_MODEL_
#ifdef _ELITE_MOB_MODEL_
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
#endif // _ELITE_MOB_MODEL_
#ifdef _NORMAL_MOB_MODEL_
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();
#endif // _NORMAL_MOB_MODEL_
#else //_EFFECT_TOOL_
	lstrcpy(m_szLoadingText, TEXT("Loading Corvus..."));
	this->Load_CorvusModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Boss Mob..."));
	this->Load_BossMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Rare Mob..."));
	this->Load_RareMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Elite Mob..."));
	this->Load_EliteMobModel();
	lstrcpy(m_szLoadingText, TEXT("Loading Normal Mob..."));
	this->Load_NormalMobModel();
#endif // _EFFECT_TOOL_

#ifndef _EFFECT_TOOL_
	Load_AllMapModel();
#endif // _EFFECT_TOOL_

	// TODO : Turn off temporarily for Light_Prop
	LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

#ifdef _BRIGHT_LIGHT_
	LightDesc.eActorType	= tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection	= _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse		= _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient		= _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vSpecular		= _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vLightFlag	= _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable		= true;
	LightDesc.fIntensity	= 1.f;
#else
	LightDesc.eActorType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vSpecular = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.bEnable = true;
	LightDesc.fIntensity = 0.1f;
#endif // _BRIGHT_LIGHT_

	GAMEINSTANCE->Add_Light(LightDesc);

	GAMEINSTANCE->Set_FogDesc(_float4(0.2f, 0.15f, 0.03f, 0.f), 10000.f);
	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter%d.dds"));
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");

	GET_SINGLE(CGameManager)->Get_ClientThread()->Wait_JobDone();
	m_isFinished = true;

	return S_OK;
}

void CLoader::Loading_ForEffectGroup(const char* In_Path, const _uint& In_LevelIndex)
{
	weak_ptr<CEffectGroup> EffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(In_LevelIndex);
	EffectGroup.lock()->Load_EffectJson(In_Path, (_uint)TIMESCALE_LAYER::NONE, In_LevelIndex);
}

void CLoader::Load_AllDiffuseTexture()
{
	LOAD_TEXTURES_USE_THREAD(("UVColorDiffuse"), TEXT("../Bin/Resources/Textures/Diffuse/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllMaskMap()
{
	LOAD_TEXTURES_USE_THREAD(("UVMask"), TEXT("../Bin/Resources/Textures/Mask/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllNoiseTexture()
{
	LOAD_TEXTURES_USE_THREAD(("UVNoise"), TEXT("../Bin/Resources/Textures/Noise/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllParticleTexture()
{
	fs::directory_iterator itr("..\\Bin\\Resources\\Textures\\Effect");
	tstring szPath;
	tstring szFileName;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;
		szFileName = entry.path().filename().wstring();

		#ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
		#endif

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

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
#endif
#endif

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

		GET_SINGLE(CGameManager)->Get_ClientThread()->Enqueue_Job(bind([entry]() {
			string szFileName = entry.path().filename().string().c_str();
			szFileName = szFileName.substr(0, szFileName.size() - 4);
#ifdef _BAKE_EFFECTMESH_FBX_
			if (strcmp(entry.path().extension().string().c_str(), ".FBX") == 0)
			{
				GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::NONANIM, XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f));
			}

			if (strcmp(entry.path().extension().string().c_str(), ".fbx") == 0)
			{
				GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::NONANIM, XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f));
			}
#else // _BAKE_EFFECTMESH_FBX_
			if (strcmp(entry.path().extension().string().c_str(), ".bin") == 0)
			{
				GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), MODEL_TYPE::NONANIM, XMMatrixRotationY(XMConvertToRadians(180.0f)));
			}
#endif // _BAKE_EFFECTMESH_FBX_

			}));

		itr++;
	}
}

void CLoader::Load_AllNaviMesh()
{
}

void CLoader::Load_AllMeshes(const filesystem::path& In_Path, MODEL_TYPE In_eModelType, const MEMORY_TYPE& In_eMemoryType, _matrix TransformationMatrix, const char* In_extansion)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	wstring Text = L"[ " + wstring(In_Path) + L" ]";
	lstrcpy(m_szLoadingText, Text.c_str());

	string szFileName;
	string szExtension;
	fs::directory_iterator itr(In_Path);

	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		//string szPath
		Load_AllEffectMeshInPath_Recursive(entry.path());

		GET_SINGLE(CGameManager)->Get_ClientThread()->Enqueue_Job(bind([entry, In_eModelType, In_eMemoryType, TransformationMatrix, In_extansion]() {
			string szFileName = entry.path().filename().string().c_str();
			szFileName = szFileName.substr(0, szFileName.size() - 4);

			if (strcmp(entry.path().extension().string().c_str(), In_extansion) == 0)
			{
				cout << "Load_AllMeshes() <" << In_extansion << "> " << szFileName << endl;
				GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), In_eModelType, TransformationMatrix, In_eMemoryType);
			}

			else if (strcmp(entry.path().extension().string().c_str(), ".fbx") == 0 || strcmp(entry.path().extension().string().c_str(), ".FBX") == 0)
			{
				cout << "Load_AllMeshes() <.fbx> " << szFileName << endl;
				GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(), In_eModelType, TransformationMatrix, In_eMemoryType);
			}
			}));

		itr++;
	}
 }

//void CLoader::Load_AllMeshesInPath_Recursive(const filesystem::path& In_Path, MODEL_TYPE In_eModelType, const MEMORY_TYPE& In_eMemoryType, _matrix TransformationMatrix, const char* In_extansion)
//{
//}

void CLoader::Load_AllTexture(const filesystem::path& In_Path, const MEMORY_TYPE& In_eMemoryType)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	//tstring szPath;
	//string szFileName;


	while (itr != fs::end(itr))
	{
		const fs::directory_entry& entry = *itr;

		GET_SINGLE(CGameManager)->Get_ClientThread()->Enqueue_Job(bind([entry, In_eMemoryType]() {
			string szFileName = entry.path().filename().string();
			szFileName = szFileName.substr(0, szFileName.find("."));
			std::cout << "Load_AllTexture() : " << szFileName << std::endl;

			tstring szPath = entry.path().wstring();
			GAMEINSTANCE->Load_Textures(szFileName.c_str(), szPath.c_str(), In_eMemoryType);
			}));

		//szFileName = entry.path().filename().string();
		//szFileName = szFileName.substr(0, szFileName.find("."));

		//std::cout << "Load_AllTexture() : " << szFileName << std::endl;
		Load_AllTexture(entry.path(), In_eMemoryType);

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
#ifdef _DEBUG_COUT_
		if (!pGameObjectInstance.lock().get())
		cout << "객체가 생성되지 않음. 프로토타입 확인" << (string)Elem_GameObjects["Name"] << endl;
#endif
#endif // _DEBUG
		if (pGameObjectInstance.lock().get())
		{
			pGameObjectInstance.lock()->Set_Enable(Elem_GameObjects["Setting"]["Enable"]);
			pGameObjectInstance.lock()->Load_FromJson(Elem_GameObjects);
		}
	}
}

void CLoader::Load_UIResource()
{
#pragma region 1팀
	LOAD_TEXTURES_USE_THREAD(("Loading_SafeHouse"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Lobby_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_SeaOfTrees"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Circus_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_RoyalGarden"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Garden_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_HermesFortress"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Fortress_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_OceanOfMemories"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_MemoryOcean_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_Tutorial"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingScreen_Tutorial_01.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Loading_Icon"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/TexUI_LoadingIcon.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_Complete"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/Loading_Complete.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Loading_Font_Fortress_Title"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/Fortress_Name.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_Font_Fortress_Desc"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/Fortress_Desc.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Loading_Font_SeaOfTrees_Title"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/SeaOfTrees_Name.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_Font_SeaOfTrees_Desc"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/SeaOfTrees_Desc.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("Loading_Font_RoyalGarden_Title"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/RoyalGarden_Name.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_Font_RoyalGarden_Desc"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/RoyalGarden_Desc.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("Loading_Font_SafeHouse_Title"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/SafeHouse_Name.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Loading_Font_SafeHouse_Desc"), TEXT("../Bin/Resources/Textures/UI/LoadingScreen/LoadingFont/SafeHouse_Desc.png"), MEMORY_TYPE::MEMORY_STATIC);


	//MainMenu(LogoLevel)
	LOAD_TEXTURES_USE_THREAD(("MainMenu_Background"), TEXT("../Bin/Resources/Textures/UI/MainMenuBackrgound.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("GameLogo"), TEXT("../Bin/Resources/Textures/UI/GameLogo2.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_1"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SelectableButtonBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_2"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SelectableButtonHighlight.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_ContinueGame"), TEXT("../Bin/Resources/Textures/UI/UI_ContinueGame.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_StartGame"), TEXT("../Bin/Resources/Textures/UI/UI_StartGame.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_LoadGame"), TEXT("../Bin/Resources/Textures/UI/UI_LoadGame.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_Options"), TEXT("../Bin/Resources/Textures/UI/UI_Options.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_Credits"), TEXT("../Bin/Resources/Textures/UI/UI_Credits.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_Quit"), TEXT("../Bin/Resources/Textures/UI/UI_Quit.png"), MEMORY_TYPE::MEMORY_DYNAMIC);
	LOAD_TEXTURES_USE_THREAD(("MainMenu_SelectableButton_Tool"), TEXT("../Bin/Resources/Textures/UI/UI_Tool.png"), MEMORY_TYPE::MEMORY_DYNAMIC);

#pragma endregion 1팀
	//Player HPBar Texture
	LOAD_TEXTURES_USE_THREAD(("Player_HPBar_Border_Left"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_1Border_Left.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_HPBar_Border_Right"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_1Border_Right.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Player_HPBar_BG"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_2BG.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_HPBar_MainBar"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_3MainBar.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_HPBar_DelayBar"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_4DelayBar.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_HPBar_Track"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_HPBar_5Track.png"), MEMORY_TYPE::MEMORY_STATIC);
	//LOAD_TEXTURES_USE_THREAD(("Player_HPBar_Track_Mana"), TEXT("../Bin/Resources/Textures/UI/Track_Test.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Player_MPBar Texture
	LOAD_TEXTURES_USE_THREAD(("Player_MPBar_Border_Left"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_1Border_Left.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_MPBar_Border_Right"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_1Border_Right.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_MPBar_BG"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_2BG.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Player_MPBar_MainBar"), TEXT("../Bin/Resources/Textures/UI/HUD/PlayerHPBar/TexUI_MPBar_3MainBar.png"), MEMORY_TYPE::MEMORY_STATIC);


	//HUD_Player_Memory
	LOAD_TEXTURES_USE_THREAD(("HUD_Player_Memory_Icon"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_MemoryShardsIcon.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Player_Memory_BG"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_ContentBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Player_Memory_BG2"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SquareFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Player_Memory_Decoration"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DecorationLine_05.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Player Skill & ItemUI
	LOAD_TEXTURES_USE_THREAD(("Player_Item_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Potion Image
	//mini : 40x40
	LOAD_TEXTURES_USE_THREAD(("Potion_Default_Mini"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("HUD_Potion_Default_Mini"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_DefaultType_Mini.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Potion_Buff_Mini"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_BuffType_Mini.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Potion_Immediate_Mini"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_ImmediateType_Mini.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("Potion_Default"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_DefaultType.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Potion_Buff"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_BuffType.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Potion_Immediate"), TEXT("../Bin/Resources/Textures/UI/Icons/Potions/TexUI_Potion_ImmediateType.png"), MEMORY_TYPE::MEMORY_STATIC);

	//HUD
	LOAD_TEXTURES_USE_THREAD(("HUD_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Frame_Hover"), TEXT("../Bin/Resources/Textures/UI/Hover_Rombo_Texture.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("HUD_FrameBorder"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_FrameBorder.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Font_BG"), TEXT("../Bin/Resources/Textures/UI/HUD/TexUI_PlagueWeaponBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_Feather"), TEXT("../Bin/Resources/Textures/UI/HUD/TexUI_Feather.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Player HUD PlagueWeapon_Main & PlagueWeapon_sub & PlagueWeapon_Steal
	LOAD_TEXTURES_USE_THREAD(("HUD_PlagueWeapon_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_DefaultSlotFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_PlagueWeapon_Frame_Ready"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_DefaultSlotFrame_02.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_PlagueWeapon_Frame_Steal"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_PlunderSlotFrame_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("HUD_PlagueWeapon_Frame_Steal_Decoration"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_PlunderSlotFrame_02.png"), MEMORY_TYPE::MEMORY_STATIC);


	//skill Icon
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_Axe"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_Axe.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_VargSword"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_VargSword.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_Knife"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_Knife.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_Hammer"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_Hammer.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_Scythe"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_Scythe.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_BloodStorm"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_BatKing.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_Halberd"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_Halberd.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_BigHand"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_BigHand.dds"), MEMORY_TYPE::MEMORY_STATIC);

	
	LOAD_TEXTURES_USE_THREAD(("SkillIcon_Bankai"), TEXT("../Bin/Resources/Textures/UI/Icons/Skills/TexUI_SkillIcon_BloodSword.dds"), MEMORY_TYPE::MEMORY_STATIC);


	//Landing
	LOAD_TEXTURES_USE_THREAD(("Landing_MemoriesRetrived"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_MemoriesRetrived.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_MemoriesRetrivedBG"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_MemoriesRetrivedBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_MemoryInterrupted"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_MemoryInterruptedFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_RecallCompleted"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_RecallCompleted.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_BeconFound"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_SanctumDiscoveredFrame.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_BG"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingScreenBackground.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Landing_Stage
	LOAD_TEXTURES_USE_THREAD(("Landing_SafeHouse"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheSafeHouse.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_SeaOfTrees"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheCircus.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_RoyalGarden"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheGarden.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_HermesFortress"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheFortress.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Landing_OceanOfMemories"), TEXT("../Bin/Resources/Textures/UI/LandingMessages/TexUI_LandingMessage_TheMemorySea.png"), MEMORY_TYPE::MEMORY_STATIC);

	//UI Masking Texture
	LOAD_TEXTURES_USE_THREAD(("CloudNoise"), TEXT("../Bin/Resources/Textures/UI/GoodUIIconTrail/Noise/T_CloudNoise.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Dissolve_1"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	//	LOAD_TEXTURES_USE_THREAD(("Dissolve_2"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test2.png"), MEMORY_TYPE::MEMORY_STATIC);
	//	LOAD_TEXTURES_USE_THREAD(("Dissolve_3"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test3.png"), MEMORY_TYPE::MEMORY_STATIC);
	//	LOAD_TEXTURES_USE_THREAD(("Dissolve_4"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test4.png"), MEMORY_TYPE::MEMORY_STATIC);
	//	LOAD_TEXTURES_USE_THREAD(("Dissolve_1"), TEXT("../Bin/Resources/Textures/UI/Dissolve/Test%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("MaskColor_Vertical"), TEXT("../Bin/Resources/Textures/UI/W_PlagueWeapon_MaskColor01.png"), MEMORY_TYPE::MEMORY_STATIC);


	//TODO : UI_Menu : ESC키를 눌러서 나오는 메뉴창
	//UI_Menu
	LOAD_TEXTURES_USE_THREAD(("PauseMenu_Background0"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_00.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PauseMenu_Background1"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PauseMenu_Background2"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_02.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PauseMenu_Background3"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/TexUI_PauseMenuBackground_03.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageIndex_Indicator_Selected"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_PageIndexIndicator_Selected.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageIndex_Indicator_UnSelected"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_PageIndexIndicator_UnSelected.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageIndex_Indicator_Decoration"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_PageIndexIndicator_Decoration.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageIndex_UnderLine"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_UnderLine.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Page_Status
	LOAD_TEXTURES_USE_THREAD(("Tap_Status_BG"), TEXT("../Bin/Resources/Textures/UI/TexUI_CharacterStatusBackground.png"), MEMORY_TYPE::MEMORY_STATIC);

	//TestFontImage
	LOAD_TEXTURES_USE_THREAD(("PageFont_Status"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Status.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageFont_Talent"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Talent.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageFont_Collection"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Collection.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageFont_Item"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Item.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageFont_Option"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Option.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PageFont_Quit"), TEXT("../Bin/Resources/Textures/UI/PauseMenu_font/PageIndex/Quit.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Font_Diamond"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DiamondIcon_01.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Font_Decoration2"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DecorationLine_02.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Font_Decoration4"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_DecorationLine_04.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Mask_Horizontal"), TEXT("../Bin/Resources/Textures/UI/Mask/Mask_Horizontal%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Tap_PlagueWeapon"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);


	//MonsterHPBar
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_Border"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_3BarBorder.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_Border_Elite"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_4BarBorderElite_TYPE2_.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_RecoveryShine"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_5BarRecoveryShineB.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_Decoration_Head"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_2BarHead.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_White"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HpMain.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_Green"), TEXT("../Bin/Resources/Textures/UI/HUD/TexUI_Plague_HealthBar.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_Track"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_6BarTrack.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_GreenTrack"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_NewPreSTBar.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_HPBar_StunnedShine"), TEXT("../Bin/Resources/Textures/UI/HUD/HPBar/TexUI_HPBar_StunnedShine1.png"), MEMORY_TYPE::MEMORY_STATIC);

	//parrying system
	LOAD_TEXTURES_USE_THREAD(("Monster_ParryBorder_Default"), TEXT("../Bin/Resources/Textures/UI/Parrying/MockUp/ParrayBorder_Default.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_ParryMain_Default"), TEXT("../Bin/Resources/Textures/UI/Parrying/parry_Gauge_Main_2.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Monster_Parry_Fill"), TEXT("../Bin/Resources/Textures/UI/Parrying/Parrying_Fill.png"), MEMORY_TYPE::MEMORY_STATIC);

	//EvoleveMenu
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LeftBG"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_EvolveLeftBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_TitleBG"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_HighlightBar_01_Reverse.png"), MEMORY_TYPE::MEMORY_STATIC);

	//EvolveMenu_Font

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_Title_Eng"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Title_Eng.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_LevelUp"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_UnlockTalent"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/UnlockTalent.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_PlagueWeapon"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/PlagueWeapon.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_Potion"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Potion.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_UseForgottenFeather"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/UseForgottenFeather.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_Cease_Recall"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Cease_Recall.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_ResumeGame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/ResumeGame.png"), MEMORY_TYPE::MEMORY_STATIC);
	
	
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_SeaOfTrees"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/SeaOfTrees.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_RoyalGarden"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/RoyalGarden.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_HermessFortress"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/HermessFortress.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Text_SelectHighlight"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_SelectableButtonHighlight_02.png"), MEMORY_TYPE::MEMORY_STATIC);

	//EvolveMenu RightBG
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_RightBG"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_EvolveRightBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_MapImage_SeaOfTrees"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/MapImage/TexUI_LevelImage_Circus.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("EvolveMenu_MapImage_Garden"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/MapImage/TexUI_LevelImage_Garden.png"), MEMORY_TYPE::MEMORY_STATIC);

	GAMEINSTANCE->Load_Textures(("EvolveMenu_MapImage_HermesFortress"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/MapImage/TexUI_LevelImage_Fortress.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Quest
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_QuestTitle"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Quest/QuestTitle.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Stage1_Quest
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Stage1_Quest1"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Quest/Stage1/Quest1.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Stage2_Quest1"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Quest/Stage2/Quest1.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Stage3_Quest1"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Quest/Stage3/Quest1.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Level Up Page

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_StatusBG"), TEXT("../Bin/Resources/Textures/UI/General/Test1.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Level"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Level.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Memory"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Memory.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Required_Memory"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Required_Memory.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Status_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Status.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Status_Strength"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Strength.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Status_Vitality"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Vitality.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Status_Plague"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Plague.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_State_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/State.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_State_AttackDamgage"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/AttackDamage.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_State_Wound"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Wound.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_State_ClawDamage"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/ClawDamage.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_State_HP"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/HP.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_State_MP"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/MP.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Apply"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Apply.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LevelUp_Apply"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Apply.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Decoration_Arrow_Right"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_Arrow_04.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Keyboard_Arrow_Left"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_Arrow_05.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Keyboard_Arrow_Right"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_Arrow_06.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("LevelUp_ReconfirmWindowBG"), TEXT("../Bin/Resources/Textures/UI/TexUI_HintDialogBackground.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("LevelUp_ReconfirmWindow_ChangedStatusApply"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/ChangedStatusApply.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("LevelUp_ReconfirmWindow_Yes"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/Yes.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("LevelUp_ReconfirmWindow_No"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/LevelUp/No.png"), MEMORY_TYPE::MEMORY_STATIC);



	//Scirpt
	LOAD_TEXTURES_USE_THREAD(("Script_Varg_Tutorial_0"), TEXT("../Bin/Resources/Textures/UI/Scripts/Varg/Tutorial_0.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Script_Varg_Tutorial_1"), TEXT("../Bin/Resources/Textures/UI/Scripts/Varg/Tutorial_1.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Script_Varg_Tutorial_Appear"), TEXT("../Bin/Resources/Textures/UI/Scripts/Varg/Appear0.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("Script_Bat_Appear"), TEXT("../Bin/Resources/Textures/UI/Scripts/Bat/Appear0.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("Script_Urd_Appear"), TEXT("../Bin/Resources/Textures/UI/Scripts/Urd/Appear0.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Script_Urd_BattleStart0"), TEXT("../Bin/Resources/Textures/UI/Scripts/Urd/BattleStart_0.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Script_Urd_BattleStart1"), TEXT("../Bin/Resources/Textures/UI/Scripts/Urd/BattleStart_1.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Script_Urd_Phase2"), TEXT("../Bin/Resources/Textures/UI/Scripts/Urd/Phase2.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Script_Urd_Dead"), TEXT("../Bin/Resources/Textures/UI/Scripts/Urd/Dead.png"), MEMORY_TYPE::MEMORY_STATIC);



	//DamageFont
	LOAD_TEXTURES_USE_THREAD(("DamageFont_Claw"), TEXT("../Bin/Resources/Textures/UI/DamageFont/Claw/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("DamageFont_Normal"), TEXT("../Bin/Resources/Textures/UI/DamageFont/Normal/%d.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("DamageFont_Parry"), TEXT("../Bin/Resources/Textures/UI/DamageFont/Parry/%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Talent
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Talent/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Tap_Sword"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/Font/Talent/Tap_Sworld.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_Frame"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_Frame_Active"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Frame_Active.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_Frame_Hover"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Hover.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_Active"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Active.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_Active_Hover"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_ActiveHover.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_None"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_None.png"), MEMORY_TYPE::MEMORY_STATIC);


	//PlagueSkillIcon

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Basic"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/TexUI_TalentIcon_LAtk_Basic.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Cross"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/TexUI_TalentIcon_LAtk_Cross.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Stab"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/TexUI_TalentIcon_LAtk_Stab.dds"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Basic0_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Basic0_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Basic0_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Basic0_Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Basic1_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Basic1_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Basic1_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Basic1_Information.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Slash0_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Slash0_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Slash0_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Slash0_Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Slash1_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Slash1_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Slash1_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Slash1_Information.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Stab0_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Stab0_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Stab0_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Stab0_Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Stab1_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Stab1_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_LAttack_Stab1_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/LAtk_Stab1_Information.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_Execute"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/TexUI_TalentIcon_LAtk_Execute.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_Sharpen"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/TexUI_TalentIcon_LAtk_Sharpen.dds"), MEMORY_TYPE::MEMORY_STATIC);
	
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_Execution_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/Execution_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_Execution_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/Execution_Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_HealingExecution0_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/HealingExecution_0_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_HealingExecution0_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/HealingExceution_0_Information.png"), MEMORY_TYPE::MEMORY_STATIC);
	
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_HealingExecution1_Title"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/HealingExecution_1_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Talent_Icon_HealingExecution1_Information"), TEXT("../Bin/Resources/Textures/UI/Icons/Talents/LightAttack/Text/HealingExecution_1_Information.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("LockedIcon"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_LockedIcon.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Test"), TEXT("../Bin/Resources/Textures/Mask/545.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("MediaFrame"), TEXT("../Bin/Resources/Textures/UI/TexUI_MediaFrame.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Talent_Information_BG"), TEXT("../Bin/Resources/Textures/UI/TexUI_TipDialogBackground.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("None"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_None.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("NoneBlack"), TEXT("../Bin/Resources/Textures/UI/Black.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("UI_None"), TEXT("../Bin/Resources/Textures/UI/UI_None.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("UI_EvolveMenu_Level_BG_Mask"), TEXT("../Bin/Resources/Textures/Mask/648.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("UI_Corvus"), TEXT("../Bin/Resources/Textures/UI/corvus.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("HighLight"), TEXT("../Bin/Resources/Textures/UI/TexUI_BoxHighlightEffect.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_LeftBG_Mask"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_LeftMask2.png"), MEMORY_TYPE::MEMORY_STATIC);

	//interaction
	LOAD_TEXTURES_USE_THREAD(("Interaction_Open"), TEXT("../Bin/Resources/Textures/UI/Interaction/Open.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Interaction_Climb"), TEXT("../Bin/Resources/Textures/UI/Interaction/Climb.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Interaction_Elevator"), TEXT("../Bin/Resources/Textures/UI/Interaction/Elevator.png"), MEMORY_TYPE::MEMORY_STATIC);



	//battle
	LOAD_TEXTURES_USE_THREAD(("Target_Icon"), TEXT("../Bin/Resources/Textures/UI/Target.png"), MEMORY_TYPE::MEMORY_STATIC);


	//Inventory
	LOAD_TEXTURES_USE_THREAD(("Inventory_BG"), TEXT("../Bin/Resources/Textures/UI/Inventory/UI_InventoryBG.dds"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("Inventory_Frame"), TEXT("../Bin/Resources/Textures/UI/Inventory/Inventory_MainFrame.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Inventory_FrameBG"), TEXT("../Bin/Resources/Textures/UI/Inventory/Inventory_MainFrameBG.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("ItemSlot_Main"), TEXT("../Bin/Resources/Textures/UI/Inventory/TexUI_ItemBackground.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("ItemSlot_Frame"), TEXT("../Bin/Resources/Textures/UI/Inventory/TexUI_SquareFrame.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("ItemSlot_Hover"), TEXT("../Bin/Resources/Textures/UI/Inventory/TexUI_SquareFrame_Hover.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Inventory_SortKey"), TEXT("../Bin/Resources/Textures/UI/Inventory/SortKey.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Inventory_SortByDate"), TEXT("../Bin/Resources/Textures/UI/Inventory/Sort_RootingDate.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Inventory_SortByQuantity"), TEXT("../Bin/Resources/Textures/UI/Inventory/Sort_Quantity.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Inventory_SortByType"), TEXT("../Bin/Resources/Textures/UI/Inventory/Sort_Type.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Cursor"), TEXT("../Bin/Resources/Textures/UI/Cursor.dds"), MEMORY_TYPE::MEMORY_STATIC);


	//Scroll
	LOAD_TEXTURES_USE_THREAD(("Scroll_Head"), TEXT("../Bin/Resources/Textures/UI/TexUI_ScrollBarTrack_Edge.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Scroll_Tail"), TEXT("../Bin/Resources/Textures/UI/TexUI_ScrollBarTrack_EdgeBottom.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Scroll_Border"), TEXT("../Bin/Resources/Textures/UI/TexUI_ScrollBarTrack_Middle.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Scroll_Track"), TEXT("../Bin/Resources/Textures/UI/TexUI_ScrollBarThumb_Middle.png"), MEMORY_TYPE::MEMORY_STATIC);



	//Item
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Basil"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Ingredients/TexUI_IngredientIcon_Basil.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Cinnamon"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Ingredients/TexUI_IngredientIcon_Cinnamon.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_BlackPepper"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Ingredients/TexUI_IngredientIcon_BlackPepper.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Thyme"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Ingredients/TexUI_IngredientIcon_Thyme.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Key01"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Puzzle/TexUI_PuzzleIcon_Key01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Key02"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Puzzle/TexUI_PuzzleIcon_Key02.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Memory01"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Useable/TexUI_MemoryCollectionIcon_01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Icon_Memory02"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/Useable/TexUI_MemoryCollectionIcon_02.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Item_Icon_SkillPiece"), TEXT("../Bin/Resources/Textures/UI/Icons/Items/TexUI_PureBloodCoreIcon.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Item_Type_Ingredient"), TEXT("../Bin/Resources/Textures/UI/ItemData/Type_Ingredient.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Type_SkillPiece"), TEXT("../Bin/Resources/Textures/UI/ItemData/Type_SkillPiece.png"), MEMORY_TYPE::MEMORY_STATIC);

	
	LOAD_TEXTURES_USE_THREAD(("Item_Type_Common"), TEXT("../Bin/Resources/Textures/UI/ItemData/Type_Common.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Type_Consumption"), TEXT("../Bin/Resources/Textures/UI/ItemData/Type_Consumption.png"), MEMORY_TYPE::MEMORY_STATIC);



	LOAD_TEXTURES_USE_THREAD(("Item_Quantity"), TEXT("../Bin/Resources/Textures/UI/ItemData/Quantity.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Basil
	LOAD_TEXTURES_USE_THREAD(("Item_Basil_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/Basil/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Basil_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/Basil/Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Cinnamon
	LOAD_TEXTURES_USE_THREAD(("Item_Cinnamon_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/Cinnamon/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Cinnamon_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/Cinnamon/Information.png"), MEMORY_TYPE::MEMORY_STATIC);



	//Thyme
	LOAD_TEXTURES_USE_THREAD(("Item_Thyme_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/Thyme/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Thyme_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/Thyme/Information.png"), MEMORY_TYPE::MEMORY_STATIC);


	//GardenKey
	LOAD_TEXTURES_USE_THREAD(("Item_GardenKey_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/GardenKey/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_GardenKey_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/GardenKey/Information.png"), MEMORY_TYPE::MEMORY_STATIC);
	

	//VargKey
	LOAD_TEXTURES_USE_THREAD(("Item_VargKey_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/VargKey/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_VargKey_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/VargKey/Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	//VargKey
	LOAD_TEXTURES_USE_THREAD(("Item_VargKey_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/VargKey/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_VargKey_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/VargKey/Information.png"), MEMORY_TYPE::MEMORY_STATIC);



	//Memory01
	LOAD_TEXTURES_USE_THREAD(("Item_Memory01_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/Memory01/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Memory01_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/Memory01/Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Memory02
	LOAD_TEXTURES_USE_THREAD(("Item_Memory02_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/Memory02/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_Memory02_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/Memory02/Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	//SkillPiece
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_Axe_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/Axe_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_Hammer_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/Hammer_Title.png"), MEMORY_TYPE::MEMORY_STATIC);	
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_Knife_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/Knife_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_VargSword_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/VargSword_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_Scythe_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/Scythe_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_BloodStorm_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/BloodStorm_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_Halberd_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/Halberd_Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_BigHand_Title"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/BigHand_Title.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Item_SkillPiece_Information"), TEXT("../Bin/Resources/Textures/UI/ItemData/SkillPiece/Information.png"), MEMORY_TYPE::MEMORY_STATIC);

	//Popup
	LOAD_TEXTURES_USE_THREAD(("Popup_Item_Basil"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_Basil.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_Item_Cinnamon"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_Cinnamon.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Popup_Item_Thyme"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_Thyme.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_Item_GardenKey"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_GardenKey.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_Item_VargKey"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_VargKey.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Popup_Item_Memory01"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_Memory01.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_Item_Memory02"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_Memory02.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_Axe"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_Axe.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_Hammer"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_Hammer.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_Knife"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_Knife.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_Scythe"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_Scythe.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_Varg"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_Varg.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_BloodStorm"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_BloodStorm.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_Halberd"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_Halberds.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Popup_SkillPiece_BigHand"), TEXT("../Bin/Resources/Textures/UI/ItemData/Popup/Popup_SkillPiece_BigHandMan.png.dds"), MEMORY_TYPE::MEMORY_STATIC);

	//UI _ ItemRequirement	
	LOAD_TEXTURES_USE_THREAD(("ItemRequirement_GardenKey"), TEXT("../Bin/Resources/Textures/UI/ItemRequirement/Requirement_InnerGardenKey.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("ItemRequirement_VargKey"), TEXT("../Bin/Resources/Textures/UI/ItemRequirement/Requirement_VargKey.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Requirement_Direction"), TEXT("../Bin/Resources/Textures/UI/ItemRequirement/Requirement_Direction.dds"), MEMORY_TYPE::MEMORY_STATIC);



	LOAD_TEXTURES_USE_THREAD(("Use_GardenKey"), TEXT("../Bin/Resources/Textures/UI/ItemRequirement/Use_InnerGardenKey.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Use_VargKey"), TEXT("../Bin/Resources/Textures/UI/ItemRequirement/Use_VargKey.dds"), MEMORY_TYPE::MEMORY_STATIC);

	//BloodOverlay
	LOAD_TEXTURES_USE_THREAD(("BloodOverlay_Mask"), TEXT("../Bin/Resources/Textures/UI/BloodOverlay/Mask.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("BloodOverlay"), TEXT("../Bin/Resources/Textures/UI/BloodOverlay/Texture.dds"), MEMORY_TYPE::MEMORY_STATIC);

	//VargAppear
	LOAD_TEXTURES_USE_THREAD(("Varg_Appear_SliceTop"), TEXT("../Bin/Resources/Textures/UI/AppearEvent/Varg/Slice_Top.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Varg_Appear_SliceBottom"), TEXT("../Bin/Resources/Textures/UI/AppearEvent/Varg/Slice_Bottom.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("Varg_Appear_SliceTop_BG"), TEXT("../Bin/Resources/Textures/UI/AppearEvent/Varg/Slice_Top_BG.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("Varg_Appear_SliceBottom_BG"), TEXT("../Bin/Resources/Textures/UI/AppearEvent/Varg/Slice_Bottom_BG.png"), MEMORY_TYPE::MEMORY_STATIC);

	//EvolveMenu_Skill
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Skill_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Title.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_SkillView_Frame"), TEXT("../Bin/Resources/Textures/UI/General/SkillBackground.dds"), MEMORY_TYPE::MEMORY_STATIC);

	

	//EvolveMenu_Information_UnlockHintUI;
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_UnlockSkillHintBackground"), TEXT("../Bin/Resources/Textures/UI/General/TexUI_UnlockSkillHintBackground.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_TextEnoughSkillPiece"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Text_EnoughSkillPiece.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_TextNotEnoughSkillPiece"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Text_NotEnoughSkillPiece.png"), MEMORY_TYPE::MEMORY_STATIC);


	//EvolveMenu_Information_SkillInfo

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_PW_Background_Right"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu/PlagueWeapon/TexUI_PW_Background_Right.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("PW_Axe_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Axe/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_Axe_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Axe/Information0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_Knife_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Knife/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_Knife_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Knife/Information0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_VargSword_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/VargSword/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_VargSword_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/VargSword/Information0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_Hammer_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Hammer/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_Hammer_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Hammer/Information0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_Scythe_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Scythe/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_Scythe_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Scythe/Information0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_BloodStorm_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/BloodStorm/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_BloodStorm_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/BloodStorm/Information0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_Halberds_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Halberd/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_Halberds_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/Halberd/Informaiton0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("PW_BigHand_Title"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/BigHand/Title.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("PW_BigHand_Information"), TEXT("../Bin/Resources/Textures/UI/EvolveMenu_PW/Informaiton/BigHand/Informaiton0.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("SkillOpen_TitleText"), TEXT("../Bin/Resources/Textures/UI/UI_EffectGroup/SkillOpen/TitleText.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("SkillOpen_SkillName"), TEXT("../Bin/Resources/Textures/UI/UI_EffectGroup/SkillOpen/SkillName%d.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_Defualt"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/SettingBox_Long.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_Select"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/SettingBox_HighLight_Long.png"), MEMORY_TYPE::MEMORY_STATIC);


	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_BGHead"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/Background_Head.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_BGBody"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/Background_Body.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_BGBottom"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/Background_Bottom.dds"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_On"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/OnOff/On.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_Off"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/OnOff/Off.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_TypeText"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/OptionTypeText/%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_Quality"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/Quality/%d.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("UI_Slider_BG"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/SliderBG.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("UI_Slider_Fill"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/SliderFill.png"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("UI_Slider_Track"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/TexUI_SliderBarThumb_Hover.png"), MEMORY_TYPE::MEMORY_STATIC);

	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_BGHead"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/Background_Head.dds"), MEMORY_TYPE::MEMORY_STATIC);
	LOAD_TEXTURES_USE_THREAD(("EvolveMenu_Option_BGBody"), TEXT("../Bin/Resources/Textures/UI/PauseMenu/OptionsMenu/Background_Body.dds"), MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_CorvusModel()
{
	_matrix TransformMatrix = XMMatrixIdentity();

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Corvus", "../Bin/Resources/Meshes/Corvus/Corvus.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC, true);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("CorvusDefaultSaber", "../Bin/Resources/Meshes/Corvus/Weapon/Corvus_DefaultSaber/Corvus_DefaultSaber.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("CorvusDefaultDagger", "../Bin/Resources/Meshes/Corvus/Weapon/Corvus_DefaultDagger/Corvus_DefaultDagger2.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_NormalMobModel()
{
	_matrix TransformMatrix = XMMatrixIdentity();

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Armorman", "../Bin/Resources/Meshes/NorMonster/ArmorMan/ArmorMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Shieldman", "../Bin/Resources/Meshes/NorMonster/ShieldMan/ShieldMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
	Load_Model_UseThread("WeakArmorMan", "../Bin/Resources/Meshes/NorMonster/WeakArmorMan/WeakArmorMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Skullman", "../Bin/Resources/Meshes/NorMonster/SkullMan/SkullMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Spearman", "../Bin/Resources/Meshes/NorMonster/SpearMan/SpearMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Sword", "../Bin/Resources/Meshes/NorMonster/Weapon/Sword/Sword.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Spear", "../Bin/Resources/Meshes/NorMonster/Weapon/Spear/Spear.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Shield2", "../Bin/Resources/Meshes/NorMonster/Weapon/Shield2/Shield2.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Shield1", "../Bin/Resources/Meshes/NorMonster/Weapon/Shield1/Shield1.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Halberd", "../Bin/Resources/Meshes/NorMonster/Weapon/Halberd/Halberd.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
	//

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Balloon", "../Bin/Resources/Meshes/NorMonster/Balloon/Balloon.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.0037f, 0.0037f, 0.0037f);
	Load_Model_UseThread("Mon_AxeMan", "../Bin/Resources/Meshes/NorMonster/AxeMan/SK_C_LV1Villager_M_Skeleton.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_KnifeWoman", "../Bin/Resources/Meshes/NorMonster/KnifeWoman/SK_C_LV1Villager_F_Skeleton.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Gardner", "../Bin/Resources/Meshes/NorMonster/Gardner/Gardner.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Axe", "../Bin/Resources/Meshes/NorMonster/Weapon/Axe/Axe2.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Knife", "../Bin/Resources/Meshes/NorMonster/Weapon/Knife/Knife.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Scythe", "../Bin/Resources/Meshes/NorMonster/Weapon/Scythe/Scythe.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(190.f)) * XMMatrixRotationY(XMConvertToRadians(-170.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Weapon_Shield", "../Bin/Resources/Meshes/NorMonster/Weapon/Shield/Shield.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(-110.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Vain", "../Bin/Resources/Meshes/NorMonster/Gardner/vain/Vain.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-180.f)) * XMMatrixRotationY(XMConvertToRadians(-40.f)) * XMMatrixRotationZ(XMConvertToRadians(50.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Vine", "../Bin/Resources/Meshes/NorMonster/Gardner/Vine.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-20.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Scarf", "../Bin/Resources/Meshes/NorMonster/Gardner/Scarf.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(-40.f)) * XMMatrixRotationZ(XMConvertToRadians(50.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Vine", "../Bin/Resources/Meshes/NorMonster/Gardner/Vine.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(10.f)) * XMMatrixRotationY(XMConvertToRadians(10.f)) * XMMatrixRotationZ(XMConvertToRadians(10.f)) * XMMatrixScaling(0.1f, 0.1f, 0.1f);
	Load_Model_UseThread("Mon_Mutation1", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation1.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Mutation2", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation2.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Mutation3", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation3.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Mutation4", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation4.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Mutation5", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation5.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Mutation6", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation6.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(0.f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Mon_Mutation7", "../Bin/Resources/Meshes/NorMonster/Mutation/Mutation7.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_RareMobModel()
{

}

void CLoader::Load_EliteMobModel()
{
	_matrix TransformMatrix = XMMatrixIdentity();

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixScaling(0.0f, 0.0037f, 0.0037f);
	Load_Model_UseThread("Elite_Joker", "../Bin/Resources/Meshes/EliteMonster/Joker/Joker.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.014f, 0.014f, 0.014f);
	Load_Model_UseThread("Elite_BigHandman", "../Bin/Resources/Meshes/EliteMonster/BigHandMan/BigHandMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Elite_GreatSwordman", "../Bin/Resources/Meshes/EliteMonster/GreatSwordMan/GreatSwordMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Elite_TwinSwordman", "../Bin/Resources/Meshes/EliteMonster/TwinSwordMan/TwinSwordMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Elite_TwinSwordWoman", "../Bin/Resources/Meshes/EliteMonster/TwinSwordWoman/TwinSwordMan.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Joker_Weapon", "../Bin/Resources/Meshes/EliteMonster/Joker/Hammer/Hammer.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_BossMobModel()
{
	_matrix TransformMatrix = XMMatrixIdentity();

	//TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixScaling(0.0035f, 0.0035f, 0.0035f);
	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Boss_Varg", "../Bin/Resources/Meshes/Boss/Varg/Varg.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	//TransformMatrix = XMMatrixRotationZ(XMConvertToRadians(-90.0f)) * XMMatrixRotationX(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixRotationZ(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Boss_VargWeapon", "../Bin/Resources/Meshes/Boss/Varg/Weapon/VargWeapon.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	 /*TransformMatrix = XMMatrixRotationX() * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	 GAMEINSTANCE->Load_Model_UseThread("Boss_VargWeapon", "../Bin/Resources/Meshes/Boss/Varg/Weapon/VargWeapon.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);*/

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixScaling(0.0037f, 0.0037f, 0.0037f);
	Load_Model_UseThread("Boss_Urd", "../Bin/Resources/Meshes/Boss/Urd/Urd2.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC, true);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(290.0f)) * XMMatrixRotationY(XMConvertToRadians(0.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Boss_UrdWeapon", "../Bin/Resources/Meshes/Boss/Urd/Weapon/SK_W_UrdSword01.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(260.f)) * XMMatrixRotationZ(XMConvertToRadians(90.f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Boss_UrdWeapon2", "../Bin/Resources/Meshes/Boss/Urd/Weapon/SK_W_UrdSword02.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixScaling(0.0037f, 0.0037f, 0.0037f);
	Load_Model_UseThread("Boss_Bat", "../Bin/Resources/Meshes/Boss/Bat/Bat.fbx", MODEL_TYPE::ANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC,true);

	TransformMatrix = XMMatrixRotationZ(XMConvertToRadians(-90.0f)) * XMMatrixRotationX(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);
	Load_Model_UseThread("Boss_BatWeapon", "../Bin/Resources/Meshes/Boss/Bat/Weapon/BushBottom.fbx", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);

	//Masking
	GAMEINSTANCE->Load_Textures(("Mask"), TEXT("../Bin/Resources/Textures/Mask/193.png"), MEMORY_TYPE::MEMORY_STATIC);

	TransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_Model_UseThread("Torch", "../Bin/Resources/Meshes/LightProp/Torch/Torch.FBX", MODEL_TYPE::NONANIM, TransformMatrix, MEMORY_TYPE::MEMORY_STATIC);
}

void CLoader::Load_AllMapModel()
{
	lstrcpy(m_szLoadingText, TEXT("Loading all Ground Info..."));
	Load_AllTexture("../Bin/Resources/Textures/Prop/", MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllTexture("../Bin/GroundInfo/Texture/"     , MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllTexture("../Bin/GroundInfo/Filter/"      , MEMORY_TYPE::MEMORY_DYNAMIC);

	lstrcpy(m_szLoadingText, TEXT("Loading all Prop Mesh Info..."));
	Load_AllMeshes("../Bin/Resources/Meshes/ForTest_Mesh/"           , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Else/Binary/"        , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv1_Circus/Binary/"  , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv2_Fortress/Binary/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Map_Lv3_Garden/Binary/"  , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/GroundInfo/Mesh/"                         , MODEL_TYPE::GROUND , MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/NPC/Binary/"			 , MODEL_TYPE::ANIM   , MEMORY_TYPE::MEMORY_DYNAMIC);
	Load_AllMeshes("../Bin/Resources/Meshes/Prop/Binary/"            , MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_DYNAMIC);

	_matrix TransformMatrix;
	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);
	Load_AllMeshes("../Bin/Resources/Meshes/Destructable/Wagon03/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_STATIC, TransformMatrix, ".fbx");

	TransformMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixScaling(0.0001f, 0.0001f, 0.0001f);
	Load_AllMeshes("../Bin/Resources/Meshes/Destructable/Fence_16a/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_STATIC, TransformMatrix, ".fbx");

	TransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	Load_AllMeshes("../Bin/Resources/Meshes/Destructable/Column/", MODEL_TYPE::NONANIM, MEMORY_TYPE::MEMORY_STATIC, TransformMatrix, ".fbx");
}

void CLoader::Load_Model_UseThread(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType, _bool Is_bAnimZero)
{
	shared_ptr<CSubThread_Pool> pClientThread = GET_SINGLE(CGameManager)->Get_ClientThread();

	pClientThread->Enqueue_Job(bind(&CGameInstance::Load_Model, CGameInstance::Get_Instance(), sKey, sModelFilePath, eModelType, In_TransformMatrix, eMemType, Is_bAnimZero));
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
