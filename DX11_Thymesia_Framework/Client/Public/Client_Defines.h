#pragma once
#include "Engine_Defines.h"
#include "SMath.h"
#include <filesystem>

// #define _JOJO_EFFECT_TOOL_
                 
#include "Client_Structs.h"
#include "Client_Presets.h"
#include "Client_Enum.h"	
#include "imgui.h"

//////////////// DEBUG MODE DEFINES ////////////////
#ifdef _DEBUG

#pragma region System
//#define _ONLY_UI_
//#define _BAKE_MIPMAPS_
#define _USE_THREAD_
#define _144HZ_
#define _RENDER_FPS_
#define _Actor_Culling_
//#define _INSTANCE_CULLING_t
// #define _LOAD_CAPTURED_RESOURCE_
#pragma endregion // System

#define _SKYBOX_

#define _USE_GRAVITY_
 #define _LIFEGUARD_FOR_FALL_

#pragma region Console Outputs
#define _DEBUG_COUT_
// #define _DEBUG_COUT_JOJO
#pragma endregion // Console Outputs

#pragma region Map
#define _BRIGHT_LIGHT_
// #define _GENERATE_PROP_COLLIDER_ true
// #define _STAGE_1_
#define _STAGE_2_
#define _TEST_STATIC_PROPS_
#define _TEST_DYNAMIC_PROPS_
#pragma endregion // Map

#pragma region Tool
#define _MAP_TOOL_
// #define _EFFECT_TOOL_
#pragma endregion // Tool

#pragma region Boss Mobs
 #define _BOSS_VARG_
// #define _BOSS_URD_
#pragma endregion // Boss Mobs

#pragma region Normal Mobs
#define _STAGE_1_MONSTER_
// #define _STAGE_2_MONSTER_
#pragma endregion // Normal Mobs

#pragma region Effects
// #define _DAGGER_TRAIL_
#define _CORVUS_EFFECT_
#define _JOJO_EFFECTS_
#undef _BAKE_PARTICLE_
#pragma endregion // Effects

#endif // _DEBUG
//////////////// DEBUG MODE DEFINES ////////////////

//////////////// RELEASE MODE DEFINES ////////////////
#ifdef NDEBUG

#undef _JOJO_EFFECT_TOOL_
#undef _ONLY_UI_

#pragma region System
#define _USE_THREAD_
#define _144HZ_
#define _RENDER_FPS_
//#define	_INSTANCE_CULLING_
#define _LOAD_CAPTURED_RESOURCE_
#define     _Actor_Culling_
#pragma endregion // System

#define _SKYBOX_

#define _USE_GRAVITY_
// #define _LIFEGUARD_FOR_FALL_

#pragma region Map
// #define _BRIGHT_LIGHT_
#define _GENERATE_PROP_COLLIDER_ true
// #define _STAGE_1_
#define _STAGE_2_
// #define _MAP_TOOL_
#pragma endregion // Map

#pragma region Boss Mobs
#define _BOSS_VARG_
// #define _BOSS_URD_
#pragma endregion // Boss Mobs

#pragma region Normal Mobs
#define _STAGE_1_MONSTER_
#define _STAGE_2_MONSTER_
#pragma endregion // Normal Mobs

#pragma region Effects
// #define _DAGGER_TRAIL_
#define _CORVUS_EFFECT_
#define _JOJO_EFFECTS_
#pragma endregion // Effects

#endif // NDEBUG
//////////////// RELEASE MODE DEFINES ////////////////

/////////////// For. JoJo Effect Tool ///////////////
#ifdef _JOJO_EFFECT_TOOL_

#pragma region System
// #define _ONLY_UI_
#define _USE_THREAD_
#define _144HZ_
#define _RENDER_FPS_
#undef _INSTANCE_CULLING_
#undef _LOAD_CAPTURED_RESOURCE_
#pragma endregion // System

#undef _USE_GRAVITY_
#undef _LIFEGUARD_FOR_FALL_

#pragma region Console Outputs
#undef _DEBUG_COUT_
#define _DEBUG_COUT_JOJO
#pragma endregion // Console Outputs

#pragma region Map
#undef _BRIGHT_LIGHT_
#undef _GENERATE_PROP_COLLIDER_ true
#define _STAGE_1_
#undef _STAGE_2_
#undef _TEST_STATIC_PROPS_
#undef _TEST_DYNAMIC_PROPS_
#pragma endregion // Map

#pragma region Tool
#undef _MAP_TOOL_
#define _EFFECT_TOOL_
#pragma endregion // Tool

#pragma region Boss Mobs
#undef _BOSS_VARG_
#undef _BOSS_URD_
#pragma endregion // Boss Mobs

#pragma region Normal Mobs
#undef _STAGE_1_MONSTER_
#undef _STAGE_2_MONSTER_
#pragma endregion // Normal Mobs

#pragma region Effects
#undef _DAGGER_TRAIL_
#define _CORVUS_EFFECT_
#undef _JOJO_EFFECTS_
#define _ANIMATION_TOOL_WEAPON_
// #define _BAKE_EFFECTMESH_FBX_
#pragma endregion // Effects

#endif // _JOJO_EFFECT_TOOL_
/////////////// For. JoJo Effect Tool ///////////////

#define CLIENT_DECLATION_UI class CUI; class CCustomUI; class CProgressBar; class CHUD_Hover;
#define ENGINE_DECLATION_UI class CTexture; class CShader;   
#define ADD_STATIC_CUSTOMUI GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC)

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace Client;
namespace fs = std::filesystem;

#define EVENT_DRAW_EDITER 1
#define EVENT_INIT_EDITER 0
