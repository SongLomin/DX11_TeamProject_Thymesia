#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, _uint iNumTimeScales, _uint iNumCollsionLayer, const GRAPHICDESC& GraphicDesc)
{
	m_pGraphic_Device = CGraphic_Device::Create_Instance();
	m_pInput_Device = CInput_Device::Create_Instance();
	m_pLevel_Manager = CLevel_Manager::Create_Instance();
	m_pObject_Manager = CObject_Manager::Create_Instance();
	m_pComponent_Manager = CComponent_Manager::Create_Instance();
	m_pTimer_Manager = CTimer_Manager::Create_Instance();
	m_pRender_Manager = CRender_Manager::Create_Instance();
	m_pPipeLine = CPipeLine::Create_Instance();
	m_pResource_Manager = CResource_Manager::Create_Instance();
	m_pFont_Manager = CFont_Manager::Create_Instance();
	m_pEvent_Manager = CEvent_Manager::Create_Instance();
	m_pCollision_Manager = CCollision_Manager::Create_Instance();
	m_pLight_Manager = CLight_Manager::Create_Instance();
	m_pTarget_Manager = CRenderTarget_Manager::Create_Instance();
	m_pFrustum = CFrustum::Create_Instance();
	m_pSound_Manager = CSound_Manager::Create_Instance();
	m_pPhysX_Manager = CPhysX_Manager::Create_Instance();
	m_pThread_Manager = CThread_Manager::Create_Instance();

	m_GraphicDesc = GraphicDesc;
	m_WindowHandle = GraphicDesc.hWnd;

	if (nullptr == GET_SINGLE(CGraphic_Device))
		return E_FAIL;	

	/* 그래픽디바이스. */
	if (GET_SINGLE(CGraphic_Device)->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.isWindowMode, GraphicDesc.iWinCX, GraphicDesc.iWinCY))
		return E_FAIL;

	/* 인풋 디바이스. */
	if (FAILED(GET_SINGLE(CInput_Device)->Initialize(hInst, GraphicDesc.hWnd)))
		return E_FAIL;


	/* 오브젝트 매니져의 예약. */
	if (FAILED(GET_SINGLE(CObject_Manager)->Reserve_Container(iNumLevels)))
		return E_FAIL;

	m_pTimer_Manager->Reserve_TimeScaleLayer(iNumTimeScales);
	m_pCollision_Manager->Initialize(iNumCollsionLayer);

	if (FAILED(m_pFrustum->Initialize()))
		return E_FAIL;

	if (FAILED(m_pRender_Manager->Initialize()))
	{
		DEBUG_ASSERT;
	}

	/* 컴포넌트 매니져의 예약. */
	/*if (FAILED(GET_SINGLE(CComponent_Manager)->Reserve_Container(iNumLevels)))
		return E_FAIL;*/


	m_pFrustum->Initialize();
	
	GAMEINSTANCE->Load_Textures("NullTexture", TEXT("../Bin/NullTexture.png"), MEMORY_TYPE::MEMORY_STATIC);

	m_pSound_Manager->Initialize();

	m_pPhysX_Manager->Initialize(iNumCollsionLayer);
	m_pThread_Manager->Initialize(8);

	return S_OK;	
}

HRESULT CGameInstance::Tick_Engine(_float fTimeDelta)
{
	m_fDeltaTime = fTimeDelta;

	


	GET_SINGLE(CInput_Device)->SetUp_DeviceState();
	m_pInput_Device->Tick();

	GET_SINGLE(CThread_Manager)->Bind_GameObjectWorks((1 << (_flag)THREAD_TYPE::PRE_TICK));

	GET_SINGLE(CLevel_Manager)->Tick(fTimeDelta);

	GET_SINGLE(CThread_Manager)->Wait_JobDone();

	GET_SINGLE(CObject_Manager)->Tick(fTimeDelta);

	GET_SINGLE(CThread_Manager)->Bind_GameObjectWorks((1 << (_flag)THREAD_TYPE::TICK));

	GET_SINGLE(CThread_Manager)->Wait_JobDone();

	GET_SINGLE(CThread_Manager)->Bind_GameObjectWorks((1 << (_flag)THREAD_TYPE::PRE_LATETICK));

	GET_SINGLE(CThread_Manager)->Wait_JobDone();

	GET_SINGLE(CObject_Manager)->LateTick(fTimeDelta);

	GET_SINGLE(CThread_Manager)->Bind_GameObjectWorks((1 << (_flag)THREAD_TYPE::LATETICK));

	GET_SINGLE(CThread_Manager)->Wait_JobDone();

	GET_SINGLE(CThread_Manager)->Enqueue_Job(bind(&CCollision_Manager::Tick, m_pCollision_Manager));

	//m_pTimer_Manager->Tick();

	m_pPipeLine->Tick();

	m_pFrustum->Update();

	if(KEY_INPUT(KEY::B, KEY_STATE::TAP))
	{
		m_bDebug = !m_bDebug;
	}

	m_pPhysX_Manager->Tick(fTimeDelta);

	++m_iLoopIndex;

	return S_OK;
}

