#include "stdafx.h"
#include "CustomEffectMesh.h"
#include "Client_Components.h"
#include "GameManager.h"
#include "SMath.h"
#include "Json/Json_Utility.h"
#include "Window_EffectHierarchyView.h"
#include "Window_AnimationModelView.h"
#include "Attack_Area.h"
#include "PreViewAnimationModel.h"
#include "Easing_Utillity.h"

#include "BoneNode.h"


GAMECLASS_C(CCustomEffectMesh)
CLONE_C(CCustomEffectMesh, CGameObject)

const _char* CCustomEffectMesh::Get_EffectName() const
{
	return m_szEffectName.c_str();
}

_bool CCustomEffectMesh::Is_Finish() const
{
	return m_bFinish;
}

HRESULT CCustomEffectMesh::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_eRenderGroup = RENDERGROUP::RENDER_ALPHABLEND;
	m_iPassIndex = 2;
	m_vUv = { 0.f ,0.f };

	return S_OK;
}

HRESULT CCustomEffectMesh::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pShaderCom.lock()->Set_ShaderInfo(
		TEXT("Shader_EffectMesh"),
		VTXANIM_DECLARATION::Element,
		VTXANIM_DECLARATION::iNumElements);

	m_pColorDiffuseTextureCom = CGameObject::Add_Component<CTexture>();
	m_pNoiseTextureCom = CGameObject::Add_Component<CTexture>();
	m_pMaskTextureCom = CGameObject::Add_Component<CTexture>();
	// m_pGradientTextureCom = CGameObject::Add_Component<CTexture>();

	m_pColorDiffuseTextureCom.lock()->Use_Texture(("UVColorDiffuse"));
	m_pNoiseTextureCom.lock()->Use_Texture(("UVNoise"));
	m_pMaskTextureCom.lock()->Use_Texture(("UVMask"));
	// m_pGradientTextureCom.lock()->Use_Texture(("Gradient"));

	Set_Enable(false);

	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());

	ZeroMemory(&m_tEffectMeshDesc, sizeof(m_tEffectMeshDesc));


#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::TICK);
#endif

	return S_OK;
}

HRESULT CCustomEffectMesh::Start()
{
	__super::Start();

	return S_OK;
}

void CCustomEffectMesh::Tick(_float fTimeDelta)
{
	if (!Get_Enable())
		return;

	__super::Tick(fTimeDelta);

	Play(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex));
#ifdef _DEBUG
	this->Key_Input_ControlMesh(fTimeDelta);
	this->Apply_ImGui_Controls_to_Mesh();
#endif // _DEBUG
}

void CCustomEffectMesh::LateTick(_float fTimeDelta)
{
	if (!Get_Enable())
		return;

	if (m_fCurrentInitTime > 0.f)
		return;

	// __super::LateTick(fTimeDelta);
	if (m_tEffectMeshDesc.bDistortion)
		GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_DISTORTION, Weak_Cast<CGameObject>(m_this));
	else
	{
		// __super::LateTick(fTimeDelta);
		CallBack_LateTick(fTimeDelta);
		m_pRendererCom.lock()->Add_RenderGroup(m_eRenderGroup, Cast<CGameObject>(m_this));
	}
}

HRESULT CCustomEffectMesh::Render()
{
	if (!Get_Enable())
		return E_FAIL;

	this->SetUp_ShaderResource();
	CGameObject::CallBack_Render();

	_uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

		m_pShaderCom.lock()->Begin(m_tEffectMeshDesc.iShaderPassIndex);

		m_pModelCom.lock()->Render_Mesh(i);
	}

	return S_OK;
}

void CCustomEffectMesh::SetUp_ShaderResource()
{
	if (m_tEffectMeshDesc.bFollowTransform && m_pParentTransformCom.lock())
		XMStoreFloat4x4(&m_ParentMatrix, m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());

#pragma region World, View, Proj, Camera
	/*_matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	WorldMatrix *= XMLoadFloat4x4(&m_ParentMatrix);
	WorldMatrix = XMMatrixTranspose(WorldMatrix);

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", (void*)&WorldMatrix, sizeof(_float4x4));*/
	_matrix		BoneMatrix	= XMMatrixIdentity();
	_matrix		WorldMatrix = XMMatrixIdentity();

	if (m_pBoneNode.lock())
	{
		_float4x4 TempMat = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Get_TransformationMatrix();
		_matrix ModelTranMat = XMLoadFloat4x4(&TempMat);
		BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	}

	WorldMatrix = BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix();
	WorldMatrix *= XMLoadFloat4x4(&m_ParentMatrix);
	WorldMatrix = XMMatrixTranspose(WorldMatrix);

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	_vector vCamDir = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2];
	m_pShaderCom.lock()->Set_RawValue("g_vCamDirection", &vCamDir, sizeof(_vector));
#pragma endregion
	
	m_pColorDiffuseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_tEffectMeshDesc.iDiffuseTextureIndex);
	m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_tEffectMeshDesc.iMaskTextureIndex);
	m_pNoiseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_tEffectMeshDesc.iNoiseTextureIndex);

	m_pShaderCom.lock()->Set_RawValue("g_bDynamicNoiseOption", &m_tEffectMeshDesc.bDynamicNoiseOption, sizeof(_bool));

	m_pShaderCom.lock()->Set_RawValue("g_bDiffuseWrap", &m_tEffectMeshDesc.bDiffuseWrap, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bNoiseWrap", &m_tEffectMeshDesc.bNoiseWrap, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bMaskWrap", &m_tEffectMeshDesc.bMaskWrap, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_vWrapWeight", &m_tEffectMeshDesc.vWrapWeight, sizeof(_float4));

	m_pShaderCom.lock()->Set_RawValue("g_vUVDiff", &m_vDiffuseCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vUVNoise", &m_vNoiseCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vUVMask", &m_vMaskCurrentUV, sizeof(_float2));

	m_pShaderCom.lock()->Set_RawValue("g_fDiscardRatio", &m_tEffectMeshDesc.fDiscardRatio, sizeof(_float));

	m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_vCurrentColor, sizeof(_float4));

	//m_pGradientTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_GradientTexture", 0);

	m_pShaderCom.lock()->Set_RawValue("g_bBillboard", &m_tEffectMeshDesc.bBillBoard, sizeof(_bool));

	m_pShaderCom.lock()->Set_RawValue("g_bBloom", &m_tEffectMeshDesc.bBloom, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bGlow", &m_tEffectMeshDesc.bGlow, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &m_vCurrentGlowColor, sizeof(_float4));
}

void CCustomEffectMesh::Init_EffectMesh(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey)
{
	m_szEffectName = In_szModelKey;

	memcpy(&m_tEffectMeshDesc, &In_tEffectMeshDesc, sizeof(EFFECTMESH_DESC));

	m_pModelCom.lock()->Init_Model(m_szEffectName.c_str());
}

void CCustomEffectMesh::Sync_Animation()
{
	if (!m_tEffectMeshDesc.bSyncAnimation)
		return;

	Reset_Effect();
}

void CCustomEffectMesh::Play(_float fTimeDelta)
{
	if (m_fCurrentInitTime > 0.f)
	{
		m_fCurrentInitTime -= fTimeDelta;
		return;
	}

	if (m_fCurrentLifeTime >= m_tEffectMeshDesc.fLifeTime)
	{
		Set_Enable(false);
		m_bFinish = true;
		return;
	}

	_float	fFrameTime(HZ_144);
	_int	iTickCount(0);

	m_fPreFrame += fFrameTime;
	while (m_fPreFrame >= fFrameTime)
	{
		++iTickCount;
		m_fPreFrame -= fFrameTime;
	}

	for (_int i(0); i < iTickCount; ++i)
		Play_Internal(fFrameTime);

	m_fCurrentLifeTime += fTimeDelta;
}

