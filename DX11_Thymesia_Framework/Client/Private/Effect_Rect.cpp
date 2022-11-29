#include "stdafx.h"
#include "Effect_Rect.h"
#include "Client_Components.h"
#include "Window_AnimationModelView.h"
#include "PreViewAnimationModel.h"

#include "Easing_Utillity.h"
#include "Model.h"
#include "BoneNode.h"

// undefines at bottom
#define PASS_SPRITE_BLACKDISCARD 0
#define PASS_ALPHADISCARD 1
#define PASS_BLACKDISCARD 2
#define PASS_SPRITE_ALPHADISCARD 3

GAMECLASS_C(CEffect_Rect)
CLONE_C(CEffect_Rect, CGameObject)

const _char* CEffect_Rect::Get_EffectName() const
{
	return m_szEffectName.c_str();
}

HRESULT CEffect_Rect::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	ZeroMemory(&m_tEffectParticleDesc, sizeof(EFFECTPARTICLE_DESC));

	m_tEffectParticleDesc.iMaxInstance = 1;

	m_tEffectParticleDesc.iParticleType = _int(PARTICLETYPE::NONE);
	m_tEffectParticleDesc.iFollowTransformType = _int(TRANSFORMTYPE::STATIC);

	m_tEffectParticleDesc.iShaderPassIndex = 1;

	m_tEffectParticleDesc.iSyncAnimationKey = -1;

	// m_tParticleSpawns.clear();

	m_tEffectParticleDesc.fMinLifeTime = 1.f;

	m_tEffectParticleDesc.vMinStartScale = { 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartScale = { 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxLimitScale = { 1.f, 1.f, 1.f };

	m_tEffectParticleDesc.fDiscardRatio = 0.1f;
	m_tEffectParticleDesc.vMinStartColor = { 1.f, 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartColor = { 1.f, 1.f, 1.f, 1.f };

	m_tEffectParticleDesc.vMaxColor = { 1.f, 1.f, 1.f, 1.f };

	m_tEffectParticleDesc.vDiffuseUVMax = { 1.f, 1.f };
	m_tEffectParticleDesc.vMaskUVMax = { 1.f, 1.f };
	m_tEffectParticleDesc.vNoiseUVMax = { 1.f, 1.f };

	m_tEffectParticleDesc.vStartGlowColor = { 1.f, 1.f, 1.f, 1.f };

	ZeroMemory(&m_vDiffuseCurrentUV, sizeof(_float2));
	ZeroMemory(&m_vDiffuseCurrentUVForce, sizeof(_float2));

	ZeroMemory(&m_vMaskCurrentUV, sizeof(_float2));
	ZeroMemory(&m_vMaskCurrentUVForce, sizeof(_float2));

	ZeroMemory(&m_vNoiseCurrentUV, sizeof(_float2));
	ZeroMemory(&m_vNoiseCurrentUVForce, sizeof(_float2));

	m_vCurrentGlowColor = { 1.f, 1.f, 1.f, 1.f };
	ZeroMemory(&m_vCurrentGlowColorForce, sizeof(_float2));

	m_bFinish = false;
	m_bStopParticle = false;

	return S_OK;
}


HRESULT CEffect_Rect::Initialize(void* pArg)
{
	m_eRenderGroup = RENDERGROUP::RENDER_ALPHABLEND;

	__super::Initialize(pArg);

	m_pVIBuffer = Add_Component<CVIBuffer_Rect_Instance>();
	//m_pVIBuffer.lock()->Init_Particle(10);
	//m_pVIBuffer.lock()->Init_Particle(3);

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxInstance"),
		VTXTEXCOLOR_INSTANCE_DECLARATION::Element,
		VTXTEXCOLOR_INSTANCE_DECLARATION::iNumElements);

	m_pColorDiffuseTextureCom = Add_Component<CTexture>();
	m_pColorDiffuseTextureCom.lock()->Use_Texture("UVColorDiffuse");

	m_pMaskTextureCom = Add_Component<CTexture>();
	m_pMaskTextureCom.lock()->Use_Texture("UVMask");

	m_pNoiseTextureCom = Add_Component<CTexture>();
	m_pNoiseTextureCom.lock()->Use_Texture("UVNoise");
	Set_Enable(false);


#ifdef _USE_THREAD_
	Use_Thread(THREAD_TYPE::TICK);
#endif // _USE_THREAD_


	return S_OK;
}

void CEffect_Rect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pVIBuffer.lock()->Get_InstanceCount() != m_tEffectParticleDesc.iMaxInstance)
		Reset_Instance(m_tEffectParticleDesc.iMaxInstance);

	Play(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex));
}

void CEffect_Rect::LateTick(_float fTimeDelta)
{
	_bool bUseParentMatrix = false;
	if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
		bUseParentMatrix = true;

	m_pVIBuffer.lock()->Update(m_tParticleDescs, bUseParentMatrix);

	__super::LateTick(fTimeDelta);

	if (Check_DisableAllParticle())
	{
		Set_Enable(false);
	}
}

HRESULT CEffect_Rect::Render()
{
	SetUp_ShaderResource();
	__super::Render();
	m_pShaderCom.lock()->Begin(m_tEffectParticleDesc.iShaderPassIndex);
	m_pVIBuffer.lock()->Render();
	return S_OK;
}

void CEffect_Rect::Init_EffectParticle(const _char* In_szName, const _char* In_szTextureKey)
{
	m_szEffectName = In_szName;
	m_pTextureCom.lock()->Use_Texture(In_szTextureKey);
}

void CEffect_Rect::Reset_Effect(weak_ptr<CTransform> pParentTransform)
{
	Set_Enable(true);
	m_bFinish = false;
	m_bStopParticle = false;
	m_bStopSprite = false;

	m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());

	m_fCurrentInitTime = m_tEffectParticleDesc.fInitTime;

	m_vDiffuseCurrentUV = m_tEffectParticleDesc.vDiffuseStartUV;
	m_vDiffuseCurrentUVForce = { 0.f, 0.f };

	m_vMaskCurrentUV = m_tEffectParticleDesc.vMaskStartUV;
	m_vMaskCurrentUVForce = { 0.f, 0.f };

	m_vNoiseCurrentUV = m_tEffectParticleDesc.vNoiseStartUV;
	m_vNoiseCurrentUVForce = { 0.f, 0.f };

	m_vCurrentGlowColor = m_tEffectParticleDesc.vStartGlowColor;
	m_vCurrentGlowColorForce = { 0.f, 0.f, 0.f, 0.f };
	m_fPreFrame = 0.f;

	if (m_tEffectParticleDesc.iFollowTransformType != (_int)TRANSFORMTYPE::STATIC)
	{
		if (nullptr == m_pParentTransformCom.lock())
			m_pParentTransformCom = pParentTransform;
	}

	Update_ParentTransform();
	Reset_ParticleDescs();
}

void CEffect_Rect::SetUp_ShaderResource()
{
#pragma region Base
	if ((_uint)TRANSFORMTYPE::STATIC != m_tEffectParticleDesc.iFollowTransformType)
		Update_ParentTransform();

	_matrix		BoneMatrix = XMMatrixIdentity();
	_matrix		WorldMatrix = XMMatrixIdentity();

	if (m_pBoneNode.lock() && ((_uint)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType))
	{
		_float4x4 TempMat = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Get_TransformationMatrix();
		_matrix ModelTranMat = XMLoadFloat4x4(&TempMat);
		BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	}

	WorldMatrix = BoneMatrix * m_pTransformCom.lock()->Get_WorldMatrix();

	WorldMatrix = XMMatrixTranspose(WorldMatrix);

	m_pShaderCom.lock()->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));
#pragma endregion
#pragma region Textures
	m_pShaderCom.lock()->Set_RawValue("g_fDiscardRatio", &m_tEffectParticleDesc.fDiscardRatio, sizeof(_float));

	m_pColorDiffuseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DiffuseTexture", m_tEffectParticleDesc.iDiffuseIndex);
	m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_tEffectParticleDesc.iMaskIndex);
	m_pNoiseTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_NoiseTexture", m_tEffectParticleDesc.iNoiseIndex);

	m_pShaderCom.lock()->Set_RawValue("g_vDiffuseUV", &m_vDiffuseCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vMaskUV", &m_vMaskCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vNoiseUV", &m_vNoiseCurrentUV, sizeof(_float2));

	m_pShaderCom.lock()->Set_RawValue("g_iNumFrameX", &m_tEffectParticleDesc.iNumFrameX, sizeof(_uint));
	m_pShaderCom.lock()->Set_RawValue("g_iNumFrameY", &m_tEffectParticleDesc.iNumFrameY, sizeof(_uint));
#pragma endregion
#pragma region Billboard
	_vector vCamDir = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2];

	m_pShaderCom.lock()->Set_RawValue("g_vCamDirection", &vCamDir, sizeof(_vector));
	_bool isBillboard = m_tEffectParticleDesc.iParticleType == (_int)PARTICLETYPE::BILLBOARD;
	m_pShaderCom.lock()->Set_RawValue("g_bBillboard", &isBillboard, sizeof(_bool));
#pragma endregion
#pragma region Bloom & Glow
	m_pShaderCom.lock()->Set_RawValue("g_bBloom", &m_tEffectParticleDesc.bBloom, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bGlow", &m_tEffectParticleDesc.bGlow, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &m_vCurrentGlowColor, sizeof(_float4));
#pragma endregion
}

void CEffect_Rect::Write_EffectJson(json& Out_Json)
{
	Out_Json["Name"] = m_szEffectName;

	Out_Json["Max_Instance"] = m_tEffectParticleDesc.iMaxInstance;

	Out_Json["Is_Looping"] = m_tEffectParticleDesc.bLooping;

	Out_Json["ParticleType"] = m_tEffectParticleDesc.iParticleType;
	Out_Json["Follow_Transform"] = m_tEffectParticleDesc.iFollowTransformType;

	Out_Json["ShaderPassIndex"] = m_tEffectParticleDesc.iShaderPassIndex;

	Out_Json["Sync_Animation"] = m_tEffectParticleDesc.bSyncAnimation;
	Out_Json["Sync_AnimationKey"] = m_tEffectParticleDesc.iSyncAnimationKey;

#pragma region Life Time
	Out_Json["Init_Time"] = m_tEffectParticleDesc.fInitTime;
	Out_Json["Min_Spawn_Time"] = m_tEffectParticleDesc.fMinSpawnTime;
	Out_Json["Max_Spawn_Time"] = m_tEffectParticleDesc.fMaxSpawnTime;
	Out_Json["Min_Life_Time"] = m_tEffectParticleDesc.fMinLifeTime;
	Out_Json["Max_Life_Time"] = m_tEffectParticleDesc.fMaxLifeTime;
#pragma endregion

	Out_Json["Is_Boner"] = m_tEffectParticleDesc.bBoner;

	if (m_tEffectParticleDesc.bBoner)
	{
		if (!m_strBoneName.empty())
			Out_Json["Bone_Name"] = m_strBoneName;
	}

#pragma region Spawn Position
	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Position"], m_tEffectParticleDesc.vMinSpawnPosition);
	CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);

	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMinSpawnOffsetDirection);
	CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);

	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Offset_Range"], m_tEffectParticleDesc.vMinSpawnOffsetRange);
	CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);
#pragma endregion

	Out_Json["Is_MoveLook"] = m_tEffectParticleDesc.bMoveLook;
	
	Out_Json["Is_Use_Gravity"] = m_tEffectParticleDesc.bUseGravity;

	if (m_tEffectParticleDesc.bUseGravity)
		CJson_Utility::Write_Float3(Out_Json["Gravity_Force"], m_tEffectParticleDesc.vGravityForce);


	Out_Json["Is_Easing_Position"] = m_tEffectParticleDesc.bEasingPosition;

	if (m_tEffectParticleDesc.bEasingPosition)
	{
		Out_Json["Position_Easing_Type"] = m_tEffectParticleDesc.iSpeedEasingType;
		Out_Json["Position_Easing_Total_Time"] = m_tEffectParticleDesc.fSpeedEasingTotalTime;

		CJson_Utility::Write_Float3(Out_Json["Min_Goal_Offset_Position"], m_tEffectParticleDesc.vMinSpeed);
		CJson_Utility::Write_Float3(Out_Json["Max_Goal_Offset_Position"], m_tEffectParticleDesc.vMaxSpeed);
	}
	else
	{
#pragma region Speed
		Out_Json["Is_Easing_Speed"] = m_tEffectParticleDesc.bEasingSpeed;

		if (m_tEffectParticleDesc.bEasingSpeed)
		{
			Out_Json["Speed_Easing_Type"] = m_tEffectParticleDesc.iSpeedEasingType;
			Out_Json["Speed_Easing_Total_Time"] = m_tEffectParticleDesc.fSpeedEasingTotalTime;
		}
		else
		{
			CJson_Utility::Write_Float3(Out_Json["Min_Speed"], m_tEffectParticleDesc.vMinSpeed);
			CJson_Utility::Write_Float3(Out_Json["Max_Speed"], m_tEffectParticleDesc.vMaxSpeed);

			CJson_Utility::Write_Float3(Out_Json["Min_Speed_Force"], m_tEffectParticleDesc.vMinSpeedForce);
			CJson_Utility::Write_Float3(Out_Json["Max_Speed_Force"], m_tEffectParticleDesc.vMaxSpeedForce);
		}

		CJson_Utility::Write_Float3(Out_Json["Min_Limit_Speed"], m_tEffectParticleDesc.vMinLimitSpeed);
		CJson_Utility::Write_Float3(Out_Json["Max_Limit_Speed"], m_tEffectParticleDesc.vMaxLimitSpeed);
#pragma endregion
	}