HRESULT CGameInstance::Render_Engine()
{
	GET_SINGLE(CThread_Manager)->Bind_GameObjectWorks((1 << (_flag)THREAD_TYPE::PRE_BEFORERENDER));

	GET_SINGLE(CThread_Manager)->Wait_JobDone();

	m_pObject_Manager->Before_Render(m_fDeltaTime);

	GET_SINGLE(CThread_Manager)->Bind_GameObjectWorks((1 << (_flag)THREAD_TYPE::PRE_RENDER));

	GET_SINGLE(CThread_Manager)->Wait_JobDone();

	GET_SINGLE(CLevel_Manager)->Render(DEVICECONTEXT);

	m_pObject_Manager->After_Render();


	return S_OK;
}

HRESULT CGameInstance::Clear(_uint iLevelID)
{
	GET_SINGLE(CObject_Manager)->Clear(iLevelID);

	return S_OK;
}

HRESULT CGameInstance::LevelEnter()
{
	m_pGraphic_Device->OnLevelEnter();
	m_pInput_Device->OnLevelEnter();
	m_pLevel_Manager->OnLevelEnter();
	m_pObject_Manager->OnLevelEnter();
	m_pComponent_Manager->OnLevelEnter();
	m_pTimer_Manager->OnLevelEnter();
	m_pPipeLine->OnLevelEnter();
	m_pResource_Manager->OnLevelEnter();
	m_pFont_Manager->OnLevelEnter();
	m_pSound_Manager->OnLevelEnter();
	return S_OK;
}

HRESULT CGameInstance::LevelExit()
{
	m_pGraphic_Device->OnLevelExit();
	m_pInput_Device->OnLevelExit();
	m_pLevel_Manager->OnLevelExit();
	m_pObject_Manager->OnLevelExit();
	m_pComponent_Manager->OnLevelExit();
	m_pTimer_Manager->OnLevelExit();
	m_pPipeLine->OnLevelExit();
	m_pResource_Manager->OnLevelExit();
	m_pFont_Manager->OnLevelExit();
	m_pSound_Manager->OnLevelExit();

	return S_OK;
}

_float CGameInstance::Get_DeltaTime() const
{
	return m_fDeltaTime;
}

HWND CGameInstance::Get_WindowHandle() const
{
	return m_WindowHandle;
}


HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	GET_SINGLE(CGraphic_Device)->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{

	GET_SINGLE(CGraphic_Device)->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Present()
{
	GET_SINGLE(CGraphic_Device)->Present();

	return S_OK;
}

ID3D11Device* CGameInstance::Get_Device()
{
	return GET_SINGLE(CGraphic_Device)->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_DeviceContext()
{
	return GET_SINGLE(CGraphic_Device)->Get_DeviceContext();
}

_bool CGameInstance::Is_KeyState(KEY _Key, KEY_STATE _KeyState)
{
	return m_pInput_Device->Get_KeyState(_Key) == _KeyState;
}

_byte CGameInstance::Get_DIMouseKeyState(MOUSEBUTTON eMouseButtonID)
{
	return m_pInput_Device->Get_DIMouseKeyState(eMouseButtonID);
}

_long CGameInstance::Get_DIMouseMoveState(MOUSEMOVE eMouseMove)
{
	return m_pInput_Device->Get_DIMouseMoveState(eMouseMove);
}

HRESULT CGameInstance::Open_Level(_uint iLevelID, shared_ptr<CLevel> pLevel)
{
	return GET_SINGLE(CLevel_Manager)->Open_Level(iLevelID, pLevel);
}

weak_ptr<CLevel> CGameInstance::Get_CurrentLevel()
{
	return m_pLevel_Manager->Get_CurrentLevel();
}

_uint CGameInstance::Get_LastCreatedLevelIndex()
{
	return m_pLevel_Manager->Get_iLastCreatedLevelID();
}

void CGameInstance::Set_CreatedLevelIndex(_uint iCreatedLevelIndex)
{
	m_pLevel_Manager->Set_iLastCreatedLevelID(iCreatedLevelIndex);
}

weak_ptr<CGameObject> CGameInstance::Add_GameObject(size_t iTypeHash, _uint iLevelIndex, void* pArg)
{
	
	return m_pObject_Manager->Add_GameObject(iTypeHash, iLevelIndex, pArg);
}

FDelegate<>& CGameInstance::Get_CallbackStart()
{
	return m_pObject_Manager->CallBack_Start;
}

HRESULT CGameInstance::Add_Timer(_uint eTimer)
{
	if (nullptr == m_pTimer_Manager.get())
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(eTimer);
}

HRESULT CGameInstance::Add_TimerEvent(_uint _iEventNum, weak_ptr<CBase> _Instance, _float _fTime, _bool _bLoop, _bool _bUseTimeScale, _bool _bAlwaysCall)
{
	if (nullptr == m_pTimer_Manager.get())
		return E_FAIL;

	return m_pTimer_Manager->Add_TimerEvent(_iEventNum, _Instance, _fTime, _bLoop, _bUseTimeScale, _bAlwaysCall);
}

_float CGameInstance::Compute_Timer(_uint eTimer)
{
	if (nullptr == m_pTimer_Manager.get())
		return 0.0f;

	return m_pTimer_Manager->Compute_Timer(eTimer);
}

_float CGameInstance::Get_TimeScale(_uint In_iIndex)
{
	return m_pTimer_Manager->Get_TimeScale(In_iIndex);
}

void CGameInstance::Set_TimeScale(_uint In_iIndex, _float In_fTimeScale)
{
	m_pTimer_Manager->Set_TimeScale(In_iIndex, In_fTimeScale);
}

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eGroup, weak_ptr<CGameObject> pGameObject)
{
	return m_pRender_Manager->Add_RenderGroup(eGroup, pGameObject);
}

HRESULT CGameInstance::Draw_RenderGroup()
{
	return m_pRender_Manager->Draw_RenderGroup();
}

HRESULT CGameInstance::Set_Chromatic(const _float In_fChormaticStrangth)
{
	return m_pRender_Manager->Set_Chromatic(In_fChormaticStrangth);
}

HRESULT CGameInstance::Add_Chromatic(const _float In_fChormaticStrangth)
{
	return m_pRender_Manager->Add_Chromatic(In_fChormaticStrangth);
}

HRESULT CGameInstance::Set_ShadowLight(_fvector In_vEye, _fvector In_vLookAt)
{
	return m_pRender_Manager->Set_ShadowLight(In_vEye, In_vLookAt);
}
HRESULT CGameInstance::Set_DynamicShadowLight(_fvector In_vEye, _fvector In_vLookAt)
{
	return m_pRender_Manager->Set_DynamicShadowLight(In_vEye, In_vLookAt);
}

HRESULT CGameInstance::Set_GrayScale(const _float In_fGrayScale)
{
	return m_pRender_Manager->Set_GrayScale(In_fGrayScale);
}
_float4 CGameInstance::Get_FogColor()
{
	return m_pRender_Manager->Get_FogColor();
}
_float CGameInstance::Get_FogRange()
{
	return m_pRender_Manager->Get_FogRange();

}
LIFTGAMMAGAIN_DESC& CGameInstance::Get_LiftGammaGain()
{
	return m_pRender_Manager->Get_LiftGammaGain();
}

ComPtr<ID3D11DeviceContext> CGameInstance::Get_BeforeRenderContext()
{
	return m_pRender_Manager->Get_BeforeRenderContext();
}

void CGameInstance::Release_BeforeRenderContext(ComPtr<ID3D11DeviceContext> pDeviceContext)
{
	m_pRender_Manager->Release_BeforeRenderContext(pDeviceContext);
}


HRESULT CGameInstance::Set_MotionBlur(const _float In_fBlurScale)
{
	return m_pRender_Manager->Set_MotionBlur(In_fBlurScale);
}
HRESULT CGameInstance::Add_MotionBlur(const _float In_fBlurScale)
{
	return m_pRender_Manager->Add_MotionBlur(In_fBlurScale);

}
HRESULT CGameInstance::Set_RadialBlur(const _float In_fRadialBlurStength, _float3 In_vBlurWorldPosition)
{
	return m_pRender_Manager->Set_RadialBlur(In_fRadialBlurStength, In_vBlurWorldPosition);

}
HRESULT CGameInstance::Add_RadialBlur(const _float In_fRadialBlurStrength)
{
	return m_pRender_Manager->Add_RadialBlur(In_fRadialBlurStrength);

}

HRESULT CGameInstance::Set_LiftGammaGain(const _float4 In_vLift, const _float4 In_vGamma, const _float4 In_vGain)
{
	return m_pRender_Manager->Set_LiftGammaGain(In_vLift, In_vGamma, In_vGain);

}

HRESULT  CGameInstance::Set_FogDesc(_float4 In_vFogColor, const _float In_fFogRange)
{
	return m_pRender_Manager->Set_FogDesc(In_vFogColor,In_fFogRange);
	//return E_FAIL;
}



void CGameInstance::Release_Engine()
{
	
	GET_SINGLE(CThread_Manager)->Destroy_Instance();
	GET_SINGLE(CComponent_Manager)->Destroy_Instance();
	GET_SINGLE(CLevel_Manager)->Destroy_Instance();
	GET_SINGLE(CTimer_Manager)->Destroy_Instance();
	GET_SINGLE(CInput_Device)->Destroy_Instance();
	GET_SINGLE(CGraphic_Device)->Destroy_Instance();
	GET_SINGLE(CRender_Manager)->Destroy_Instance();
	GET_SINGLE(CPipeLine)->Destroy_Instance();
	GET_SINGLE(CResource_Manager)->Destroy_Instance();
	GET_SINGLE(CFont_Manager)->Destroy_Instance();
	GET_SINGLE(CEvent_Manager)->Destroy_Instance();
	GET_SINGLE(CCollision_Manager)->Destroy_Instance();
	GET_SINGLE(CLight_Manager)->Destroy_Instance();
	GET_SINGLE(CFrustum)->Destroy_Instance();
	GET_SINGLE(CRenderTarget_Manager)->Destroy_Instance();
	GET_SINGLE(CSound_Manager)->Destroy_Instance();
	GET_SINGLE(CObject_Manager)->Destroy_Instance();
	GET_SINGLE(CPhysX_Manager)->Destroy_Instance();
	GET_SINGLE(CGameInstance)->Destroy_Instance();
}

_int CGameInstance::BGMVolumeDown(_float _vol)
{
	return _int();
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformState)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformState);
}