void CCustomEffectMesh::Reset_Effect()
{
	Set_Enable(true);
	m_bFinish = false;

	m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());

	m_fCurrentInitTime = m_tEffectMeshDesc.fInitTime;
	m_fCurrentLifeTime = 0.f;

	_vector StartPosition = XMLoadFloat3(&m_tEffectMeshDesc.vStartPosition);
	StartPosition = XMVectorSetW(StartPosition, 1.f);
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, StartPosition);

	m_vCurrentSpeed     = m_tEffectMeshDesc.vSpeed;
	m_vCurrentRotation  = m_tEffectMeshDesc.vStartRotation;
	m_vCurrentScale     = m_tEffectMeshDesc.vStartScale;
	m_vCurrentColor     = m_tEffectMeshDesc.vStartColor;
	m_vDiffuseCurrentUV = m_tEffectMeshDesc.vDiffuseStartUV;
	m_vNoiseCurrentUV   = m_tEffectMeshDesc.vNoiseStartUV;
	m_vMaskCurrentUV    = m_tEffectMeshDesc.vMaskStartUV;
	m_vCurrentGlowColor = m_tEffectMeshDesc.vStartGlowColor;

	ZeroMemory(&m_vCurrentForce         , sizeof(_float3));
	ZeroMemory(&m_vCurrentScaleForce    , sizeof(_float3));
	ZeroMemory(&m_vCurrentColorForce    , sizeof(_float4));
	ZeroMemory(&m_vDiffuseCurrentUVForce, sizeof(_float2));
	ZeroMemory(&m_vNoiseCurrentUVForce  , sizeof(_float2));
	ZeroMemory(&m_vMaskCurrentUVForce   , sizeof(_float2));
	ZeroMemory(&m_vCurrentGlowForce     , sizeof(_float4));
	ZeroMemory(&m_vCurrentRotationForce , sizeof(_float3));

	//XMStoreFloat3(&m_vCurrentForce,          XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat3(&m_vCurrentScaleForce,     XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat4(&m_vCurrentColorForce,     XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat2(&m_vDiffuseCurrentUVForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat2(&m_vNoiseCurrentUVForce,   XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat2(&m_vMaskCurrentUVForce,    XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat4(&m_vCurrentGlowForce,      XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat3(&m_vCurrentRotationForce,  XMVectorSet(0.f, 0.f, 0.f, 0.f));

	m_fPreFrame = 0.f;

	m_pTransformCom.lock()->Set_Scaled(m_vCurrentScale);

	_vector StartRotation = XMLoadFloat3(&m_vCurrentRotation);

	_vector Quaternion = XMQuaternionRotationRollPitchYaw(XMVectorGetX(StartRotation), XMVectorGetY(StartRotation), XMVectorGetZ(StartRotation));
	m_pTransformCom.lock()->Rotation_Quaternion(Quaternion);

	if (m_tEffectMeshDesc.bCollider)
	{
		if (GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock())
		{
			if (!m_pAttackArea.lock())
				m_pAttackArea = GAMEINSTANCE->Add_GameObject<CAttackArea>(m_CreatedLevel);

			ATTACKAREA_DESC WeaponDesc;
			ZeroMemory(&WeaponDesc, sizeof(ATTACKAREA_DESC));

			WeaponDesc.fWeaponScale  = m_tEffectMeshDesc.fWeaponScale;
			WeaponDesc.iHitType      = m_tEffectMeshDesc.iHitType;
			WeaponDesc.iOptionType   = m_tEffectMeshDesc.iOptionType;
			WeaponDesc.fDamage       = m_tEffectMeshDesc.fDamage;
			WeaponDesc.vWeaponOffset = m_tEffectMeshDesc.vWeaponOffset;
			WeaponDesc.fHitFreq      = m_tEffectMeshDesc.fHitFreq;

			m_pAttackArea.lock()->Init_AttackArea(WeaponDesc, GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Component<CTransform>());
			m_pAttackArea.lock()->Enable_Weapon(m_tEffectMeshDesc.fWeaponLifeTime, m_tEffectMeshDesc.bWeaponSyncTransform);
		}
	}
}

void CCustomEffectMesh::Reset_Effect(weak_ptr<CTransform> pParentTransform)
{
	Set_Enable(true);
	m_bFinish = false;
	m_pParentTransformCom = pParentTransform;
	XMStoreFloat4x4(&m_ParentMatrix, pParentTransform.lock()->Get_UnScaledWorldMatrix());

	m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());

	m_fCurrentInitTime = m_tEffectMeshDesc.fInitTime;
	m_fCurrentLifeTime = 0.f;

	_vector StartPosition = XMLoadFloat3(&m_tEffectMeshDesc.vStartPosition);
	StartPosition = XMVectorSetW(StartPosition, 1.f);

	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, StartPosition);

	m_fPreFrame = 0.f;

	m_vCurrentSpeed     = m_tEffectMeshDesc.vSpeed;
	m_vCurrentRotation  = m_tEffectMeshDesc.vStartRotation;
	m_vCurrentScale     = m_tEffectMeshDesc.vStartScale;
	m_vCurrentColor     = m_tEffectMeshDesc.vStartColor;
	m_vDiffuseCurrentUV = m_tEffectMeshDesc.vDiffuseStartUV;
	m_vNoiseCurrentUV   = m_tEffectMeshDesc.vNoiseStartUV;
	m_vMaskCurrentUV    = m_tEffectMeshDesc.vMaskStartUV;
	m_vCurrentGlowColor = m_tEffectMeshDesc.vStartGlowColor;

	ZeroMemory(&m_vCurrentForce         , sizeof(_float3));
	ZeroMemory(&m_vCurrentScaleForce    , sizeof(_float3));
	ZeroMemory(&m_vCurrentColorForce    , sizeof(_float4));
	ZeroMemory(&m_vDiffuseCurrentUVForce, sizeof(_float2));
	ZeroMemory(&m_vNoiseCurrentUVForce  , sizeof(_float2));
	ZeroMemory(&m_vMaskCurrentUVForce   , sizeof(_float2));
	ZeroMemory(&m_vCurrentGlowForce     , sizeof(_float4));
	ZeroMemory(&m_vCurrentRotationForce , sizeof(_float3));

	//XMStoreFloat3(&m_vCurrentForce,          XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat3(&m_vCurrentScaleForce,     XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat4(&m_vCurrentColorForce,     XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat2(&m_vDiffuseCurrentUVForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat2(&m_vNoiseCurrentUVForce,   XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat2(&m_vMaskCurrentUVForce,    XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat4(&m_vCurrentGlowForce,      XMVectorSet(0.f, 0.f, 0.f, 0.f));
	//XMStoreFloat3(&m_vCurrentRotationForce,  XMVectorSet(0.f, 0.f, 0.f, 0.f));

	m_pTransformCom.lock()->Set_Scaled(m_vCurrentScale);
	_vector StartRotation = XMLoadFloat3(&m_vCurrentRotation);

	_vector Quaternion = XMQuaternionRotationRollPitchYaw(XMVectorGetX(StartRotation), XMVectorGetY(StartRotation), XMVectorGetZ(StartRotation));
	m_pTransformCom.lock()->Rotation_Quaternion(Quaternion);

	if (m_tEffectMeshDesc.bCollider)
	{
		if (!m_pAttackArea.lock())
		{
			m_pAttackArea = GAMEINSTANCE->Add_GameObject<CAttackArea>(m_CreatedLevel);
		}

		ATTACKAREA_DESC WeaponDesc;
		ZeroMemory(&WeaponDesc, sizeof(ATTACKAREA_DESC));
		WeaponDesc.fWeaponScale = m_tEffectMeshDesc.fWeaponScale;
		WeaponDesc.iHitType = m_tEffectMeshDesc.iHitType;
		WeaponDesc.iOptionType = m_tEffectMeshDesc.iOptionType;
		WeaponDesc.fDamage = m_tEffectMeshDesc.fDamage;
		WeaponDesc.vWeaponOffset = m_tEffectMeshDesc.vWeaponOffset;
		WeaponDesc.fHitFreq = m_tEffectMeshDesc.fHitFreq;

		m_pAttackArea.lock()->Init_AttackArea(WeaponDesc, pParentTransform);
		m_pAttackArea.lock()->Enable_Weapon(m_tEffectMeshDesc.fWeaponLifeTime, m_tEffectMeshDesc.bWeaponSyncTransform);
	}
}

void CCustomEffectMesh::Write_FromBinary(ostream& os)
{
	write_typed_data(os, m_szEffectName.size());
	os.write(&m_szEffectName[0], m_szEffectName.size());

	write_typed_data(os, m_tEffectMeshDesc);
}

void CCustomEffectMesh::Load_FromBinary(istream& is)
{

}