#pragma region Rotation
	if (_int(PARTICLETYPE::OUTBURST) != m_tEffectParticleDesc.iParticleType)
	{
		Out_Json["Is_Easing_Rotation"] = m_tEffectParticleDesc.bEasingRotation;

		if (m_tEffectParticleDesc.bEasingRotation)
		{
			Out_Json["Rotation_Easing_Type"] = m_tEffectParticleDesc.iRotationEasingType;
			Out_Json["Rotation_Easing_Total_Time"] = m_tEffectParticleDesc.fRotationEasingTotalTime;
		}
		else
		{
			CJson_Utility::Write_Float3(Out_Json["Min_Start_Rotation"], m_tEffectParticleDesc.vMinStartRotation);
			CJson_Utility::Write_Float3(Out_Json["Max_Start_Rotation"], m_tEffectParticleDesc.vMaxStartRotation);

			CJson_Utility::Write_Float3(Out_Json["Min_Rotation_Speed"], m_tEffectParticleDesc.vMinRotationSpeed);
			CJson_Utility::Write_Float3(Out_Json["Max_Rotation_Speed"], m_tEffectParticleDesc.vMaxRotationSpeed);

			CJson_Utility::Write_Float3(Out_Json["Min_Rotation_Force"], m_tEffectParticleDesc.vMinRotationForce);
			CJson_Utility::Write_Float3(Out_Json["Max_Rotation_Force"], m_tEffectParticleDesc.vMaxRotationForce);
		}

		CJson_Utility::Write_Float3(Out_Json["Min_Rotation"], m_tEffectParticleDesc.vMinLimitRotation);
		CJson_Utility::Write_Float3(Out_Json["Max_Rotation"], m_tEffectParticleDesc.vMaxLimitRotation);
	}
#pragma endregion

#pragma region Scale
	Out_Json["Is_Easing_Scale"] = m_tEffectParticleDesc.bEasingScale;

	if (m_tEffectParticleDesc.bEasingScale)
	{
		Out_Json["Scale_Easing_Type"] = m_tEffectParticleDesc.iScaleEasingType;
		Out_Json["Scale_Easing_Total_Time"] = m_tEffectParticleDesc.fScaleEasingTotalTime;
	}
	else
	{
		CJson_Utility::Write_Float3(Out_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
		CJson_Utility::Write_Float3(Out_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);

		CJson_Utility::Write_Float3(Out_Json["Min_Scale_Speed"], m_tEffectParticleDesc.vMinScaleSpeed);
		CJson_Utility::Write_Float3(Out_Json["Max_Scale_Speed"], m_tEffectParticleDesc.vMaxScaleSpeed);

		CJson_Utility::Write_Float3(Out_Json["Min_Scale_Force"], m_tEffectParticleDesc.vMinScaleForce);
		CJson_Utility::Write_Float3(Out_Json["Max_Scale_Force"], m_tEffectParticleDesc.vMaxScaleForce);

		// CJson_Utility::Write_Float3(Out_Json["Scale_Speed"], m_tEffectParticleDesc.vScaleSpeed);
		// CJson_Utility::Write_Float3(Out_Json["Scale_Force"], m_tEffectParticleDesc.vScaleForce);
	}
	CJson_Utility::Write_Float3(Out_Json["Min_Scale"], m_tEffectParticleDesc.vMinLimitScale);
	CJson_Utility::Write_Float3(Out_Json["Max_Scale"], m_tEffectParticleDesc.vMaxLimitScale);
#pragma endregion

#pragma region Color
	Out_Json["Discard_Ratio"] = m_tEffectParticleDesc.fDiscardRatio;
	Out_Json["Is_Gray_Only_Use_Red"] = m_tEffectParticleDesc.IsGrayOnlyUseRed;

	CJson_Utility::Write_Float4(Out_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	CJson_Utility::Write_Float4(Out_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);

	CJson_Utility::Write_Float4(Out_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Max_Color_Speed"], m_tEffectParticleDesc.vMaxColorSpeed);

	CJson_Utility::Write_Float4(Out_Json["Min_Color_Force"], m_tEffectParticleDesc.vMinColorForce);
	CJson_Utility::Write_Float4(Out_Json["Max_Color_Force"], m_tEffectParticleDesc.vMaxColorForce);

	CJson_Utility::Write_Float4(Out_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);
#pragma endregion

#pragma region Texture
#pragma region Diffuse
	Out_Json["UV_Diffuse_Index"] = m_tEffectParticleDesc.iDiffuseIndex;

	CJson_Utility::Write_Float2(Out_Json["Diffuse_Start_UV"], m_tEffectParticleDesc.vDiffuseStartUV);

	CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Speed"], m_tEffectParticleDesc.vDiffuseUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Force"], m_tEffectParticleDesc.vDiffuseUVForce);
	CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Max"], m_tEffectParticleDesc.vDiffuseUVMax);
#pragma endregion
#pragma region Mask
	Out_Json["UV_Mask_Index"] = m_tEffectParticleDesc.iMaskIndex;

	CJson_Utility::Write_Float2(Out_Json["Mask_Start_UV"], m_tEffectParticleDesc.vMaskStartUV);

	CJson_Utility::Write_Float2(Out_Json["Mask_UV_Speed"], m_tEffectParticleDesc.vMaskUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["Mask_UV_Force"], m_tEffectParticleDesc.vMaskUVForce);
	CJson_Utility::Write_Float2(Out_Json["Mask_UV_Max"], m_tEffectParticleDesc.vMaskUVMax);
#pragma endregion
#pragma region Noise
	Out_Json["UV_Noise_Index"] = m_tEffectParticleDesc.iNoiseIndex;

	CJson_Utility::Write_Float2(Out_Json["Noise_Start_UV"], m_tEffectParticleDesc.vNoiseStartUV);

	CJson_Utility::Write_Float2(Out_Json["Noise_UV_Speed"], m_tEffectParticleDesc.vNoiseUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["Noise_UV_Force"], m_tEffectParticleDesc.vNoiseUVForce);
	CJson_Utility::Write_Float2(Out_Json["Noise_UV_Max"], m_tEffectParticleDesc.vNoiseUVMax);
#pragma endregion
#pragma endregion

#pragma region Bloom & Glow
	Out_Json["Is_Bloom"] = m_tEffectParticleDesc.bBloom;
	Out_Json["Is_Glow"] = m_tEffectParticleDesc.bGlow;

	if (m_tEffectParticleDesc.bGlow)
	{
		CJson_Utility::Write_Float4(Out_Json["Start_Glow_Color"], m_tEffectParticleDesc.vStartGlowColor);
		CJson_Utility::Write_Float4(Out_Json["Glow_Color_Speed"], m_tEffectParticleDesc.vGlowColorSpeed);
		CJson_Utility::Write_Float4(Out_Json["Glow_Color_Force"], m_tEffectParticleDesc.vGlowColorForce);
	}
#pragma endregion

#pragma region For. Sprite
	if (
		(PASS_SPRITE_BLACKDISCARD == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_ALPHADISCARD == m_tEffectParticleDesc.iShaderPassIndex)
		)
	{
		Out_Json["Loop_Sprite"] = m_tEffectParticleDesc.bLoopSprite;

		Out_Json["Sprite_NumFrameX"] = m_tEffectParticleDesc.iNumFrameX;
		Out_Json["Sprite_NumFrameY"] = m_tEffectParticleDesc.iNumFrameY;

		Out_Json["Sprite_FrameSpeed"] = m_tEffectParticleDesc.fSpriteSpeed;
	}
#pragma endregion
}

