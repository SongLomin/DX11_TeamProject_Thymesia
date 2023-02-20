#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Cuda_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Timer_Manager.h"
#include "Render_Manager.h"
#include "PipeLine.h"
#include "Resource_Manager.h"
#include "Font_Manager.h"
#include "Event_Manager.h"
#include "Collision_Manager.h"
#include "LightManager.h"
#include "ModelData.h"
#include "RenderTarget_Manager.h"
#include "Frustum.h"
#include "SoundManager.h"
#include "PhysX_Manager.h"
#include "Thread_Manager.h"
#include "NvCloth_Manager.h"
#include "Optimization_Manager.h"

/* 1. 게임내에 필요한 객체(매니져등)들을 모아서 보관한다. */
/* 2. 클라이언트 개발자가 접근하기좋은 루트를 제공해준다. 나. */
/* 3. 진짜 필요한 함수만 클라잉언트개발자에게 ㅇ오픈해주낟. */

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

public: /* 엔진 라이브럴를 사용할때 필요한 초기화 과정으르 거칠거야. */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, _uint iNumTimeScales,  _uint iNumCollsionLayer, const GRAPHICDESC& GraphicDesc, _uint iNumThread);
	HRESULT Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelID);

	HRESULT LevelEnter();
	HRESULT LevelExit();

	_float Get_DeltaTime() const;
	_bool  Is_Debug() const { return m_bDebug; }
	_uint	Get_LoopIndex() const { return m_iLoopIndex; }

	HWND	Get_WindowHandle() const;
	GRAPHICDESC Get_GraphicDesc() const { return m_GraphicDesc; }


public: /* For.Graphic_Device */	
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();

	ID3D11Device* Get_Device();
	ID3D11DeviceContext* Get_DeviceContext();


public: /* For.Input_Device */
	_bool Is_KeyState(KEY _Key, KEY_STATE _KeyState);
	_byte Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID);
	_long Get_DIMouseMoveState(MOUSEMOVE eMouseMove);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iLevelID, shared_ptr<class CLevel> pLevel);
	weak_ptr<CLevel> Get_CurrentLevel();
	_uint	Get_LastCreatedLevelIndex();
	void	Set_CreatedLevelIndex(_uint iCreatedLevelIndex);

public: /* For.Object_Manager */
	weak_ptr<CGameObject> Add_GameObject(size_t iTypeHash, _uint iLevelIndex,void* pArg = nullptr);


	template <typename T>
	weak_ptr<T> Add_Prototype_GameObject()
	{
		return m_pObject_Manager->Add_Prototype<T>();
	}
	template <typename T>
	weak_ptr<T> Add_GameObject(_uint iLevelIndex, void* pArg = nullptr)
	{
		return m_pObject_Manager->Add_GameObject<T>(iLevelIndex, pArg);
	}

	template <typename T>
	weak_ptr<T> Add_SingleGameObject(_uint iLevelIndex, /*CTransform* pParent = nullptr,*/ void* pArg = nullptr)
	{
		return m_pObject_Manager->Add_SingleGameObject<T>(iLevelIndex, pArg);
	}

	template <typename T>
	list<weak_ptr<T>> Get_GameObjects(_uint iLevelIndex)
	{
		return m_pObject_Manager->Get_GameObjects<T>(iLevelIndex);
	}

	template<typename T>
	weak_ptr<T> Get_GameObject_UseMemoryPool(_uint iLevelIndex)
	{
		return m_pObject_Manager->Get_GameObject_UseMemoryPool<T>(iLevelIndex);
	}

	FDelegate<>& Get_CallbackStart();


public: /* For.Component_Mananger */
	//HRESULT Add_Prototype_Component(const _char* pPrototypeTag, class CComponent* pPrototype);
	template <typename T>
	shared_ptr<T> Clone_Component(void* pArg, weak_ptr<CGameObject> pOwner)
	{

		return m_pComponent_Manager->Clone_Component<T>(pArg, pOwner);
	}

	
