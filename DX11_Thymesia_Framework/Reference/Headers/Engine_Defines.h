#pragma once
#pragma warning (disable : 4251)
#pragma warning (disable : 4390)
#pragma warning (disable : 6011)
#pragma warning (disable : 26451)
#pragma warning (disable : 26495)
#pragma warning (disable : 26800)
#pragma warning (disable : 26819)
#pragma warning (disable : 28020)
#pragma warning (disable : 33010)

#define HZ_144 0.006944f

#include <d3d11.h>
#include <DirectXMath.h>
#include <Effects11/d3dx11effect.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/ScreenGrab.h>
#include <d3dcompiler.h>

#include <DirectXCollision.h>

#include <DirectXTex.h>



using namespace DirectX;

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define DIRECTINPUT_VERSION  0x0800
#include <dinput.h>

#pragma region PhysX
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
using namespace physx;
#pragma endregion

#include <NvCloth/Factory.h>
#include <NvCloth/Cloth.h>
#include <NvCloth/Callbacks.h>
#include <NvCloth/DxContextManagerCallback.h>
#include <NvCloth/Fabric.h>
#include <NvCloth/Solver.h>
#include <NvCloth/Allocator.h>
#include <NvCloth/Callbacks.h>
#include <NvClothExt/ClothFabricCooker.h>
#include <NvClothExt/ClothMeshDesc.h>
#include <NvClothExt/ClothMeshQuadifier.h>
#include <NvClothExt/ClothTetherCooker.h>

using namespace nv::cloth;

#include <process.h>
#include <iostream>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <functional>
#include <sal.h>
#include <future>
#include <random>
#include <mutex>
#include <queue>
#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"

#include <typeinfo>

#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include "Delegate.h"
#include "HBAO/GFSDK_SSAO.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "dxgidebug.h"

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 

#endif // _DEBUG

#else // _DEBUG
#define DBG_NEW new

#endif // _DEBUG

#ifdef _DEBUG
#define DEBUG_ASSERT assert(false)
#endif

#ifndef _DEBUG
#define DEBUG_ASSERT void(0);
#endif // _DEBUG

using namespace Engine;
using namespace std;

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

#include <io.h>
#pragma comment(lib, "fmodex64_vc.lib")

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include "FMOD/fmod.h"