void CEffect_Rect::Load_EffectJson(const json& In_Json, const _uint& In_iTimeScaleLayer)
{
	m_iTimeScaleLayerIndex = In_iTimeScaleLayer;

	m_szEffectName = In_Json["Name"];
	m_tEffectParticleDesc.iMaxInstance         = In_Json["Max_Instance"];
	m_tEffectParticleDesc.bLooping             = In_Json["Is_Looping"];
	m_tEffectParticleDesc.iParticleType        = In_Json["ParticleType"];
	m_tEffectParticleDesc.iFollowTransformType = In_Json["Follow_Transform"];

	if (In_Json.find("ShaderPassIndex") != In_Json.end())
		m_tEffectParticleDesc.iShaderPassIndex = In_Json["ShaderPassIndex"];

#pragma region Animation Sync
	if (In_Json.find("Sync_Animation") != In_Json.end())
		m_tEffectParticleDesc.bSyncAnimation = In_Json["Sync_Animation"];
	if (In_Json.find("Sync_AnimationKey") != In_Json.end())
		m_tEffectParticleDesc.iSyncAnimationKey = In_Json["Sync_AnimationKey"];
#pragma endregion

#pragma region Life Time
	if (In_Json.find("Init_Time") != In_Json.end())
		m_tEffectParticleDesc.fInitTime = In_Json["Init_Time"];
	if (In_Json.find("Min_Spawn_Time") != In_Json.end())
		m_tEffectParticleDesc.fMinSpawnTime = In_Json["Min_Spawn_Time"];
	if (In_Json.find("Max_Spawn_Time") != In_Json.end())
		m_tEffectParticleDesc.fMaxSpawnTime = In_Json["Max_Spawn_Time"];
	if (In_Json.find("Min_Life_Time") != In_Json.end())
		m_tEffectParticleDesc.fMinLifeTime = In_Json["Min_Life_Time"];
	if (In_Json.find("Max_Life_Time") != In_Json.end())
		m_tEffectParticleDesc.fMaxLifeTime = In_Json["Max_Life_Time"];
#pragma endregion

	if (In_Json.find("Is_Boner") != In_Json.end())
		m_tEffectParticleDesc.bBoner = In_Json["Is_Boner"];

	if (m_tEffectParticleDesc.bBoner)
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

#pragma region Spawn Position
	if (In_Json.find("Min_Spawn_Position") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Position"], m_tEffectParticleDesc.vMinSpawnPosition);
	if (In_Json.find("Max_Spawn_Position") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);
	if (In_Json.find("Min_Spawn_Offset_Direction") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMinSpawnOffsetDirection);
	if (In_Json.find("Max_Spawn_Offset_Direction") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);
	if (In_Json.find("Min_Spawn_Offset_Range") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Offset_Range"], m_tEffectParticleDesc.vMinSpawnOffsetRange);
	if (In_Json.find("Max_Spawn_Offset_Range") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);
#pragma endregion

	if (In_Json.find("Is_MoveLook") != In_Json.end())
		m_tEffectParticleDesc.bMoveLook = In_Json["Is_MoveLook"];

	if (In_Json.find("Is_Use_Gravity") != In_Json.end())
		m_tEffectParticleDesc.bUseGravity = In_Json["Is_Use_Gravity"];

	if (m_tEffectParticleDesc.bUseGravity)
	{
		if (In_Json.find("Gravity_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Gravity_Force"], m_tEffectParticleDesc.vGravityForce);
	}

	if (In_Json.find("Is_Easing_Position") != In_Json.end())
		m_tEffectParticleDesc.bEasingPosition = In_Json["Is_Easing_Position"];

	if (m_tEffectParticleDesc.bEasingPosition)
	{
		if (In_Json.find("Position_Easing_Type") != In_Json.end())
			m_tEffectParticleDesc.iSpeedEasingType = In_Json["Position_Easing_Type"];
		if (In_Json.find("Position_Easing_Total_Time") != In_Json.end())
			m_tEffectParticleDesc.fSpeedEasingTotalTime = In_Json["Position_Easing_Total_Time"];

		if (In_Json.find("Min_Goal_Offset_Position") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Goal_Offset_Position"], m_tEffectParticleDesc.vMinSpeed);
		if (In_Json.find("Max_Goal_Offset_Position") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Goal_Offset_Position"], m_tEffectParticleDesc.vMaxSpeed);
	}
	else
	{
#pragma region Speed
		if (In_Json.find("Is_Easing_Speed") != In_Json.end())
			m_tEffectParticleDesc.bEasingSpeed = In_Json["Is_Easing_Speed"];

		if (m_tEffectParticleDesc.bEasingSpeed)
		{
			if (In_Json.find("Speed_Easing_Type") != In_Json.end())
				m_tEffectParticleDesc.iSpeedEasingType = In_Json["Speed_Easing_Type"];
			if (In_Json.find("Speed_Easing_Total_Time") != In_Json.end())
				m_tEffectParticleDesc.fSpeedEasingTotalTime = In_Json["Speed_Easing_Total_Time"];
		}
		else
		{
			if (In_Json.find("Min_Speed") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Min_Speed"], m_tEffectParticleDesc.vMinSpeed);
			if (In_Json.find("Max_Speed") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Max_Speed"], m_tEffectParticleDesc.vMaxSpeed);
			if (In_Json.find("Min_Speed_Force") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Min_Speed_Force"], m_tEffectParticleDesc.vMinSpeedForce);
			if (In_Json.find("Max_Speed_Force") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Max_Speed_Force"], m_tEffectParticleDesc.vMaxSpeedForce);
		}

		if (In_Json.find("Min_Limit_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Limit_Speed"], m_tEffectParticleDesc.vMinLimitSpeed);
		if (In_Json.find("Max_Limit_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Limit_Speed"], m_tEffectParticleDesc.vMaxLimitSpeed);
#pragma endregion
	}

#pragma region Rotation
	if (_int(PARTICLETYPE::OUTBURST) != m_tEffectParticleDesc.iParticleType)
	{
		if (In_Json.find("Is_Easing_Rotation") != In_Json.end())
			m_tEffectParticleDesc.bEasingRotation = In_Json["Is_Easing_Rotation"];

		if (m_tEffectParticleDesc.bEasingRotation)
		{
			if (In_Json.find("Rotation_Easing_Type") != In_Json.end())
				m_tEffectParticleDesc.iRotationEasingType = In_Json["Rotation_Easing_Type"];
			if (In_Json.find("Rotation_Easing_Total_Time") != In_Json.end())
				m_tEffectParticleDesc.fRotationEasingTotalTime = In_Json["Rotation_Easing_Total_Time"];
		}
		else
		{
			if (In_Json.find("Min_Start_Rotation") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Min_Start_Rotation"], m_tEffectParticleDesc.vMinStartRotation);
			if (In_Json.find("Max_Start_Rotation") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Max_Start_Rotation"], m_tEffectParticleDesc.vMaxStartRotation);
			if (In_Json.find("Min_Rotation_Speed") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Min_Rotation_Speed"], m_tEffectParticleDesc.vMinRotationSpeed);
			if (In_Json.find("Max_Rotation_Speed") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Max_Rotation_Speed"], m_tEffectParticleDesc.vMaxRotationSpeed);
			if (In_Json.find("Min_Rotation_Force") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Min_Rotation_Force"], m_tEffectParticleDesc.vMinRotationForce);
			if (In_Json.find("Max_Rotation_Force") != In_Json.end())
				CJson_Utility::Load_Float3(In_Json["Max_Rotation_Force"], m_tEffectParticleDesc.vMaxRotationForce);
		}

		if (In_Json.find("Min_Rotation") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Rotation"], m_tEffectParticleDesc.vMinLimitRotation);
		if (In_Json.find("Max_Rotation") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Rotation"], m_tEffectParticleDesc.vMaxLimitRotation);
	}
#pragma endregion

#pragma region Scale
	if (In_Json.find("Is_Easing_Scale") != In_Json.end())
		m_tEffectParticleDesc.bEasingScale = In_Json["Is_Easing_Scale"];

	if (m_tEffectParticleDesc.bEasingScale)
	{
		if (In_Json.find("Scale_Easing_Type") != In_Json.end())
			m_tEffectParticleDesc.iScaleEasingType = In_Json["Scale_Easing_Type"];
		if (In_Json.find("Scale_Easing_Total_Time") != In_Json.end())
			m_tEffectParticleDesc.fScaleEasingTotalTime = In_Json["Scale_Easing_Total_Time"];
	}
	else
	{
		if (In_Json.find("Min_Start_Scale") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
		if (In_Json.find("Max_Start_Scale") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);

		if (In_Json.find("Min_Scale_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Scale_Speed"], m_tEffectParticleDesc.vMinScaleSpeed);
		if (In_Json.find("Max_Scale_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Scale_Speed"], m_tEffectParticleDesc.vMaxScaleSpeed);

		if (In_Json.find("Min_Scale_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Scale_Force"], m_tEffectParticleDesc.vMinScaleForce);
		if (In_Json.find("Max_Scale_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Scale_Force"], m_tEffectParticleDesc.vMaxScaleForce);

		/*if (In_Json.find("Scale_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Scale_Speed"], m_tEffectParticleDesc.vScaleSpeed);
		if (In_Json.find("Scale_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Scale_Force"], m_tEffectParticleDesc.vScaleForce);*/
	}
	if (In_Json.find("Min_Scale") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Scale"], m_tEffectParticleDesc.vMinLimitScale);
	if (In_Json.find("Max_Scale") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Scale"], m_tEffectParticleDesc.vMaxLimitScale);
#pragma endregion

#pragma region Color
	if (In_Json.find("Discard_Ratio") != In_Json.end())
		m_tEffectParticleDesc.fDiscardRatio = In_Json["Discard_Ratio"];
	if (In_Json.find("Is_Gray_Only_Use_Red") != In_Json.end())
		m_tEffectParticleDesc.IsGrayOnlyUseRed = In_Json["Is_Gray_Only_Use_Red"];

	if (In_Json.find("Min_Start_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	if (In_Json.find("Max_Start_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);

	if (In_Json.find("Min_Color_Speed") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
	if (In_Json.find("Max_Color_Speed") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Color_Speed"], m_tEffectParticleDesc.vMaxColorSpeed);

	if (In_Json.find("Min_Color_Force") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Color_Force"], m_tEffectParticleDesc.vMinColorForce);
	if (In_Json.find("Max_Color_Force") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Color_Force"], m_tEffectParticleDesc.vMaxColorForce);

	if (In_Json.find("Min_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Color"], m_tEffectParticleDesc.vMinColor);
	if (In_Json.find("Max_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);
#pragma endregion

#pragma region Texture
#pragma region Diffuse
	m_tEffectParticleDesc.iDiffuseIndex = In_Json["UV_Diffuse_Index"];
	CJson_Utility::Load_Float2(In_Json["Diffuse_Start_UV"], m_tEffectParticleDesc.vDiffuseStartUV);
	CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Speed"], m_tEffectParticleDesc.vDiffuseUVSpeed);
	CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Force"], m_tEffectParticleDesc.vDiffuseUVForce);
	CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Max"], m_tEffectParticleDesc.vDiffuseUVMax);
#pragma endregion
#pragma region Mask
	m_tEffectParticleDesc.iMaskIndex = In_Json["UV_Mask_Index"];
	CJson_Utility::Load_Float2(In_Json["Mask_Start_UV"], m_tEffectParticleDesc.vMaskStartUV);
	CJson_Utility::Load_Float2(In_Json["Mask_UV_Speed"], m_tEffectParticleDesc.vMaskUVSpeed);
	CJson_Utility::Load_Float2(In_Json["Mask_UV_Force"], m_tEffectParticleDesc.vMaskUVForce);
	CJson_Utility::Load_Float2(In_Json["Mask_UV_Max"], m_tEffectParticleDesc.vMaskUVMax);
#pragma endregion
#pragma region Noise
	m_tEffectParticleDesc.iNoiseIndex = In_Json["UV_Noise_Index"];
	CJson_Utility::Load_Float2(In_Json["Noise_Start_UV"], m_tEffectParticleDesc.vNoiseStartUV);
	CJson_Utility::Load_Float2(In_Json["Noise_UV_Speed"], m_tEffectParticleDesc.vNoiseUVSpeed);
	CJson_Utility::Load_Float2(In_Json["Noise_UV_Force"], m_tEffectParticleDesc.vNoiseUVForce);
	CJson_Utility::Load_Float2(In_Json["Noise_UV_Max"], m_tEffectParticleDesc.vNoiseUVMax);
#pragma endregion
#pragma endregion

#pragma region Bloom & Glow
	if (In_Json.find("Is_Bloom") != In_Json.end())
		m_tEffectParticleDesc.bBloom = In_Json["Is_Bloom"];
	if (In_Json.find("Is_Glow") != In_Json.end())
		m_tEffectParticleDesc.bGlow = In_Json["Is_Glow"];

	if (m_tEffectParticleDesc.bGlow)
	{
		CJson_Utility::Load_Float4(In_Json["Start_Glow_Color"], m_tEffectParticleDesc.vStartGlowColor);
		CJson_Utility::Load_Float4(In_Json["Glow_Color_Speed"], m_tEffectParticleDesc.vGlowColorSpeed);
		CJson_Utility::Load_Float4(In_Json["Glow_Color_Force"], m_tEffectParticleDesc.vGlowColorForce);
	}
#pragma endregion

#pragma region For. Sprite
	if (
		(PASS_SPRITE_BLACKDISCARD == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_ALPHADISCARD == m_tEffectParticleDesc.iShaderPassIndex)
		)
	{
		if (In_Json.find("Loop_Sprite") != In_Json.end())
			m_tEffectParticleDesc.bLoopSprite = In_Json["Loop_Sprite"];

		m_tEffectParticleDesc.iNumFrameX = In_Json["Sprite_NumFrameX"];
		m_tEffectParticleDesc.iNumFrameY = In_Json["Sprite_NumFrameY"];
		m_tEffectParticleDesc.fSpriteSpeed = In_Json["Sprite_FrameSpeed"];
	}
#pragma endregion

	m_pTextureCom.lock()->Use_Texture(m_szEffectName.c_str());
	Reset_Instance(m_tEffectParticleDesc.iMaxInstance);
}

void CEffect_Rect::Play(_float fTimeDelta)
{
	if (0.f < m_fCurrentInitTime)
	{
		m_fCurrentInitTime -= fTimeDelta;
		return;
	}

	_float	fFrameTime(0.006944f);
	_int	iTickCount(0);
	//iTickCount = fTimeDelta / fFrameTime;
	//m_fPreFrame += fTimeDelta - (iTickCount * fTimeDelta / fFrameTime);
	m_fPreFrame += fTimeDelta;

	while (fFrameTime <= m_fPreFrame)
	{
		++iTickCount;
		m_fPreFrame -= fFrameTime;
	}

	//_matrix		BoneMatrix = XMMatrixIdentity();
	//_matrix		WorldMatrix = XMMatrixIdentity();

	//if (m_pBoneNode.lock() && ((_uint)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType))
	//{
	//	_float4x4	TempMat = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Get_TransformationMatrix();
	//	_matrix		ModelTranMat = XMLoadFloat4x4(&TempMat);
	//	BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

	//	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	//	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	//	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	//}

	//if (m_pParentTransformCom.lock())
	//	WorldMatrix = BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix();

	for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{
		if (!m_tParticleDescs[i].bEnable)
		{
			if (m_tParticleDescs[i].fCurrentSpawnTime > m_tParticleDescs[i].fTargetSpawnTime)
			{
				m_tParticleDescs[i].bEnable = true;

				if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType && m_pParentTransformCom.lock())
					XMStoreFloat4x4(&m_tParticleDescs[i].matParentMatrix, m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
			}
			else
			{
				m_tParticleDescs[i].fCurrentSpawnTime += fTimeDelta;
				continue;
			}
		}

		else
		{
			if (m_tParticleDescs[i].fCurrentLifeTime > m_tParticleDescs[i].fTargetLifeTime)
			{
				if (m_tEffectParticleDesc.bLooping && !m_bStopParticle)
				{
					this->Reset_ParticleDesc((_uint)i);
				}
				else
				{
					m_tParticleDescs[i].bEnable = false;
					m_tParticleDescs[i].fCurrentSpawnTime = -999999.f;
				}
				continue;
			}

			m_tParticleDescs[i].fCurrentLifeTime += fTimeDelta;
		}


		for (_int x(0); x < iTickCount; ++x)
		{
			this->Update_ParticleRotation(i, fFrameTime);
			this->Update_ParticlePosition(i, fFrameTime);
			this->Update_ParticleScale(i, fFrameTime);
			this->Update_ParticleColor(i, fFrameTime);
			this->Update_ParticleSpriteFrame(i, fFrameTime);
		}
	}

	for (_int x(0); x < iTickCount; ++x)
	{
		this->Update_ParticleUV(fFrameTime);
		this->Update_ParticleGlowColor(fFrameTime);
	}
}

void CEffect_Rect::Sync_Animation()
{
	if (!m_tEffectParticleDesc.bSyncAnimation)
		return;

	this->Reset_Effect(weak_ptr<CTransform>());
}

void CEffect_Rect::UnUse_EffectParticle()
{
	if (0.f < m_fCurrentInitTime)
	{
		CBase::Set_Enable(false);
		return;
	}

	m_bStopParticle = true;
}

void CEffect_Rect::ReBake_EditParticle()
{
	this->Reset_Instance(m_tEffectParticleDesc.iMaxInstance);
}

void CEffect_Rect::Reset_Instance(const _uint& In_ParticleCount)
{
	m_tParticleDescs.clear();
	m_bStopParticle = false;
	m_bStopSprite = false;

	m_tParticleDescs = vector<PARTICLE_DESC>(In_ParticleCount, PARTICLE_DESC());
	m_tOriginalParticleDescs = vector<PARTICLE_DESC>(In_ParticleCount, PARTICLE_DESC());

	m_pVIBuffer.lock()->Init_Particle(In_ParticleCount);
	this->Generate_RandomOriginalParticleDesc();
	this->Reset_ParticleDescs();
}

void CEffect_Rect::Reset_ParticleDescs()
{
	for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
		this->Reset_ParticleDesc((_uint)i);
}

void CEffect_Rect::Reset_ParticleDesc(const _uint& In_iIndex)
{
	memcpy(&m_tParticleDescs[In_iIndex], &m_tOriginalParticleDescs[In_iIndex], sizeof(PARTICLE_DESC));

#pragma region Backup
	//_vector vOffset;
	//vOffset = XMLoadFloat3(&m_tParticleDescs[In_iIndex].vOffsetPosition);

	//if (m_pBoneNode.lock())
	//{
	//	_matrix		BoneMatrix = XMMatrixIdentity();

	//	_float4x4	TempMat = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Get_TransformationMatrix();
	//	_matrix		ModelTranMat = XMLoadFloat4x4(&TempMat);
	//	BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

	//	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	//	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	//	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	//	vOffset = XMVector3TransformCoord(vOffset, SMath::Get_RotationMatrix(BoneMatrix));
	//}

	//_float3 f3Offset;
	//ZeroMemory(&f3Offset, sizeof(_float3));
	//XMStoreFloat3(&f3Offset, vOffset);
#pragma endregion

	SMath::Add_Float3(&m_tParticleDescs[In_iIndex].vCurrentTranslation, m_tParticleDescs[In_iIndex].vOffsetPosition);
}


void CEffect_Rect::Generate_RandomOriginalParticleDesc()
{
	for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{


		m_tOriginalParticleDescs[i].Reset();

		m_tOriginalParticleDescs[i].fTargetLifeTime =
			SMath::fRandom(m_tEffectParticleDesc.fMinLifeTime, m_tEffectParticleDesc.fMaxLifeTime);

		m_tOriginalParticleDescs[i].fTargetSpawnTime =
			SMath::fRandom(m_tEffectParticleDesc.fMinSpawnTime, m_tEffectParticleDesc.fMaxSpawnTime);

		_float3 vRandomDir = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnOffsetDirection, m_tEffectParticleDesc.vMaxSpawnOffsetDirection);
		_vector vRandomDirFromVector = XMLoadFloat3(&vRandomDir);
		vRandomDirFromVector = XMVector3Normalize(vRandomDirFromVector);

		_float3 vRandomScalar = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnOffsetRange, m_tEffectParticleDesc.vMaxSpawnOffsetRange);
		_vector vRandomScalarFromVector = XMLoadFloat3(&vRandomScalar);

		_matrix RotationMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vRandomDirFromVector);
		RotationMatrix = SMath::Go_Right(RotationMatrix, XMVectorGetX(vRandomScalarFromVector));
		RotationMatrix = SMath::Go_Up(RotationMatrix, XMVectorGetY(vRandomScalarFromVector));
		RotationMatrix = SMath::Go_Straight(RotationMatrix, XMVectorGetZ(vRandomScalarFromVector));

		XMStoreFloat3(&m_tOriginalParticleDescs[i].vCurrentTranslation, RotationMatrix.r[3]);

		m_tOriginalParticleDescs[i].vOffsetPosition = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnPosition, m_tEffectParticleDesc.vMaxSpawnPosition);

		m_tOriginalParticleDescs[i].vCurrentRotation =
			SMath::vRandom(m_tEffectParticleDesc.vMinStartRotation, m_tEffectParticleDesc.vMaxStartRotation);

		m_tOriginalParticleDescs[i].vTargetRotationSpeed =
			SMath::vRandom(m_tEffectParticleDesc.vMinRotationSpeed, m_tEffectParticleDesc.vMaxRotationSpeed);

		m_tOriginalParticleDescs[i].vTargetRotationForce =
			SMath::vRandom(m_tEffectParticleDesc.vMinRotationForce, m_tEffectParticleDesc.vMaxRotationForce);

		if (m_tEffectParticleDesc.bEasingPosition)
		{
			m_tOriginalParticleDescs[i].vTargetSpeed = SMath::Add_Float3(m_tOriginalParticleDescs[i].vCurrentTranslation, m_tOriginalParticleDescs[i].vOffsetPosition);

			_float3 vGoalPos = SMath::vRandom(m_tEffectParticleDesc.vMinSpeed, m_tEffectParticleDesc.vMaxSpeed);
			_vector vGoalOffsetPosition = XMLoadFloat3(&vGoalPos);
			vGoalOffsetPosition = XMVectorSetX(vGoalOffsetPosition, XMVectorGetX(vGoalOffsetPosition) * m_tOriginalParticleDescs[i].vCurrentRotation.x);
			vGoalOffsetPosition = XMVectorSetY(vGoalOffsetPosition, XMVectorGetY(vGoalOffsetPosition) * m_tOriginalParticleDescs[i].vCurrentRotation.y);
			vGoalOffsetPosition = XMVectorSetZ(vGoalOffsetPosition, XMVectorGetZ(vGoalOffsetPosition) * m_tOriginalParticleDescs[i].vCurrentRotation.z);

			m_tOriginalParticleDescs[i].vTargetSpeedForce.x = m_tOriginalParticleDescs[i].vTargetSpeed.x + XMVectorGetX(vGoalOffsetPosition);
			m_tOriginalParticleDescs[i].vTargetSpeedForce.y = m_tOriginalParticleDescs[i].vTargetSpeed.y + XMVectorGetY(vGoalOffsetPosition);
			m_tOriginalParticleDescs[i].vTargetSpeedForce.z = m_tOriginalParticleDescs[i].vTargetSpeed.z + XMVectorGetZ(vGoalOffsetPosition);
		}
		else
		{
			m_tOriginalParticleDescs[i].vTargetSpeed =
				SMath::vRandom(m_tEffectParticleDesc.vMinSpeed, m_tEffectParticleDesc.vMaxSpeed);

			m_tOriginalParticleDescs[i].vTargetSpeedForce =
				SMath::vRandom(m_tEffectParticleDesc.vMinSpeedForce, m_tEffectParticleDesc.vMaxSpeedForce);
		}

		m_tOriginalParticleDescs[i].vCurrentScale =
			SMath::vRandom(m_tEffectParticleDesc.vMinStartScale, m_tEffectParticleDesc.vMaxStartScale);

		m_tOriginalParticleDescs[i].vTargetScaleSpeed =
			SMath::vRandom(m_tEffectParticleDesc.vMinScaleSpeed, m_tEffectParticleDesc.vMaxScaleSpeed);

		m_tOriginalParticleDescs[i].vTargetScaleForce =
			SMath::vRandom(m_tEffectParticleDesc.vMinScaleForce, m_tEffectParticleDesc.vMaxScaleForce);

		if (m_tEffectParticleDesc.IsGrayOnlyUseRed)
		{
			_float fGrayColor = SMath::fRandom(m_tEffectParticleDesc.vMinStartColor.x, m_tEffectParticleDesc.vMaxStartColor.x);
			_float fAhlpaColor = SMath::fRandom(m_tEffectParticleDesc.vMinStartColor.w, m_tEffectParticleDesc.vMaxStartColor.w);
			m_tOriginalParticleDescs[i].vCurrentColor = { fGrayColor, fGrayColor, fGrayColor, fAhlpaColor };
		}
		else
		{
			m_tOriginalParticleDescs[i].vCurrentColor =
				SMath::vRandom(m_tEffectParticleDesc.vMinStartColor, m_tEffectParticleDesc.vMaxStartColor);
		}

		m_tOriginalParticleDescs[i].vTargetColorSpeed =
			SMath::vRandom(m_tEffectParticleDesc.vMinColorSpeed, m_tEffectParticleDesc.vMaxColorSpeed);

		m_tOriginalParticleDescs[i].vTargetColorForce =
			SMath::vRandom(m_tEffectParticleDesc.vMinColorForce, m_tEffectParticleDesc.vMaxColorForce);

		m_tOriginalParticleDescs[i].vCurrentUV = m_tEffectParticleDesc.vMaskStartUV;

		m_tOriginalParticleDescs[i].vCurrentScale.x = max(0.00001f, m_tOriginalParticleDescs[i].vCurrentScale.x);
		m_tOriginalParticleDescs[i].vCurrentScale.y = max(0.00001f, m_tOriginalParticleDescs[i].vCurrentScale.y);
		m_tOriginalParticleDescs[i].vCurrentScale.z = max(0.00001f, m_tOriginalParticleDescs[i].vCurrentScale.z);
	}
}

_bool CEffect_Rect::Check_DisableAllParticle()
{
	if (m_tEffectParticleDesc.bLooping && !m_bStopParticle)
		return false;

	if (0.f < m_fCurrentInitTime)
		return false;

	if ((_uint)TIMESCALE_LAYER::EDITER == m_iTimeScaleLayerIndex)
		return false;

	for (auto& elem : m_tParticleDescs)
	{
		if (elem.bEnable || elem.fCurrentSpawnTime >= 0.f)
			return false;
	}

	return true;
}

void CEffect_Rect::Update_ParticlePosition(const _uint& i, _float fTimeDelta)
{
	if (m_pBoneNode.lock())
	{
		_float4x4 TempMat = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Get_TransformationMatrix();
		_matrix ModelTranMat = XMLoadFloat4x4(&TempMat);
		_matrix BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		if ((_int)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType)
		{
			if (m_pParentTransformCom.lock())
				m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
			else
				m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix);
		}
	}

	_float3 vMove;
	ZeroMemory(&vMove, sizeof(_float3));

	if (m_tEffectParticleDesc.bEasingPosition)
	{
		_float fElapsedTime = m_tParticleDescs[i].fCurrentLifeTime;

		if (0.f > fElapsedTime)
			return;

		this->Apply_Easing
		(
			vMove
			, (EASING_TYPE)m_tEffectParticleDesc.iSpeedEasingType
			, XMLoadFloat3(&m_tParticleDescs[i].vTargetSpeed)
			, XMLoadFloat3(&m_tParticleDescs[i].vTargetSpeedForce)
			, fElapsedTime
			, m_tEffectParticleDesc.fSpeedEasingTotalTime
		);

		if (m_tEffectParticleDesc.bMoveLook)
		{
			_vector vSpawnPos = XMLoadFloat3(&m_tParticleDescs[i].vTargetSpeed);
			_vector vMovePosition = XMLoadFloat3(&vMove);
			vMovePosition -= vSpawnPos;
			_vector vRotatedPosition = XMVector3TransformCoord(vMovePosition, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation)));
			vRotatedPosition += vSpawnPos;
			// _vector vCurrentPosition = XMLoadFloat3(&m_tParticleDescs[i].vCurrentTranslation);
			// vCurrentPosition +       = vRotatedPosition;
			XMStoreFloat3(&m_tParticleDescs[i].vCurrentTranslation, vRotatedPosition);
		}
		else
			m_tParticleDescs[i].vCurrentTranslation = vMove;

	}
	else
	{
		if (m_tEffectParticleDesc.bEasingSpeed)
		{
			_float fElapsedTime = m_tParticleDescs[i].fCurrentLifeTime;

			if (0.f > fElapsedTime)
				return;

			this->Apply_Easing
			(
				vMove
				, (EASING_TYPE)m_tEffectParticleDesc.iSpeedEasingType
				, XMLoadFloat3(&m_tEffectParticleDesc.vMinLimitSpeed)
				, XMLoadFloat3(&m_tEffectParticleDesc.vMaxLimitSpeed)
				, fElapsedTime
				, m_tEffectParticleDesc.fSpeedEasingTotalTime
			);

			vMove.x = max(m_tEffectParticleDesc.vMinLimitSpeed.x, min(m_tEffectParticleDesc.vMaxLimitSpeed.x, vMove.x));
			vMove.y = max(m_tEffectParticleDesc.vMinLimitSpeed.y, min(m_tEffectParticleDesc.vMaxLimitSpeed.y, vMove.y));
			vMove.z = max(m_tEffectParticleDesc.vMinLimitSpeed.z, min(m_tEffectParticleDesc.vMaxLimitSpeed.z, vMove.z));

			m_tParticleDescs[i].vTargetSpeed = vMove;
		}
		else
		{
			vMove = SMath::Mul_Float3(m_tParticleDescs[i].vTargetSpeed, fTimeDelta);

			m_tParticleDescs[i].vCurrentSpeedForce =
				SMath::Add_Float3(m_tParticleDescs[i].vCurrentSpeedForce, SMath::Mul_Float3(m_tParticleDescs[i].vTargetSpeedForce, fTimeDelta));

			vMove = SMath::Add_Float3(vMove, m_tParticleDescs[i].vCurrentSpeedForce);

			vMove.x = max(m_tEffectParticleDesc.vMinLimitSpeed.x, min(m_tEffectParticleDesc.vMaxLimitSpeed.x, vMove.x));
			vMove.y = max(m_tEffectParticleDesc.vMinLimitSpeed.y, min(m_tEffectParticleDesc.vMaxLimitSpeed.y, vMove.y));
			vMove.z = max(m_tEffectParticleDesc.vMinLimitSpeed.z, min(m_tEffectParticleDesc.vMaxLimitSpeed.z, vMove.z));
		}

		if (m_tEffectParticleDesc.bMoveLook)
		{
			if ((_int)PARTICLETYPE::OUTBURST == m_tEffectParticleDesc.iParticleType)
			{
				_vector vMovePosition = XMLoadFloat3(&vMove);
				_vector vRotation = XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation);
				_matrix vRotationMatrix = XMMatrixRotationRollPitchYawFromVector(vRotation);
				vRotationMatrix.r[3] = vRotationMatrix.r[1];
				vRotationMatrix.r[1] = vRotationMatrix.r[2];
				vRotationMatrix.r[2] = vRotationMatrix.r[3];
				vRotationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
				_vector vRotatedPosition = XMVector3TransformCoord(vMovePosition, vRotationMatrix);
				_vector vCurrentPosition = XMLoadFloat3(&m_tParticleDescs[i].vCurrentTranslation);
				vCurrentPosition += vRotatedPosition;
				XMStoreFloat3(&m_tParticleDescs[i].vCurrentTranslation, vCurrentPosition);
			}
			else
			{
				_vector vMovePosition = XMLoadFloat3(&vMove);
				_vector vRotatedPosition = XMVector3TransformCoord(vMovePosition, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation)));
				_vector vCurrentPosition = XMLoadFloat3(&m_tParticleDescs[i].vCurrentTranslation);
				vCurrentPosition += vRotatedPosition;
				XMStoreFloat3(&m_tParticleDescs[i].vCurrentTranslation, vCurrentPosition);
			}
		}
		else
		{
			if (m_tEffectParticleDesc.bEasingSpeed)
				m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, m_tParticleDescs[i].vTargetSpeed);
			else
				m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, vMove);
		}
	}

	if (m_tEffectParticleDesc.bUseGravity)
	{
		_vector vDeltaGravity;
		ZeroMemory(&vDeltaGravity, sizeof(_vector));

		vDeltaGravity = XMVectorSetX(vDeltaGravity, m_tEffectParticleDesc.vGravityForce.x * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta));
		vDeltaGravity = XMVectorSetY(vDeltaGravity, m_tEffectParticleDesc.vGravityForce.y * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta));
		vDeltaGravity = XMVectorSetZ(vDeltaGravity, m_tEffectParticleDesc.vGravityForce.z * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta));

		_float3 f3DeltaGravity;
		XMStoreFloat3(&f3DeltaGravity, vDeltaGravity);

		m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, f3DeltaGravity);
		// vMove = SMath::Add_Float3(vMove, vSpeed);
	}

	if (m_tEffectParticleDesc.bUseGravity)
	{
		_vector vDeltaGravity;
		ZeroMemory(&vDeltaGravity, sizeof(_vector));

		vDeltaGravity = XMVectorSetX(vDeltaGravity, m_tEffectParticleDesc.vGravityForce.x * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta));
		vDeltaGravity = XMVectorSetY(vDeltaGravity, m_tEffectParticleDesc.vGravityForce.y * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta));
		vDeltaGravity = XMVectorSetZ(vDeltaGravity, m_tEffectParticleDesc.vGravityForce.z * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta));

		_float3 f3DeltaGravity;
		XMStoreFloat3(&f3DeltaGravity, vDeltaGravity);

		m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, f3DeltaGravity);
		// vMove = SMath::Add_Float3(vMove, vSpeed);
	}
}

void CEffect_Rect::Update_ParticleRotation(const _uint& i, _float fTimeDelta)
{

	if ((_int)PARTICLETYPE::OUTBURST == m_tEffectParticleDesc.iParticleType)
	{
		_vector vUp = XMLoadFloat3(&m_tOriginalParticleDescs[i].vCurrentTranslation); //커런트 포지션
		_matrix ReverseAxisRotationMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vUp);
		_matrix RotationMatrix = { ReverseAxisRotationMatrix.r[0],
			ReverseAxisRotationMatrix.r[2],
			ReverseAxisRotationMatrix.r[1],
			ReverseAxisRotationMatrix.r[3]
		};

		m_tParticleDescs[i].vCurrentRotation = SMath::Extract_PitchYawRollFromRotationMatrix(RotationMatrix);
	}
	else
	{
		_float3 vRotation;
		ZeroMemory(&vRotation, sizeof(_float3));

		if (m_tEffectParticleDesc.bEasingRotation)
		{
			_float fElapsedTime = m_tParticleDescs[i].fCurrentLifeTime;

			if (0.f > fElapsedTime)
				return;

			this->Apply_Easing
			(
				vRotation
				, (EASING_TYPE)m_tEffectParticleDesc.iRotationEasingType
				, XMLoadFloat3(&m_tEffectParticleDesc.vMinLimitRotation)
				, XMLoadFloat3(&m_tEffectParticleDesc.vMaxLimitRotation)
				, fElapsedTime
				, m_tEffectParticleDesc.fRotationEasingTotalTime
			);

			vRotation.x = max(m_tEffectParticleDesc.vMinLimitRotation.x, min(m_tEffectParticleDesc.vMaxLimitRotation.x, vRotation.x));
			vRotation.y = max(m_tEffectParticleDesc.vMinLimitRotation.y, min(m_tEffectParticleDesc.vMaxLimitRotation.y, vRotation.y));
			vRotation.z = max(m_tEffectParticleDesc.vMinLimitRotation.z, min(m_tEffectParticleDesc.vMaxLimitRotation.z, vRotation.z));

			m_tParticleDescs[i].vCurrentRotation = vRotation;
		}
		else
		{
			vRotation = SMath::Mul_Float3(m_tParticleDescs[i].vTargetRotationSpeed, fTimeDelta);
			m_tParticleDescs[i].vCurrentRotationForce =
				SMath::Add_Float3(m_tParticleDescs[i].vCurrentRotationForce, SMath::Mul_Float3(m_tParticleDescs[i].vTargetRotationForce, fTimeDelta));

			vRotation = SMath::Add_Float3(vRotation, m_tParticleDescs[i].vCurrentRotationForce);

			vRotation.x = max(m_tEffectParticleDesc.vMinLimitRotation.x, min(m_tEffectParticleDesc.vMaxLimitRotation.x, vRotation.x));
			vRotation.y = max(m_tEffectParticleDesc.vMinLimitRotation.y, min(m_tEffectParticleDesc.vMaxLimitRotation.y, vRotation.y));
			vRotation.z = max(m_tEffectParticleDesc.vMinLimitRotation.z, min(m_tEffectParticleDesc.vMaxLimitRotation.z, vRotation.z));

			m_tParticleDescs[i].vCurrentRotation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentRotation, vRotation);
		}
	}
}

void CEffect_Rect::Update_ParticleScale(const _uint& i, _float fTimeDelta)
{
	_float3 vScale;
	ZeroMemory(&vScale, sizeof(_float3));

	if (m_tEffectParticleDesc.bEasingScale)
	{
		_float fElapsedTime = m_tParticleDescs[i].fCurrentLifeTime;

		if (0.f > fElapsedTime)
			return;

		this->Apply_Easing
		(
			vScale
			, (EASING_TYPE)m_tEffectParticleDesc.iScaleEasingType
			, XMLoadFloat3(&m_tEffectParticleDesc.vMinLimitScale)
			, XMLoadFloat3(&m_tEffectParticleDesc.vMaxLimitScale)
			, fElapsedTime
			, m_tEffectParticleDesc.fScaleEasingTotalTime
		);

		m_tParticleDescs[i].vCurrentScale = vScale;
	}
	else
	{
		vScale = SMath::Mul_Float3(m_tParticleDescs[i].vTargetScaleSpeed, fTimeDelta);

		m_tParticleDescs[i].vCurrentScaleForce =
			SMath::Add_Float3(m_tParticleDescs[i].vCurrentScaleForce, SMath::Mul_Float3(m_tParticleDescs[i].vTargetScaleForce, fTimeDelta));

		SMath::Add_Float3(&m_tParticleDescs[i].vCurrentScale, m_tParticleDescs[i].vCurrentScaleForce);

		m_tParticleDescs[i].vCurrentScale = SMath::Add_Float3(m_tParticleDescs[i].vCurrentScale, vScale);
	}

	m_tParticleDescs[i].vCurrentScale.x = max(m_tEffectParticleDesc.vMinLimitScale.x, min(m_tEffectParticleDesc.vMaxLimitScale.x, m_tParticleDescs[i].vCurrentScale.x));
	m_tParticleDescs[i].vCurrentScale.y = max(m_tEffectParticleDesc.vMinLimitScale.y, min(m_tEffectParticleDesc.vMaxLimitScale.y, m_tParticleDescs[i].vCurrentScale.y));
	m_tParticleDescs[i].vCurrentScale.z = max(m_tEffectParticleDesc.vMinLimitScale.z, min(m_tEffectParticleDesc.vMaxLimitScale.z, m_tParticleDescs[i].vCurrentScale.z));
}

void CEffect_Rect::Update_ParticleUV(_float fTimeDelta)
{
#pragma region Diffuse
	_vector vDiffuseUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vDiffuseUVSpeed) * fTimeDelta;
	m_vDiffuseCurrentUVForce.x += m_tEffectParticleDesc.vDiffuseUVForce.x * fTimeDelta;
	m_vDiffuseCurrentUVForce.y += m_tEffectParticleDesc.vDiffuseUVForce.y * fTimeDelta;

	_vector vDiffuseMoveUV = vDiffuseUVSpeed + XMLoadFloat2(&m_vDiffuseCurrentUVForce);
	_vector vDiffuseCurrentUV = XMLoadFloat2(&m_vDiffuseCurrentUV);
	vDiffuseCurrentUV += vDiffuseMoveUV;

	XMVectorSetX(vDiffuseCurrentUV, min(m_tEffectParticleDesc.vDiffuseUVMax.x, XMVectorGetX(vDiffuseCurrentUV)));
	XMVectorSetY(vDiffuseCurrentUV, min(m_tEffectParticleDesc.vDiffuseUVMax.y, XMVectorGetY(vDiffuseCurrentUV)));

	XMStoreFloat2(&m_vDiffuseCurrentUV, vDiffuseCurrentUV);
#pragma endregion

#pragma region Mask
	_vector vMaskUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vMaskUVSpeed) * fTimeDelta;
	m_vMaskCurrentUVForce.x += m_tEffectParticleDesc.vMaskUVForce.x * fTimeDelta;
	m_vMaskCurrentUVForce.y += m_tEffectParticleDesc.vMaskUVForce.y * fTimeDelta;

	_vector vMaskMoveUV = vMaskUVSpeed + XMLoadFloat2(&m_vMaskCurrentUVForce);
	_vector vMaskCurrentUV = XMLoadFloat2(&m_vMaskCurrentUV);
	vMaskCurrentUV += vMaskMoveUV;

	XMVectorSetX(vMaskCurrentUV, min(m_tEffectParticleDesc.vMaskUVMax.x, XMVectorGetX(vMaskCurrentUV)));
	XMVectorSetY(vMaskCurrentUV, min(m_tEffectParticleDesc.vMaskUVMax.y, XMVectorGetY(vMaskCurrentUV)));

	XMStoreFloat2(&m_vMaskCurrentUV, vMaskCurrentUV);
#pragma endregion

#pragma region Noise
	_vector vNoiseUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vNoiseUVSpeed) * fTimeDelta;
	m_vNoiseCurrentUVForce.x += m_tEffectParticleDesc.vNoiseUVForce.x * fTimeDelta;
	m_vNoiseCurrentUVForce.y += m_tEffectParticleDesc.vNoiseUVForce.y * fTimeDelta;

	_vector vNoiseMoveUV = vNoiseUVSpeed + XMLoadFloat2(&m_vNoiseCurrentUVForce);
	_vector vNoiseCurrentUV = XMLoadFloat2(&m_vNoiseCurrentUV);
	vNoiseCurrentUV += vNoiseMoveUV;

	XMVectorSetX(vNoiseCurrentUV, min(m_tEffectParticleDesc.vNoiseUVMax.x, XMVectorGetX(vNoiseCurrentUV)));
	XMVectorSetY(vNoiseCurrentUV, min(m_tEffectParticleDesc.vNoiseUVMax.y, XMVectorGetY(vNoiseCurrentUV)));

	XMStoreFloat2(&m_vNoiseCurrentUV, vNoiseCurrentUV);
#pragma endregion
}