void CCustomEffectMesh::Write_EffectJson(json& Out_Json)
{
	Out_Json["Name"]              = m_szEffectName;

	Out_Json["Init_Time"]         = m_tEffectMeshDesc.fInitTime;
	Out_Json["Life_Time"]         = m_tEffectMeshDesc.fLifeTime;

	Out_Json["Sync_Animation"]    = m_tEffectMeshDesc.bSyncAnimation;
	Out_Json["Sync_AnimationKey"] = m_tEffectMeshDesc.iSyncAnimationKey;

	Out_Json["Follow_Transform"]  = m_tEffectMeshDesc.bFollowTransform;
	Out_Json["BillBoard"]         = m_tEffectMeshDesc.bBillBoard;

	Out_Json["Is_Boner"]          = m_tEffectMeshDesc.bBoner;

	if (m_tEffectMeshDesc.bBoner)
	{
		if (!m_strBoneName.empty())
			Out_Json["Bone_Name"] = m_strBoneName;
	}

	CJson_Utility::Write_Float3(Out_Json["Start_Position"], m_tEffectMeshDesc.vStartPosition);

	CJson_Utility::Write_Float3(Out_Json["Speed"],          m_tEffectMeshDesc.vSpeed);
	CJson_Utility::Write_Float3(Out_Json["Force"],          m_tEffectMeshDesc.vForce);
	CJson_Utility::Write_Float3(Out_Json["Min_Speed"],      m_tEffectMeshDesc.vMinSpeed);
	CJson_Utility::Write_Float3(Out_Json["Max_Speed"],      m_tEffectMeshDesc.vMaxSpeed);

	CJson_Utility::Write_Float3(Out_Json["Start_Rotation"], m_tEffectMeshDesc.vStartRotation);
	CJson_Utility::Write_Float3(Out_Json["Rotation_Speed"], m_tEffectMeshDesc.vRotationSpeed);
	CJson_Utility::Write_Float3(Out_Json["Rotation_Force"], m_tEffectMeshDesc.vRotationForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Rotation"],   m_tEffectMeshDesc.vMaxRotation);

	CJson_Utility::Write_Float3(Out_Json["Start_Scale"],    m_tEffectMeshDesc.vStartScale);
	CJson_Utility::Write_Float3(Out_Json["Scale_Speed"],    m_tEffectMeshDesc.vScaleSpeed);
	CJson_Utility::Write_Float3(Out_Json["Scale_Force"],    m_tEffectMeshDesc.vScaleForce);
	CJson_Utility::Write_Float3(Out_Json["Min_Scale"],      m_tEffectMeshDesc.vMinScale);
	CJson_Utility::Write_Float3(Out_Json["Max_Scale"],      m_tEffectMeshDesc.vMaxScale);

	Out_Json["Shader_Pass_Index"] = m_tEffectMeshDesc.iShaderPassIndex;

	Out_Json["Alpha_Discard_Ratio"] = m_tEffectMeshDesc.fDiscardRatio;

	CJson_Utility::Write_Float4(Out_Json["Start_Color"], m_tEffectMeshDesc.vStartColor);
	CJson_Utility::Write_Float4(Out_Json["Color_Speed"], m_tEffectMeshDesc.vColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Color_Force"], m_tEffectMeshDesc.vColorForce);
	CJson_Utility::Write_Float4(Out_Json["Min_Color"],   m_tEffectMeshDesc.vMinColor);
	CJson_Utility::Write_Float4(Out_Json["Max_Color"],   m_tEffectMeshDesc.vMaxColor);

	Out_Json["UV_Diffuse_Index"]       = m_tEffectMeshDesc.iDiffuseTextureIndex;
	Out_Json["UV_Noise_Index"]         = m_tEffectMeshDesc.iNoiseTextureIndex;
	Out_Json["UV_Mask_Index"]          = m_tEffectMeshDesc.iMaskTextureIndex;

	Out_Json["Dynamic_Noise_Option"]   = m_tEffectMeshDesc.bDynamicNoiseOption;

	Out_Json["UV_Diffuse_Wrap_Option"] = m_tEffectMeshDesc.bDiffuseWrap;
	Out_Json["UV_Noise_Wrap_Option"]   = m_tEffectMeshDesc.bNoiseWrap;
	Out_Json["UV_Mask_Wrap_Option"]    = m_tEffectMeshDesc.bMaskWrap;
	CJson_Utility::Write_Float4(Out_Json["UV_Wrap_Weight"],   m_tEffectMeshDesc.vWrapWeight);

	CJson_Utility::Write_Float2(Out_Json["Diffuse_Start_UV"], m_tEffectMeshDesc.vDiffuseStartUV);
	CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Speed"], m_tEffectMeshDesc.vDiffuseUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Force"], m_tEffectMeshDesc.vDiffuseUVForce);
	CJson_Utility::Write_Float2(Out_Json["Diffuse_Min_UV"],   m_tEffectMeshDesc.vDiffuseUVMin);
	CJson_Utility::Write_Float2(Out_Json["Diffuse_Max_UV"],   m_tEffectMeshDesc.vDiffuseUVMax);

	CJson_Utility::Write_Float2(Out_Json["Noise_Start_UV"],   m_tEffectMeshDesc.vNoiseStartUV);
	CJson_Utility::Write_Float2(Out_Json["Noise_UV_Speed"],   m_tEffectMeshDesc.vNoiseUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["Noise_UV_Force"],   m_tEffectMeshDesc.vNoiseUVForce);
	CJson_Utility::Write_Float2(Out_Json["Noise_Min_UV"],     m_tEffectMeshDesc.vNoiseUVMin);
	CJson_Utility::Write_Float2(Out_Json["Noise_Max_UV"],     m_tEffectMeshDesc.vNoiseUVMax);

	CJson_Utility::Write_Float2(Out_Json["Mask_Start_UV"],    m_tEffectMeshDesc.vMaskStartUV);
	CJson_Utility::Write_Float2(Out_Json["Mask_UV_Speed"],    m_tEffectMeshDesc.vMaskUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["Mask_UV_Force"],    m_tEffectMeshDesc.vMaskUVForce);
	CJson_Utility::Write_Float2(Out_Json["Mask_Min_UV"],      m_tEffectMeshDesc.vMaskUVMin);
	CJson_Utility::Write_Float2(Out_Json["Mask_Max_UV"],      m_tEffectMeshDesc.vMaskUVMax);

	Out_Json["Bloom"]      = m_tEffectMeshDesc.bBloom;
	Out_Json["Glow"]       = m_tEffectMeshDesc.bGlow;
	Out_Json["Distortion"] = m_tEffectMeshDesc.bDistortion;

	CJson_Utility::Write_Float4(Out_Json["Start_Glow_Color"], m_tEffectMeshDesc.vStartGlowColor);
	CJson_Utility::Write_Float4(Out_Json["Glow_Color_Speed"], m_tEffectMeshDesc.vGlowColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Glow_Color_Force"], m_tEffectMeshDesc.vGlowColorForce);

	Out_Json["Collider"]        = m_tEffectMeshDesc.bCollider;
	Out_Json["Sync_Transform"]  = m_tEffectMeshDesc.bWeaponSyncTransform;
	Out_Json["Weapon_LifeTime"] = m_tEffectMeshDesc.fWeaponLifeTime;
	Out_Json["Weapon_Scale"]    = m_tEffectMeshDesc.fWeaponScale;
	Out_Json["Hit_Type"]        = m_tEffectMeshDesc.iHitType;
	Out_Json["Option_Type"]     = m_tEffectMeshDesc.iOptionType;
	Out_Json["Damage"]          = m_tEffectMeshDesc.fDamage;

	CJson_Utility::Write_Float3(Out_Json["Weapon_Offset"], m_tEffectMeshDesc.vWeaponOffset);

	Out_Json["HitFreq"]         = m_tEffectMeshDesc.fHitFreq;
}

void CCustomEffectMesh::Load_EffectJson(const json& In_Json, const _uint& In_iTimeScaleLayer)
{
	m_iTimeScaleLayerIndex = In_iTimeScaleLayer;

	if (In_Json.find("Name") != In_Json.end())
		m_szEffectName = In_Json["Name"];

	if (In_Json.find("Init_Time") != In_Json.end())
		m_tEffectMeshDesc.fInitTime = In_Json["Init_Time"];

	if (In_Json.find("Life_Time") != In_Json.end())
		m_tEffectMeshDesc.fLifeTime = In_Json["Life_Time"];

	if (In_Json.find("Sync_Animation") != In_Json.end())
		m_tEffectMeshDesc.bSyncAnimation = In_Json["Sync_Animation"];

	if (In_Json.find("Sync_AnimationKey") != In_Json.end())
		m_tEffectMeshDesc.iSyncAnimationKey = In_Json["Sync_AnimationKey"];

	if (In_Json.find("Follow_Transform") != In_Json.end())
		m_tEffectMeshDesc.bFollowTransform = In_Json["Follow_Transform"];

	if (In_Json.find("Is_Boner") != In_Json.end())
		m_tEffectMeshDesc.bBoner = In_Json["Is_Boner"];

	if (m_tEffectMeshDesc.bBoner)
	{
		if (In_Json.find("Bone_Name") != In_Json.end())
			m_strBoneName = In_Json["Bone_Name"];

		try
		{
			if (m_strBoneName.empty())
				throw;

			m_pParentTransformCom = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Component<CTransform>().lock();
			if (!m_pParentTransformCom.lock())
				throw;

			m_pBoneNode = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Find_BoneNode(m_strBoneName);
			if (!m_pBoneNode.lock())
				throw;
		}
		catch (const std::exception&)
		{
			assert(0);
		}
	}

	if (In_Json.find("BillBoard") != In_Json.end())
		m_tEffectMeshDesc.bBillBoard = In_Json["BillBoard"];
	
	if (In_Json.find("Start_Position") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Start_Position"], m_tEffectMeshDesc.vStartPosition);

	if (In_Json.find("Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Speed"], m_tEffectMeshDesc.vSpeed);

	if (In_Json.find("Force") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Force"], m_tEffectMeshDesc.vForce);

	if (In_Json.find("Min_Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Speed"], m_tEffectMeshDesc.vMinSpeed);

	if (In_Json.find("Max_Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Speed"], m_tEffectMeshDesc.vMaxSpeed);

	if (In_Json.find("Start_Rotation") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Start_Rotation"], m_tEffectMeshDesc.vStartRotation);
	
	if (In_Json.find("Rotation_Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Rotation_Speed"], m_tEffectMeshDesc.vRotationSpeed);
	
	if (In_Json.find("Rotation_Force") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Rotation_Force"], m_tEffectMeshDesc.vRotationForce);
	
	if (In_Json.find("Max_Rotation") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Rotation"], m_tEffectMeshDesc.vMaxRotation);

	if (In_Json.find("Start_Scale") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Start_Scale"], m_tEffectMeshDesc.vStartScale);
	
	if (In_Json.find("Scale_Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Scale_Speed"], m_tEffectMeshDesc.vScaleSpeed);
	
	if (In_Json.find("Scale_Force") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Scale_Force"], m_tEffectMeshDesc.vScaleForce);
	
	if (In_Json.find("Min_Scale") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Scale"], m_tEffectMeshDesc.vMinScale);

	if (In_Json.find("Max_Scale") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Scale"], m_tEffectMeshDesc.vMaxScale);

	if (In_Json.find("Shader_Pass_Index") != In_Json.end())
		m_tEffectMeshDesc.iShaderPassIndex = In_Json["Shader_Pass_Index"];

	if (In_Json.find("Alpha_Discard_Ratio") != In_Json.end())
		m_tEffectMeshDesc.fDiscardRatio = In_Json["Alpha_Discard_Ratio"];

	if (In_Json.find("Start_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Start_Color"], m_tEffectMeshDesc.vStartColor);
	if (In_Json.find("Color_Speed") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Color_Speed"], m_tEffectMeshDesc.vColorSpeed);
	if (In_Json.find("Color_Force") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Color_Force"], m_tEffectMeshDesc.vColorForce);
	if (In_Json.find("Min_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Color"],   m_tEffectMeshDesc.vMinColor);
	if (In_Json.find("Max_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Color"],   m_tEffectMeshDesc.vMaxColor);

	if (In_Json.find("UV_Diffuse_Index") != In_Json.end())
		m_tEffectMeshDesc.iDiffuseTextureIndex = In_Json["UV_Diffuse_Index"];

	if (In_Json.find("UV_Noise_Index") != In_Json.end())
		m_tEffectMeshDesc.iNoiseTextureIndex = In_Json["UV_Noise_Index"];

	if (In_Json.find("UV_Mask_Index") != In_Json.end())
		m_tEffectMeshDesc.iMaskTextureIndex = In_Json["UV_Mask_Index"];

	if (In_Json.find("Dynamic_Noise_Option") != In_Json.end())
		m_tEffectMeshDesc.bDynamicNoiseOption = In_Json["Dynamic_Noise_Option"];

	if (In_Json.find("UV_Diffuse_Wrap_Option") != In_Json.end())
		m_tEffectMeshDesc.bDiffuseWrap = In_Json["UV_Diffuse_Wrap_Option"];
	if (In_Json.find("UV_Noise_Wrap_Option") != In_Json.end())
		m_tEffectMeshDesc.bNoiseWrap = In_Json["UV_Noise_Wrap_Option"];
	if (In_Json.find("UV_Mask_Wrap_Option") != In_Json.end())
		m_tEffectMeshDesc.bMaskWrap = In_Json["UV_Mask_Wrap_Option"];

	if (In_Json.find("UV_Wrap_Weight") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["UV_Wrap_Weight"], m_tEffectMeshDesc.vWrapWeight);

	if (In_Json.find("Diffuse_Start_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Diffuse_Start_UV"], m_tEffectMeshDesc.vDiffuseStartUV);
	if (In_Json.find("Diffuse_UV_Speed") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Speed"], m_tEffectMeshDesc.vDiffuseUVSpeed);
	if (In_Json.find("Diffuse_UV_Force") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Force"], m_tEffectMeshDesc.vDiffuseUVForce);
	if (In_Json.find("Diffuse_Min_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Diffuse_Min_UV"],   m_tEffectMeshDesc.vDiffuseUVMin);
	if (In_Json.find("Diffuse_Max_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Diffuse_Max_UV"],   m_tEffectMeshDesc.vDiffuseUVMax);

	if (In_Json.find("Noise_Start_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Noise_Start_UV"], m_tEffectMeshDesc.vNoiseStartUV);
	if (In_Json.find("Noise_UV_Speed") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Noise_UV_Speed"], m_tEffectMeshDesc.vNoiseUVSpeed);
	if (In_Json.find("Noise_UV_Force") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Noise_UV_Force"], m_tEffectMeshDesc.vNoiseUVForce);
	if (In_Json.find("Noise_Min_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Noise_Min_UV"],   m_tEffectMeshDesc.vNoiseUVMin);
	if (In_Json.find("Noise_Max_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Noise_Max_UV"],   m_tEffectMeshDesc.vNoiseUVMax);

	if (In_Json.find("Mask_Start_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Mask_Start_UV"], m_tEffectMeshDesc.vMaskStartUV);
	if (In_Json.find("Mask_UV_Speed") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Mask_UV_Speed"], m_tEffectMeshDesc.vMaskUVSpeed);
	if (In_Json.find("Mask_UV_Force") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Mask_UV_Force"], m_tEffectMeshDesc.vMaskUVForce);
	if (In_Json.find("Mask_Min_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Mask_Min_UV"],   m_tEffectMeshDesc.vMaskUVMin);
	if (In_Json.find("Mask_Max_UV") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Mask_Max_UV"],   m_tEffectMeshDesc.vMaskUVMax);

	if (In_Json.find("Bloom") != In_Json.end())
		m_tEffectMeshDesc.bBloom = In_Json["Bloom"];
	if (In_Json.find("Glow") != In_Json.end())
		m_tEffectMeshDesc.bGlow = In_Json["Glow"];
	if (In_Json.find("Distortion") != In_Json.end())
		m_tEffectMeshDesc.bDistortion = In_Json["Distortion"];

	if (In_Json.find("Start_Glow_Color") != In_Json.end())
	{
		CJson_Utility::Load_Float4(In_Json["Start_Glow_Color"], m_tEffectMeshDesc.vStartGlowColor);
		CJson_Utility::Load_Float4(In_Json["Glow_Color_Speed"], m_tEffectMeshDesc.vGlowColorSpeed);
		CJson_Utility::Load_Float4(In_Json["Glow_Color_Force"], m_tEffectMeshDesc.vGlowColorForce);
	}

	if (In_Json.find("Collider") != In_Json.end())
	{
		m_tEffectMeshDesc.bCollider            = In_Json["Collider"];
		m_tEffectMeshDesc.bWeaponSyncTransform = In_Json["Sync_Transform"];
		m_tEffectMeshDesc.fWeaponLifeTime      = In_Json["Weapon_LifeTime"];
		m_tEffectMeshDesc.fWeaponScale         = In_Json["Weapon_Scale"];
		m_tEffectMeshDesc.iHitType             = In_Json["Hit_Type"];
		m_tEffectMeshDesc.fDamage              = In_Json["Damage"];
		CJson_Utility::Load_Float3(In_Json["Weapon_Offset"], m_tEffectMeshDesc.vWeaponOffset);
		m_tEffectMeshDesc.fHitFreq             = In_Json["HitFreq"];
	}

	if (In_Json.find("Option_Type") != In_Json.end())
		m_tEffectMeshDesc.iOptionType = In_Json["Option_Type"];

	m_pModelCom.lock()->Init_Model(m_szEffectName.c_str());
}

#ifdef _DEBUG
void CCustomEffectMesh::Key_Input_ControlMesh(_float fTimeDelta)
{
	if (m_tEffectMeshDesc.bOnFocus)
	{
		if (KEY_INPUT(KEY::NUMPAD8, KEY_STATE::HOLD))
		{
			if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Straight_WorldAxis(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
			else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Straight(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
		}

		if (KEY_INPUT(KEY::NUMPAD2, KEY_STATE::HOLD))
		{
			if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Backward_WorldAxis(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
			else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Backward(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
		}

		if (KEY_INPUT(KEY::NUMPAD4, KEY_STATE::HOLD))
		{
			if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Left_WorldAxis(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
			else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Left(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
		}

		if (KEY_INPUT(KEY::NUMPAD6, KEY_STATE::HOLD))
		{
			if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Right_WorldAxis(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
			else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Right(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
		}

		if (KEY_INPUT(KEY::NUMPAD1, KEY_STATE::HOLD))
		{
			if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Up_WorldAxis(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
			else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Up(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
		}

		if (KEY_INPUT(KEY::NUMPAD3, KEY_STATE::HOLD))
		{
			if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Down_WorldAxis(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
			else if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
				m_pTransformCom.lock()->Go_Down(fTimeDelta * m_tEffectMeshDesc.fSpeedPerSec);
		}
	}
}

void CCustomEffectMesh::Apply_ImGui_Controls_to_Mesh()
{
	if (m_tEffectMeshDesc.bSyncStartPositionToController)
	{
		XMStoreFloat3(&m_tEffectMeshDesc.vStartPosition, m_pTransformCom.lock()->Get_State(CTransform::STATE::STATE_TRANSLATION));
	}

	if (m_tEffectMeshDesc.bSyncStartRotationToController)
	{
		XMStoreFloat3(&m_vCurrentRotation, XMLoadFloat3(&m_tEffectMeshDesc.vStartRotation));
	}
}
#endif // _DEBUG

void CCustomEffectMesh::Play_Internal(_float fFrameTime)
{
	this->Update_Position(fFrameTime);
	this->Update_Rotation(fFrameTime);
	this->Update_Scale(fFrameTime);
	this->Update_Color(fFrameTime);
	this->Update_Diffuse(fFrameTime);
	this->Updaste_Noise(fFrameTime);
	this->Update_Mask(fFrameTime);
	this->Update_Glow(fFrameTime);
}

void CCustomEffectMesh::Update_Position(_float fFrameTime)
{
	if (m_pBoneNode.lock())
	{
		_float4x4 TempMat = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Get_TransformationMatrix();
		_matrix ModelTranMat = XMLoadFloat4x4(&TempMat);
		_matrix BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		if (m_tEffectMeshDesc.bFollowTransform)
		{
			if (m_pParentTransformCom.lock())
				m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
			else
				m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix);
		}
	}

	m_vCurrentSpeed = SMath::Mul_Float3(m_tEffectMeshDesc.vSpeed, fFrameTime);
	m_vCurrentForce = SMath::Add_Float3(m_vCurrentForce, SMath::Mul_Float3(m_tEffectMeshDesc.vForce, fFrameTime));

	_vector vMovePosition = XMLoadFloat3(&m_vCurrentSpeed) + XMLoadFloat3(&m_vCurrentForce);
	_vector vAbsolutePosition = vMovePosition / fFrameTime;

	XMVectorSetX(vAbsolutePosition, max(m_tEffectMeshDesc.vMinSpeed.x, min(m_tEffectMeshDesc.vMaxSpeed.x, XMVectorGetX(vAbsolutePosition))));
	XMVectorSetY(vAbsolutePosition, max(m_tEffectMeshDesc.vMinSpeed.y, min(m_tEffectMeshDesc.vMaxSpeed.y, XMVectorGetY(vAbsolutePosition))));
	XMVectorSetZ(vAbsolutePosition, max(m_tEffectMeshDesc.vMinSpeed.z, min(m_tEffectMeshDesc.vMaxSpeed.z, XMVectorGetZ(vAbsolutePosition))));

	vMovePosition = vAbsolutePosition * fFrameTime;

	m_pTransformCom.lock()->Add_Position(vMovePosition);
}

void CCustomEffectMesh::Update_Rotation(_float fFrameTime)
{
	_vector CurrentRotation = XMLoadFloat3(&m_vCurrentRotation);
	_vector MoveRotation    = XMLoadFloat3(&m_tEffectMeshDesc.vRotationSpeed) * fFrameTime;
	_vector MoveForce       = XMLoadFloat3(&m_tEffectMeshDesc.vRotationForce) * fFrameTime;
	_vector CurrentForce    = XMLoadFloat3(&m_vCurrentRotationForce);
	CurrentForce    += MoveForce;
	MoveRotation    += CurrentForce;
	CurrentRotation += MoveRotation;

	XMStoreFloat3(&m_vCurrentRotation, CurrentRotation);
	XMStoreFloat3(&m_vCurrentRotationForce, CurrentForce);

	_vector Quaternion = 
		XMQuaternionRotationRollPitchYaw
		(
			XMVectorGetX(CurrentRotation)
			, XMVectorGetY(CurrentRotation)
			, XMVectorGetZ(CurrentRotation)
		);

	m_pTransformCom.lock()->Rotation_Quaternion(Quaternion);
}

void CCustomEffectMesh::Update_Scale(_float fFrameTime)
{
	m_vCurrentScaleSpeed = SMath::Mul_Float3(m_tEffectMeshDesc.vScaleSpeed, fFrameTime);
	m_vCurrentScaleForce = SMath::Add_Float3(m_vCurrentScaleForce, SMath::Mul_Float3(m_tEffectMeshDesc.vScaleForce, fFrameTime));

	_vector vMoveScale = XMLoadFloat3(&m_vCurrentScaleSpeed) + XMLoadFloat3(&m_vCurrentScaleForce);
	_vector vCurrentScale = XMLoadFloat3(&m_vCurrentScale);
	vCurrentScale += vMoveScale;

	XMVectorSetX(vCurrentScale, max(m_tEffectMeshDesc.vMinScale.x, min(m_tEffectMeshDesc.vMaxScale.x, XMVectorGetX(vCurrentScale))));
	XMVectorSetY(vCurrentScale, max(m_tEffectMeshDesc.vMinScale.y, min(m_tEffectMeshDesc.vMaxScale.y, XMVectorGetY(vCurrentScale))));
	XMVectorSetZ(vCurrentScale, max(m_tEffectMeshDesc.vMinScale.z, min(m_tEffectMeshDesc.vMaxScale.z, XMVectorGetZ(vCurrentScale))));

	XMStoreFloat3(&m_vCurrentScale, vCurrentScale);

	m_pTransformCom.lock()->Set_Scaled(m_vCurrentScale);
}

void CCustomEffectMesh::Update_Color(_float fFrameTime)
{
	_vector vColorSpeed = XMLoadFloat4(&m_tEffectMeshDesc.vColorSpeed) * fFrameTime;
	m_vCurrentColorForce = SMath::Add_Float4(m_vCurrentColorForce, SMath::Mul_Float4(m_tEffectMeshDesc.vColorForce, fFrameTime));

	_vector vMoveColor = vColorSpeed + XMLoadFloat4(&m_vCurrentColorForce);
	_vector vCurrentColor = XMLoadFloat4(&m_vCurrentColor);
	vCurrentColor += vMoveColor;

	XMVectorSetX(vCurrentColor, max(m_tEffectMeshDesc.vMinColor.x, min(m_tEffectMeshDesc.vMaxColor.x, XMVectorGetX(vCurrentColor))));
	XMVectorSetY(vCurrentColor, max(m_tEffectMeshDesc.vMinColor.y, min(m_tEffectMeshDesc.vMaxColor.y, XMVectorGetY(vCurrentColor))));
	XMVectorSetZ(vCurrentColor, max(m_tEffectMeshDesc.vMinColor.z, min(m_tEffectMeshDesc.vMaxColor.z, XMVectorGetZ(vCurrentColor))));
	XMVectorSetW(vCurrentColor, max(m_tEffectMeshDesc.vMinColor.w, min(m_tEffectMeshDesc.vMaxColor.w, XMVectorGetW(vCurrentColor))));

	XMStoreFloat4(&m_vCurrentColor, vCurrentColor);
}

void CCustomEffectMesh::Update_Diffuse(_float fFrameTime)
{
	_vector vDiffuseUVSpeed = XMLoadFloat2(&m_tEffectMeshDesc.vDiffuseUVSpeed) * fFrameTime;
	m_vDiffuseCurrentUVForce.x += m_tEffectMeshDesc.vDiffuseUVForce.x * fFrameTime;
	m_vDiffuseCurrentUVForce.y += m_tEffectMeshDesc.vDiffuseUVForce.y * fFrameTime;

	_vector vDiffuseMoveUV = vDiffuseUVSpeed + XMLoadFloat2(&m_vDiffuseCurrentUVForce);
	_vector vDiffuseCurrentUV = XMLoadFloat2(&m_vDiffuseCurrentUV);
	vDiffuseCurrentUV += vDiffuseMoveUV;

	XMVectorSetX(vDiffuseCurrentUV, max(m_tEffectMeshDesc.vDiffuseUVMin.x, min(m_tEffectMeshDesc.vDiffuseUVMax.x, XMVectorGetX(vDiffuseCurrentUV))));
	XMVectorSetY(vDiffuseCurrentUV, max(m_tEffectMeshDesc.vDiffuseUVMin.y, min(m_tEffectMeshDesc.vDiffuseUVMax.y, XMVectorGetY(vDiffuseCurrentUV))));

	XMStoreFloat2(&m_vDiffuseCurrentUV, vDiffuseCurrentUV);
}

void CCustomEffectMesh::Updaste_Noise(_float fFrameTime)
{
	_vector vNoiseUVSpeed = XMLoadFloat2(&m_tEffectMeshDesc.vNoiseUVSpeed) * fFrameTime;
	m_vNoiseCurrentUVForce.x += m_tEffectMeshDesc.vNoiseUVForce.x * fFrameTime;
	m_vNoiseCurrentUVForce.y += m_tEffectMeshDesc.vNoiseUVForce.y * fFrameTime;

	_vector vNoiseMoveUV = vNoiseUVSpeed + XMLoadFloat2(&m_vNoiseCurrentUVForce);
	_vector vNoiseCurrentUV = XMLoadFloat2(&m_vNoiseCurrentUV);
	vNoiseCurrentUV += vNoiseMoveUV;

	XMVectorSetX(vNoiseCurrentUV, max(m_tEffectMeshDesc.vNoiseUVMin.x, min(m_tEffectMeshDesc.vNoiseUVMax.x, XMVectorGetX(vNoiseCurrentUV))));
	XMVectorSetY(vNoiseCurrentUV, max(m_tEffectMeshDesc.vNoiseUVMin.y, min(m_tEffectMeshDesc.vNoiseUVMax.y, XMVectorGetY(vNoiseCurrentUV))));

	XMStoreFloat2(&m_vNoiseCurrentUV, vNoiseCurrentUV);
}

void CCustomEffectMesh::Update_Mask(_float fFrameTime)
{
	_vector vMaskUVSpeed = XMLoadFloat2(&m_tEffectMeshDesc.vMaskUVSpeed) * fFrameTime;
	m_vMaskCurrentUVForce.x += m_tEffectMeshDesc.vMaskUVForce.x * fFrameTime;
	m_vMaskCurrentUVForce.y += m_tEffectMeshDesc.vMaskUVForce.y * fFrameTime;

	_vector vMaskMoveUV = vMaskUVSpeed + XMLoadFloat2(&m_vMaskCurrentUVForce);
	_vector vMaskCurrentUV = XMLoadFloat2(&m_vMaskCurrentUV);
	vMaskCurrentUV += vMaskMoveUV;

	XMVectorSetX(vMaskCurrentUV, max(m_tEffectMeshDesc.vMaskUVMin.x, min(m_tEffectMeshDesc.vMaskUVMax.x, XMVectorGetX(vMaskCurrentUV))));
	XMVectorSetY(vMaskCurrentUV, max(m_tEffectMeshDesc.vMaskUVMin.y, min(m_tEffectMeshDesc.vMaskUVMax.y, XMVectorGetY(vMaskCurrentUV))));

	XMStoreFloat2(&m_vMaskCurrentUV, vMaskCurrentUV);
}

void CCustomEffectMesh::Update_Glow(_float fFrameTime)
{
	_vector vGlowSpeed = XMLoadFloat4(&m_tEffectMeshDesc.vGlowColorSpeed) * fFrameTime;
	m_vCurrentGlowForce.x += m_tEffectMeshDesc.vGlowColorForce.x * fFrameTime;
	m_vCurrentGlowForce.y += m_tEffectMeshDesc.vGlowColorForce.y * fFrameTime;
	m_vCurrentGlowForce.z += m_tEffectMeshDesc.vGlowColorForce.z * fFrameTime;
	m_vCurrentGlowForce.w += m_tEffectMeshDesc.vGlowColorForce.w * fFrameTime;

	_vector vMoveGlowColor = vGlowSpeed + XMLoadFloat4(&m_vCurrentGlowForce);
	_vector vCurrentGlowColor = XMLoadFloat4(&m_vCurrentGlowColor);
	vCurrentGlowColor += vMoveGlowColor;

	XMStoreFloat4(&m_vCurrentGlowColor, vCurrentGlowColor);
}

void CCustomEffectMesh::Apply_Easing
(
	_float2& vTarget
	, EASING_TYPE eEasingType
	, _vector vStartPoint
	, _vector vTargetPoint
	, _float fElapsedTime
	, _float fTotalTime
)
{
#define MACRO(FunctionName)\
XMStoreFloat2(&vTarget, FunctionName(vStartPoint, vTargetPoint, fElapsedTime, fTotalTime))
	switch (eEasingType)
	{
	case EASING_TYPE::LINEAR:
		MACRO(CEasing_Utillity::Linear);
		break;
	case EASING_TYPE::QUAD_IN:
		MACRO(CEasing_Utillity::QuadIn);
		break;
	case EASING_TYPE::QUAD_OUT:
		MACRO(CEasing_Utillity::QuadOut);
		break;
	case EASING_TYPE::QUAD_INOUT:
		MACRO(CEasing_Utillity::QuadInOut);
		break;
	case EASING_TYPE::CUBIC_IN:
		MACRO(CEasing_Utillity::CubicIn);
		break;
	case EASING_TYPE::CUBIC_OUT:
		MACRO(CEasing_Utillity::CubicOut);
		break;
	case EASING_TYPE::CUBIC_INOUT:
		MACRO(CEasing_Utillity::CubicInOut);
		break;
	case EASING_TYPE::QUART_IN:
		MACRO(CEasing_Utillity::QuadIn);
		break;
	case EASING_TYPE::QUART_OUT:
		MACRO(CEasing_Utillity::QuadOut);
		break;
	case EASING_TYPE::QUART_INOUT:
		MACRO(CEasing_Utillity::QuadInOut);
		break;
	case EASING_TYPE::QUINT_IN:
		MACRO(CEasing_Utillity::QuintIn);
		break;
	case EASING_TYPE::QUINT_OUT:
		MACRO(CEasing_Utillity::QuintOut);
		break;
	case EASING_TYPE::QUINT_INOUT:
		MACRO(CEasing_Utillity::QuintInOut);
		break;
	case EASING_TYPE::SINE_IN:
		MACRO(CEasing_Utillity::SineIn);
		break;
	case EASING_TYPE::SINE_OUT:
		MACRO(CEasing_Utillity::SineOut);
		break;
	case EASING_TYPE::SINE_INOUT:
		MACRO(CEasing_Utillity::SineInOut);
		break;
	case EASING_TYPE::EXPO_IN:
		MACRO(CEasing_Utillity::ExpoIn);
		break;
	case EASING_TYPE::EXPO_OUT:
		MACRO(CEasing_Utillity::ExpoOut);
		break;
	case EASING_TYPE::EXPO_INOUT:
		MACRO(CEasing_Utillity::ExpoInOut);
		break;
	case EASING_TYPE::CIRC_IN:
		MACRO(CEasing_Utillity::CircIn);
		break;
	case EASING_TYPE::CIRC_OUT:
		MACRO(CEasing_Utillity::CircOut);
		break;
	case EASING_TYPE::CIRC_INOUT:
		MACRO(CEasing_Utillity::CircInOut);
		break;
	case EASING_TYPE::ELASTIC_IN:
		MACRO(CEasing_Utillity::ElasticIn);
		break;
	case EASING_TYPE::ELASTIC_OUT:
		MACRO(CEasing_Utillity::ElasticOut);
		break;
	case EASING_TYPE::ELASTIC_INOUT:
		MACRO(CEasing_Utillity::ElasticInOut);
		break;
	case EASING_TYPE::BOUNCE_IN:
		MACRO(CEasing_Utillity::BounceIn);
		break;
	case EASING_TYPE::BOUNCE_OUT:
		MACRO(CEasing_Utillity::BounceOut);
		break;
	default:
		return;
	}
#undef MACRO
}

#ifdef _DEBUG
void CCustomEffectMesh::Clone_EffectMesh()
{
	GET_SINGLE(CWindow_EffectHierarchyView)->Add_EffectMesh_Internal(m_tEffectMeshDesc, m_szEffectName.c_str());

}
#endif // _DEBUG


void CCustomEffectMesh::OnEventMessage(_uint iArg)
{

#ifdef _DEBUG
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("CustomEffectMesh"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			if (ImGui::Button("Clone"))
				Clone_EffectMesh();	

			// TODO : for imgui - mesh keyboard control
			ImGui::Text("Focus Control");
			ImGui::SameLine();
			ImGui::Checkbox("##Control Focus", &m_tEffectMeshDesc.bOnFocus);
			if (m_tEffectMeshDesc.bOnFocus)
			{
				ImGui::Separator();
				ImGui::Text("Control Keys");
				ImGui::Text("NumPad 8 : move  forward"); ImGui::SameLine();
				ImGui::Text("NumPad 2 : move  backward");
				ImGui::Text("NumPad 4 : move  leftward"); ImGui::SameLine();
				ImGui::Text("NumPad 6 : move rightward");
				ImGui::Text("NumPad 1 : move      down"); ImGui::SameLine();
				ImGui::Text("NumPad 3 : move        up");
				ImGui::Text("! Hold LShift to move by World Axis !");
				ImGui::Separator();
				ImGui::Separator();

				ImGui::Text("Control Speed");
				ImGui::DragFloat("##Control Speed", &m_tEffectMeshDesc.fSpeedPerSec);
				ImGui::Separator();
			}

			ImGui::Separator();

			ImGui::Text("[ Init Time ]");
			ImGui::SetNextItemWidth(100.f);
			ImGui::DragFloat("##Init Time", &m_tEffectMeshDesc.fInitTime, 0.2f);

			ImGui::Text("[ Life Time ]");
			ImGui::SetNextItemWidth(100.f);
			ImGui::DragFloat("##Life Time", &m_tEffectMeshDesc.fLifeTime, 0.2f);
			ImGui::Separator();

			ImGui::Text("Follow Transform");
			ImGui::SameLine();
			ImGui::Checkbox("##Follow_Transform", &m_tEffectMeshDesc.bFollowTransform);

			ImGui::Text("BillBoard");
			ImGui::SameLine();
			ImGui::Checkbox("##BillBoard", &m_tEffectMeshDesc.bBillBoard);

			ImGui::Separator();

			ImGui::Text("Sync Animation");
			ImGui::SameLine();
			ImGui::Checkbox("##SyncAnimation", &m_tEffectMeshDesc.bSyncAnimation);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt("##SyncAnimationKey", &m_tEffectMeshDesc.iSyncAnimationKey);
			ImGui::Separator();
#pragma region Boner
			ImGui::Text("Boner"); ImGui::SameLine();
			ImGui::Checkbox("##Is_Boner", &m_tEffectMeshDesc.bBoner);

			if (m_tEffectMeshDesc.bBoner)
			{
				if (ImGui::Button("Get Bone List"))
				{
					m_pParentTransformCom = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Component<CTransform>().lock();
					m_AllBoneNames = GET_SINGLE(CWindow_AnimationModelView)->Get_AllBoneNames();
				}

				if (0 == m_AllBoneNames.size())
					ImGui::Text("!!!! No Bone !!!!");
				else
				{
					if (ImGui::BeginListBox("Bone List - Mesh Effect"))
					{
						for (_int n(0); n < m_AllBoneNames.size(); n++)
						{
							const _bool is_selected = (m_iCurrentBoneIndex == n);
							if (ImGui::Selectable(m_AllBoneNames[n].c_str(), is_selected))
							{
								m_strBoneName = m_AllBoneNames[n];
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}

				}

				if (ImGui::Button("Bind Bone"))
				{
					m_pBoneNode = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Find_BoneNode(m_strBoneName);
					if (nullptr == m_pBoneNode.lock())
					{
						MSG_BOX("!!! Invalid Bone Name !!!");
						assert(0);
					}
				}

				if (m_pBoneNode.lock())
				{
					ImGui::Text("Binded to [ "); ImGui::SameLine();
					ImGui::Text(m_pBoneNode.lock()->Get_Name()); ImGui::SameLine();
					ImGui::Text(" ]");
				}
			}
			else
			{
				m_pBoneNode.reset();
				m_strBoneName.clear();
			}
#pragma endregion
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, "[ Position ]");
			ImGui::NewLine();
#pragma region Position
			ImGui::Text("Sync Controller"); ImGui::SameLine();
			ImGui::Checkbox("##Sync_Start_Position_to_Controller", &m_tEffectMeshDesc.bSyncStartPositionToController);
			ImGui::Text("Start Position");
			ImGui::DragFloat3("##Start Position", &m_tEffectMeshDesc.vStartPosition.x, 0.1f);
#pragma endregion

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::TextColored(ImVec4{ 0.f, 1.f, 0.f, 1.f }, "[ Speed ]");
			ImGui::NewLine();
#pragma region Speed
			ImGui::Text("Speed");
			ImGui::DragFloat3("##Speed", &m_tEffectMeshDesc.vSpeed.x, 0.1f);

			ImGui::Text("Force");
			ImGui::DragFloat3("##Force", &m_tEffectMeshDesc.vForce.x, 0.1f);

			ImGui::Text("Min Speed");
			ImGui::DragFloat3("##Min Speed", &m_tEffectMeshDesc.vMinSpeed.x, 0.1f);

			ImGui::Text("Max Speed");
			ImGui::DragFloat3("##Max Speed", &m_tEffectMeshDesc.vMaxSpeed.x, 0.1f);
#pragma endregion

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::TextColored(ImVec4{ 0.f, 0.f, 1.f, 1.f }, "[ Rotation ]");
			ImGui::NewLine();
#pragma region Rotation
			ImGui::Text("Sync Controller"); ImGui::SameLine();
			ImGui::Checkbox("##Sync_Start_Rotation_to_Controller", &m_tEffectMeshDesc.bSyncStartRotationToController);

			ImGui::Text("Start Rotation");
			ImGui::DragFloat3("##Start Rotation", &m_tEffectMeshDesc.vStartRotation.x, 0.01f);

			ImGui::Text("Rotation Speed");
			ImGui::DragFloat3("##Rotation Speed", &m_tEffectMeshDesc.vRotationSpeed.x, 0.01f);

			ImGui::Text("Rotation Force");
			ImGui::DragFloat3("##Rotation Force", &m_tEffectMeshDesc.vRotationForce.x, 0.01f);

			ImGui::Text("Max Rotation");
			ImGui::DragFloat3("##Max Rotation", &m_tEffectMeshDesc.vMaxRotation.x, 0.01f);
#pragma endregion

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::TextColored(ImVec4{ 0.3f, 0.f, 0.5f, 1.f }, "[ Scale ]");
			ImGui::NewLine();
#pragma region Scale
			ImGui::Text("Start Scale");
			ImGui::DragFloat3("##Start Scale", &m_tEffectMeshDesc.vStartScale.x, 0.01f);

			m_tEffectMeshDesc.vStartScale.x = max(0.00001f, m_tEffectMeshDesc.vStartScale.x);
			m_tEffectMeshDesc.vStartScale.y = max(0.00001f, m_tEffectMeshDesc.vStartScale.y);
			m_tEffectMeshDesc.vStartScale.z = max(0.00001f, m_tEffectMeshDesc.vStartScale.z);

			ImGui::Text("Scale Speed");
			ImGui::DragFloat3("##Scale Speed", &m_tEffectMeshDesc.vScaleSpeed.x, 0.01f);

			ImGui::Text("Scale Force");
			ImGui::DragFloat3("##Scale Force", &m_tEffectMeshDesc.vScaleForce.x, 0.01f);

			ImGui::Text("Min Scale");
			ImGui::DragFloat3("##Min Scale", &m_tEffectMeshDesc.vMinScale.x, 0.01f);

			ImGui::Text("Max Scale");
			ImGui::DragFloat3("##Max Scale", &m_tEffectMeshDesc.vMaxScale.x, 0.01f);
#pragma endregion

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::Text("Shaders");

			ImGui::Separator();
			ImGui::Text("Shader Pass");
			ImGui::Text("[0]Default"); ImGui::SameLine(); ImGui::Text(" | "); ImGui::SameLine();
			ImGui::Text("[1]Distortion");
			ImGui::Text("[2]Soft");
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt("##Shader_Pass_Index", &m_tEffectMeshDesc.iShaderPassIndex);
			ImGui::Separator();

			ImGui::Text("Distortion"); ImGui::SameLine();
			ImGui::Checkbox("##Distortion", &m_tEffectMeshDesc.bDistortion); ImGui::SameLine();

			ImGui::Text("Bloom"); ImGui::SameLine();
			ImGui::Checkbox("##Bloom", &m_tEffectMeshDesc.bBloom); ImGui::SameLine();

			ImGui::Text("Glow"); ImGui::SameLine();
			ImGui::Checkbox("##Glow", &m_tEffectMeshDesc.bGlow);

			if (m_tEffectMeshDesc.bGlow)
			{
				ImGui::Text("Start Glow Color");
				ImGui::DragFloat4("##Start_Glow_Color", &m_tEffectMeshDesc.vStartGlowColor.x, 0.01f);

				ImGui::Text("Glow Color Speed ");
				ImGui::DragFloat4("##Glow_Color_Speed", &m_tEffectMeshDesc.vGlowColorSpeed.x, 0.01f);

				ImGui::Text("Glow Color Force");
				ImGui::DragFloat4("##Glow_Color_Force", &m_tEffectMeshDesc.vGlowColorForce.x, 0.01f);
			}

			ImGui::Separator();

			ImGui::TextColored(ImVec4{ 1.f, 0.f, 1.f, 1.f }, "[ Color ]");
			ImGui::NewLine();
#pragma region Color
			ImGui::Text("Discard Ratio"); ImGui::SameLine();
			ImGui::SetNextItemWidth(100.f);
			ImGui::DragFloat("##Discard Ratio", &m_tEffectMeshDesc.fDiscardRatio, 0.01f, 0.f, 1.f, "%.3f", 0);

			ImGui::Text("Start Color");
			ImGui::DragFloat4("##Start Color", &m_tEffectMeshDesc.vStartColor.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::Text("Color Speed");
			ImGui::DragFloat4("##Color Speed", &m_tEffectMeshDesc.vColorSpeed.x, 0.01f, -1.f, 1.f, "%.5f");

			ImGui::Text("Color Force");
			ImGui::DragFloat4("##Color Force", &m_tEffectMeshDesc.vColorForce.x, 0.01f, -1.f, 1.f, "%.5f");

			ImGui::Text("Min Color");
			ImGui::DragFloat4("##Min Color", &m_tEffectMeshDesc.vMinColor.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::Text("Max Color");
			ImGui::DragFloat4("##Max Color", &m_tEffectMeshDesc.vMaxColor.x, 0.01f, 0.f, 1.f, "%.5f");
#pragma endregion

			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();

			ImGui::TextColored(ImVec4{ 0.f, 0.5f, 0.5f, 1.f }, "[ Textures ]");
			ImGui::NewLine();
#pragma region Textures
			ImGui::Text("Texture Index");
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt("Diffuse", &m_tEffectMeshDesc.iDiffuseTextureIndex, 1, 0);
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt("Noise", &m_tEffectMeshDesc.iNoiseTextureIndex, 1, 0);
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt("Mask", &m_tEffectMeshDesc.iMaskTextureIndex, 1, 0);

			ImGui::Text("Dynamic Noise"); ImGui::SameLine();
			ImGui::Checkbox("##Dynamic Noise Option", &m_tEffectMeshDesc.bDynamicNoiseOption);

			ImGui::NewLine();

			ImGui::Text("Checked : Wrap | Unchecked : Clamp");
			ImGui::Text("Diffuse"); ImGui::SameLine();
			ImGui::Checkbox("##WrapOption_Diffuse", &m_tEffectMeshDesc.bDiffuseWrap); ImGui::SameLine();
			ImGui::Text("| Noise"); ImGui::SameLine();
			ImGui::Checkbox("##WrapOption_Noise", &m_tEffectMeshDesc.bNoiseWrap); ImGui::SameLine();
			ImGui::Text("| Mask"); ImGui::SameLine();
			ImGui::Checkbox("##WrapOption_Mask", &m_tEffectMeshDesc.bMaskWrap);

			ImGui::Separator();

			ImGui::Text("Wrap Weight");
			if (m_tEffectMeshDesc.bDiffuseWrap)
			{
				ImGui::SetNextItemWidth(100.f);
				ImGui::DragFloat("##DiffuseTextureWrapWeight", &m_tEffectMeshDesc.vWrapWeight.x, 0.01f); ImGui::SameLine();
				ImGui::Text("[Diffuse]");
			}
			if (m_tEffectMeshDesc.bNoiseWrap)
			{
				ImGui::SetNextItemWidth(100.f);
				ImGui::DragFloat("##NoiseTextureWrapWeight", &m_tEffectMeshDesc.vWrapWeight.y, 0.01f); ImGui::SameLine();
				ImGui::Text("[Noise]");
			}
			if (m_tEffectMeshDesc.bMaskWrap)
			{
				ImGui::SetNextItemWidth(100.f);
				ImGui::DragFloat("##MaskTextureWrapWeight", &m_tEffectMeshDesc.vWrapWeight.z, 0.01f); ImGui::SameLine();
				ImGui::Text("[Mask]");
			}
#pragma endregion

			ImGui::Separator();

#pragma region UV Options
#pragma region Diffuse UV
			ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, "[ Diffuse ]");
			ImGui::NewLine();

			ImGui::Text("Start UV");
			ImGui::DragFloat2("##Diffuse Start UV", &m_tEffectMeshDesc.vDiffuseStartUV.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::Text("UV Speed");
			ImGui::DragFloat2("##Diffuse UV Speed", &m_tEffectMeshDesc.vDiffuseUVSpeed.x, 0.01f);

			ImGui::Text("UV Force");
			ImGui::DragFloat2("##Diffuse UV Force", &m_tEffectMeshDesc.vDiffuseUVForce.x, 0.01f);

			ImGui::Text("UV Min");
			ImGui::DragFloat2("##Diffuse UV Min", &m_tEffectMeshDesc.vDiffuseUVMin.x, 0.01f);

			ImGui::Text("Diffuse vUV Max");
			ImGui::DragFloat2("##Diffuse UV Max", &m_tEffectMeshDesc.vDiffuseUVMax.x, 0.01f);
#pragma endregion
			ImGui::Separator();
#pragma region Noise UV
			ImGui::TextColored(ImVec4{ 0.f, 1.f, 0.f, 1.f }, "[ Noise ]");
			ImGui::NewLine();

			ImGui::Text("Start UV");
			ImGui::DragFloat2("##Noise Start UV", &m_tEffectMeshDesc.vNoiseStartUV.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::Text("UV Speed");
			ImGui::DragFloat2("##Noise UV Speed", &m_tEffectMeshDesc.vNoiseUVSpeed.x, 0.01f);

			ImGui::Text("UV Force");
			ImGui::DragFloat2("##Noise UV Force", &m_tEffectMeshDesc.vNoiseUVForce.x, 0.01f);

			ImGui::Text("vUV Min");
			ImGui::DragFloat2("##Noise UV Min", &m_tEffectMeshDesc.vNoiseUVMin.x, 0.01f);

			ImGui::Text("vUV Max");
			ImGui::DragFloat2("##Noise UV Max", &m_tEffectMeshDesc.vNoiseUVMax.x, 0.01f);
#pragma endregion
			ImGui::Separator();
#pragma region Mask UV
			ImGui::TextColored(ImVec4{ 0.f, 0.f, 1.f, 1.f }, "[ Mask ]");
			ImGui::NewLine();

			ImGui::Text("Start UV");
			ImGui::DragFloat2("Mask ##Start UV", &m_tEffectMeshDesc.vMaskStartUV.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::Text("UV Speed");
			ImGui::DragFloat2("##Mask UV Speed", &m_tEffectMeshDesc.vMaskUVSpeed.x, 0.01f);

			ImGui::Text("UV Force");
			ImGui::DragFloat2("##Mask UV Force", &m_tEffectMeshDesc.vMaskUVForce.x, 0.01f);

			ImGui::Text("vUV Min");
			ImGui::DragFloat2("##Mask UV Min", &m_tEffectMeshDesc.vMaskUVMin.x, 0.01f);

			ImGui::Text("vUV Max");
			ImGui::DragFloat2("##Mask UV Max", &m_tEffectMeshDesc.vMaskUVMax.x, 0.01f);
#pragma endregion
#pragma endregion
			ImGui::Separator();

			_bool bPreCollider = m_tEffectMeshDesc.bCollider;

			ImGui::Text("Collider");
			ImGui::SameLine();
			ImGui::Checkbox("##Collider", &m_tEffectMeshDesc.bCollider);

			if (m_tEffectMeshDesc.bCollider)
			{
				ImGui::Text("Sync Transform");
				ImGui::SameLine();
				ImGui::Checkbox("##Sync_Transform", &m_tEffectMeshDesc.bWeaponSyncTransform);

				ImGui::DragFloat("Collider_Life_Time", &m_tEffectMeshDesc.fWeaponLifeTime, 0.05f);

				ImGui::Text("Weapon Scale");
				ImGui::SameLine();
				ImGui::DragFloat("##Weapon_Scale", &m_tEffectMeshDesc.fWeaponScale, 0.1f);

				const char* HitType_items[] = { "Normal", "Upper", "Down", "Warning","left","right","excute","Parry"};

				if (ImGui::BeginListBox("Hit Type"))
				{
					for (int n = 0; n < IM_ARRAYSIZE(HitType_items); n++)
					{
						const bool is_selected = (m_tEffectMeshDesc.iHitType == n);
						if (ImGui::Selectable(HitType_items[n], is_selected))
						{
							m_tEffectMeshDesc.iHitType = n;
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				const char* Option_items[] = { "None", "Normal", "Plague", "Special_Attack","Parry"};

				if (ImGui::BeginListBox("Option Type"))
				{
					for (int n = 0; n < IM_ARRAYSIZE(Option_items); n++)
					{
						const bool is_selected = (m_tEffectMeshDesc.iHitType == n);
						if (ImGui::Selectable(Option_items[n], is_selected))
						{
							m_tEffectMeshDesc.iOptionType = n;
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				ImGui::Text("Damage");
				ImGui::SameLine();
				ImGui::DragFloat("##Damage", &m_tEffectMeshDesc.fDamage);

				ImGui::Text("Weapon Offset");
				ImGui::DragFloat3("##Weapon_Offset", &m_tEffectMeshDesc.vWeaponOffset.x, 0.05f);

				ImGui::Text("HitFreq");
				ImGui::SameLine();
				ImGui::DragFloat("##HitFreq", &m_tEffectMeshDesc.fHitFreq);
				ImGui::Separator();
			}
		}
	}
#endif // _DEBUG

}

void CCustomEffectMesh::OnEnable(void* _Arg)
{
	__super::OnEnable(_Arg);
}

void CCustomEffectMesh::OnDisable()
{
	__super::OnDisable();
}

void CCustomEffectMesh::OnDestroy()
{
	__super::OnDestroy();
}

void CCustomEffectMesh::OnChangeAnimationKey(const _uint& In_Key)
{
	if (m_tEffectMeshDesc.iSyncAnimationKey != (_int)In_Key)
		return;

	Reset_Effect();
}

void CCustomEffectMesh::Free()
{
}
