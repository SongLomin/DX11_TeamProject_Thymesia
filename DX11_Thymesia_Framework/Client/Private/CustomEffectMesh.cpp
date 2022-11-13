﻿#include "stdafx.h"
#include "CustomEffectMesh.h"
#include "Client_Components.h"
#include "GameManager.h"
#include "SMath.h"
#include "Json/Json_Utility.h"
#include "Window_EffectHierarchyView.h"
#include "Window_AnimationModelView.h"
#include "Attack_Area.h"
#include "PreViewAnimationModel.h"


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

	m_pMaskTextureCom = CGameObject::Add_Component<CTexture>();
	m_pNoiseTextureCom = CGameObject::Add_Component<CTexture>();
	m_pGradientTextureCom = CGameObject::Add_Component<CTexture>();

	m_pMaskTextureCom.lock()->Use_Texture(("UVMask"));
	m_pNoiseTextureCom.lock()->Use_Texture(("UVNoise"));
	m_pGradientTextureCom.lock()->Use_Texture(("Gradient"));

	Set_Enable(false);

	XMStoreFloat4x4(&m_ParentMatrix, XMMatrixIdentity());

	ZeroMemory(&m_tEffectMeshDesc, sizeof(m_tEffectMeshDesc));

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

#pragma region World, View, Proj
	_matrix WorldMatrix = m_pTransformCom.lock()->Get_WorldMatrix();
	WorldMatrix *= XMLoadFloat4x4(&m_ParentMatrix);
	WorldMatrix = XMMatrixTranspose(WorldMatrix);

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", (void*)&WorldMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
#pragma endregion

	m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_tEffectMeshDesc.iUVMaskIndex);
	m_pNoiseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_tEffectMeshDesc.iUVNoiseIndex);


	m_pShaderCom.lock()->Set_RawValue("g_vUV", &m_vCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vColor", &m_vCurrentColor, sizeof(_float4));

	m_pShaderCom.lock()->Set_RawValue("g_fDiscardRatio", &m_tEffectMeshDesc.fDiscardRatio, sizeof(_float));
	m_pShaderCom.lock()->Set_RawValue("g_vWrapWeight", &m_tEffectMeshDesc.vWrapWeight, sizeof(_float4));
	//m_pGradientTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_GradientTexture", 0);

	_vector vCamDir = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2];

	m_pShaderCom.lock()->Set_RawValue("g_vCamDirection", &vCamDir, sizeof(_vector));
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

	_float fFrameTime = 1.f / 144.f;
	_int	iTickCount = 0;
	//iTickCount = fTimeDelta / fFrameTime;
	//m_fPreFrame += fTimeDelta - (iTickCount * fTimeDelta / fFrameTime);

	m_fPreFrame += fFrameTime;

	while (m_fPreFrame >= fFrameTime)
	{
		++iTickCount;
		m_fPreFrame -= fFrameTime;
	}

	for (_int i = 0; i < iTickCount; ++i)
	{
		Play_Internal(fFrameTime);
	}

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
	StartPosition.m128_f32[3] = 1.f;
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, StartPosition);

	m_vCurrentSpeed = m_tEffectMeshDesc.vSpeed;
	m_vCurrentRotation = m_tEffectMeshDesc.vStartRotation;
	m_vCurrentScale = m_tEffectMeshDesc.vStartScale;
	m_vCurrentColor = m_tEffectMeshDesc.vStartColor;
	m_vCurrentUV = m_tEffectMeshDesc.vStartUV;
	m_vCurrentGlowColor = m_tEffectMeshDesc.vStartGlowColor;

	XMStoreFloat3(&m_vCurrentForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat3(&m_vCurrentScaleForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat4(&m_vCurrentColorForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat2(&m_vCurrentUVForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat4(&m_vCurrentGlowForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat3(&m_vCurrentRotationForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));

	m_fPreFrame = 0.f;


	//½ÃÀÛ ·ÎÅ×ÀÌ¼Ç
	//½ÃÀÛ ½ºÄÉÀÏ
	//½ÃÀÛ ÄÃ·¯
	m_pTransformCom.lock()->Set_Scaled(m_vCurrentScale);
	_vector StartRotation = XMLoadFloat3(&m_vCurrentRotation);
	_vector Quaternion = XMQuaternionRotationRollPitchYaw(StartRotation.m128_f32[0], StartRotation.m128_f32[1], StartRotation.m128_f32[2]);
	m_pTransformCom.lock()->Rotation_Quaternion(Quaternion);

	if (m_tEffectMeshDesc.bCollider)
	{
		if (GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock())
		{
			if (!m_pAttackArea.lock())
			{
				m_pAttackArea = GAMEINSTANCE->Add_GameObject<CAttackArea>(m_CreatedLevel);
			}

			WEAPON_DESC WeaponDesc;
			ZeroMemory(&WeaponDesc, sizeof(WEAPON_DESC));
			WeaponDesc.fWeaponScale = m_tEffectMeshDesc.fWeaponScale;
			WeaponDesc.iHitType = m_tEffectMeshDesc.iHitType;
			WeaponDesc.fDamage = m_tEffectMeshDesc.fDamage;
			WeaponDesc.vWeaponOffset = m_tEffectMeshDesc.vWeaponOffset;
			WeaponDesc.fHitFreq = m_tEffectMeshDesc.fHitFreq;

			m_pAttackArea.lock()->Init_Weapon(WeaponDesc,
				GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Component<CTransform>());

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
	StartPosition.m128_f32[3] = 1.f;
	m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, StartPosition);

	m_fPreFrame = 0.f;

	m_vCurrentSpeed = m_tEffectMeshDesc.vSpeed;
	//m_vCurrentRotation = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(UnscaledMatrix));
	//m_vCurrentRotation = SMath::Add_Float3(m_vCurrentRotation, m_tEffectMeshDesc.vStartRotation);

	m_vCurrentRotation = m_tEffectMeshDesc.vStartRotation;
	m_vCurrentScale = m_tEffectMeshDesc.vStartScale;
	m_vCurrentColor = m_tEffectMeshDesc.vStartColor;
	m_vCurrentUV = m_tEffectMeshDesc.vStartUV;
	m_vCurrentGlowColor = m_tEffectMeshDesc.vStartGlowColor;

	XMStoreFloat3(&m_vCurrentForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat3(&m_vCurrentScaleForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat4(&m_vCurrentColorForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat2(&m_vCurrentUVForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat4(&m_vCurrentGlowForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));
	XMStoreFloat3(&m_vCurrentRotationForce, XMVectorSet(0.f, 0.f, 0.f, 0.f));


	m_pTransformCom.lock()->Set_Scaled(m_vCurrentScale);
	_vector StartRotation = XMLoadFloat3(&m_vCurrentRotation);
	_vector Quaternion = XMQuaternionRotationRollPitchYaw(StartRotation.m128_f32[0], StartRotation.m128_f32[1], StartRotation.m128_f32[2]);
	m_pTransformCom.lock()->Rotation_Quaternion(Quaternion);

	if (m_tEffectMeshDesc.bCollider)
	{
		if (!m_pAttackArea.lock())
		{
			m_pAttackArea = GAMEINSTANCE->Add_GameObject<CAttackArea>(m_CreatedLevel);
		}

		WEAPON_DESC WeaponDesc;
		ZeroMemory(&WeaponDesc, sizeof(WEAPON_DESC));
		WeaponDesc.fWeaponScale = m_tEffectMeshDesc.fWeaponScale;
		WeaponDesc.iHitType = m_tEffectMeshDesc.iHitType;
		WeaponDesc.fDamage = m_tEffectMeshDesc.fDamage;
		WeaponDesc.vWeaponOffset = m_tEffectMeshDesc.vWeaponOffset;
		WeaponDesc.fHitFreq = m_tEffectMeshDesc.fHitFreq;

		m_pAttackArea.lock()->Init_Weapon(WeaponDesc, pParentTransform);
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
	Out_Json["Name"] = m_szEffectName;

	Out_Json["Init_Time"] = m_tEffectMeshDesc.fInitTime;
	Out_Json["Life_Time"] = m_tEffectMeshDesc.fLifeTime;

	Out_Json["Sync_Animation"] = m_tEffectMeshDesc.bSyncAnimation;
	Out_Json["Sync_AnimationKey"] = m_tEffectMeshDesc.iSyncAnimationKey;

	Out_Json["Follow_Transform"] = m_tEffectMeshDesc.bFollowTransform;
	Out_Json["BillBoard"] = m_tEffectMeshDesc.bBillBoard;

	CJson_Utility::Write_Float3(Out_Json["Start_Position"], m_tEffectMeshDesc.vStartPosition);

	CJson_Utility::Write_Float3(Out_Json["Speed"], m_tEffectMeshDesc.vSpeed);
	CJson_Utility::Write_Float3(Out_Json["Force"], m_tEffectMeshDesc.vForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Speed"], m_tEffectMeshDesc.vMaxSpeed);

	CJson_Utility::Write_Float3(Out_Json["Start_Rotation"], m_tEffectMeshDesc.vStartRotation);
	CJson_Utility::Write_Float3(Out_Json["Rotation_Speed"], m_tEffectMeshDesc.vRotationSpeed);
	CJson_Utility::Write_Float3(Out_Json["Rotation_Force"], m_tEffectMeshDesc.vRotationForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Rotation"], m_tEffectMeshDesc.vMaxRotation);

	CJson_Utility::Write_Float3(Out_Json["Start_Scale"], m_tEffectMeshDesc.vStartScale);
	CJson_Utility::Write_Float3(Out_Json["Scale_Speed"], m_tEffectMeshDesc.vScaleSpeed);
	CJson_Utility::Write_Float3(Out_Json["Scale_Force"], m_tEffectMeshDesc.vScaleForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Scale"], m_tEffectMeshDesc.vMaxScale);

	Out_Json["Shader_Pass_Index"] = m_tEffectMeshDesc.iShaderPassIndex;

	Out_Json["Alpha_Discard_Ratio"] = m_tEffectMeshDesc.fDiscardRatio;

	CJson_Utility::Write_Float4(Out_Json["Start_Color"], m_tEffectMeshDesc.vStartColor);
	CJson_Utility::Write_Float4(Out_Json["Color_Speed"], m_tEffectMeshDesc.vColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Color_Force"], m_tEffectMeshDesc.vColorForce);
	CJson_Utility::Write_Float4(Out_Json["Max_Color"], m_tEffectMeshDesc.vMaxColor);

	Out_Json["UV_Diffuse_Index"] = m_tEffectMeshDesc.iUVDiffuseIndex;
	Out_Json["UV_Mask_Index"] = m_tEffectMeshDesc.iUVMaskIndex;
	Out_Json["UV_Noise_Index"] = m_tEffectMeshDesc.iUVNoiseIndex;

	// TODO : bDynamicNoiseOption temporary for test
	Out_Json["Dynamic_Noise_Option"] = m_tEffectMeshDesc.bDynamicNoiseOption;

	CJson_Utility::Write_Float4(Out_Json["UV_Wrap_Weight"], m_tEffectMeshDesc.vWrapWeight);

	CJson_Utility::Write_Float2(Out_Json["Start_UV"], m_tEffectMeshDesc.vStartUV);
	CJson_Utility::Write_Float2(Out_Json["UV_Speed"], m_tEffectMeshDesc.vUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["UV_Force"], m_tEffectMeshDesc.vUVForce);
	CJson_Utility::Write_Float2(Out_Json["Max_UV"], m_tEffectMeshDesc.vUVMax);

	Out_Json["Bloom"] = m_tEffectMeshDesc.bBloom;
	Out_Json["Glow"] = m_tEffectMeshDesc.bGlow;
	Out_Json["Distortion"] = m_tEffectMeshDesc.bDistortion;

	CJson_Utility::Write_Float4(Out_Json["Start_Glow_Color"], m_tEffectMeshDesc.vStartGlowColor);
	CJson_Utility::Write_Float4(Out_Json["Glow_Color_Speed"], m_tEffectMeshDesc.vGlowColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Glow_Color_Force"], m_tEffectMeshDesc.vGlowColorForce);

	Out_Json["Collider"] = m_tEffectMeshDesc.bCollider;
	Out_Json["Sync_Transform"] = m_tEffectMeshDesc.bWeaponSyncTransform;
	Out_Json["Weapon_LifeTime"] = m_tEffectMeshDesc.fWeaponLifeTime;
	Out_Json["Weapon_Scale"] = m_tEffectMeshDesc.fWeaponScale;
	Out_Json["Hit_Type"] = m_tEffectMeshDesc.iHitType;
	Out_Json["Damage"] = m_tEffectMeshDesc.fDamage;
	CJson_Utility::Write_Float3(Out_Json["Weapon_Offset"], m_tEffectMeshDesc.vWeaponOffset);
	Out_Json["HitFreq"] = m_tEffectMeshDesc.fHitFreq;
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

if (In_Json.find("BillBoard") != In_Json.end())
m_tEffectMeshDesc.bBillBoard = In_Json["BillBoard"];

CJson_Utility::Load_Float3(In_Json["Start_Position"], m_tEffectMeshDesc.vStartPosition);

CJson_Utility::Load_Float3(In_Json["Speed"], m_tEffectMeshDesc.vSpeed);
CJson_Utility::Load_Float3(In_Json["Force"], m_tEffectMeshDesc.vForce);
CJson_Utility::Load_Float3(In_Json["Max_Speed"], m_tEffectMeshDesc.vMaxSpeed);

CJson_Utility::Load_Float3(In_Json["Start_Rotation"], m_tEffectMeshDesc.vStartRotation);
CJson_Utility::Load_Float3(In_Json["Rotation_Speed"], m_tEffectMeshDesc.vRotationSpeed);
CJson_Utility::Load_Float3(In_Json["Rotation_Force"], m_tEffectMeshDesc.vRotationForce);
CJson_Utility::Load_Float3(In_Json["Max_Rotation"], m_tEffectMeshDesc.vMaxRotation);

CJson_Utility::Load_Float3(In_Json["Start_Scale"], m_tEffectMeshDesc.vStartScale);
CJson_Utility::Load_Float3(In_Json["Scale_Speed"], m_tEffectMeshDesc.vScaleSpeed);
CJson_Utility::Load_Float3(In_Json["Scale_Force"], m_tEffectMeshDesc.vScaleForce);
CJson_Utility::Load_Float3(In_Json["Max_Scale"], m_tEffectMeshDesc.vMaxScale);

if (In_Json.find("Shader_Pass_Index") != In_Json.end())
m_tEffectMeshDesc.iShaderPassIndex = In_Json["Shader_Pass_Index"];

if (In_Json.find("Alpha_Discard_Ratio") != In_Json.end())
m_tEffectMeshDesc.fDiscardRatio = In_Json["Alpha_Discard_Ratio"];

CJson_Utility::Load_Float4(In_Json["Start_Color"], m_tEffectMeshDesc.vStartColor);
CJson_Utility::Load_Float4(In_Json["Color_Speed"], m_tEffectMeshDesc.vColorSpeed);
CJson_Utility::Load_Float4(In_Json["Color_Force"], m_tEffectMeshDesc.vColorForce);
CJson_Utility::Load_Float4(In_Json["Max_Color"], m_tEffectMeshDesc.vMaxColor);

if (In_Json.find("UV_Diffuse_Index") != In_Json.end())
m_tEffectMeshDesc.iUVDiffuseIndex = In_Json["UV_Diffuse_Index"];

if (In_Json.find("UV_Mask_Index") != In_Json.end())
m_tEffectMeshDesc.iUVMaskIndex = In_Json["UV_Mask_Index"];

if (In_Json.find("UV_Noise_Index") != In_Json.end())
m_tEffectMeshDesc.iUVNoiseIndex = In_Json["UV_Noise_Index"];

if (In_Json.find("Dynamic_Noise_Option") != In_Json.end())
m_tEffectMeshDesc.bDynamicNoiseOption = In_Json["Dynamic_Noise_Option"];

if (In_Json.find("UV_Wrap_Weight") != In_Json.end())
CJson_Utility::Load_Float4(In_Json["UV_Wrap_Weight"], m_tEffectMeshDesc.vWrapWeight);

CJson_Utility::Load_Float2(In_Json["Start_UV"], m_tEffectMeshDesc.vStartUV);
CJson_Utility::Load_Float2(In_Json["UV_Speed"], m_tEffectMeshDesc.vUVSpeed);
CJson_Utility::Load_Float2(In_Json["UV_Force"], m_tEffectMeshDesc.vUVForce);
CJson_Utility::Load_Float2(In_Json["Max_UV"], m_tEffectMeshDesc.vUVMax);

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
	m_tEffectMeshDesc.bCollider = In_Json["Collider"];
	m_tEffectMeshDesc.bWeaponSyncTransform = In_Json["Sync_Transform"];
	m_tEffectMeshDesc.fWeaponLifeTime = In_Json["Weapon_LifeTime"];
	m_tEffectMeshDesc.fWeaponScale = In_Json["Weapon_Scale"];
	m_tEffectMeshDesc.iHitType = In_Json["Hit_Type"];
	m_tEffectMeshDesc.fDamage = In_Json["Damage"];
	CJson_Utility::Load_Float3(In_Json["Weapon_Offset"], m_tEffectMeshDesc.vWeaponOffset);
	m_tEffectMeshDesc.fHitFreq = In_Json["HitFreq"];
}

m_pModelCom.lock()->Init_Model(m_szEffectName.c_str());
}

#ifdef _DEBUG
void CCustomEffectMesh::Key_Input_ControlMesh(_float fTimeDelta)
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

void CCustomEffectMesh::Apply_ImGui_Controls_to_Mesh()
{
	if (m_tEffectMeshDesc.bSyncStartPositionToController)
	{
		XMStoreFloat3(&m_tEffectMeshDesc.vStartPosition, m_pTransformCom.lock()->Get_State(CTransform::STATE::STATE_TRANSLATION));
	}
}
#endif // _DEBUG

void CCustomEffectMesh::Play_Internal(_float fFrameTime)
{
	// ÀÌµ¿ ¼¼ÆÃ
	m_vCurrentSpeed = SMath::Mul_Float3(m_tEffectMeshDesc.vSpeed, fFrameTime);
	m_vCurrentForce = SMath::Add_Float3(m_vCurrentForce, SMath::Mul_Float3(m_tEffectMeshDesc.vForce, fFrameTime));

	_vector vMovePosition = XMLoadFloat3(&m_vCurrentSpeed) + XMLoadFloat3(&m_vCurrentForce);
	_vector vAbsolutePosition = vMovePosition / fFrameTime;

	// ÃÖ´ë°ª Á¦ÇÑ
	vAbsolutePosition.m128_f32[0] = min(m_tEffectMeshDesc.vMaxSpeed.x, vAbsolutePosition.m128_f32[0]);
	vAbsolutePosition.m128_f32[1] = min(m_tEffectMeshDesc.vMaxSpeed.y, vAbsolutePosition.m128_f32[1]);
	vAbsolutePosition.m128_f32[2] = min(m_tEffectMeshDesc.vMaxSpeed.z, vAbsolutePosition.m128_f32[2]);

	vMovePosition = vAbsolutePosition * fFrameTime;

	m_pTransformCom.lock()->Add_Position(vMovePosition);

	// For. Rotation


	_vector CurrentRotation = XMLoadFloat3(&m_vCurrentRotation);
	_vector MoveRotation = XMLoadFloat3(&m_tEffectMeshDesc.vRotationSpeed) * fFrameTime;
	_vector MoveForce = XMLoadFloat3(&m_tEffectMeshDesc.vRotationForce) * fFrameTime;
	_vector CurrentForce = XMLoadFloat3(&m_vCurrentRotationForce);
	CurrentForce += MoveForce;
	MoveRotation += CurrentForce;
	CurrentRotation += MoveRotation;

	XMStoreFloat3(&m_vCurrentRotation, CurrentRotation);
	XMStoreFloat3(&m_vCurrentRotationForce, CurrentForce);

	_vector Quaternion = XMQuaternionRotationRollPitchYaw(CurrentRotation.m128_f32[0], CurrentRotation.m128_f32[1], CurrentRotation.m128_f32[2]);
	m_pTransformCom.lock()->Rotation_Quaternion(Quaternion);


	// For. Scale

	m_vCurrentScaleSpeed = SMath::Mul_Float3(m_tEffectMeshDesc.vScaleSpeed, fFrameTime);
	m_vCurrentScaleForce = SMath::Add_Float3(m_vCurrentScaleForce, SMath::Mul_Float3(m_tEffectMeshDesc.vScaleForce, fFrameTime));

	_vector vMoveScale = XMLoadFloat3(&m_vCurrentScaleSpeed) + XMLoadFloat3(&m_vCurrentScaleForce);
	_vector vCurrentScale = XMLoadFloat3(&m_vCurrentScale);
	vCurrentScale += vMoveScale;

	// ÃÖ´ë°ª Á¦ÇÑ
	vCurrentScale.m128_f32[0] = max(0.00001f, min(m_tEffectMeshDesc.vMaxScale.x, vCurrentScale.m128_f32[0]));
	vCurrentScale.m128_f32[1] = max(0.00001f, min(m_tEffectMeshDesc.vMaxScale.y, vCurrentScale.m128_f32[1]));
	vCurrentScale.m128_f32[2] = max(0.00001f, min(m_tEffectMeshDesc.vMaxScale.z, vCurrentScale.m128_f32[2]));

	XMStoreFloat3(&m_vCurrentScale, vCurrentScale);

	m_pTransformCom.lock()->Set_Scaled(m_vCurrentScale);

	// For. Color
	_vector vColorSpeed = XMLoadFloat4(&m_tEffectMeshDesc.vColorSpeed) * fFrameTime;
	m_vCurrentColorForce = SMath::Add_Float4(m_vCurrentColorForce, SMath::Mul_Float4(m_tEffectMeshDesc.vColorForce, fFrameTime));

	_vector vMoveColor = vColorSpeed + XMLoadFloat4(&m_vCurrentColorForce);
	_vector vCurrentColor = XMLoadFloat4(&m_vCurrentColor);
	vCurrentColor += vMoveColor;

	vCurrentColor.m128_f32[0] = min(m_tEffectMeshDesc.vMaxColor.x, vCurrentColor.m128_f32[0]);
	vCurrentColor.m128_f32[1] = min(m_tEffectMeshDesc.vMaxColor.y, vCurrentColor.m128_f32[1]);
	vCurrentColor.m128_f32[2] = min(m_tEffectMeshDesc.vMaxColor.z, vCurrentColor.m128_f32[2]);
	vCurrentColor.m128_f32[3] = min(m_tEffectMeshDesc.vMaxColor.w, vCurrentColor.m128_f32[3]);

	XMStoreFloat4(&m_vCurrentColor, vCurrentColor);

	// For. UV
	_vector vUVSpeed = XMLoadFloat2(&m_tEffectMeshDesc.vUVSpeed) * fFrameTime;
	m_vCurrentUVForce.x += m_tEffectMeshDesc.vUVForce.x * fFrameTime;
	m_vCurrentUVForce.y += m_tEffectMeshDesc.vUVForce.y * fFrameTime;

	_vector vMoveUV = vUVSpeed + XMLoadFloat2(&m_vCurrentUVForce);
	_vector vCurrentUV = XMLoadFloat2(&m_vCurrentUV);
	vCurrentUV += vMoveUV;

	vCurrentUV.m128_f32[0] = min(m_tEffectMeshDesc.vUVMax.x, vCurrentUV.m128_f32[0]);
	vCurrentUV.m128_f32[1] = min(m_tEffectMeshDesc.vUVMax.y, vCurrentUV.m128_f32[1]);

	XMStoreFloat2(&m_vCurrentUV, vCurrentUV);

	// For. Glow
	_vector vGlowSpeed = XMLoadFloat4(&m_tEffectMeshDesc.vGlowColorSpeed) * fFrameTime;
	m_vCurrentGlowForce.x += m_tEffectMeshDesc.vGlowColorForce.x * fFrameTime;
	m_vCurrentGlowForce.y += m_tEffectMeshDesc.vGlowColorForce.y * fFrameTime;

	_vector vMoveGlowColor = vGlowSpeed + XMLoadFloat4(&m_vCurrentGlowForce);
	_vector vCurrentGlowColor = XMLoadFloat4(&m_vCurrentGlowColor);
	vCurrentGlowColor += vMoveGlowColor;

	vCurrentGlowColor = XMVectorMin(XMVectorSet(1.f, 1.f, 1.f, 1.f),
		XMVectorMax(XMVectorSet(0.f, 0.f, 0.f, 0.f), vCurrentGlowColor));

	XMStoreFloat4(&m_vCurrentGlowColor, vCurrentGlowColor);



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
			{
				Clone_EffectMesh();
			}

			// TODO : for imgui - mesh keyboard control
			ImGui::Text("On Control Focus");
			ImGui::SameLine();
			ImGui::Checkbox("##Control Focus", &m_tEffectMeshDesc.bOnFocus);
			if (m_tEffectMeshDesc.bOnFocus)
			{
				ImGui::Separator();
				ImGui::Text("Control Keys");
				ImGui::Text("NumPad 8 : move forward"); ImGui::SameLine();
				ImGui::Text("NumPad 2 : move backward");
				ImGui::Text("NumPad 4 : move leftward"); ImGui::SameLine();
				ImGui::Text("NumPad 6 : move rightward");
				ImGui::Text("NumPad 1 : move down"); ImGui::SameLine();
				ImGui::Text("NumPad 3 : move up");
				ImGui::Text("Hold LShift to move by World Axis");
				ImGui::Separator();
				ImGui::Separator();

				ImGui::Text("Control Speed");
				ImGui::DragFloat("##Control Speed", &m_tEffectMeshDesc.fSpeedPerSec);
				ImGui::Separator();
			}

			ImGui::Separator();

			ImGui::Text("Init Time");
			ImGui::DragFloat("##Init Time", &m_tEffectMeshDesc.fInitTime, 0.2f);

			ImGui::Text("Life Time");
			ImGui::DragFloat("##Life Time", &m_tEffectMeshDesc.fLifeTime, 0.2f);
			ImGui::Separator();

			ImGui::Text("Follow Transform");
			ImGui::SameLine();
			ImGui::Checkbox("##Follow_Transform", &m_tEffectMeshDesc.bFollowTransform);

			ImGui::Text("BillBoard");
			ImGui::SameLine();
			ImGui::Checkbox("##BillBoard", &m_tEffectMeshDesc.bBillBoard);

			ImGui::Text("Sync Animation");
			ImGui::SameLine();
			ImGui::Checkbox("##Sync Animation", &m_tEffectMeshDesc.bSyncAnimation);

			ImGui::InputInt("Sync Animation Key", &m_tEffectMeshDesc.iSyncAnimationKey);
			ImGui::Separator();

			ImGui::Text("Sync Start Position to Controller");
			ImGui::Checkbox("##Sync Start Position to Controller", &m_tEffectMeshDesc.bSyncStartPositionToController);

			ImGui::Text("Start Position");
			ImGui::DragFloat3("##Start Position", &m_tEffectMeshDesc.vStartPosition.x, 0.1f);

			ImGui::Text("Speed");
			ImGui::DragFloat3("##Speed", &m_tEffectMeshDesc.vSpeed.x, 0.1f);

			ImGui::Text("Force");
			ImGui::DragFloat3("##Force", &m_tEffectMeshDesc.vForce.x, 0.1f);

			ImGui::Text("Max Speed");
			ImGui::DragFloat3("##Max Speed", &m_tEffectMeshDesc.vMaxSpeed.x, 0.1f);
			ImGui::Separator();

			ImGui::Text("Start Rotation");
			ImGui::DragFloat3("##Start Rotation", &m_tEffectMeshDesc.vStartRotation.x, 0.01f);

			ImGui::Text("Rotation Speed");
			ImGui::DragFloat3("##Rotation Speed", &m_tEffectMeshDesc.vRotationSpeed.x, 0.01f);

			ImGui::Text("Rotation Force");
			ImGui::DragFloat3("##Rotation Force", &m_tEffectMeshDesc.vRotationForce.x, 0.01f);

			ImGui::Text("Max Rotation");
			ImGui::DragFloat3("##Max Rotation", &m_tEffectMeshDesc.vMaxRotation.x, 0.01f);
			ImGui::Separator();

			ImGui::Text("Start Scale");
			ImGui::DragFloat3("##Start Scale", &m_tEffectMeshDesc.vStartScale.x, 0.01f);

			m_tEffectMeshDesc.vStartScale.x = max(0.00001f, m_tEffectMeshDesc.vStartScale.x);
			m_tEffectMeshDesc.vStartScale.y = max(0.00001f, m_tEffectMeshDesc.vStartScale.y);
			m_tEffectMeshDesc.vStartScale.z = max(0.00001f, m_tEffectMeshDesc.vStartScale.z);

			ImGui::Text("Scale Speed");
			ImGui::DragFloat3("##Scale Speed", &m_tEffectMeshDesc.vScaleSpeed.x, 0.01f);

			ImGui::Text("Scale Force");
			ImGui::DragFloat3("##Scale Force", &m_tEffectMeshDesc.vScaleForce.x, 0.01f);

			ImGui::Text("Max Scale");
			ImGui::DragFloat3("##Max Scale", &m_tEffectMeshDesc.vMaxScale.x, 0.01f);
			ImGui::Separator();

			m_tEffectMeshDesc.vMaxScale.x = max(0.00001f, m_tEffectMeshDesc.vMaxScale.x);
			m_tEffectMeshDesc.vMaxScale.y = max(0.00001f, m_tEffectMeshDesc.vMaxScale.y);
			m_tEffectMeshDesc.vMaxScale.z = max(0.00001f, m_tEffectMeshDesc.vMaxScale.z);

			ImGui::Text("Alpha Discard Ratio");
			ImGui::DragFloat("##Alpha Discard Ratio", &m_tEffectMeshDesc.fDiscardRatio, 0.01f, 0.f, 1.f, "%.3f", 0);

			ImGui::Text("Start Color");
			ImGui::DragFloat4("##Start Color", &m_tEffectMeshDesc.vStartColor.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::Text("Color Speed");
			ImGui::DragFloat4("##Color Speed", &m_tEffectMeshDesc.vColorSpeed.x, 0.01f, -1.f, 1.f, "%.5f");

			ImGui::Text("Color Force");
			ImGui::DragFloat4("##Color Force", &m_tEffectMeshDesc.vColorForce.x, 0.01f, -1.f, 1.f, "%.5f");

			ImGui::Text("Max Color");
			ImGui::DragFloat4("##Max Color", &m_tEffectMeshDesc.vMaxColor.x, 0.01f, 0.f, 1.f, "%.5f");
			ImGui::Separator();

			ImGui::Text("Start UV");
			ImGui::DragFloat2("##Start UV", &m_tEffectMeshDesc.vStartUV.x, 0.01f, 0.f, 1.f, "%.5f");

			ImGui::DragInt("UV Diffuse Index", &m_tEffectMeshDesc.iUVDiffuseIndex, 1, 0);
			ImGui::DragInt("UV Mask Index", &m_tEffectMeshDesc.iUVMaskIndex, 1, 0);
			ImGui::DragInt("UV Noise Index", &m_tEffectMeshDesc.iUVNoiseIndex, 1, 0);

			ImGui::Text("Dynamic Noise Option");
			ImGui::Checkbox("##Dynamic Noise Option", &m_tEffectMeshDesc.bDynamicNoiseOption);

			ImGui::Text("Texture Wrap Weight");
			ImGui::Text("x : Diffuse | y : Mask | z : Noise");
			ImGui::DragFloat4("##Texture Wrap Weight", &m_tEffectMeshDesc.vWrapWeight.x, 0.01f);

			ImGui::Text("UV Speed");
			ImGui::DragFloat2("##UV Speed", &m_tEffectMeshDesc.vUVSpeed.x, 0.01f);

			ImGui::Text("UV Force");
			ImGui::DragFloat2("##UV Force", &m_tEffectMeshDesc.vUVForce.x, 0.01f);

			ImGui::Text("vUV Max");
			ImGui::DragFloat2("##UV Max", &m_tEffectMeshDesc.vUVMax.x, 0.01f);
			ImGui::Separator();

			ImGui::Text("Distortion");
			ImGui::SameLine();
			ImGui::Checkbox("##Distortion", &m_tEffectMeshDesc.bDistortion);

			ImGui::Text("Bloom");
			ImGui::SameLine();
			ImGui::Checkbox("##Bloom", &m_tEffectMeshDesc.bBloom);

			ImGui::Text("Glow");
			ImGui::SameLine();
			ImGui::Checkbox("##Glow", &m_tEffectMeshDesc.bGlow);

			ImGui::Text("Start Glow Color");
			ImGui::DragFloat4("##Start_Glow_Color", &m_tEffectMeshDesc.vStartGlowColor.x, 0.01f);

			ImGui::Text("Glow Color Speed ");
			ImGui::DragFloat4("##Glow_Color_Speed", &m_tEffectMeshDesc.vGlowColorSpeed.x, 0.01f);

			ImGui::Text("Glow Color Force");
			ImGui::DragFloat4("##Glow_Color_Force", &m_tEffectMeshDesc.vGlowColorForce.x, 0.01f);
			ImGui::Separator();

			ImGui::Separator();
			ImGui::Text("Shader Pass");
			ImGui::Text("2 : NoneDiffuseEffect_UVWrap");
			ImGui::Text("3 : NoneDiffuseEffect_UVClamp");
			ImGui::Text("4 : PerfectCustom");
			ImGui::Text("5 : Distortion_UVWrap");
			ImGui::Text("6 : Distortion_UVClamp");
			ImGui::InputInt("##Shader_Pass_Index", &m_tEffectMeshDesc.iShaderPassIndex);
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

				const char* HitType_items[] = { "Normal", "Upper", "Down", "Warning" };

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