void CEffect_Rect::Update_ParticleColor(const _uint& i, _float fTimeDelta)
{
	_vector vColor;
	ZeroMemory(&vColor, sizeof(_vector));

	if (m_tEffectParticleDesc.IsGrayOnlyUseRed)
	{
		XMVectorSetX(vColor, m_tParticleDescs[i].vTargetColorSpeed.x * fTimeDelta);
		XMVectorSetW(vColor, m_tParticleDescs[i].vTargetColorSpeed.w * fTimeDelta);

		m_tParticleDescs[i].vCurrentColorForce.x += m_tParticleDescs[i].vTargetColorForce.x * fTimeDelta;
		m_tParticleDescs[i].vCurrentColorForce.w += m_tParticleDescs[i].vTargetColorForce.w * fTimeDelta;

		XMVectorSetX(vColor, XMVectorGetX(vColor) + m_tParticleDescs[i].vCurrentColorForce.x);
		XMVectorSetW(vColor, XMVectorGetW(vColor) + m_tParticleDescs[i].vCurrentColorForce.w);
		XMVectorSetX(vColor, XMVectorGetX(vColor) + m_tParticleDescs[i].vCurrentColor.x);
		XMVectorSetW(vColor, XMVectorGetW(vColor) + m_tParticleDescs[i].vCurrentColor.w);

		XMVectorSetY(vColor, XMVectorGetX(vColor));
		XMVectorSetZ(vColor, XMVectorGetX(vColor));

		XMVectorSetX(vColor, max(m_tEffectParticleDesc.vMinColor.x, min(m_tEffectParticleDesc.vMaxColor.x, XMVectorGetX(vColor))));
		XMVectorSetY(vColor, max(m_tEffectParticleDesc.vMinColor.y, min(m_tEffectParticleDesc.vMaxColor.y, XMVectorGetY(vColor))));
		XMVectorSetZ(vColor, max(m_tEffectParticleDesc.vMinColor.z, min(m_tEffectParticleDesc.vMaxColor.z, XMVectorGetZ(vColor))));
		XMVectorSetW(vColor, max(m_tEffectParticleDesc.vMinColor.w, min(m_tEffectParticleDesc.vMaxColor.w, XMVectorGetW(vColor))));
	}
	else
	{
		_float4 Temp = SMath::Mul_Float4(m_tParticleDescs[i].vTargetColorSpeed, fTimeDelta);
		vColor = XMLoadFloat4(&Temp);
		SMath::Add_Float4(&m_tParticleDescs[i].vCurrentColorForce, SMath::Mul_Float4(m_tParticleDescs[i].vTargetColorForce, fTimeDelta));

		_float4 float4Color;
		ZeroMemory(&float4Color, sizeof(_float4));
		XMStoreFloat4(&float4Color, vColor);

		_float4 Temp2 = SMath::Add_Float4(float4Color, m_tParticleDescs[i].vCurrentColorForce);
		vColor = XMLoadFloat4(&Temp2);
		XMStoreFloat4(&float4Color, vColor);

		_float4 Temp3 = SMath::Add_Float4(float4Color, m_tParticleDescs[i].vCurrentColor);
		vColor = XMLoadFloat4(&Temp3);

		XMVectorSetX(vColor, max(m_tEffectParticleDesc.vMinColor.x, min(m_tEffectParticleDesc.vMaxColor.x, XMVectorGetX(vColor))));
		XMVectorSetY(vColor, max(m_tEffectParticleDesc.vMinColor.y, min(m_tEffectParticleDesc.vMaxColor.y, XMVectorGetY(vColor))));
		XMVectorSetZ(vColor, max(m_tEffectParticleDesc.vMinColor.z, min(m_tEffectParticleDesc.vMaxColor.z, XMVectorGetZ(vColor))));
		XMVectorSetW(vColor, max(m_tEffectParticleDesc.vMinColor.w, min(m_tEffectParticleDesc.vMaxColor.w, XMVectorGetW(vColor))));
	}

	XMStoreFloat4(&m_tParticleDescs[i].vCurrentColor, vColor);
}