_matrix CGameInstance::Get_Transform(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform(eState);
}

const _float4x4* CGameInstance::Get_Transform_float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_float4x4(eState);
}

const _float4x4* CGameInstance::Get_Transform_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return nullptr;

	return m_pPipeLine->Get_Transform_TP(eState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4(0.f, 0.f, 0.f, 1.f);

	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Write_JsonUsingResource(const char* In_szFilePath)
{
	return m_pResource_Manager->Write_JsonUsingResource(In_szFilePath);
}

void CGameInstance::Load_ResourcesFromJson(const char* In_szFilePath)
{
	return m_pResource_Manager->Load_ResourcesFromJson(In_szFilePath);
}

HRESULT CGameInstance::Load_Textures(const _char* _strKey, const _tchar* pTextureFilePath, MEMORY_TYPE eMemType)
{
	return m_pResource_Manager->Load_Textures(_strKey, pTextureFilePath, eMemType);
}

HRESULT CGameInstance::Generate_MipMap(const _tchar* pTextureFilePath, const _int In_iMipMapLevels)
{
	return m_pResource_Manager->Generate_MipMap(pTextureFilePath, In_iMipMapLevels);
}

vector<ComPtr<ID3D11ShaderResourceView>> CGameInstance::Get_TexturesFromKey(const _char* _Str_Key, MEMORY_TYPE _eType)
{
	return m_pResource_Manager->Get_TexturesFromKey(_Str_Key, _eType);
}

HRESULT CGameInstance::Load_Model(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType, _bool Is_bAnimZero)
{
	return m_pResource_Manager->Load_Model(sKey, sModelFilePath, eModelType, In_TransformMatrix, eMemType, Is_bAnimZero);
}

shared_ptr<MODEL_DATA> CGameInstance::Get_ModelFromKey(const _char* _sKey, MEMORY_TYPE _eType)
{
	return m_pResource_Manager->Get_ModelFromKey(_sKey, _eType);
}

vector<string> CGameInstance::Get_AllModelKeys()
{
	return m_pResource_Manager->Get_AllModelKeys();
}

vector<string> CGameInstance::Get_AllNoneAnimModelKeys()
{
	return m_pResource_Manager->Get_AllNoneAnimModelKeys();
}

vector<string> CGameInstance::Get_AllAnimModelKeys()
{
	return m_pResource_Manager->Get_AllAnimModelKeys();
}

HRESULT CGameInstance::Load_Shader(const _tchar* sKey, const _tchar* sShaderFilePath)
{
	m_pResource_Manager->Load_Shader(sKey, sShaderFilePath);

	return S_OK;
}

ID3DX11Effect* CGameInstance::Get_ShaderEffect(const _tchar* sKey)
{
	return m_pResource_Manager->Get_ShaderEffect(sKey);
}

HRESULT CGameInstance::ReLoad_AllShader(list<pair<_bool, string>>& Out_CompileMessage)
{
	return m_pResource_Manager->ReLoad_AllShader(Out_CompileMessage);
}

HRESULT CGameInstance::Release_ResourceByMemoryType(MEMORY_TYPE _eMemType)
{
	return m_pResource_Manager->Release_ResourceByMemoryType(_eMemType);
}

HRESULT CGameInstance::Add_Font(_uint iFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(iFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(_uint iFontTag, const _tchar* pString, const _float2& vPosition, _fvector vColor)
{
	return m_pFont_Manager->Render_Font(iFontTag, pString, vPosition, vColor);
}

void CGameInstance::Render_Font(_uint iFontTag)
{
	m_pFont_Manager->Render_Font(iFontTag);

}

void CGameInstance::Add_Text(const _uint& iFontTag, const TEXTINFO& In_tTextInfo)
{
	m_pFont_Manager->Add_Text(iFontTag, In_tTextInfo);
}

void CGameInstance::Reserve_Event(_uint iMaxIndex)
{
	m_pEvent_Manager->m_Events.reserve(iMaxIndex);
}

FDelegate<void*>& CGameInstance::Get_Event(_uint iIndex)
{

	return m_pEvent_Manager->m_Events[iIndex];
}

void CGameInstance::Check_Group(const _uint& In_iLeftLayer, const _uint& In_iRightLayer)
{
	m_pCollision_Manager->Check_Group(In_iLeftLayer, In_iRightLayer);

}

const LIGHTDESC& CGameInstance::Get_LightDesc(_uint iIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

void CGameInstance::Set_LightDesc(const LIGHTDESC& LightDesc)
{
	m_pLight_Manager->Set_LightDesc(LightDesc);
}

const LIGHTDESC& CGameInstance::Add_Light(const LIGHTDESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}

_bool CGameInstance::Remove_Light(const _uint& iIndex)
{
	return m_pLight_Manager->Remove_Light(iIndex);
}

_bool CGameInstance::isIn_Frustum_InWorldSpace(_fvector vWorldPoint, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_Frustum_InWorldSpace(vWorldPoint, fRange);
}

ComPtr<ID3D11ShaderResourceView> CGameInstance::Get_RenderTarget_SRV(const _tchar* pTargetTag)
{
	if (nullptr == m_pTarget_Manager)
		return nullptr;

	return m_pTarget_Manager->Get_SRV(pTargetTag);
}

_int CGameInstance::VolumeUp(CHANNELID eID, _float _vol)
{
	return m_pSound_Manager->VolumeUp(eID, _vol);
}

_int CGameInstance::VolumeDown(CHANNELID eID, _float _vol)
{
	return m_pSound_Manager->VolumeDown(eID, _vol);
}

_int CGameInstance::BGMVolumeUp(_float _vol)
{
	return m_pSound_Manager->BGMVolumeUp(_vol);
}

_int CGameInstance::Pause(CHANNELID eID)
{
	return m_pSound_Manager->Pause(eID);
}

_uint CGameInstance::PlaySound(const TCHAR* pSoundKey, _uint _iIndex, _float _vol)
{
	return m_pSound_Manager->PlaySound(pSoundKey, _iIndex, _vol);
}

_uint CGameInstance::PlaySound(const TCHAR* pSoundKey, _float _vol)
{
	return m_pSound_Manager->PlaySound(pSoundKey, _vol);
}

void CGameInstance::PlayBGM(const TCHAR* pSoundKey, _float _vol)
{
	m_pSound_Manager->PlayBGM(pSoundKey, _vol);
}

void CGameInstance::StopSound(_uint _iChannelIndex)
{
	m_pSound_Manager->StopSound(_iChannelIndex);
}

void CGameInstance::StopAll()
{
	m_pSound_Manager->StopAll();
}

void CGameInstance::Check_PhysXFilterGroup(const _uint In_iLeftLayer, const _uint In_iRightLayer)
{
	m_pPhysX_Manager->Check_PhysXFilterGroup(In_iLeftLayer, In_iRightLayer);
}

HRESULT CGameInstance::Create_Scene(CPhysX_Manager::Scene eScene, PxVec3 Gravity)
{
	return m_pPhysX_Manager->Create_Scene(eScene, Gravity);
}

HRESULT CGameInstance::Delete_Scene(CPhysX_Manager::Scene eScene)
{
	return m_pPhysX_Manager->Delete_Scene(eScene);
}

HRESULT CGameInstance::Change_Scene(CPhysX_Manager::Scene eNextScene, PxVec3 Gravity)
{
	return m_pPhysX_Manager->Change_Scene(eNextScene, Gravity);
}

void CGameInstance::Create_CylinderMesh(_float fRadiusBelow, _float fRadiusUpper, _float fHight, PxConvexMesh** ppOut)
{
	m_pPhysX_Manager->Create_CylinderMesh(fRadiusBelow, fRadiusUpper, fHight, ppOut);
}

void CGameInstance::Create_ConvexMesh(PxVec3** pVertices, _uint iNumVertice, PxConvexMesh** ppOut)
{
	m_pPhysX_Manager->Create_ConvexMesh(pVertices, iNumVertice, ppOut);
}

void CGameInstance::Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution, PxMaterial** ppOut)
{
	m_pPhysX_Manager->Create_Material(fStaticFriction, fDynamicFriction, fRestitution, ppOut);
}

weak_ptr<CPhysXCollider> CGameInstance::Find_PhysXCollider(const _uint In_iPhysXColliderIndex)
{
	return m_pPhysX_Manager->Find_PhysXCollider(In_iPhysXColliderIndex);
}

weak_ptr<CPhysXController> CGameInstance::Find_PhysXController(const _uint In_iPhysXControllerIndex)
{
	return m_pPhysX_Manager->Find_PhysXController(In_iPhysXControllerIndex);
}

_uint CGameInstance::Get_PhysXFilterGroup(const _uint In_iIndex)
{
	return m_pPhysX_Manager->Get_PhysXFilterGroup(In_iIndex);
}

void CGameInstance::Free()
{
	__super::Free();

	m_pGraphic_Device.reset();
	m_pInput_Device.reset();
	m_pLevel_Manager.reset();
	m_pObject_Manager.reset();
	m_pComponent_Manager.reset();
	m_pTimer_Manager.reset();
	m_pPipeLine.reset();
	m_pResource_Manager.reset();
	m_pFont_Manager.reset();
	m_pEvent_Manager.reset();
	m_pCollision_Manager.reset();
	m_pLight_Manager.reset();
	m_pFrustum.reset();
	m_pTarget_Manager.reset();
	m_pSound_Manager.reset();

	m_pPhysX_Manager.reset();
	m_pThread_Manager.reset();
}