public: /* For.Timer_Manager */
	HRESULT Add_Timer(_uint eTimer);
	HRESULT Add_TimerEvent(_uint _iEventNum, weak_ptr<CBase> _Instance, _float _fTime, _bool _bLoop = false, _bool _bUseTimeScale = false, _bool _bAlwaysCall = false);
	_float Compute_Timer(_uint eTimer);

	_float Get_TimeScale(_uint In_iIndex);
	void Set_TimeScale(_uint In_iIndex, _float In_fTimeScale);

public: /* For.Render_Manager */
	HRESULT Add_RenderGroup(RENDERGROUP	eGroup, weak_ptr<CGameObject> pGameObject);
	HRESULT Draw_RenderGroup();
	HRESULT Set_Chromatic(const _float In_fChormaticStrangth);
	HRESULT Add_Chromatic(const _float In_fChormaticStrangth);
	HRESULT Set_MotionBlur(const _float In_fBlurScale);
	HRESULT Add_MotionBlur(const _float In_fBlurScale);
	HRESULT Set_RadialBlur(const _float In_fRadialBlurStength, _float3 In_vBlurWorldPosition);
	HRESULT Add_RadialBlur(const _float In_fRadialBlurStrength);
	HRESULT Set_LiftGammaGain(const _float4 In_vLift, const _float4 In_vGamma, const _float4 In_vGain);
	HRESULT Set_FogDesc(const _float4 In_vFogColor, const _float In_fFogRange);
	HRESULT	Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt);
	HRESULT	Set_DynamicShadowLight(_fvector In_vEye, _fvector In_vLookAt);
	HRESULT Set_GrayScale(const _float In_fGrayScale);
	HRESULT Set_Exposure(const _float In_fExposure);
	_float4 Get_FogColor();
	_float Get_FogRange();
	LIFTGAMMAGAIN_DESC& Get_LiftGammaGain();
	HRESULT Set_Contrast(const _float In_fContrast);
	HRESULT Set_Saturation(const _float In_fSaturation);
	HRESULT Set_IrradianceMap(const _char* In_szIrradianceMap);
	HRESULT Set_IrradianceColorScale(const _float3& In_vIrradianceColorScale);
	_float3 Get_IrradianceColorScale();
	HRESULT Set_PreFilteredMap(const _char* In_szPreFiltered);
	HRESULT Set_MaskingTexture(const _char* In_szPreFiltered);
	HRESULT Set_GodRayDesc(const _float4& In_vColor, const _float4& In_vPosition);
	HRESULT Set_GodRayScale(const _float& In_fScale);
	HRESULT Set_Sharpness(const _float In_fSharpness);
	HRESULT Set_ColorInversion(const _float& In_fInversionStrength, const _float& In_fInversionRatio);

	HRESULT Set_MotionBlur_Enable(const _bool& In_bMotionBlurEnable);
	HRESULT Set_GodRay_Enable(const _bool& In_bGodRayEnable);
	HRESULT Set_Chromatic_Enable(const _bool& In_bChromaticEnable);
	HRESULT Set_RadialBlur_Enable(const _bool& In_bRadialBlurEnable);
	HRESULT Set_DynamicShadow_Enable(const _bool& In_bDynamicShadowEnable);
	HRESULT Set_StaticShadow_Enable(const _bool& In_bStaticShadowEnable);
	HRESULT Set_SSAA_Enable(const _bool& In_bSSAAEnable);
	HRESULT Set_HBAO_Enable(const _bool& In_bHBAOEnable);
	HRESULT Set_PBR_Enable(const _bool& In_bPBR);


	HRESULT Set_BrightnessOffset(const _float& In_fBrightnessOffset);
	HRESULT Set_SaturationOffset(const _float& In_fSaturationOffset);
	HRESULT Set_ContrastOffset(const _float& In_fContrastOffset);
	HRESULT Set_SSRLevel(const _uint& In_iSSRLevel);

	ID3D11DeviceContext* Get_BeforeRenderContext();
	void Release_BeforeRenderContext(ID3D11DeviceContext* pDeviceContext);