void CEffect_Rect::Update_ParticleGlowColor(_float fTimeDelta)
{
	_float4 vColor = SMath::Mul_Float4(m_tEffectParticleDesc.vGlowColorSpeed, fTimeDelta);
	SMath::Add_Float4(&m_vCurrentGlowColorForce, SMath::Mul_Float4(m_tEffectParticleDesc.vGlowColorForce, fTimeDelta));

	SMath::Add_Float4(&vColor, m_vCurrentGlowColorForce);
	SMath::Add_Float4(&vColor, m_vCurrentGlowColor);

	vColor.x = min(1.f, max(0.f, vColor.x));
	vColor.y = min(1.f, max(0.f, vColor.y));
	vColor.z = min(1.f, max(0.f, vColor.z));
	vColor.w = min(1.f, max(0.f, vColor.w));

	m_vCurrentGlowColor = vColor;
}

void CEffect_Rect::Update_ParticleSpriteFrame(const _uint& i, _float fTimeDelta)
{
	if (!m_bStopSprite)
	{
		m_tParticleDescs[i].fCurrentSpriteTime += fTimeDelta;
		if (m_tEffectParticleDesc.fSpriteSpeed <= m_tParticleDescs[i].fCurrentSpriteTime)
		{
			m_tParticleDescs[i].fCurrentSpriteTime = 0.f;

			m_tParticleDescs[i].vSpriteUV.x += (1.f / m_tEffectParticleDesc.iNumFrameX);

			if ((1.f - 1.f / m_tEffectParticleDesc.iNumFrameX) <= m_tParticleDescs[i].vSpriteUV.x &&
				(1.f - 1.f / m_tEffectParticleDesc.iNumFrameY) <= m_tParticleDescs[i].vSpriteUV.y)
			{
				if (m_tEffectParticleDesc.bLoopSprite)
				{
					ZeroMemory(&m_tParticleDescs[i].vSpriteUV, sizeof(_float2));
					return;
				}
				else
				{
					m_tParticleDescs[i].vSpriteUV = { 1.f, 1.f };
					m_bStopSprite = true;
					return;
				}
			}

			if ((1.f - 1.f / m_tEffectParticleDesc.iNumFrameX) <= m_tParticleDescs[i].vSpriteUV.x)
			{
				m_tParticleDescs[i].vSpriteUV.x = 0.f;
				m_tParticleDescs[i].vSpriteUV.y += (1.f / m_tEffectParticleDesc.iNumFrameY);
				return;
			}
		}
	}
}

