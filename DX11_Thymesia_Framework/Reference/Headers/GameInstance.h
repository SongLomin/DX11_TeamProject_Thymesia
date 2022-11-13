#pragma once

#include "Graphic_Device.h"
#include "Input_Device.h"
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

/* 1. 게임내에 필요한 객체(매니져등)들을 모아서 보관한다. */
/* 2. 클라이언트 개발자가 접근하기좋은 루트를 제공해준다. 나. */
/* 3. 진짜 필요한 함수만 클라잉언트개발자에게 ㅇ오픈해주낟. */

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

public: /* 엔진 라이브럴를 사용할때 필요한 초기화 과정으르 거칠거야. */
	HRESULT Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, _uint iNumTimeScales,  _uint iNumCollsionLayer, const GRAPHICDESC& GraphicDesc);
	HRESULT Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear(_uint iLevelID);

	HRESULT LevelEnter();
	HRESULT LevelExit();

	_float Get_DeltaTime() const;
	_bool  Is_Debug() const { return m_bDebug; }

	HWND	Get_WindowHandle() const;

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
		int i = 0;
		return m_pObject_Manager->Add_GameObject<T>(iLevelIndex, pArg);
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
	HRESULT Set_MotionBlurScale(const _float& In_fScale);
	HRESULT Add_MotionBlurScale(const _float& In_fScale);
	HRESULT	Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt);


public: /* For.PipeLine */
	void Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState);
	_matrix Get_Transform(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4* Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState);

	_float4 Get_CamPosition();


public: /* For.Resource_Manager */
	HRESULT Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC);
	//vector*를 반환하지 않는 이유: 셰이더리소스뷰의 레퍼런스 카운트를 올리기 위해서 vector*를 반환하지 않는다.
	vector<ComPtr<ID3D11ShaderResourceView>> Get_TexturesFromKey(const _char* _Str_Key, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	
	HRESULT Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType = MEMORY_TYPE::MEMORY_STATIC,const _bool Is_bAnimZero = false);
	shared_ptr<MODEL_DATA> Get_ModelFromKey(const _char* _sKey, MEMORY_TYPE _eType = MEMORY_TYPE::MEMORY_END);
	vector<string> Get_AllModelKeys();
	vector<string> Get_AllNoneAnimModelKeys();
	vector<string> Get_AllAnimModelKeys();

	HRESULT Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath);
	ID3DX11Effect* Get_ShaderEffect(const _tchar* sKey);

	HRESULT Release_ResourceByMemoryType(MEMORY_TYPE _eMemType);

public: /* For.Font_Manager */
	HRESULT Add_Font(_uint iFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(_uint iFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor);
	void	Render_Font(_uint iFontTag);
	void	Add_Text(const _uint& iFontTag, const tstring& pString, const _float2& vPosition, _fvector vColor, const _bool& bAlways, _bool bCenterAlign = false);
	void	Add_Text(const _uint& iFontTag, const TEXTINFO& In_tTextInfo);
	
public: /* For.Event_Manager */
	void					Reserve_Event(_uint iMaxIndex);
	FDelegate<void*>&		Get_Event(_uint iIndex);

public: /* For.Collision_Manager */
	void Check_Group(const _uint& In_iLeftLayer, const _uint& In_iRightLayer);

public: /* For.Light_Manager */
	const LIGHTDESC& Get_LightDesc(_uint iIndex) const;
	void Set_LightDesc(const LIGHTDESC& LightDesc);
	_uint Add_Light(const LIGHTDESC& LightDesc); 
	_bool Remove_Light(const _uint& iIndex);

public: /* For.Frustum */
	_bool isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange = 0.f);


public: /* For.Target_Manager */
	ComPtr<ID3D11ShaderResourceView> Get_RenderTarget_SRV(const _tchar* pTargetTag);

public: /* For.Sound_Manager */
	_int  VolumeUp(CHANNELID eID, _float _vol);
	_int  VolumeDown(CHANNELID eID, _float _vol);
	_int  BGMVolumeUp(_float _vol);
	_int  BGMVolumeDown(_float _vol);
	_int  Pause(CHANNELID eID);
	_uint PlaySound(const TCHAR* pSoundKey, _uint _iIndex, _float _vol);
	_uint PlaySound(const TCHAR* pSoundKey, _float _vol);
	void PlayBGM(const TCHAR* pSoundKey, _float _vol);
	void StopSound(_uint _iChannelIndex);
	void StopAll();

public: /* For.PhysX_Manager */
	HRESULT			Create_Scene(CPhysX_Manager::Scene eScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));
	HRESULT			Delete_Scene(CPhysX_Manager::Scene eScene);
	HRESULT			Change_Scene(CPhysX_Manager::Scene eNextScene, PxVec3 Gravity = PxVec3(0.0f, -9.81f, 0.0f));

	void			Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut);
	void			Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut);
	void			Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut);
	void			Create_Shape(const PxGeometry& Geometry, PxMaterial* pMaterial, PxShape** ppOut);

private:
	shared_ptr<CGraphic_Device> m_pGraphic_Device;
	shared_ptr<CInput_Device> m_pInput_Device;
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
	

private:
	_float	m_fDeltaTime = 0.f;
	_bool	m_bDebug = false;
	HWND	m_WindowHandle;


public:
	static void Release_Engine();
	void Free();
};

END