#ifdef _DEBUG
	HRESULT Set_DebugSize(const _float2 vSize);
	HRESULT Set_OldSchoolView(const _bool bOldSchool);
	HRESULT	Add_DebugSRT(const _tchar* In_szMRTName);
	HRESULT Render_EditTexture(ComPtr<ID3D11ShaderResourceView> pSRV, const _short In_Red, const _short In_Green, const _short In_Blue, const _short In_Alpha);
	HRESULT Extract_Texture(const tstring& In_szFilePath);
#endif // _DEBUG





public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState);
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState);

	_float4 Get_CamPosition();
	void Set_CameraFar(const _float In_fCameraFar);
	_float Get_CameraFar();



public: /* For.Resource_Manager */
	void	Write_JsonUsingResource(const char* In_szFilePath);
	void	Load_ResourcesFromJson(const char* In_szFilePath);

	HRESULT Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC);
	HRESULT Generate_MipMap(const _tchar* pTextureFilePath, const _int In_iMipMapLevels = 8);
	//vector*를 반환하지 않는 이유: 셰이더리소스뷰의 레퍼런스 카운트를 올리기 위해서 vector*를 반환하지 않는다.
	vector<ComPtr<ID3D11ShaderResourceView>> Get_TexturesFromKey(const _char* _Str_Key, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	
	HRESULT Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC,const _bool Is_bAnimZero = false);
	//void Load_Model_UseThread(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC,const _bool Is_bAnimZero = false);
	shared_ptr<MODEL_DATA> Get_ModelFromKey(const _char* _sKey, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	vector<const string*> Get_AllModelKeys();
	vector<const string*> Get_AllNoneAnimModelKeys();
	vector<const string*> Get_AllAnimModelKeys();

	HRESULT Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath);
	ID3DX11Effect* Get_ShaderEffect(const _tchar* sKey);
	HRESULT	ReLoad_AllShader(list<pair<_bool, string>>& Out_CompileMessage);

	HRESULT Release_ResourceByMemoryType(MEMORY_TYPE _eMemType);


public: /* For.Font_Manager */
	HRESULT Add_Font(_uint iFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(_uint iFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor);
	void	Render_Font(_uint iFontTag);
	void	Add_Text(const _uint& iFontTag, const TEXTINFO& In_tTextInfo);
	
public: /* For.Event_Manager */
	void					Reserve_Event(_uint iMaxIndex);
	FDelegate<void*>&		Get_Event(_uint iIndex);

public: /* For.Collision_Manager */
	void Check_Group(const _uint& In_iLeftLayer, const _uint& In_iRightLayer);

public: /* For.Light_Manager */
	const LIGHTDESC& Get_LightDesc(_uint iIndex) const;
	void Set_LightDesc(const LIGHTDESC& LightDesc);
	const LIGHTDESC& Add_Light(const LIGHTDESC& LightDesc);
	_bool Remove_Light(const _uint& iIndex);
	void Clear_Lights();

public: /* For.Frustum */
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange = 0.f);


public: /* For.Target_Manager */
	ComPtr<ID3D11ShaderResourceView> Get_RenderTarget_SRV(const _tchar* pTargetTag);