void CEffect_Rect::Update_ParentTransform()
{
	if ((_int)TRANSFORMTYPE::STATIC != m_tEffectParticleDesc.iFollowTransformType)
	{
		if ((_int)PARTICLETYPE::BILLBOARD == m_tEffectParticleDesc.iParticleType)
		{
			if (m_pParentTransformCom.lock())
				m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix().r[3]);
		}

		if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
		{
			m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());
		}
		else if ((_int)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType)
		{
			if (m_pParentTransformCom.lock())
				m_pTransformCom.lock()->Set_WorldMatrix(m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
		}
	}

}

void CEffect_Rect::Apply_Easing
(
	_float3& vTarget
	, EASING_TYPE eEasingType
	, _vector vStartPoint
	, _vector vTargetPoint
	, _float fElapsedTime
	, _float fTotalTime
)
{
#define MACRO(FunctionName)\
XMStoreFloat3(&vTarget, FunctionName(vStartPoint, vTargetPoint, fElapsedTime, fTotalTime));

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

void CEffect_Rect::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Effect_Particle"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("Max Instance");
			ImGui::SameLine();
			ImGui::DragInt("##Max_Instance", &m_tEffectParticleDesc.iMaxInstance, 1, 0, 9999, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::Separator();

			ImGui::Text("Is Looping");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Looping", &m_tEffectParticleDesc.bLooping);
			ImGui::Separator();
#pragma region Shader Pass
			ImGui::Text("Pass %d : Sprite Image (Discard Black)", PASS_SPRITE_BLACKDISCARD);
			ImGui::Text("Pass %d : Default		(Discard Alpha)", PASS_ALPHADISCARD);
			ImGui::Text("Pass %d : Default		(Discard Black)", PASS_BLACKDISCARD);
			ImGui::Text("Pass %d : Sprite Image (Discard Alpha)", PASS_SPRITE_ALPHADISCARD);
			ImGui::InputInt("Shader Pass", &m_tEffectParticleDesc.iShaderPassIndex);
#pragma endregion
			ImGui::Separator();
#pragma region For. Sprite
			if (
				(PASS_SPRITE_BLACKDISCARD == m_tEffectParticleDesc.iShaderPassIndex)
				||
				(PASS_SPRITE_ALPHADISCARD == m_tEffectParticleDesc.iShaderPassIndex)
				)
			{
				ImGui::Text("Loop Sprite"); ImGui::SameLine();
				ImGui::Checkbox("##LoopSprite", &m_tEffectParticleDesc.bLoopSprite);

				ImGui::InputInt("NumFramesX", &m_tEffectParticleDesc.iNumFrameX);
				ImGui::InputInt("NumFramesY", &m_tEffectParticleDesc.iNumFrameY);
				ImGui::InputFloat("FrameSpeed", &m_tEffectParticleDesc.fSpriteSpeed);
			}
#pragma endregion
			ImGui::Separator();
#pragma region Particle Type
			const char* items[] = { "None", "Outburst", "Attraction", "Billboard" };

			if (ImGui::BeginListBox("ParticleType"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (m_tEffectParticleDesc.iParticleType == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						m_tEffectParticleDesc.iParticleType = n;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
#pragma endregion
			ImGui::Separator();
#pragma region Transform Type
			const char* Transform_items[] = { "Static", "Just Spawn", "Child" };

			if (ImGui::BeginListBox("Transform Type"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(Transform_items); n++)
				{
					const bool is_selected = (m_tEffectParticleDesc.iFollowTransformType == n);
					if (ImGui::Selectable(Transform_items[n], is_selected))
					{
						m_tEffectParticleDesc.iFollowTransformType = n;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
#pragma endregion
			ImGui::Separator();
#pragma region Animation Sync
			ImGui::Text("Sync Animation");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Sync_Animation", &m_tEffectParticleDesc.bSyncAnimation);

			ImGui::InputInt("Sync Animation Key", &m_tEffectParticleDesc.iSyncAnimationKey);
#pragma endregion
			ImGui::Separator();
#pragma region Life Time & Spawns
			ImGui::Text("Init Time");
			ImGui::DragFloat("##Init_Time", &m_tEffectParticleDesc.fInitTime, 0.1f);

			ImGui::Text("Min Spawn Time");
			ImGui::DragFloat("##Min_Spawn_Time", &m_tEffectParticleDesc.fMinSpawnTime, 0.1f);

			ImGui::Text("Max Spawn Time");
			ImGui::DragFloat("##Max_Spawn_Time", &m_tEffectParticleDesc.fMaxSpawnTime, 0.1f);

			ImGui::Text("Min Life Time");
			ImGui::DragFloat("##Min_Life_Time", &m_tEffectParticleDesc.fMinLifeTime, 0.1f);

			ImGui::Text("Max Life Time");
			ImGui::DragFloat("##Max_Life_Time", &m_tEffectParticleDesc.fMaxLifeTime, 0.1f);
			ImGui::Separator();
#pragma endregion
			ImGui::Separator();

			ImGui::Text("Is Move Look"); ImGui::SameLine();
			ImGui::Checkbox("##Is_MoveLook", &m_tEffectParticleDesc.bMoveLook);

			ImGui::Text("Use Gravity"); ImGui::SameLine();
			ImGui::Checkbox("##Use_Gravity", &m_tEffectParticleDesc.bUseGravity);

			ImGui::Text("Gravity Force");
			ImGui::DragFloat3("##Gravity_Force", &m_tEffectParticleDesc.vGravityForce.x, 0.1f, -100.f, 100.f, "%.5f");

			ImGui::Separator();
#pragma region Positions
			ImGui::Text("Follow Bone"); ImGui::SameLine();
			ImGui::Checkbox("##Is_Boner", &m_tEffectParticleDesc.bBoner);

			if (m_tEffectParticleDesc.bBoner)
			{
				if (ImGui::Button("Get Bone List"))
				{
					m_pParentTransformCom = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Component<CTransform>().lock();
					m_AllBoneNames = GET_SINGLE(CWindow_AnimationModelView)->Get_AllBoneNames();
				}

				if (0 == m_AllBoneNames.size())
					ImGui::Text("No Bones!");
				else
				{
					if (ImGui::BeginListBox("Bone List 2"))
					{
						for (_int n{ 0 }; n < m_AllBoneNames.size(); n++)
						{
							const _bool is_selected = (m_iCurrentBoneIndex == n);
							if (ImGui::Selectable(m_AllBoneNames[n].c_str(), is_selected))
								m_strBoneName = m_AllBoneNames[n];

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}
				}

				if (ImGui::Button("Bind to Bone"))
				{
					m_pBoneNode = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Find_BoneNode(m_strBoneName);
					if (nullptr == m_pBoneNode.lock())
					{
						MSG_BOX("Invalid Bone Name!");
						assert(0);
					}
				}

				if (m_pBoneNode.lock())
				{
					ImGui::Text("Binded to Bone : ");
					ImGui::Text(m_pBoneNode.lock()->Get_Name());
				}

			}

			ImGui::Text("Use Easing Position"); ImGui::SameLine();
			ImGui::Checkbox("##Is_Easing_Position", &m_tEffectParticleDesc.bEasingPosition);

#pragma region Spawn Position
			ImGui::Text("Min = Max"); ImGui::SameLine();
			static _bool bIsMinMaxSame_SpawnPosition = false;
			ImGui::Checkbox("##Is_MinMaxSame_SpawnPosition", &bIsMinMaxSame_SpawnPosition);

			ImGui::Text("Min Spawn Position");
			ImGui::DragFloat3("##Min_Spawn_Position", &m_tEffectParticleDesc.vMinSpawnPosition.x, 0.1f);

			if (bIsMinMaxSame_SpawnPosition)
				m_tEffectParticleDesc.vMaxSpawnPosition = m_tEffectParticleDesc.vMinSpawnPosition;

			ImGui::Text("Max Spawn Position");
			ImGui::DragFloat3("##Max_Spawn_Position", &m_tEffectParticleDesc.vMaxSpawnPosition.x, 0.1f);
#pragma endregion

#pragma region Spawn Offset Direction
			ImGui::Text("Min = Max"); ImGui::SameLine();
			static _bool bIsMinMaxSame_OffsetDirection = false;
			ImGui::Checkbox("##Is_MinMaxSame_OffsetDirection", &bIsMinMaxSame_OffsetDirection);

			ImGui::Text("Min Offset Direction");
			ImGui::DragFloat3("##Min_Offset_Direction", &m_tEffectParticleDesc.vMinSpawnOffsetDirection.x, 0.1f);

			if (bIsMinMaxSame_OffsetDirection)
				m_tEffectParticleDesc.vMaxSpawnOffsetDirection = m_tEffectParticleDesc.vMinSpawnOffsetDirection;

			ImGui::Text("Max Offset Direction");
			ImGui::DragFloat3("##Max_Offset_Direction", &m_tEffectParticleDesc.vMaxSpawnOffsetDirection.x, 0.1f);
#pragma endregion

#pragma region Spawn Offset Range
			ImGui::Text("Min = Max"); ImGui::SameLine();
			static _bool bIsMinMaxSame_OffsetRange = false;
			ImGui::Checkbox("##Is_MinMaxSame_OffsetRange", &bIsMinMaxSame_OffsetRange);

			ImGui::Text("Min Offset Range");
			ImGui::DragFloat3("##Min_Offset_Range", &m_tEffectParticleDesc.vMinSpawnOffsetRange.x, 0.1f);

			if (bIsMinMaxSame_OffsetRange)
				m_tEffectParticleDesc.vMaxSpawnOffsetRange = m_tEffectParticleDesc.vMinSpawnOffsetRange;

			ImGui::Text("Max Offset Range");
			ImGui::DragFloat3("##Max_Offset_Range", &m_tEffectParticleDesc.vMaxSpawnOffsetRange.x, 0.1f);
#pragma endregion

#pragma endregion
			ImGui::Separator();
#pragma region Speed & Force
			if (!m_tEffectParticleDesc.bEasingPosition)
			{
				ImGui::Text("Speed");
				ImGui::NewLine();

				ImGui::Text("Apply Easing"); ImGui::SameLine();
				ImGui::Checkbox("##Is_Easing_Speed", &m_tEffectParticleDesc.bEasingSpeed);

				if (m_tEffectParticleDesc.bEasingSpeed)
				{
#pragma region Easing
					// open easing list
					const char* EasingItems[] =
					{
						"Linear"   ,
						"QuadIn"   , "QuadOut"	 , "QuadInOut"	 ,
						"CubicIn"  , "CubicOut"	 , "CubicInOut"	 ,
						"QuartIn"  , "QuartOut"	 , "QuartInOut"	 ,
						"QuintIn"  , "QuintOut"	 , "QuintInOut"	 ,
						"SineIn"   , "SineOut"	 , "SineInOut"	 ,
						"ExpoIn"   , "ExpoOut"	 , "ExpoInOut"	 ,
						"CircIn"   , "CircOut"	 , "CircInOut"	 ,
						"ElasticIn", "ElasticOut", "ElasticInOut",
						"BounceIn" , "BounceOut"
					};

					if (ImGui::BeginListBox("Speed Easing Type"))
					{
						for (int n(0); n < IM_ARRAYSIZE(EasingItems); n++)
						{
							const bool is_selected = (m_tEffectParticleDesc.iSpeedEasingType == n);
							if (ImGui::Selectable(EasingItems[n], is_selected))
								m_tEffectParticleDesc.iSpeedEasingType = n;

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}

					ImGui::Text("Total Easing Time"); ImGui::SameLine();
					ImGui::DragFloat("##Speed_Total_Easing_Time", &m_tEffectParticleDesc.fSpeedEasingTotalTime, 0.01f, -100.f, 100.f, "%.5f");
#pragma endregion
				}
				else
				{
#pragma region Start Speed
					static _bool bIsMinMaxSame_StartSpeed;
					ImGui::Text("Min = Max"); ImGui::SameLine();
					ImGui::Checkbox("##Is_MinMaxSame_StartSpeed", &bIsMinMaxSame_StartSpeed);

					ImGui::Text("Min Speed");
					ImGui::DragFloat3("##Min_Speed", &m_tEffectParticleDesc.vMinSpeed.x, 0.1f);

					if (bIsMinMaxSame_StartSpeed)
						m_tEffectParticleDesc.vMaxSpeed = m_tEffectParticleDesc.vMinSpeed;

					ImGui::Text("Max Speed");
					ImGui::DragFloat3("##Max_Speed", &m_tEffectParticleDesc.vMaxSpeed.x, 0.1f);
#pragma endregion

#pragma region Speed Force
					static _bool bIsMinMaxSame_SpeedForce;
					ImGui::Text("Min = Max"); ImGui::SameLine();
					ImGui::Checkbox("##Is_MinMaxSame_SpeedForce", &bIsMinMaxSame_SpeedForce);

					ImGui::Text("Min Force");
					ImGui::DragFloat3("##Min_Speed_Force", &m_tEffectParticleDesc.vMinSpeedForce.x, 0.1f);

					if (bIsMinMaxSame_SpeedForce)
						m_tEffectParticleDesc.vMaxSpeedForce = m_tEffectParticleDesc.vMinSpeedForce;

					ImGui::Text("Max Force");
					ImGui::DragFloat3("##Max_Speed_Force", &m_tEffectParticleDesc.vMaxSpeedForce.x, 0.1f);
#pragma endregion
				}
#pragma region Speed Limit
				static _bool bIsMinMaxSame_SpeedLimit;
				ImGui::Text("Min = Max"); ImGui::SameLine();
				ImGui::Checkbox("##Is_MinMaxSame_SpeedLimit", &bIsMinMaxSame_SpeedLimit);

				ImGui::Text("Min Limit Speed");
				ImGui::DragFloat3("##Min_Limit_Speed", &m_tEffectParticleDesc.vMinLimitSpeed.x, 0.1f, -100.f, 100.f, "%.5f");

				if (bIsMinMaxSame_SpeedLimit)
					m_tEffectParticleDesc.vMaxLimitSpeed = m_tEffectParticleDesc.vMinLimitSpeed;

				ImGui::Text("Max Limit Speed");
				ImGui::DragFloat3("##Max_Limit_Speed", &m_tEffectParticleDesc.vMaxLimitSpeed.x, 0.1f, -100.f, 100.f, "%.5f");
#pragma endregion
			}
			else
			{
#pragma region Easing
				// open easing list
				const char* EasingItems[] =
				{
					"Linear"   ,
					"QuadIn"   , "QuadOut"	 , "QuadInOut"	 ,
					"CubicIn"  , "CubicOut"	 , "CubicInOut"	 ,
					"QuartIn"  , "QuartOut"	 , "QuartInOut"	 ,
					"QuintIn"  , "QuintOut"	 , "QuintInOut"	 ,
					"SineIn"   , "SineOut"	 , "SineInOut"	 ,
					"ExpoIn"   , "ExpoOut"	 , "ExpoInOut"	 ,
					"CircIn"   , "CircOut"	 , "CircInOut"	 ,
					"ElasticIn", "ElasticOut", "ElasticInOut",
					"BounceIn" , "BounceOut"
				};

				if (ImGui::BeginListBox("Position Easing Type"))
				{
					for (int n = 0; n < IM_ARRAYSIZE(EasingItems); n++)
					{
						const bool is_selected = (m_tEffectParticleDesc.iSpeedEasingType == n);
						if (ImGui::Selectable(EasingItems[n], is_selected))
						{
							m_tEffectParticleDesc.iSpeedEasingType = n;
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				ImGui::Text("Position Easing Time"); ImGui::SameLine();
				ImGui::DragFloat("##Position_Total_Easing_Time", &m_tEffectParticleDesc.fSpeedEasingTotalTime, 0.01f, -100.f, 100.f, "%.5f");

				ImGui::Text("Min Goal Offset Position");
				ImGui::DragFloat3("##Min_Goal_Offset_Position", &m_tEffectParticleDesc.vMinSpeed.x, 0.1f);

				ImGui::Text("Max Goal Offset Position");
				ImGui::DragFloat3("##Max_Goal_Offset_Position", &m_tEffectParticleDesc.vMaxSpeed.x, 0.1f);
			}
#pragma endregion
			ImGui::Separator();

#pragma region Rotation
			if (_int(PARTICLETYPE::OUTBURST) != m_tEffectParticleDesc.iParticleType)
			{
				ImGui::Text("Rotation");
				ImGui::NewLine();

				ImGui::Text("Apply Easing"); ImGui::SameLine();
				ImGui::Checkbox("##Is_Easing_Rotation", &m_tEffectParticleDesc.bEasingRotation);

				if (m_tEffectParticleDesc.bEasingRotation)
				{
#pragma region Easing
					// open easing list
					const char* EasingItems[] =
					{
						"Linear"   ,
						"QuadIn"   , "QuadOut"	 , "QuadInOut"	 ,
						"CubicIn"  , "CubicOut"	 , "CubicInOut"	 ,
						"QuartIn"  , "QuartOut"	 , "QuartInOut"	 ,
						"QuintIn"  , "QuintOut"	 , "QuintInOut"	 ,
						"SineIn"   , "SineOut"	 , "SineInOut"	 ,
						"ExpoIn"   , "ExpoOut"	 , "ExpoInOut"	 ,
						"CircIn"   , "CircOut"	 , "CircInOut"	 ,
						"ElasticIn", "ElasticOut", "ElasticInOut",
						"BounceIn" , "BounceOut"
					};

					if (ImGui::BeginListBox("Rotation Easing Type"))
					{
						for (int n(0); n < IM_ARRAYSIZE(EasingItems); n++)
						{
							const bool is_selected = (m_tEffectParticleDesc.iRotationEasingType == n);
							if (ImGui::Selectable(EasingItems[n], is_selected))
							{
								m_tEffectParticleDesc.iRotationEasingType = n;
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndListBox();
					}

					ImGui::Text("Total Easing Time"); ImGui::SameLine();
					ImGui::DragFloat("##Rotation_Total_Easing_Time", &m_tEffectParticleDesc.fRotationEasingTotalTime, 0.01f, -100.f, 100.f, "%.5f");
#pragma endregion
				}
				else
				{
#pragma region Start Rotation
					static _bool bIsMinMaxSame_StartRotation;
					ImGui::Text("Min = Max"); ImGui::SameLine();
					ImGui::Checkbox("##Is_MinMaxSame_StartRotation", &bIsMinMaxSame_StartRotation);

					ImGui::Text("Min Start Rotation");
					ImGui::DragFloat3("##Min_Start_Rotation", &m_tEffectParticleDesc.vMinStartRotation.x, 0.1f);

					if (bIsMinMaxSame_StartRotation)
						m_tEffectParticleDesc.vMaxStartRotation = m_tEffectParticleDesc.vMinStartRotation;

					ImGui::Text("Max Start Rotation");
					ImGui::DragFloat3("##Max_Start_Rotation", &m_tEffectParticleDesc.vMaxStartRotation.x, 0.1f);
#pragma endregion

#pragma region Rotation Speed
					static _bool bIsMinMaxSame_RotationSpeed;
					ImGui::Text("Min = Max"); ImGui::SameLine();
					ImGui::Checkbox("##Is_MinMaxSame_RotationSpeed", &bIsMinMaxSame_RotationSpeed);

					ImGui::Text("Min Rotation Speed");
					ImGui::DragFloat3("##Min_Rotation_Speed", &m_tEffectParticleDesc.vMinRotationSpeed.x, 0.1f);

					if (bIsMinMaxSame_RotationSpeed)
						m_tEffectParticleDesc.vMaxRotationSpeed = m_tEffectParticleDesc.vMinRotationSpeed;

					ImGui::Text("Max Rotation Speed");
					ImGui::DragFloat3("##Max_Rotation_Speed", &m_tEffectParticleDesc.vMaxRotationSpeed.x, 0.1f);
#pragma endregion

#pragma region Rotation Force
					static _bool bIsMinMaxSame_RotationForce;
					ImGui::Text("Min = Max"); ImGui::SameLine();
					ImGui::Checkbox("##Is_MinMaxSame_RotationForce", &bIsMinMaxSame_RotationForce);

					ImGui::Text("Min Rotation Force");
					ImGui::DragFloat3("##Min_Rotation_Force", &m_tEffectParticleDesc.vMinRotationForce.x, 0.1f);

					if (bIsMinMaxSame_RotationForce)
						m_tEffectParticleDesc.vMaxRotationForce = m_tEffectParticleDesc.vMinRotationForce;

					ImGui::Text("Max Rotation Force");
					ImGui::DragFloat3("##Max_Rotation_Force", &m_tEffectParticleDesc.vMaxRotationForce.x, 0.1f);
#pragma endregion
				}
#pragma region Rotation Limit
				static _bool bIsMinMaxSame_RotationLimit;
				ImGui::Text("Min = Max"); ImGui::SameLine();
				ImGui::Checkbox("##Is_MinMaxSame_RotationLimit", &bIsMinMaxSame_RotationLimit);

				ImGui::Text("Min Limit Rotation");
				ImGui::DragFloat3("##Min_Limit_Rotation", &m_tEffectParticleDesc.vMinLimitRotation.x, 0.1f);

				if (bIsMinMaxSame_RotationLimit)
					m_tEffectParticleDesc.vMaxLimitRotation = m_tEffectParticleDesc.vMinLimitRotation;

				ImGui::Text("Max Limit Rotation");
				ImGui::DragFloat3("##Max_Limit_Rotation", &m_tEffectParticleDesc.vMaxLimitRotation.x, 0.1f);
#pragma endregion
			}

#pragma endregion
			ImGui::Separator();
#pragma region Scale
			ImGui::Text("Scale");
			ImGui::NewLine();

			ImGui::Text("Apply Easing"); ImGui::SameLine();
			ImGui::Checkbox("##Is_Easing_Scale", &m_tEffectParticleDesc.bEasingScale);

			if (m_tEffectParticleDesc.bEasingScale)
			{
#pragma region Easing
				// open easing list
				const char* EasingItems[] =
				{
					"Linear"   ,
					"QuadIn"   , "QuadOut"	 , "QuadInOut"	 ,
					"CubicIn"  , "CubicOut"	 , "CubicInOut"	 ,
					"QuartIn"  , "QuartOut"	 , "QuartInOut"	 ,
					"QuintIn"  , "QuintOut"	 , "QuintInOut"	 ,
					"SineIn"   , "SineOut"	 , "SineInOut"	 ,
					"ExpoIn"   , "ExpoOut"	 , "ExpoInOut"	 ,
					"CircIn"   , "CircOut"	 , "CircInOut"	 ,
					"ElasticIn", "ElasticOut", "ElasticInOut",
					"BounceIn" , "BounceOut"
				};

				if (ImGui::BeginListBox("Scale Easing Type"))
				{
					for (int n = 0; n < IM_ARRAYSIZE(EasingItems); n++)
					{
						const bool is_selected = (m_tEffectParticleDesc.iScaleEasingType == n);

						if (ImGui::Selectable(EasingItems[n], is_selected))
							m_tEffectParticleDesc.iScaleEasingType = n;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}

				ImGui::Text("Total Easing Time"); ImGui::SameLine();
				ImGui::DragFloat("##Scale_Total_Easing_Time", &m_tEffectParticleDesc.fScaleEasingTotalTime, 0.01f, 0.f, 0.f, "%.5f");
#pragma endregion
			}
			else
			{
#pragma region Start Scale
				static _bool bIsMinMaxSame_StartScale;
				ImGui::Text("Min = Max"); ImGui::SameLine();
				ImGui::Checkbox("##Is_MinMaxSame_StartScale", &bIsMinMaxSame_StartScale);

				ImGui::Text("Min Start Scale");
				ImGui::DragFloat3("##Min_Start_Scale", &m_tEffectParticleDesc.vMinStartScale.x, 0.1f);

				if (bIsMinMaxSame_StartScale)
					m_tEffectParticleDesc.vMaxStartScale = m_tEffectParticleDesc.vMinStartScale;

				ImGui::Text("Max Start Scale");
				ImGui::DragFloat3("##Max_Start_Scale", &m_tEffectParticleDesc.vMaxStartScale.x, 0.1f);
#pragma endregion

#pragma region Scale Speed
				static _bool bIsMinMaxSame_ScaleSpeed;
				ImGui::Text("Min = Max"); ImGui::SameLine();
				ImGui::Checkbox("##Is_MinMaxSame_ScaleSpeed", &bIsMinMaxSame_ScaleSpeed);

				ImGui::Text("Min Scale Speed");
				ImGui::DragFloat3("##Min_Scale_Speed", &m_tEffectParticleDesc.vMinScaleSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

				if (bIsMinMaxSame_ScaleSpeed)
					m_tEffectParticleDesc.vMaxScaleSpeed = m_tEffectParticleDesc.vMinScaleSpeed;

				ImGui::Text("Max Scale Speed");
				ImGui::DragFloat3("##Max_Scale_Speed", &m_tEffectParticleDesc.vMaxScaleSpeed.x, 0.1f, 0.f, 0.f, "%.5f");
#pragma endregion

#pragma region Scale Force
				static _bool bIsMinMaxSame_ScaleForce;
				ImGui::Text("Min = Max"); ImGui::SameLine();
				ImGui::Checkbox("##Is_MinMaxSame_ScaleForce", &bIsMinMaxSame_ScaleForce);

				ImGui::Text("Min Scale Force");
				ImGui::DragFloat3("##Min_Scale_Force", &m_tEffectParticleDesc.vMinScaleForce.x, 0.1f, 0.f, 0.f, "%.5f");

				if (bIsMinMaxSame_ScaleForce)
					m_tEffectParticleDesc.vMaxScaleForce = m_tEffectParticleDesc.vMinScaleForce;

				ImGui::Text("Max Scale Force");
				ImGui::DragFloat3("##Max_Scale_Force", &m_tEffectParticleDesc.vMaxScaleForce.x, 0.1f, 0.f, 0.f, "%.5f");
#pragma endregion
			}
#pragma region Scale Limit
			static _bool bIsMinMaxSame_ScaleLimit;
			ImGui::Text("Min = Max"); ImGui::SameLine();
			ImGui::Checkbox("##Is_MinMaxSame_ScaleLimit", &bIsMinMaxSame_ScaleLimit);

			ImGui::Text("Min Limit Scale");
			ImGui::DragFloat3("##Min_Limit_Scale", &m_tEffectParticleDesc.vMinLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");

			if (bIsMinMaxSame_ScaleLimit)
				m_tEffectParticleDesc.vMaxLimitScale = m_tEffectParticleDesc.vMinLimitScale;

			ImGui::Text("Max Limit Scale");
			ImGui::DragFloat3("##Max_Limit_Scale", &m_tEffectParticleDesc.vMaxLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");
#pragma endregion
#pragma endregion

			ImGui::Separator();
			ImGui::Separator();

#pragma region Colors
			ImGui::Text("Discard Ratio");
			ImGui::DragFloat("##Discard_Ratio", &m_tEffectParticleDesc.fDiscardRatio, 0.01f, 0.f, 3.f);

			ImGui::Text("Is Gray Only Use Red");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Gray_Only_Use_Red", &m_tEffectParticleDesc.IsGrayOnlyUseRed);

#pragma region Start Color
			static _bool bIsMinMaxSame_StartColor;
			ImGui::Text("Min = Max"); ImGui::SameLine();
			ImGui::Checkbox("##Is_MinMaxSame_StartColor", &bIsMinMaxSame_StartColor);

			ImGui::Text("Min Start Color");
			ImGui::DragFloat4("##Min_Start_Color", &m_tEffectParticleDesc.vMinStartColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

			if (bIsMinMaxSame_StartColor)
				m_tEffectParticleDesc.vMaxStartColor = m_tEffectParticleDesc.vMinStartColor;

			ImGui::Text("Max Start Color");
			ImGui::DragFloat4("##Max_Start_Color", &m_tEffectParticleDesc.vMaxStartColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
#pragma endregion

#pragma region Color Speed 
			static _bool bIsMinMaxSame_ColorSpeed;
			ImGui::Text("Min = Max"); ImGui::SameLine();
			ImGui::Checkbox("##Is_MinMaxSame_ColorSpeed", &bIsMinMaxSame_ColorSpeed);

			ImGui::Text("Min Color Speed");
			ImGui::DragFloat4("##Min_Color_Speed", &m_tEffectParticleDesc.vMinColorSpeed.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

			if (bIsMinMaxSame_ColorSpeed)
				m_tEffectParticleDesc.vMaxColorSpeed = m_tEffectParticleDesc.vMinColorSpeed;

			ImGui::Text("Max Color Speed");
			ImGui::DragFloat4("##Max_Color_Speed", &m_tEffectParticleDesc.vMaxColorSpeed.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
#pragma endregion

#pragma region Color Force
			static _bool bIsMinMaxSame_ColorForce;
			ImGui::Text("Min = Max"); ImGui::SameLine();
			ImGui::Checkbox("##Is_MinMaxSame_ColorForce", &bIsMinMaxSame_ColorForce);

			ImGui::Text("Min Color Force");
			ImGui::DragFloat4("##Min_Color_Force", &m_tEffectParticleDesc.vMinColorForce.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

			if (bIsMinMaxSame_ColorForce)
				m_tEffectParticleDesc.vMaxColorForce = m_tEffectParticleDesc.vMinColorForce;

			ImGui::Text("Max Color Force");
			ImGui::DragFloat4("##Max_Color_Force", &m_tEffectParticleDesc.vMaxColorForce.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
#pragma endregion

#pragma region Color Limit
			static _bool bIsMinMaxSame_ColorLimit;
			ImGui::Text("Min = Max"); ImGui::SameLine();
			ImGui::Checkbox("##Is_MinMaxSame_ColorLimit", &bIsMinMaxSame_ColorLimit);

			ImGui::Text("Min Limit Color");
			ImGui::DragFloat4("##Min_Color", &m_tEffectParticleDesc.vMinColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

			if (bIsMinMaxSame_ColorLimit)
				m_tEffectParticleDesc.vMaxColor = m_tEffectParticleDesc.vMinColor;

			ImGui::Text("Max Limit Color");
			ImGui::DragFloat4("##Max_Color", &m_tEffectParticleDesc.vMaxColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);
#pragma endregion

#pragma endregion
			ImGui::Separator();
#pragma region Textures
#pragma region Diffuse Texture
			ImGui::Text("Diffuse Start UV");
			ImGui::DragFloat2("##Diffuse_Start_UV", &m_tEffectParticleDesc.vDiffuseStartUV.x, 0.1f);

			ImGui::InputInt("Diffuse Index", &m_tEffectParticleDesc.iDiffuseIndex);

			ImGui::Text("Diffuse UV Speed");
			ImGui::DragFloat2("##Diffuse_UV_Speed", &m_tEffectParticleDesc.vDiffuseUVSpeed.x, 0.1f);

			ImGui::Text("Diffuse UV Force");
			ImGui::DragFloat2("##Diffuse UV_Force", &m_tEffectParticleDesc.vDiffuseUVForce.x, 0.1f);

			ImGui::Text("Diffuse UV Max");
			ImGui::DragFloat2("##Diffuse_UV_Max", &m_tEffectParticleDesc.vDiffuseUVMax.x, 0.1f);
#pragma endregion
			ImGui::Separator();
#pragma region Mask Texture
			ImGui::Text("Mask Start UV");
			ImGui::DragFloat2("##Mask_Start_UV", &m_tEffectParticleDesc.vMaskStartUV.x, 0.1f);

			ImGui::InputInt("Mask Index", &m_tEffectParticleDesc.iMaskIndex);

			ImGui::Text("Mask UV Speed");
			ImGui::DragFloat2("##Mask_UV_Speed", &m_tEffectParticleDesc.vMaskUVSpeed.x, 0.1f);

			ImGui::Text("Mask UV Force");
			ImGui::DragFloat2("##Mask UV_Force", &m_tEffectParticleDesc.vMaskUVForce.x, 0.1f);

			ImGui::Text("Mask UV Max");
			ImGui::DragFloat2("##Mask_UV_Max", &m_tEffectParticleDesc.vMaskUVMax.x, 0.1f);
#pragma endregion
			ImGui::Separator();
#pragma region Noise Texture
			ImGui::Text("Noise Start UV");
			ImGui::DragFloat2("##Noise_Start_UV", &m_tEffectParticleDesc.vNoiseStartUV.x, 0.1f);

			ImGui::InputInt("Noise Index", &m_tEffectParticleDesc.iNoiseIndex);

			ImGui::Text("Noise UV Speed");
			ImGui::DragFloat2("##Noise_UV_Speed", &m_tEffectParticleDesc.vNoiseUVSpeed.x, 0.1f);

			ImGui::Text("Noise UV Force");
			ImGui::DragFloat2("##Noise UV_Force", &m_tEffectParticleDesc.vNoiseUVForce.x, 0.1f);

			ImGui::Text("Noise UV Max");
			ImGui::DragFloat2("##Noise_UV_Max", &m_tEffectParticleDesc.vNoiseUVMax.x, 0.1f);
#pragma endregion
#pragma endregion
			ImGui::Separator();
#pragma region Bloom & Glow
			ImGui::Text("Bloom");
			ImGui::SameLine();
			ImGui::Checkbox("##Bloom", &m_tEffectParticleDesc.bBloom);

			ImGui::Text("Glow");
			ImGui::SameLine();
			ImGui::Checkbox("##Glow", &m_tEffectParticleDesc.bGlow);

			ImGui::Text("Start Glow Color");
			ImGui::DragFloat4("##Start_Glow_Color", &m_tEffectParticleDesc.vStartGlowColor.x, 0.01f);

			ImGui::Text("Glow Color Speed ");
			ImGui::DragFloat4("##Glow_Color_Speed", &m_tEffectParticleDesc.vGlowColorSpeed.x, 0.01f);

			ImGui::Text("Glow Color Force");
			ImGui::DragFloat4("##Glow_Color_Force", &m_tEffectParticleDesc.vGlowColorForce.x, 0.01f);
			ImGui::Separator();
#pragma endregion
		}
	}
}

void CEffect_Rect::Free()
{
}

void CEffect_Rect::OnChangeAnimationKey(const _uint& In_Key)
{
	if (m_tEffectParticleDesc.iSyncAnimationKey != (_int)In_Key)
		return;

	Reset_Effect(weak_ptr<CTransform>());
}

#undef PASS_SPRITE_BLACKDISCARD
#undef PASS_ALPHADISCARD
#undef PASS_BLACKDISCARD
#undef PASS_SPRITE_ALPHADISCARD