public: /* For.Sound_Manager */
	void Update_VolumeScale(const _float In_VolumeScale);
	void Update_BGMVolumeScale(const _float In_VolumeScale);
	_int  Pause(CHANNELID eID);
	_uint PlaySound3D(const string& In_szSoundKey, _uint _iIndex, _float _vol, _fvector In_WorldPosition);
	_uint PlaySound3D(const string& In_szSoundKey, _float _vol, _fvector In_WorldPosition, const _float In_fMinDistance = Sound_Min_Distance, const _float In_fMaxDistance = Sound_Max_Distance);
	_uint PlaySound2D(const string& In_szSoundKey, _uint _iIndex, _float _vol);
	_uint PlaySound2D(const string& In_szSoundKey, _float _vol, const _bool isLoop = false);
	void PlayBGM(const string& In_szSoundKey, _float _vol, const _float In_fFadeSound = 0.f);
	void StopBGM();
	void StopSound(_uint _iChannelIndex);
	void StopAll();
	void Load_SoundFileFromFolderPath(const string& In_szFolderPath);
	vector<const string*> Get_AllSoundNames();

public: /* For.PhysX_Manager */
	void			Check_PhysXFilterGroup(const _uint In_iLeftLayer, const _uint In_iRightLayer);

	HRESULT			Create_Scene(CPhysX_Manager::Scene eScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	HRESULT			Delete_Scene(CPhysX_Manager::Scene eScene);
	HRESULT			Change_Scene(CPhysX_Manager::Scene eNextScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));

	void			Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut);
	void			Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut);
	void			Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut);

	weak_ptr<CPhysXCollider>			Find_PhysXCollider(const _uint In_iPhysXColliderIndex);
	weak_ptr<CPhysXController>			Find_PhysXController(const _uint In_iPhysXControllerIndex);
	_uint			Get_PhysXFilterGroup(const _uint In_iIndex);

public: /* For. Thread_Manager */
	/*template <class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> Enqueue_Job(
		F&& f, Args&&... args) 
	{
		return m_pThread_Manager->Enqueue_Job(f, args);
	}

	void Wait_JobDone(const _char* In_szConsoleText = nullptr);*/

public: /* For. RenderTarget_Manager */
	list<const _tchar*> Get_AllSRVNames();

public: /* For. NvCloth_Manager */

public: /* For. Optimization_Manager*/
	void Begin_PerformanceTime(const string& In_szTag);
	void End_PerformanceTime(const string& In_szTag);
	_time Get_PerfromanceTime(const string& In_szTag);
	void Write_Log();
	void Clear_Logs();

private:
	shared_ptr<CGraphic_Device> m_pGraphic_Device;
	shared_ptr<CInput_Device> m_pInput_Device;
	shared_ptr<CCuda_Device> m_pCuda_Device;
	shared_ptr<CLevel_Manager> m_pLevel_Manager;
	shared_ptr<CObject_Manager> m_pObject_Manager;
	shared_ptr<CComponent_Manager> m_pComponent_Manager;
	shared_ptr<CTimer_Manager> m_pTimer_Manager;
	shared_ptr<CRender_Manager> m_pRender_Manager;
	shared_ptr<CPipeLine> m_pPipeLine;
	shared_ptr<CResource_Manager> m_pResource_Manager;
	shared_ptr<CFont_Manager> m_pFont_Manager;
	shared_ptr<CEvent_Manager> m_pEvent_Manager;
	shared_ptr<CCollision_Manager> m_pCollision_Manager;
	shared_ptr<CLight_Manager> m_pLight_Manager;
	shared_ptr<CFrustum> m_pFrustum;
	shared_ptr<CRenderTarget_Manager> m_pTarget_Manager;
	shared_ptr<CSound_Manager> m_pSound_Manager;
	shared_ptr<CPhysX_Manager> m_pPhysX_Manager;
	shared_ptr<CThread_Manager> m_pThread_Manager;
	shared_ptr<CNvCloth_Manager> m_pNvCloth_Manager;
	shared_ptr<COptimization_Manager> m_pOptimization_Manager;



private:
	_float			m_fDeltaTime = 0.f;
	_bool			m_bDebug = false;
	HWND			m_WindowHandle;
	GRAPHICDESC		m_GraphicDesc;
	_uint			m_iLoopIndex = 0;


public:
	static void Release_Engine();
	void Free();
};

END