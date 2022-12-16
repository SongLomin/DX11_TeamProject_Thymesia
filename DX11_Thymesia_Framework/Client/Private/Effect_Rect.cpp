#include "stdafx.h"
#include "Effect_Rect.h"
#include "Client_Components.h"
#include "Window_AnimationModelView.h"
#include "Window_EffectHierarchyView.h"
#include "PreViewAnimationModel.h"

#include "Easing_Utillity.h"
#include "Model.h"
#include "BoneNode.h"

#include "Level.h"

#include "GameManager.h"

#ifdef _JOJO_EFFECT_TOOL_
#include "JoJoParticleShaderManager.h"
#endif // _JOJO_EFFECT_TOOL_

#define PASS_SPRITE_BLACKDISCARD				0
#define PASS_ALPHADISCARD						1
#define PASS_BLACKDISCARD						2
#define PASS_SPRITE_ALPHADISCARD				3

#define PASS_ALPHADISCARD_SOFT					4
#define PASS_BLACKDISCARD_SOFT					5
#define PASS_SPRITE_ALPHADISCARD_SOFT			6
#define PASS_SPRITE_BLACKDISCARD_SOFT			7

#define PASS_SPRITE_BLACKDISCARD_SPECIAL		8
#define PASS_ALPHADISCARD_SPECIAL				9
#define PASS_BLACKDISCARD_SPECIAL				10
#define PASS_SPRITE_ALPHADISCARD_SPECIAL		11

#define PASS_ALPHADISCARD_SOFT_SPECIAL			12
#define PASS_BLACKDISCARD_SOFT_SPECIAL			13
#define PASS_SPRITE_ALPHADISCARD_SOFT_SPECIAL	14
#define PASS_SPRITE_BLACKDISCARD_SOFT_SPECIAL	15

GAMECLASS_C(CEffect_Rect)
CLONE_C(CEffect_Rect, CGameObject)

#ifdef _DEBUG
#ifdef _JOJO_EFFECT_TOOL_
const _int CEffect_Rect::m_iScaleType_None = 0;
const _int CEffect_Rect::m_iScaleType_Square = 1;
const _int CEffect_Rect::m_iScaleType_Ratio = 2;
#endif // _JOJO_EFFECT_TOOL_
#endif // _DEBUG

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

	m_tEffectParticleDesc.fMinLifeTime = 1.f;

	m_tEffectParticleDesc.vMinStartScale = { 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartScale = { 1.f, 1.f };
	m_tEffectParticleDesc.vMaxLimitScale = { 1.f, 1.f };

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

	m_bFinish		= false;
	m_bStopParticle = false;
	return S_OK;
}

HRESULT CEffect_Rect::Initialize(void* pArg)
{
	m_eRenderGroup = RENDERGROUP::RENDER_ALPHABLEND;

	__super::Initialize(pArg);

	m_pVIBuffer = Add_Component<CVIBuffer_Rect_Instance>();

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxInstance"), VTXTEXCOLOR_INSTANCE_DECLARATION::Element, VTXTEXCOLOR_INSTANCE_DECLARATION::iNumElements);

	m_pColorDiffuseTextureCom = Add_Component<CTexture>();
	m_pColorDiffuseTextureCom.lock()->Use_Texture("UVColorDiffuse");

	m_pMaskTextureCom = Add_Component<CTexture>();
	m_pMaskTextureCom.lock()->Use_Texture("UVMask");

	m_pNoiseTextureCom = Add_Component<CTexture>();
	m_pNoiseTextureCom.lock()->Use_Texture("UVNoise");
	Set_Enable(false);

#ifdef _USE_THREAD_
	CGameObject::Use_Thread(THREAD_TYPE::TICK);
#endif // _USE_THREAD_

	return S_OK;
}

void CEffect_Rect::Tick(_float fTimeDelta)
{
	CGameObject::CallBack_Tick(fTimeDelta);

#ifdef _DEBUG
	if (m_bResetTrigger && m_pParentTransformCom.lock())
	{
		ReBake_EditParticle();
		Reset_Effect(m_pParentTransformCom.lock());
		m_bResetTrigger = false;
	}
#endif // _DEBUG

	if (m_pVIBuffer.lock()->Get_InstanceCount() != m_tEffectParticleDesc.iMaxInstance)
		Reset_Instance(m_tEffectParticleDesc.iMaxInstance);

	Play(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex));
}

void CEffect_Rect::LateTick(_float fTimeDelta)
{
	m_pVIBuffer.lock()->Update(m_tParticleDescs, ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType));
	__super::LateTick(fTimeDelta);

	if (Check_DisableAllParticle())
		Set_Enable(false);
}

HRESULT CEffect_Rect::Render(ID3D11DeviceContext* pDeviceContext)
{
	SetUp_ShaderResource();
	__super::Render(pDeviceContext);
	m_pShaderCom.lock()->Begin(m_tEffectParticleDesc.iShaderPassIndex, pDeviceContext);
	m_pVIBuffer.lock()->Render(pDeviceContext);
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

	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner))
	{
#ifdef _DEBUG
		if (pParentTransform.lock())
		{
			m_pParentModel = pParentTransform.lock()->Get_Owner().lock()->Get_Component<CModel>();
			m_pBoneNode = m_pParentModel.lock()->Find_BoneNode(m_strBoneName);
		}
#else // _DEBUG
		m_pParentModel = pParentTransform.lock()->Get_Owner().lock()->Get_Component<CModel>();
		m_pBoneNode = m_pParentModel.lock()->Find_BoneNode(m_strBoneName);
#endif // _DEBUG
	}

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
		m_pParentTransformCom = pParentTransform;
	}
	else
	{
		if (m_pBoneNode.lock())
		{
			_float4x4 TempMat(m_pParentModel.lock()->Get_TransformationMatrix());
			_matrix ModelTranMat(XMLoadFloat4x4(&TempMat));
			_matrix BoneMatrix(m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat);

			BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
			BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
			BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

#ifdef _DEBUG
			if (pParentTransform.lock())
				m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * pParentTransform.lock()->Get_UnScaledWorldMatrix());
#else _DEBUG
			m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * pParentTransform.lock()->Get_UnScaledWorldMatrix());
#endif // _DEBUG
		}
		else
		{
#ifdef _DEBUG
			if (pParentTransform.lock())
				m_pTransformCom.lock()->Set_WorldMatrix(pParentTransform.lock()->Get_UnScaledWorldMatrix());
#else // _DEBUG
			m_pTransformCom.lock()->Set_WorldMatrix(pParentTransform.lock()->Get_UnScaledWorldMatrix());
#endif // _DEBUG
		}
	}

	Update_ParentTransform();
	Reset_ParticleDescs();
}

void CEffect_Rect::SetUp_ShaderResource()
{
#pragma region Base
	if ((_uint)TRANSFORMTYPE::STATIC != m_tEffectParticleDesc.iFollowTransformType)
		Update_ParentTransform();

	_matrix WorldMatrix(XMMatrixIdentity()), BoneMatrix(XMMatrixIdentity());

	if (m_pBoneNode.lock() && ((_uint)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType))
	{
		_float4x4 TempMat(m_pParentModel.lock()->Get_TransformationMatrix());
		_matrix ModelTranMat(XMLoadFloat4x4(&TempMat));
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

	m_pShaderCom.lock()->Set_RawValue("g_bXInverse", &m_tEffectParticleDesc.bDiffuseXInverse, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bYInverse", &m_tEffectParticleDesc.bDiffuseYInverse, sizeof(_bool));

	m_pShaderCom.lock()->Set_ShaderResourceView("g_DepthTexture", GAMEINSTANCE->Get_RenderTarget_SRV(TEXT("Target_Depth")));

	m_pShaderCom.lock()->Set_RawValue("g_vDiffuseUV", &m_vDiffuseCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vMaskUV", &m_vMaskCurrentUV, sizeof(_float2));
	m_pShaderCom.lock()->Set_RawValue("g_vNoiseUV", &m_vNoiseCurrentUV, sizeof(_float2));

	// For. Sprite
	m_pShaderCom.lock()->Set_RawValue("g_iNumFrameX", &m_tEffectParticleDesc.iNumFrameX, sizeof(_uint));
	m_pShaderCom.lock()->Set_RawValue("g_iNumFrameY", &m_tEffectParticleDesc.iNumFrameY, sizeof(_uint));
#pragma endregion

#pragma region Billboard
	_vector vCamDir(GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamDirection", &vCamDir, sizeof(_vector));

	_bool isBillboard(m_tEffectParticleDesc.iParticleType == (_int)PARTICLETYPE::BILLBOARD);
	m_pShaderCom.lock()->Set_RawValue("g_bBillboard", &isBillboard, sizeof(_bool));	
#pragma endregion

#pragma region Bloom & Glow
	m_pShaderCom.lock()->Set_RawValue("g_bBloom",     &m_tEffectParticleDesc.bBloom, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bGlow",      &m_tEffectParticleDesc.bGlow,  sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &m_vCurrentGlowColor,          sizeof(_float4));
#pragma endregion
}

void CEffect_Rect::Write_EffectJson(json& Out_Json)
{
	Out_Json["Name"] = m_szEffectName;

	Out_Json["Max_Instance"] = m_tEffectParticleDesc.iMaxInstance;

	Out_Json["Is_Looping"] = m_tEffectParticleDesc.bLooping;

	Out_Json["ParticleType"] = m_tEffectParticleDesc.iParticleType;
	Out_Json["Follow_Transform"] = m_tEffectParticleDesc.iFollowTransformType;

	// Out_Json["Is_Attraction"] = m_tEffectParticleDesc.bAttraction;
	Out_Json["ParticleOption1"] = m_tEffectParticleDesc.byParticleOption1;
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction))
		CJson_Utility::Write_Float3(Out_Json["Goal_Position"], m_tEffectParticleDesc.vGoalPosition);

	Out_Json["ShaderPassIndex"] = m_tEffectParticleDesc.iShaderPassIndex;

	Out_Json["Sync_Animation"] = m_tEffectParticleDesc.bSyncAnimation;
	Out_Json["Sync_AnimationKey"] = m_tEffectParticleDesc.iSyncAnimationKey;
	
#pragma region Life Time
	Out_Json["Init_Time"]      = m_tEffectParticleDesc.fInitTime;

	Out_Json["Min_Spawn_Time"] = m_tEffectParticleDesc.fMinSpawnTime;
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime))
		Out_Json["Max_Spawn_Time"] = m_tEffectParticleDesc.fMaxSpawnTime;

	Out_Json["Min_Life_Time"]  = m_tEffectParticleDesc.fMinLifeTime;
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime))
		Out_Json["Max_Life_Time"] = m_tEffectParticleDesc.fMaxLifeTime;
#pragma endregion

	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner))
	{
		if (!m_strBoneName.empty())
			Out_Json["Bone_Name"] = m_strBoneName;
	}

#pragma region Spawn Position
	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Position"], m_tEffectParticleDesc.vMinSpawnPosition);
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos))
		CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);

	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMinSpawnOffsetDirection);
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection))
		CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);

	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Offset_Range"], m_tEffectParticleDesc.vMinSpawnOffsetRange);
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange))
		CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);
#pragma endregion

	// Out_Json["Is_MoveLook"] = m_tEffectParticleDesc.bMoveLook;
	
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
	Out_Json["Is_Square_Scale"] = m_tEffectParticleDesc.bSquareScale;
	Out_Json["Is_Ratio_Scale"] = m_tEffectParticleDesc.bRatioScale;

	if (m_tEffectParticleDesc.bRatioScale)
	{
		Out_Json["Min_Y_Scale_Ratio"] = m_tEffectParticleDesc.fMinYScaleRatio;
		Out_Json["Max_Y_Scale_Ratio"] = m_tEffectParticleDesc.fMaxYScaleRatio;
	}
	
	Out_Json["Is_Easing_Scale"] = m_tEffectParticleDesc.bEasingScale;

	if (m_tEffectParticleDesc.bEasingScale)
	{
		Out_Json["Scale_Easing_Type"] = m_tEffectParticleDesc.iScaleEasingType;
		Out_Json["Scale_Easing_Total_Time"] = m_tEffectParticleDesc.fScaleEasingTotalTime;
	}
	else
	{
		CJson_Utility::Write_Float2(Out_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
		CJson_Utility::Write_Float2(Out_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);

		CJson_Utility::Write_Float2(Out_Json["Min_Scale_Speed"], m_tEffectParticleDesc.vMinScaleSpeed);
		CJson_Utility::Write_Float2(Out_Json["Max_Scale_Speed"], m_tEffectParticleDesc.vMaxScaleSpeed);

		CJson_Utility::Write_Float2(Out_Json["Min_Scale_Force"], m_tEffectParticleDesc.vMinScaleForce);
		CJson_Utility::Write_Float2(Out_Json["Max_Scale_Force"], m_tEffectParticleDesc.vMaxScaleForce);
	}
	CJson_Utility::Write_Float2(Out_Json["Min_Scale"], m_tEffectParticleDesc.vMinLimitScale);
	CJson_Utility::Write_Float2(Out_Json["Max_Scale"], m_tEffectParticleDesc.vMaxLimitScale);
#pragma endregion

#pragma region Color
	Out_Json["Render_Group"] = m_eRenderGroup;

	Out_Json["Discard_Ratio"] = m_tEffectParticleDesc.fDiscardRatio;
	Out_Json["Is_Gray_Only_Use_Red"] = m_tEffectParticleDesc.IsGrayOnlyUseRed;
	Out_Json["Is_Easing_Alpha"] = m_tEffectParticleDesc.bEasingAlpha;

	CJson_Utility::Write_Float4(Out_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	CJson_Utility::Write_Float4(Out_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);

	CJson_Utility::Write_Float4(Out_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Max_Color_Speed"], m_tEffectParticleDesc.vMaxColorSpeed);

	CJson_Utility::Write_Float4(Out_Json["Min_Color_Force"], m_tEffectParticleDesc.vMinColorForce);
	CJson_Utility::Write_Float4(Out_Json["Max_Color_Force"], m_tEffectParticleDesc.vMaxColorForce);

	if (m_tEffectParticleDesc.bEasingAlpha)
	{
		Out_Json["Alpha_Easing_Type"] = m_tEffectParticleDesc.iAlphaEasingType;
		Out_Json["Alpha_Easing_Total_Time"] = m_tEffectParticleDesc.fAlphaEasingTotalTime;
	}

	CJson_Utility::Write_Float4(Out_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);
#pragma endregion

#pragma region Texture
	Out_Json["Diffuse_XInverse"] = m_tEffectParticleDesc.bDiffuseXInverse;
	Out_Json["Diffuse_YInverse"] = m_tEffectParticleDesc.bDiffuseYInverse;

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
	if (Is_Sprite())
	{
		Out_Json["Loop_Sprite"] = m_tEffectParticleDesc.bLoopSprite;
		Out_Json["Is_Stop_At_End_Frame"] = m_tEffectParticleDesc.bStopAtEndFrame;

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

	if (In_Json.find("ParticleOption1") != In_Json.end())
		m_tEffectParticleDesc.byParticleOption1 = In_Json["ParticleOption1"];
#ifndef _BAKE_PARTICLE_
	else
		assert(0);
#endif // _BAKE_PARTICLE_

#pragma region Attraction

#ifdef _BAKE_PARTICLE_
	if (In_Json.find("Is_Attraction") != In_Json.end())
	{
		if (In_Json["Is_Attraction"])
			TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction);
		else
			TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction);
	}
#endif // _BAKE_PARTICLE_

	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction))
	{
		if (In_Json.find("Goal_Position") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Goal_Position"], m_tEffectParticleDesc.vGoalPosition);
	}

#pragma endregion // Attraction

	if (In_Json.find("ShaderPassIndex") != In_Json.end())
		m_tEffectParticleDesc.iShaderPassIndex = In_Json["ShaderPassIndex"];

#pragma region Animation Sync
	if (In_Json.find("Sync_Animation") != In_Json.end())
		m_tEffectParticleDesc.bSyncAnimation = In_Json["Sync_Animation"];
	if (In_Json.find("Sync_AnimationKey") != In_Json.end())
		m_tEffectParticleDesc.iSyncAnimationKey = In_Json["Sync_AnimationKey"];
#pragma endregion // Animation Sync

	if (In_Json.find("Init_Time") != In_Json.end())
		m_tEffectParticleDesc.fInitTime = In_Json["Init_Time"];

#pragma region Spawn Time

	if (In_Json.find("Min_Spawn_Time") != In_Json.end())
		m_tEffectParticleDesc.fMinSpawnTime = In_Json["Min_Spawn_Time"];

#ifndef _BAKE_PARTICLE_
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime))
	{
		if (In_Json.find("Max_Spawn_Time") != In_Json.end())
			m_tEffectParticleDesc.fMaxSpawnTime = In_Json["Max_Spawn_Time"];
	}
#else // _BAKE_PARTICLE_
	if (In_Json.find("Max_Spawn_Time") != In_Json.end())
		m_tEffectParticleDesc.fMaxSpawnTime = In_Json["Max_Spawn_Time"];

	if (m_tEffectParticleDesc.fMaxSpawnTime > m_tEffectParticleDesc.fMinSpawnTime)
		TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime);
	else
		TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime);
#endif // _BAKE_PARTICLE_

#pragma endregion // Spawn Time

#pragma region Life Time

	if (In_Json.find("Min_Life_Time") != In_Json.end())
		m_tEffectParticleDesc.fMinLifeTime = In_Json["Min_Life_Time"];

#ifndef _BAKE_PARTICLE_
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime))
	{
		if (In_Json.find("Max_Life_Time") != In_Json.end())
			m_tEffectParticleDesc.fMaxLifeTime = In_Json["Max_Life_Time"];
	}
#else // _BAKE_PARTICLE_
	if (In_Json.find("Max_Life_Time") != In_Json.end())
		m_tEffectParticleDesc.fMaxLifeTime = In_Json["Max_Life_Time"];

	if (m_tEffectParticleDesc.fMaxLifeTime > m_tEffectParticleDesc.fMinLifeTime)
		TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime);
	else
		TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime);
#endif // _BAKE_PARTICLE_

#pragma endregion // Life Time

#pragma region Boner

#ifdef _BAKE_PARTICLE_
	if (In_Json.find("Is_Boner") != In_Json.end())
	{
		if (In_Json["Is_Boner"])
			TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner);
		else
			TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner);
	}
#endif // _BAKE_PARTICLE_

	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner))
	{
		if (In_Json.find("Bone_Name") != In_Json.end())
			m_strBoneName = In_Json["Bone_Name"];

		if (m_strBoneName.empty())
			assert(0);

#ifdef _DEBUG
		if ((_uint)LEVEL_EDIT == m_CreatedLevel)
		{
			m_pParentTransformCom = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Component<CTransform>().lock();
			m_pBoneNode = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_CurrentModel().lock()->Find_BoneNode(m_strBoneName);
		}
#endif // _DEBUG
	}

#pragma endregion // Boner

#pragma region Spawn Position

	if (In_Json.find("Min_Spawn_Position") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Position"], m_tEffectParticleDesc.vMinSpawnPosition);
#ifndef _BAKE_PARTICLE_
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos))
	{
		if (In_Json.find("Max_Spawn_Position") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);
	}
#else // _BAKE_PARTICLE_
	if (In_Json.find("Max_Spawn_Position") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);

	if (SMath::Is_Equal(m_tEffectParticleDesc.vMinSpawnPosition, m_tEffectParticleDesc.vMaxSpawnPosition))
		TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos);
	else
		TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos);
#endif // _BAKE_PARTICLE_

#pragma endregion  // Spawn Position

#pragma region Spawn Offset Direction

	if (In_Json.find("Min_Spawn_Offset_Direction") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMinSpawnOffsetDirection);

#ifndef _BAKE_PARTICLE_
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection))
	{
		if (In_Json.find("Max_Spawn_Offset_Direction") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);
	}
#else // _BAKE_PARTICLE_
	if (In_Json.find("Max_Spawn_Offset_Direction") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);

	if (SMath::Is_Equal(m_tEffectParticleDesc.vMinSpawnOffsetDirection, m_tEffectParticleDesc.vMaxSpawnOffsetDirection))
		TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection);
	else
		TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection);
#endif // _BAKE_PARTICLE_

#pragma endregion // Spawn Offset Direction

#pragma region Spawn Offset Range

	if (In_Json.find("Min_Spawn_Offset_Range") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Offset_Range"], m_tEffectParticleDesc.vMinSpawnOffsetRange);

#ifndef _BAKE_PARTICLE_
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange))
	{
		if (In_Json.find("Max_Spawn_Offset_Range") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);
	}
#else // _BAKE_PARTICLE_
	if (In_Json.find("Max_Spawn_Offset_Range") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);

	if (SMath::Is_Equal(m_tEffectParticleDesc.vMinSpawnOffsetRange, m_tEffectParticleDesc.vMaxSpawnOffsetRange))
		TurnOff_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange);
	else
		TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange);
#endif // _BAKE_PARTICLE_

#pragma endregion // Spawn Offset Range

#ifdef _BAKE_PARTICLE_
	if (In_Json.find("Is_MoveLook") != In_Json.end())
	{
		if (In_Json["Is_MoveLook"])
			TurnOn_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_MoveLook);
	}
#endif // _BAKE_PARTICLE_

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
	if (In_Json.find("Is_Square_Scale") != In_Json.end())
		m_tEffectParticleDesc.bSquareScale = In_Json["Is_Square_Scale"];

	if (!m_tEffectParticleDesc.bSquareScale)
	{
		if (In_Json.find("Is_Ratio_Scale") != In_Json.end())
			m_tEffectParticleDesc.bRatioScale = In_Json["Is_Ratio_Scale"];
	}

#ifdef _DEBUG
#ifdef _JOJO_EFFECT_TOOL_
	if (m_tEffectParticleDesc.bSquareScale && !m_tEffectParticleDesc.bRatioScale)
		m_iScaleType = m_iScaleType_Square;
	else if (!m_tEffectParticleDesc.bSquareScale && m_tEffectParticleDesc.bRatioScale)
		m_iScaleType = m_iScaleType_Ratio;
	else
		m_iScaleType = 0;
#endif // _JOJO_EFFECT_TOOL_
#endif // _DEBUG

	if (m_tEffectParticleDesc.bRatioScale)
	{
		m_tEffectParticleDesc.fMinYScaleRatio = In_Json["Min_Y_Scale_Ratio"];
		m_tEffectParticleDesc.fMaxYScaleRatio = In_Json["Max_Y_Scale_Ratio"];
	}
	
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
			CJson_Utility::Load_Float2(In_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
		if (In_Json.find("Max_Start_Scale") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);

		if (In_Json.find("Min_Scale_Speed") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Min_Scale_Speed"], m_tEffectParticleDesc.vMinScaleSpeed);
		if (In_Json.find("Max_Scale_Speed") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Max_Scale_Speed"], m_tEffectParticleDesc.vMaxScaleSpeed);

		if (In_Json.find("Min_Scale_Force") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Min_Scale_Force"], m_tEffectParticleDesc.vMinScaleForce);
		if (In_Json.find("Max_Scale_Force") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Max_Scale_Force"], m_tEffectParticleDesc.vMaxScaleForce);
	}
	if (In_Json.find("Min_Scale") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Min_Scale"], m_tEffectParticleDesc.vMinLimitScale);
	if (In_Json.find("Max_Scale") != In_Json.end())
		CJson_Utility::Load_Float2(In_Json["Max_Scale"], m_tEffectParticleDesc.vMaxLimitScale);
#pragma endregion

#pragma region Color
	if (In_Json.find("Render_Group") != In_Json.end())
		m_eRenderGroup = In_Json["Render_Group"];
	if (In_Json.find("Discard_Ratio") != In_Json.end())
		m_tEffectParticleDesc.fDiscardRatio = In_Json["Discard_Ratio"];
	if (In_Json.find("Is_Gray_Only_Use_Red") != In_Json.end())
		m_tEffectParticleDesc.IsGrayOnlyUseRed = In_Json["Is_Gray_Only_Use_Red"];
	if (In_Json.find("Is_Easing_Alpha") != In_Json.end())
		m_tEffectParticleDesc.bEasingAlpha = In_Json["Is_Easing_Alpha"];

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

	if (m_tEffectParticleDesc.bEasingAlpha)
	{
		if (In_Json.find("Alpha_Easing_Type") != In_Json.end())
			m_tEffectParticleDesc.iAlphaEasingType = In_Json["Alpha_Easing_Type"];

		if (In_Json.find("Alpha_Easing_Total_Time") != In_Json.end())
			m_tEffectParticleDesc.fAlphaEasingTotalTime = In_Json["Alpha_Easing_Total_Time"];
	}
#pragma endregion

#pragma region Texture

	if (In_Json.find("Diffuse_XInverse") != In_Json.end())
		m_tEffectParticleDesc.bDiffuseXInverse = In_Json["Diffuse_XInverse"];
	if (In_Json.find("Diffuse_YInverse") != In_Json.end())
		m_tEffectParticleDesc.bDiffuseYInverse = In_Json["Diffuse_YInverse"];

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
	if (Is_Sprite())
	{
		if (In_Json.find("Loop_Sprite") != In_Json.end())
			m_tEffectParticleDesc.bLoopSprite = In_Json["Loop_Sprite"];

		if (In_Json.find("Is_Stop_At_End_Frame") != In_Json.end())
			m_tEffectParticleDesc.bStopAtEndFrame = In_Json["Is_Stop_At_End_Frame"];

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

	_float	fFrameTime(HZ_144);
	_int	iTickCount(0);
	m_fPreFrame += fTimeDelta;

	while (fFrameTime <= m_fPreFrame)
	{
		++iTickCount;
		m_fPreFrame -= fFrameTime;
	}

	_matrix BoneMatrix(XMMatrixIdentity());
	if (m_pBoneNode.lock())
	{
		_float4x4 ModelTransMat(m_pParentModel.lock()->Get_TransformationMatrix());
		BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * XMLoadFloat4x4(&ModelTransMat);

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
	}

	for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{
		if (!m_tParticleDescs[i].bEnable)
		{
			if (m_tParticleDescs[i].fCurrentSpawnTime > m_tParticleDescs[i].fTargetSpawnTime)
			{
				m_tParticleDescs[i].bEnable = true;

				if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
					XMStoreFloat4x4(&m_tParticleDescs[i].matParentMatrix, BoneMatrix * m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
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
					Reset_ParticleDesc((_uint)i);
				else
				{
					m_tParticleDescs[i].bEnable = false;
					m_tParticleDescs[i].fCurrentSpawnTime = -1.f * FLT_MAX;
				}
				continue;
			}

			m_tParticleDescs[i].fCurrentLifeTime += fTimeDelta;
		}


		for (_int x(0); x < iTickCount; ++x)
			Play_Internal(i, fFrameTime, BoneMatrix);
	}

	for (_int x(0); x < iTickCount; ++x)
	{
		Update_ParticleUV(fFrameTime);
		Update_ParticleGlowColor(fFrameTime);
	}
}

void CEffect_Rect::Sync_Animation()
{
	if (!m_tEffectParticleDesc.bSyncAnimation)
		return;

	Reset_Effect(weak_ptr<CTransform>());
}

void CEffect_Rect::UnUse_EffectParticle()
{
	if (0.f < m_fCurrentInitTime)
	{
		Set_Enable(false);
		return;
	}

	m_bStopParticle = true;
}

void CEffect_Rect::ReBake_EditParticle()
{
	Reset_Instance(m_tEffectParticleDesc.iMaxInstance);
}

void CEffect_Rect::Reset_Instance(const _uint& In_ParticleCount)
{
	m_tParticleDescs.clear();
	m_bStopParticle = false;
	m_bStopSprite = false;

	m_tParticleDescs.reserve(In_ParticleCount);
	m_tParticleDescs = vector<PARTICLE_DESC>(In_ParticleCount, PARTICLE_DESC());
	m_tOriginalParticleDescs.reserve(In_ParticleCount);
	m_tOriginalParticleDescs = vector<PARTICLE_DESC>(In_ParticleCount, PARTICLE_DESC());

	m_pVIBuffer.lock()->Init_Particle(In_ParticleCount);
	Generate_RandomOriginalParticleDesc();
	Reset_ParticleDescs();
}

void CEffect_Rect::Reset_ParticleDescs()
{
	for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
		Reset_ParticleDesc((_uint)i);
}

void CEffect_Rect::Reset_ParticleDesc(const _uint& In_iIndex)
{
#ifdef _DEBUG
	if (0 == m_tParticleDescs.size() || 0 == m_tOriginalParticleDescs.size())
		return;
#endif // _DEBUG

	memcpy(&m_tParticleDescs[In_iIndex], &m_tOriginalParticleDescs[In_iIndex], sizeof(PARTICLE_DESC));

	SMath::Add_Float3(&m_tParticleDescs[In_iIndex].vCurrentTranslation, m_tParticleDescs[In_iIndex].vOffsetPosition);

	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction))
	{
		_float3	f3LookAt(SMath::Add_Float3(m_tEffectParticleDesc.vGoalPosition, SMath::Mul_Float3(m_tParticleDescs[In_iIndex].vCurrentTranslation, -1.f)));
		XMStoreFloat3(&m_tParticleDescs[In_iIndex].vTargetLookAt, XMVector3Normalize(XMLoadFloat3(&f3LookAt)));
	}
}


void CEffect_Rect::Generate_RandomOriginalParticleDesc()
{
	for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{
		m_tOriginalParticleDescs[i].Reset();

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime))
			m_tOriginalParticleDescs[i].fTargetLifeTime = SMath::fRandom(m_tEffectParticleDesc.fMinLifeTime, m_tEffectParticleDesc.fMaxLifeTime);
		else
			m_tOriginalParticleDescs[i].fTargetLifeTime = m_tEffectParticleDesc.fMinLifeTime;

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime))
			m_tOriginalParticleDescs[i].fTargetSpawnTime = SMath::fRandom(m_tEffectParticleDesc.fMinSpawnTime, m_tEffectParticleDesc.fMaxSpawnTime);
		else
			m_tOriginalParticleDescs[i].fTargetSpawnTime = m_tEffectParticleDesc.fMinSpawnTime;

		_float3 vRandomDir;
		ZeroMemory(&vRandomDir, sizeof(_float3));
		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection))
			vRandomDir = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnOffsetDirection, m_tEffectParticleDesc.vMaxSpawnOffsetDirection);
		else
			vRandomDir = m_tEffectParticleDesc.vMinSpawnOffsetDirection;

		_vector vRandomDirFromVector(XMLoadFloat3(&vRandomDir));
		vRandomDirFromVector = XMVector3Normalize(vRandomDirFromVector);

		_float3 vRandomScalar;
		ZeroMemory(&vRandomScalar, sizeof(_float3));
		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange))
			vRandomScalar = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnOffsetRange, m_tEffectParticleDesc.vMaxSpawnOffsetRange);
		else
			vRandomScalar = m_tEffectParticleDesc.vMinSpawnOffsetRange;

		_vector vRandomScalarFromVector(XMLoadFloat3(&vRandomScalar));

		_matrix RotationMatrix(SMath::Bake_MatrixNormalizeUseLookVector(vRandomDirFromVector));
		RotationMatrix = SMath::Go_Right(RotationMatrix, XMVectorGetX(vRandomScalarFromVector));
		RotationMatrix = SMath::Go_Up(RotationMatrix, XMVectorGetY(vRandomScalarFromVector));
		RotationMatrix = SMath::Go_Straight(RotationMatrix, XMVectorGetZ(vRandomScalarFromVector));

		XMStoreFloat3(&m_tOriginalParticleDescs[i].vCurrentTranslation, RotationMatrix.r[3]);

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos))
			m_tOriginalParticleDescs[i].vOffsetPosition = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnPosition, m_tEffectParticleDesc.vMaxSpawnPosition);
		else
			m_tOriginalParticleDescs[i].vOffsetPosition = m_tEffectParticleDesc.vMinSpawnPosition;

		m_tOriginalParticleDescs[i].vCurrentRotation =
			SMath::vRandom(m_tEffectParticleDesc.vMinStartRotation, m_tEffectParticleDesc.vMaxStartRotation);

		m_tOriginalParticleDescs[i].vTargetRotationSpeed =
			SMath::vRandom(m_tEffectParticleDesc.vMinRotationSpeed, m_tEffectParticleDesc.vMaxRotationSpeed);

		m_tOriginalParticleDescs[i].vTargetRotationForce =
			SMath::vRandom(m_tEffectParticleDesc.vMinRotationForce, m_tEffectParticleDesc.vMaxRotationForce);

		if (m_tEffectParticleDesc.bEasingPosition)
		{
			m_tOriginalParticleDescs[i].vTargetSpeed = SMath::Add_Float3(m_tOriginalParticleDescs[i].vCurrentTranslation, m_tOriginalParticleDescs[i].vOffsetPosition);

			_float3 vGoalPos(SMath::vRandom(m_tEffectParticleDesc.vMinSpeed, m_tEffectParticleDesc.vMaxSpeed));
			_vector vGoalOffsetPosition(XMLoadFloat3(&vGoalPos));
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

		XMStoreFloat2(&m_tOriginalParticleDescs[i].vCurrentScale, SMath::vRandom(XMLoadFloat2(&m_tEffectParticleDesc.vMinStartScale), XMLoadFloat2(&m_tEffectParticleDesc.vMaxStartScale)));
		XMStoreFloat2(&m_tOriginalParticleDescs[i].vTargetScaleSpeed, SMath::vRandom(XMLoadFloat2(&m_tEffectParticleDesc.vMinScaleSpeed), XMLoadFloat2(&m_tEffectParticleDesc.vMaxScaleSpeed)));
		XMStoreFloat2(&m_tOriginalParticleDescs[i].vTargetScaleForce, SMath::vRandom(XMLoadFloat2(&m_tEffectParticleDesc.vMinScaleForce), XMLoadFloat2(&m_tEffectParticleDesc.vMaxScaleForce)));

		if (m_tEffectParticleDesc.IsGrayOnlyUseRed)
		{
			_float fGrayColor(SMath::fRandom(m_tEffectParticleDesc.vMinStartColor.x, m_tEffectParticleDesc.vMaxStartColor.x));
			_float fAhlpaColor(SMath::fRandom(m_tEffectParticleDesc.vMinStartColor.w, m_tEffectParticleDesc.vMaxStartColor.w));
			m_tOriginalParticleDescs[i].vCurrentColor = { fGrayColor, fGrayColor, fGrayColor, fAhlpaColor };
		}
		else
		{
			m_tOriginalParticleDescs[i].vCurrentColor = SMath::vRandom(m_tEffectParticleDesc.vMinStartColor, m_tEffectParticleDesc.vMaxStartColor);
			if (m_tEffectParticleDesc.bEasingAlpha)
				m_tOriginalParticleDescs[i].fStartAlpha = m_tOriginalParticleDescs[i].vCurrentColor.w;
		}

		m_tOriginalParticleDescs[i].vTargetColorSpeed =
			SMath::vRandom(m_tEffectParticleDesc.vMinColorSpeed, m_tEffectParticleDesc.vMaxColorSpeed);

		m_tOriginalParticleDescs[i].vTargetColorForce =
			SMath::vRandom(m_tEffectParticleDesc.vMinColorForce, m_tEffectParticleDesc.vMaxColorForce);

		m_tOriginalParticleDescs[i].vCurrentUV = m_tEffectParticleDesc.vMaskStartUV;

		m_tOriginalParticleDescs[i].vCurrentScale.x = max(FLT_MIN, m_tOriginalParticleDescs[i].vCurrentScale.x);
		m_tOriginalParticleDescs[i].vCurrentScale.y = max(FLT_MIN, m_tOriginalParticleDescs[i].vCurrentScale.y);

		if (m_tEffectParticleDesc.bRatioScale)
			m_tOriginalParticleDescs[i].fTargetYScaleRatio = SMath::fRandom(m_tEffectParticleDesc.fMinYScaleRatio, m_tEffectParticleDesc.fMaxYScaleRatio);
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

void CEffect_Rect::Play_Internal(const _uint& i, _float fTimeDelta, _matrix BoneMatrix)
{
	Update_ParticleRotation(i, fTimeDelta);
	Update_ParticlePosition(i, fTimeDelta, BoneMatrix);
	Update_ParticleScale(i, fTimeDelta);
	Update_ParticleColor(i, fTimeDelta);
	Update_ParticleSpriteFrame(i, fTimeDelta);
}

void CEffect_Rect::Update_ParticlePosition(const _uint& i, _float fTimeDelta, _matrix BoneMatrix)
{
	if (m_pBoneNode.lock() && (_int)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType)
	{
#ifdef _DEBUG
		if (m_pParentTransformCom.lock())
			m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
#else // _DEBUG
		m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
#endif // _DEBUG
	}

	_float3 vMove;
	ZeroMemory(&vMove, sizeof(_float3));

	if (m_tEffectParticleDesc.bEasingPosition)
	{
		_float fElapsedTime = m_tParticleDescs[i].fCurrentLifeTime;

		if (0.f > fElapsedTime)
			return;

		Apply_Easing
		(
			vMove
			, (EASING_TYPE)m_tEffectParticleDesc.iSpeedEasingType
			, XMLoadFloat3(&m_tParticleDescs[i].vTargetSpeed)
			, XMLoadFloat3(&m_tParticleDescs[i].vTargetSpeedForce)
			, fElapsedTime
			, m_tEffectParticleDesc.fSpeedEasingTotalTime
		);

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_MoveLook))
		{
			_vector vSpawnPos(XMLoadFloat3(&m_tParticleDescs[i].vTargetSpeed));
			_vector vMovePosition(XMLoadFloat3(&vMove));
			vMovePosition -= vSpawnPos;
			_vector vRotatedPosition(XMVector3TransformCoord(vMovePosition, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation))));
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
			_float fElapsedTime(m_tParticleDescs[i].fCurrentLifeTime);

			if (0.f > fElapsedTime)
				return;

			Apply_Easing
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

		// For. Attraction
		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction))
		{
			m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, SMath::Mul_Float3(m_tParticleDescs[i].vTargetLookAt, vMove.z));
		}
		else
		{
			if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_MoveLook))
			{
				if ((_int)PARTICLETYPE::OUTBURST == m_tEffectParticleDesc.iParticleType)
				{
					_vector vMovePosition(XMLoadFloat3(&vMove));
					_vector vRotation(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation));
					_matrix vRotationMatrix(XMMatrixRotationRollPitchYawFromVector(vRotation));
					vRotationMatrix.r[3] = vRotationMatrix.r[1];
					vRotationMatrix.r[1] = vRotationMatrix.r[2];
					vRotationMatrix.r[2] = vRotationMatrix.r[3];
					vRotationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
					_vector vRotatedPosition(XMVector3TransformCoord(vMovePosition, vRotationMatrix));
					_vector vCurrentPosition(XMLoadFloat3(&m_tParticleDescs[i].vCurrentTranslation));
					vCurrentPosition += vRotatedPosition;
					XMStoreFloat3(&m_tParticleDescs[i].vCurrentTranslation, vCurrentPosition);
				}
				else
				{
					_vector vMovePosition(XMLoadFloat3(&vMove));
					_vector vRotatedPosition(XMVector3TransformCoord(vMovePosition, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation))));
					_vector vCurrentPosition(XMLoadFloat3(&m_tParticleDescs[i].vCurrentTranslation));
					vCurrentPosition += vRotatedPosition;
					XMStoreFloat3(&m_tParticleDescs[i].vCurrentTranslation, vCurrentPosition);
				}
			}
			else
			{
				m_tEffectParticleDesc.bEasingSpeed ?
					m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, m_tParticleDescs[i].vTargetSpeed)
					:
					m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, vMove);
			}
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
	}
}

void CEffect_Rect::Update_ParticleRotation(const _uint& i, _float fTimeDelta)
{
	if ((_int)PARTICLETYPE::OUTBURST == m_tEffectParticleDesc.iParticleType)
	{
		_vector vUp(XMLoadFloat3(&m_tOriginalParticleDescs[i].vCurrentTranslation));
		_matrix ReverseAxisRotationMatrix(SMath::Bake_MatrixNormalizeUseLookVector(vUp));
		_matrix RotationMatrix = 
		{
			ReverseAxisRotationMatrix.r[0],
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
			_float fElapsedTime(m_tParticleDescs[i].fCurrentLifeTime);

			if (0.f > fElapsedTime)
				return;

			Apply_Easing
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
	_float2 vScale;
	ZeroMemory(&vScale, sizeof(_float2));

	if (m_tEffectParticleDesc.bEasingScale)
	{
		_float fElapsedTime(m_tParticleDescs[i].fCurrentLifeTime);

		if (0.f > fElapsedTime)
			return;

		Apply_Easing
		(
			vScale
			, (EASING_TYPE)m_tEffectParticleDesc.iScaleEasingType
			, XMLoadFloat2(&m_tEffectParticleDesc.vMinLimitScale)
			, XMLoadFloat2(&m_tEffectParticleDesc.vMaxLimitScale)
			, fElapsedTime
			, m_tEffectParticleDesc.fScaleEasingTotalTime
		);

		m_tParticleDescs[i].vCurrentScale = vScale;
	}
	else
	{
		vScale = SMath::Mul_Float2(m_tParticleDescs[i].vTargetScaleSpeed, fTimeDelta);
		m_tParticleDescs[i].vCurrentScaleForce = SMath::Add_Float2(m_tParticleDescs[i].vCurrentScaleForce, SMath::Mul_Float2(m_tParticleDescs[i].vTargetScaleForce, fTimeDelta));
		SMath::Add_Float2(&m_tParticleDescs[i].vCurrentScale, m_tParticleDescs[i].vCurrentScaleForce);
		m_tParticleDescs[i].vCurrentScale = SMath::Add_Float2(m_tParticleDescs[i].vCurrentScale, vScale);
	}

	m_tParticleDescs[i].vCurrentScale.x = max(m_tEffectParticleDesc.vMinLimitScale.x, min(m_tEffectParticleDesc.vMaxLimitScale.x, m_tParticleDescs[i].vCurrentScale.x));


	if (m_tEffectParticleDesc.bSquareScale)
		m_tParticleDescs[i].vCurrentScale.y = m_tParticleDescs[i].vCurrentScale.x;
	else if (m_tEffectParticleDesc.bRatioScale)
		m_tParticleDescs[i].vCurrentScale.y = m_tParticleDescs[i].vCurrentScale.x * m_tParticleDescs[i].fTargetYScaleRatio;
	else
		m_tParticleDescs[i].vCurrentScale.y = max(m_tEffectParticleDesc.vMinLimitScale.y, min(m_tEffectParticleDesc.vMaxLimitScale.y, m_tParticleDescs[i].vCurrentScale.y));
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

	vDiffuseCurrentUV = XMVectorSetX(vDiffuseCurrentUV, min(m_tEffectParticleDesc.vDiffuseUVMax.x, XMVectorGetX(vDiffuseCurrentUV)));
	vDiffuseCurrentUV = XMVectorSetY(vDiffuseCurrentUV, min(m_tEffectParticleDesc.vDiffuseUVMax.y, XMVectorGetY(vDiffuseCurrentUV)));

	XMStoreFloat2(&m_vDiffuseCurrentUV, vDiffuseCurrentUV);
#pragma endregion

#pragma region Mask
	_vector vMaskUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vMaskUVSpeed) * fTimeDelta;
	m_vMaskCurrentUVForce.x += m_tEffectParticleDesc.vMaskUVForce.x * fTimeDelta;
	m_vMaskCurrentUVForce.y += m_tEffectParticleDesc.vMaskUVForce.y * fTimeDelta;

	_vector vMaskMoveUV = vMaskUVSpeed + XMLoadFloat2(&m_vMaskCurrentUVForce);
	_vector vMaskCurrentUV = XMLoadFloat2(&m_vMaskCurrentUV);
	vMaskCurrentUV += vMaskMoveUV;

	vMaskCurrentUV = XMVectorSetX(vMaskCurrentUV, min(m_tEffectParticleDesc.vMaskUVMax.x, XMVectorGetX(vMaskCurrentUV)));
	vMaskCurrentUV = XMVectorSetY(vMaskCurrentUV, min(m_tEffectParticleDesc.vMaskUVMax.y, XMVectorGetY(vMaskCurrentUV)));

	XMStoreFloat2(&m_vMaskCurrentUV, vMaskCurrentUV);
#pragma endregion

#pragma region Noise
	_vector vNoiseUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vNoiseUVSpeed) * fTimeDelta;
	m_vNoiseCurrentUVForce.x += m_tEffectParticleDesc.vNoiseUVForce.x * fTimeDelta;
	m_vNoiseCurrentUVForce.y += m_tEffectParticleDesc.vNoiseUVForce.y * fTimeDelta;

	_vector vNoiseMoveUV = vNoiseUVSpeed + XMLoadFloat2(&m_vNoiseCurrentUVForce);
	_vector vNoiseCurrentUV = XMLoadFloat2(&m_vNoiseCurrentUV);
	vNoiseCurrentUV += vNoiseMoveUV;

	vNoiseCurrentUV = XMVectorSetX(vNoiseCurrentUV, min(m_tEffectParticleDesc.vNoiseUVMax.x, XMVectorGetX(vNoiseCurrentUV)));
	vNoiseCurrentUV = XMVectorSetY(vNoiseCurrentUV, min(m_tEffectParticleDesc.vNoiseUVMax.y, XMVectorGetY(vNoiseCurrentUV)));

	XMStoreFloat2(&m_vNoiseCurrentUV, vNoiseCurrentUV);
#pragma endregion
}

void CEffect_Rect::Update_ParticleColor(const _uint& i, _float fTimeDelta)
{
	_vector vColor;
	ZeroMemory(&vColor, sizeof(_vector));

	if (m_tEffectParticleDesc.IsGrayOnlyUseRed)
	{
		vColor = XMVectorSetX(vColor, m_tParticleDescs[i].vTargetColorSpeed.x * fTimeDelta);
		vColor = XMVectorSetW(vColor, m_tParticleDescs[i].vTargetColorSpeed.w * fTimeDelta);

		m_tParticleDescs[i].vCurrentColorForce.x += m_tParticleDescs[i].vTargetColorForce.x * fTimeDelta;
		m_tParticleDescs[i].vCurrentColorForce.w += m_tParticleDescs[i].vTargetColorForce.w * fTimeDelta;

		vColor = XMVectorSetX(vColor, XMVectorGetX(vColor) + m_tParticleDescs[i].vCurrentColorForce.x);
		vColor = XMVectorSetW(vColor, XMVectorGetW(vColor) + m_tParticleDescs[i].vCurrentColorForce.w);
		vColor = XMVectorSetX(vColor, XMVectorGetX(vColor) + m_tParticleDescs[i].vCurrentColor.x);
		vColor = XMVectorSetW(vColor, XMVectorGetW(vColor) + m_tParticleDescs[i].vCurrentColor.w);

		vColor = XMVectorSetY(vColor, XMVectorGetX(vColor));
		vColor = XMVectorSetZ(vColor, XMVectorGetX(vColor));

		vColor = XMVectorSetX(vColor, max(m_tEffectParticleDesc.vMinColor.x, min(m_tEffectParticleDesc.vMaxColor.x, XMVectorGetX(vColor))));
		vColor = XMVectorSetY(vColor, max(m_tEffectParticleDesc.vMinColor.y, min(m_tEffectParticleDesc.vMaxColor.y, XMVectorGetY(vColor))));
		vColor = XMVectorSetZ(vColor, max(m_tEffectParticleDesc.vMinColor.z, min(m_tEffectParticleDesc.vMaxColor.z, XMVectorGetZ(vColor))));
		vColor = XMVectorSetW(vColor, max(m_tEffectParticleDesc.vMinColor.w, min(m_tEffectParticleDesc.vMaxColor.w, XMVectorGetW(vColor))));
	}
	else
	{
		_float fAlpha(0.f);

		if (m_tEffectParticleDesc.bEasingAlpha)
		{
			_float fElapsedTime(m_tParticleDescs[i].fCurrentLifeTime);

			if (0.f > fElapsedTime)
				return;

			Apply_Easing
			(
				fAlpha
				, (EASING_TYPE)m_tEffectParticleDesc.iAlphaEasingType
				, m_tParticleDescs[i].fStartAlpha
				, m_tEffectParticleDesc.vMinColorSpeed.w
				, fElapsedTime
				, m_tEffectParticleDesc.fAlphaEasingTotalTime
			);
		}

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

		vColor = XMVectorSetX(vColor, max(m_tEffectParticleDesc.vMinColor.x, min(m_tEffectParticleDesc.vMaxColor.x, XMVectorGetX(vColor))));
		vColor = XMVectorSetY(vColor, max(m_tEffectParticleDesc.vMinColor.y, min(m_tEffectParticleDesc.vMaxColor.y, XMVectorGetY(vColor))));
		vColor = XMVectorSetZ(vColor, max(m_tEffectParticleDesc.vMinColor.z, min(m_tEffectParticleDesc.vMaxColor.z, XMVectorGetZ(vColor))));
		vColor = XMVectorSetW(vColor, max(m_tEffectParticleDesc.vMinColor.w, min(m_tEffectParticleDesc.vMaxColor.w, XMVectorGetW(vColor))));

		if (m_tEffectParticleDesc.bEasingAlpha)
			vColor = XMVectorSetW(vColor, fAlpha);
	}

	XMStoreFloat4(&m_tParticleDescs[i].vCurrentColor, vColor);
}

void CEffect_Rect::Update_ParticleGlowColor(_float fTimeDelta)
{
	_float4 vColor = SMath::Mul_Float4(m_tEffectParticleDesc.vGlowColorSpeed, fTimeDelta);

	SMath::Add_Float4(&m_vCurrentGlowColorForce, SMath::Mul_Float4(m_tEffectParticleDesc.vGlowColorForce, fTimeDelta));

	SMath::Add_Float4(&vColor, m_vCurrentGlowColorForce);
	SMath::Add_Float4(&vColor, m_vCurrentGlowColor);

	//vColor.x = max(0.f, min(1.f, vColor.x));
	//vColor.y = max(0.f, min(1.f, vColor.y));
	//vColor.z = max(0.f, min(1.f, vColor.z));
	//vColor.w = max(0.f, min(1.f, vColor.w));

	vColor.w = min(1.f, max(0.f, vColor.w));
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
					if (m_tEffectParticleDesc.bStopAtEndFrame)
						m_tParticleDescs[i].vSpriteUV = { 1.f - 1.f / m_tEffectParticleDesc.iNumFrameX, 1.f - 1.f / m_tEffectParticleDesc.iNumFrameY };
					else
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
	if (!m_pParentTransformCom.lock())
		return;

	if ((_int)TRANSFORMTYPE::STATIC != m_tEffectParticleDesc.iFollowTransformType)
	{
		if ((_int)PARTICLETYPE::BILLBOARD == m_tEffectParticleDesc.iParticleType)
		{
			m_pTransformCom.lock()->Set_WorldMatrix(m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
		}

		if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
		{
			m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());

		}
		else if ((_int)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType)
		{
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
	// Macro defined for convenience. Undefined later.
#define EASING_MACRO(FunctionName)\
XMStoreFloat3(&vTarget, FunctionName(vStartPoint, vTargetPoint, fElapsedTime, fTotalTime));

	switch (eEasingType)
	{
	case EASING_TYPE::LINEAR:
		EASING_MACRO(CEasing_Utillity::Linear);
		break;
	case EASING_TYPE::QUAD_IN:
		EASING_MACRO(CEasing_Utillity::QuadIn);
		break;
	case EASING_TYPE::QUAD_OUT:
		EASING_MACRO(CEasing_Utillity::QuadOut);
		break;
	case EASING_TYPE::QUAD_INOUT:
		EASING_MACRO(CEasing_Utillity::QuadInOut);
		break;
	case EASING_TYPE::CUBIC_IN:
		EASING_MACRO(CEasing_Utillity::CubicIn);
		break;
	case EASING_TYPE::CUBIC_OUT:
		EASING_MACRO(CEasing_Utillity::CubicOut);
		break;
	case EASING_TYPE::CUBIC_INOUT:
		EASING_MACRO(CEasing_Utillity::CubicInOut);
		break;
	case EASING_TYPE::QUART_IN:
		EASING_MACRO(CEasing_Utillity::QuartIn);
		break;
	case EASING_TYPE::QUART_OUT:
		EASING_MACRO(CEasing_Utillity::QuartOut);
		break;
	case EASING_TYPE::QUART_INOUT:
		EASING_MACRO(CEasing_Utillity::QuartInOut);
		break;
	case EASING_TYPE::QUINT_IN:
		EASING_MACRO(CEasing_Utillity::QuintIn);
		break;
	case EASING_TYPE::QUINT_OUT:
		EASING_MACRO(CEasing_Utillity::QuintOut);
		break;
	case EASING_TYPE::QUINT_INOUT:
		EASING_MACRO(CEasing_Utillity::QuintInOut);
		break;
	case EASING_TYPE::SINE_IN:
		EASING_MACRO(CEasing_Utillity::SineIn);
		break;
	case EASING_TYPE::SINE_OUT:
		EASING_MACRO(CEasing_Utillity::SineOut);
		break;
	case EASING_TYPE::SINE_INOUT:
		EASING_MACRO(CEasing_Utillity::SineInOut);
		break;
	case EASING_TYPE::EXPO_IN:
		EASING_MACRO(CEasing_Utillity::ExpoIn);
		break;
	case EASING_TYPE::EXPO_OUT:
		EASING_MACRO(CEasing_Utillity::ExpoOut);
		break;
	case EASING_TYPE::EXPO_INOUT:
		EASING_MACRO(CEasing_Utillity::ExpoInOut);
		break;
	case EASING_TYPE::CIRC_IN:
		EASING_MACRO(CEasing_Utillity::CircIn);
		break;
	case EASING_TYPE::CIRC_OUT:
		EASING_MACRO(CEasing_Utillity::CircOut);
		break;
	case EASING_TYPE::CIRC_INOUT:
		EASING_MACRO(CEasing_Utillity::CircInOut);
		break;
	case EASING_TYPE::ELASTIC_IN:
		EASING_MACRO(CEasing_Utillity::ElasticIn);
		break;
	case EASING_TYPE::ELASTIC_OUT:
		EASING_MACRO(CEasing_Utillity::ElasticOut);
		break;
	case EASING_TYPE::ELASTIC_INOUT:
		EASING_MACRO(CEasing_Utillity::ElasticInOut);
		break;
	case EASING_TYPE::BOUNCE_IN:
		EASING_MACRO(CEasing_Utillity::BounceIn);
		break;
	case EASING_TYPE::BOUNCE_OUT:
		EASING_MACRO(CEasing_Utillity::BounceOut);
		break;
	default:
		return;
	}
#undef EASING_MACRO
}

void CEffect_Rect::Apply_Easing(_float2& vTarget, EASING_TYPE eEasingType, _vector vStartPoint, _vector vTargetPoint, _float fElapsedTime, _float fTotalTime)
{
	// Macro defined for convenience. Undefined later.
#define EASING_MACRO(FunctionName)\
XMStoreFloat2(&vTarget, FunctionName(vStartPoint, vTargetPoint, fElapsedTime, fTotalTime));

	switch (eEasingType)
	{
	case EASING_TYPE::LINEAR:
		EASING_MACRO(CEasing_Utillity::Linear);
		break;
	case EASING_TYPE::QUAD_IN:
		EASING_MACRO(CEasing_Utillity::QuadIn);
		break;
	case EASING_TYPE::QUAD_OUT:
		EASING_MACRO(CEasing_Utillity::QuadOut);
		break;
	case EASING_TYPE::QUAD_INOUT:
		EASING_MACRO(CEasing_Utillity::QuadInOut);
		break;
	case EASING_TYPE::CUBIC_IN:
		EASING_MACRO(CEasing_Utillity::CubicIn);
		break;
	case EASING_TYPE::CUBIC_OUT:
		EASING_MACRO(CEasing_Utillity::CubicOut);
		break;
	case EASING_TYPE::CUBIC_INOUT:
		EASING_MACRO(CEasing_Utillity::CubicInOut);
		break;
	case EASING_TYPE::QUART_IN:
		EASING_MACRO(CEasing_Utillity::QuartIn);
		break;
	case EASING_TYPE::QUART_OUT:
		EASING_MACRO(CEasing_Utillity::QuartOut);
		break;
	case EASING_TYPE::QUART_INOUT:
		EASING_MACRO(CEasing_Utillity::QuartInOut);
		break;
	case EASING_TYPE::QUINT_IN:
		EASING_MACRO(CEasing_Utillity::QuintIn);
		break;
	case EASING_TYPE::QUINT_OUT:
		EASING_MACRO(CEasing_Utillity::QuintOut);
		break;
	case EASING_TYPE::QUINT_INOUT:
		EASING_MACRO(CEasing_Utillity::QuintInOut);
		break;
	case EASING_TYPE::SINE_IN:
		EASING_MACRO(CEasing_Utillity::SineIn);
		break;
	case EASING_TYPE::SINE_OUT:
		EASING_MACRO(CEasing_Utillity::SineOut);
		break;
	case EASING_TYPE::SINE_INOUT:
		EASING_MACRO(CEasing_Utillity::SineInOut);
		break;
	case EASING_TYPE::EXPO_IN:
		EASING_MACRO(CEasing_Utillity::ExpoIn);
		break;
	case EASING_TYPE::EXPO_OUT:
		EASING_MACRO(CEasing_Utillity::ExpoOut);
		break;
	case EASING_TYPE::EXPO_INOUT:
		EASING_MACRO(CEasing_Utillity::ExpoInOut);
		break;
	case EASING_TYPE::CIRC_IN:
		EASING_MACRO(CEasing_Utillity::CircIn);
		break;
	case EASING_TYPE::CIRC_OUT:
		EASING_MACRO(CEasing_Utillity::CircOut);
		break;
	case EASING_TYPE::CIRC_INOUT:
		EASING_MACRO(CEasing_Utillity::CircInOut);
		break;
	case EASING_TYPE::ELASTIC_IN:
		EASING_MACRO(CEasing_Utillity::ElasticIn);
		break;
	case EASING_TYPE::ELASTIC_OUT:
		EASING_MACRO(CEasing_Utillity::ElasticOut);
		break;
	case EASING_TYPE::ELASTIC_INOUT:
		EASING_MACRO(CEasing_Utillity::ElasticInOut);
		break;
	case EASING_TYPE::BOUNCE_IN:
		EASING_MACRO(CEasing_Utillity::BounceIn);
		break;
	case EASING_TYPE::BOUNCE_OUT:
		EASING_MACRO(CEasing_Utillity::BounceOut);
		break;
	default:
		return;
	}
#undef EASING_MACRO
}

void CEffect_Rect::Apply_Easing
(
	_float & fTarget
	, EASING_TYPE eEasingType
	, _float fStartPoint
	, _float fTargetPoint
	, _float fElapsedTime
	, _float fTotalTime
)
{
	// Macro defined for convenience. Undefined later.
#define EASING_MACRO(FunctionName)\
fTarget = FunctionName(fStartPoint, fTargetPoint, fElapsedTime, fTotalTime);

	switch (eEasingType)
	{
	case EASING_TYPE::LINEAR:
		EASING_MACRO(CEasing_Utillity::Linear);
		break;
	case EASING_TYPE::QUAD_IN:
		EASING_MACRO(CEasing_Utillity::QuadIn_Float);
		break;
	case EASING_TYPE::QUAD_OUT:
		EASING_MACRO(CEasing_Utillity::QuadOut);
		break;
	case EASING_TYPE::QUAD_INOUT:
		EASING_MACRO(CEasing_Utillity::QuadInOut);
		break;
	case EASING_TYPE::CUBIC_IN:
		EASING_MACRO(CEasing_Utillity::CubicIn);
		break;
	case EASING_TYPE::CUBIC_OUT:
		EASING_MACRO(CEasing_Utillity::CubicOut);
		break;
	case EASING_TYPE::CUBIC_INOUT:
		EASING_MACRO(CEasing_Utillity::CubicInOut);
		break;
	case EASING_TYPE::QUART_IN:
		EASING_MACRO(CEasing_Utillity::QuartIn);
		break;
	case EASING_TYPE::QUART_OUT:
		EASING_MACRO(CEasing_Utillity::QuartOut);
		break;
	case EASING_TYPE::QUART_INOUT:
		EASING_MACRO(CEasing_Utillity::QuartInOut);
		break;
	case EASING_TYPE::QUINT_IN:
		EASING_MACRO(CEasing_Utillity::QuintIn);
		break;
	case EASING_TYPE::QUINT_OUT:
		EASING_MACRO(CEasing_Utillity::QuintOut);
		break;
	case EASING_TYPE::QUINT_INOUT:
		EASING_MACRO(CEasing_Utillity::QuintInOut);
		break;
	case EASING_TYPE::SINE_IN:
		EASING_MACRO(CEasing_Utillity::SineIn);
		break;
	case EASING_TYPE::SINE_OUT:
		EASING_MACRO(CEasing_Utillity::SineOut);
		break;
	case EASING_TYPE::SINE_INOUT:
		EASING_MACRO(CEasing_Utillity::SineInOut);
		break;
	case EASING_TYPE::EXPO_IN:
		EASING_MACRO(CEasing_Utillity::ExpoIn);
		break;
	case EASING_TYPE::EXPO_OUT:
		EASING_MACRO(CEasing_Utillity::ExpoOut);
		break;
	case EASING_TYPE::EXPO_INOUT:
		EASING_MACRO(CEasing_Utillity::ExpoInOut);
		break;
	case EASING_TYPE::CIRC_IN:
		EASING_MACRO(CEasing_Utillity::CircIn);
		break;
	case EASING_TYPE::CIRC_OUT:
		EASING_MACRO(CEasing_Utillity::CircOut);
		break;
	case EASING_TYPE::CIRC_INOUT:
		EASING_MACRO(CEasing_Utillity::CircInOut);
		break;
	case EASING_TYPE::ELASTIC_IN:
		EASING_MACRO(CEasing_Utillity::ElasticIn);
		break;
	case EASING_TYPE::ELASTIC_OUT:
		EASING_MACRO(CEasing_Utillity::ElasticOut);
		break;
	case EASING_TYPE::ELASTIC_INOUT:
		EASING_MACRO(CEasing_Utillity::ElasticInOut);
		break;
	case EASING_TYPE::BOUNCE_IN:
		EASING_MACRO(CEasing_Utillity::BounceIn);
		break;
	case EASING_TYPE::BOUNCE_OUT:
		EASING_MACRO(CEasing_Utillity::BounceOut);
		break;
	default:
		return;
	}
#undef EASING_MACRO
}

const _bool CEffect_Rect::Is_Sprite() const
{
	return (PASS_SPRITE_BLACKDISCARD == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_ALPHADISCARD == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_ALPHADISCARD_SOFT == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_BLACKDISCARD_SOFT == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_BLACKDISCARD_SPECIAL == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_ALPHADISCARD_SPECIAL == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_ALPHADISCARD_SOFT_SPECIAL == m_tEffectParticleDesc.iShaderPassIndex)
		||
		(PASS_SPRITE_BLACKDISCARD_SOFT_SPECIAL == m_tEffectParticleDesc.iShaderPassIndex);
}

const _bool CEffect_Rect::Check_Option1(const EFFECTPARTICLE_DESC::ParticleOption1 eOption) const
{
	return (m_tEffectParticleDesc.byParticleOption1 & (_ubyte)eOption) ? true : false;
}

void CEffect_Rect::TurnOn_Option1(const EFFECTPARTICLE_DESC::ParticleOption1 eOption)
{
	m_tEffectParticleDesc.byParticleOption1 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option1(const EFFECTPARTICLE_DESC::ParticleOption1 eOption)
{
	m_tEffectParticleDesc.byParticleOption1 &= ~(_ubyte)eOption;
}

#ifdef _DEBUG
void CEffect_Rect::Tool_ToggleOption1(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::ParticleOption1 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option1(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option1(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option1(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}
#endif // _DEBUG

#ifdef _DEBUG
#ifdef _JOJO_EFFECT_TOOL_
void CEffect_Rect::Show_ShaderPasses()
{
	for (auto iter : GET_SINGLE(CJoJoParticleShaderManager)->m_ParticleShaderInfos)
	{
		ImGui::Text("[%d]", iter.iShaderPassNumber); ImGui::SameLine();
		switch (iter.eRectType)
		{
		case JJ_PARTICLE_SHADER_INFO::PARTICLE_RECT_TYPE::DEFAULT:
			ImGui::TextColored(ImVec4{ 0.0705f, 0.f, 0.6510f, 1.f }, "Default");
			break;
		case JJ_PARTICLE_SHADER_INFO::PARTICLE_RECT_TYPE::SPRITE:
			ImGui::TextColored(ImVec4{ 0.6510f, 0.1490f, 0.1490f, 1.f }, "Sprite");
			break;
		default:
			assert(0);
		}

		ImGui::SameLine();

		switch (iter.eDiscardType)
		{
		case JJ_PARTICLE_SHADER_INFO::PARTICLE_DISCARD_TYPE::DISCARD_ALPHA:
			ImGui::TextColored(ImVec4{ 0.3333f, 0.3333f, 0.3333f, 1.f }, "Alpha Discard");
			break;
		case JJ_PARTICLE_SHADER_INFO::PARTICLE_DISCARD_TYPE::DISCARD_BLACK:
			ImGui::TextColored(ImVec4{ 0.f, 0.f, 0.f, 1.f }, "Black Discard");
			break;
		default:
			assert(0);
		}

		switch (iter.bSoftRendering)
		{
		case true:
			ImGui::SameLine();
			ImGui::TextColored(ImVec4{ 0.f, 0.8f, 0.f, 1.f }, "Soft");
			break;
		case false:
			break;
		}

		switch (iter.bSpecialRendering)
		{
		case true:
			ImGui::SameLine();
			ImGui::TextColored(ImVec4{ 0.8f, 0.f, 0.8f, 1.f }, "Special");
			break;
		case false:
			break;
		}
	}
}
#endif // _JOJO_EFFECT_TOOL_

void CEffect_Rect::Tool_Spawn_Life_Time()
{
	ImGui::Text("Init Time"); ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##Init_Time", &m_tEffectParticleDesc.fInitTime, 0.1f);

	Tool_ToggleOption1("Min Max Spawn Time", "##Is_MinMaxSpawnTime", EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime);
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnTime))
	{
		ImGui::Text("Min Spawn Time"); ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##Min_Spawn_Time", &m_tEffectParticleDesc.fMinSpawnTime, 0.1f);

		ImGui::Text("Max Spawn Time"); ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##Max_Spawn_Time", &m_tEffectParticleDesc.fMaxSpawnTime, 0.1f);
	}
	else
	{
		ImGui::Text("Spawn Time"); ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##Min_Spawn_Time", &m_tEffectParticleDesc.fMinSpawnTime, 0.1f);

		m_tEffectParticleDesc.fMaxSpawnTime = 0.f;
	}

	Tool_ToggleOption1("Min Max Life Time", "##Is_MinMaxLifeTime", EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime);
	if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_LifeTime))
	{
		ImGui::Text("Min Life Time"); ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##Min_Life_Time", &m_tEffectParticleDesc.fMinLifeTime, 0.1f);

		ImGui::Text("Max Life Time"); ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##Max_Life_Time", &m_tEffectParticleDesc.fMaxLifeTime, 0.1f);
	}
	else
	{
		ImGui::Text("Life Time"); ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##Min_Life_Time", &m_tEffectParticleDesc.fMinLifeTime, 0.1f);

		m_tEffectParticleDesc.fMaxLifeTime = 0.f;
	}
}

void CEffect_Rect::Tool_Boner()
{
	weak_ptr<CPreviewAnimationModel> pPreviewModel(GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel());

	if (ImGui::Button("Refresh##Get_Bone_List"))
	{
		m_pParentModel = pPreviewModel.lock()->Get_CurrentModel();
		m_pParentTransformCom = pPreviewModel.lock()->Get_Transform();
		m_AllBoneNames = GET_SINGLE(CWindow_AnimationModelView)->Get_AllBoneNames();
	}

	if (0 == m_AllBoneNames.size())
		ImGui::Text("No Bones!");
	else
	{
		if (ImGui::BeginListBox("Bone List - Particle"))
		{
			for (_int n(0); n < m_AllBoneNames.size(); n++)
			{
				const _bool is_selected = (m_iCurrentBoneIndex == n);
				if (ImGui::Selectable(m_AllBoneNames[n].c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					m_strBoneName = m_AllBoneNames[n];
				}
				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
	}

	if (ImGui::Button("Bind##Bind_To_Bone"))
	{
		m_pBoneNode = m_pParentModel.lock()->Find_BoneNode(m_strBoneName);
		if (nullptr == m_pBoneNode.lock())
		{
			MSG_BOX("Invalid Bone Name!");
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

void CEffect_Rect::Tool_Position()
{
	if (ImGui::TreeNode("Spawn Position"))
	{
		Tool_ToggleOption1("Min Max", "##Is_MinMaxSpawnPos", EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos);

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnPos))
		{
			ImGui::Text("Min Spawn Position"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Spawn_Position", &m_tEffectParticleDesc.vMinSpawnPosition.x, 0.01f, 0.f, 0.f, "%.5f");

			ImGui::Text("Max Spawn Position"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Max_Spawn_Position", &m_tEffectParticleDesc.vMaxSpawnPosition.x, 0.01f, 0.f, 0.f, "%.5f");
		}
		else
		{
			ImGui::Text("Spawn Position"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Spawn_Position", &m_tEffectParticleDesc.vMinSpawnPosition.x, 0.01f, 0.f, 0.f, "%.5f");

			m_tEffectParticleDesc.vMaxSpawnPosition = m_tEffectParticleDesc.vMinSpawnPosition;
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Spawn Offset Direction"))
	{
		Tool_ToggleOption1("Min Max", "##Is_MinMaxSpawnOffsetDirection", EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection);

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetDirection))
		{
			ImGui::Text("Min Offset Direction");
			if (ImGui::Button("Default##Default_Min_Spawn_Offset_Direction"))
				XMStoreFloat3(&m_tEffectParticleDesc.vMinSpawnOffsetDirection, XMVectorSet(-1.f, -1.f, -1.f, 0.f));

			ImGui::SameLine();
			ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Offset_Direction", &m_tEffectParticleDesc.vMinSpawnOffsetDirection.x, 0.01f, 0.f, 0.f, "%.5f");

			ImGui::Text("Max Offset Direction");
			if (ImGui::Button("Default##Default_Max_Spawn_Offset_Direction"))
				XMStoreFloat3(&m_tEffectParticleDesc.vMaxSpawnOffsetDirection, XMVectorSet(1.f, 1.f, 1.f, 0.f));

			ImGui::SameLine();
			ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Max_Offset_Direction", &m_tEffectParticleDesc.vMaxSpawnOffsetDirection.x, 0.01f, 0.f, 0.f, "%.5f");
		}
		else
		{
			ImGui::Text("Offset Direction");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Offset_Direction", &m_tEffectParticleDesc.vMinSpawnOffsetDirection.x, 0.01f, 0.f, 0.f, "%.5f");

			m_tEffectParticleDesc.vMaxSpawnOffsetDirection = m_tEffectParticleDesc.vMinSpawnOffsetDirection;
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Spawn Offset Range"))
	{
		Tool_ToggleOption1("Min Max", "##Is_MinMaxSpawnOffsetRange", EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange);

		if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Use_MinMax_SpawnOffsetRange))
		{
			ImGui::Text("Min Offset Range"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Offset_Range", &m_tEffectParticleDesc.vMinSpawnOffsetRange.x, 0.01f, 0.f, 0.f, "%.5f");

			ImGui::Text("Max Offset Range"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Max_Offset_Range", &m_tEffectParticleDesc.vMaxSpawnOffsetRange.x, 0.01f, 0.f, 0.f, "%.5f");
		}
		else
		{
			ImGui::Text("Offset Range"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Offset_Range", &m_tEffectParticleDesc.vMinSpawnOffsetRange.x, 0.01f, 0.f, 0.f, "%.5f");

			m_tEffectParticleDesc.vMaxSpawnOffsetRange = m_tEffectParticleDesc.vMinSpawnOffsetRange;
		}

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Position_Easing()
{
	if (ImGui::TreeNode("Easing Position"))
	{
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

		if (ImGui::BeginListBox("##EasingType_Position"))
		{
			for (int n(0); n < IM_ARRAYSIZE(EasingItems); n++)
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

		ImGui::Text("Easing Total Time"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat("##Position_Total_Easing_Time", &m_tEffectParticleDesc.fSpeedEasingTotalTime, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Min Goal Offset Position"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Goal_Offset_Position", &m_tEffectParticleDesc.vMinSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Goal Offset Position"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Goal_Offset_Position", &m_tEffectParticleDesc.vMaxSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Speed()
{
	if (ImGui::TreeNode("Start Speed"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_StartSpeed", &m_tEffectParticleDesc.bIsMinMaxSame_StartSpeed);

		if (m_tEffectParticleDesc.bIsMinMaxSame_StartSpeed)
			m_tEffectParticleDesc.vMaxSpeed = m_tEffectParticleDesc.vMinSpeed;

		ImGui::Text("Min Start Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Speed", &m_tEffectParticleDesc.vMinSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Start Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Speed", &m_tEffectParticleDesc.vMaxSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Speed Force"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_SpeedForce", &m_tEffectParticleDesc.bIsMinMaxSame_SpeedForce);

		if (m_tEffectParticleDesc.bIsMinMaxSame_SpeedForce)
			m_tEffectParticleDesc.vMaxSpeedForce = m_tEffectParticleDesc.vMinSpeedForce;

		ImGui::Text("Min Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Speed_Force", &m_tEffectParticleDesc.vMinSpeedForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Speed_Force", &m_tEffectParticleDesc.vMaxSpeedForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Speed Limit"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_SpeedLimit", &m_tEffectParticleDesc.bIsMinMaxSame_SpeedLimit);

		if (m_tEffectParticleDesc.bIsMinMaxSame_SpeedLimit)
			m_tEffectParticleDesc.vMaxLimitSpeed = m_tEffectParticleDesc.vMinLimitSpeed;

		ImGui::Text("Min Limit Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Limit_Speed", &m_tEffectParticleDesc.vMinLimitSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Limit Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Limit_Speed", &m_tEffectParticleDesc.vMaxLimitSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Speed_Easing()
{
	if (ImGui::TreeNode("Speed Easing List"))
	{
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

		if (ImGui::BeginListBox("##EasingType_Speed"))
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

		ImGui::TreePop();
	}

	ImGui::Text("Total Time"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat("##Speed_Total_Easing_Time", &m_tEffectParticleDesc.fSpeedEasingTotalTime, 0.01f, 0.f, 0.f, "%.5f");

	ImGui::Text("Start Speed"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat3("##Min_Limit_Speed", &m_tEffectParticleDesc.vMinLimitSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

	ImGui::Text("Target Speed"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat3("##Max_Limit_Speed", &m_tEffectParticleDesc.vMaxLimitSpeed.x, 0.1f, 0.f, 0.f, "%.5f");
}

void CEffect_Rect::Tool_Rotation()
{
	if (ImGui::TreeNode("Start Rotation"))
	{
		ImGui::Text("Min = Max"); ImGui::SameLine();
		ImGui::Checkbox("##Is_MinMaxSame_StartRotation", &m_tEffectParticleDesc.bIsMinMaxSame_StartRotation);

		if (m_tEffectParticleDesc.bIsMinMaxSame_StartRotation)
			m_tEffectParticleDesc.vMaxStartRotation = m_tEffectParticleDesc.vMinStartRotation;

		ImGui::Text("Min Start Rotation");
		if (ImGui::Button("Default##Defulat_Min_Start_Rotation"))
			m_tEffectParticleDesc.vMinStartRotation.x = m_tEffectParticleDesc.vMinStartRotation.y = m_tEffectParticleDesc.vMinStartRotation.z = -3.14f;

		ImGui::SameLine(); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Start_Rotation", &m_tEffectParticleDesc.vMinStartRotation.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Start Rotation");
		if (ImGui::Button("Default##Defulat_Max_Start_Rotation"))
			m_tEffectParticleDesc.vMaxStartRotation.x = m_tEffectParticleDesc.vMaxStartRotation.y = m_tEffectParticleDesc.vMaxStartRotation.z = 3.14f;

		ImGui::SameLine(); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Start_Rotation", &m_tEffectParticleDesc.vMaxStartRotation.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Rotation Speed"))
	{
		ImGui::Text("Min = Max"); ImGui::SameLine();
		ImGui::Checkbox("##Is_MinMaxSame_RotationSpeed", &m_tEffectParticleDesc.bIsMinMaxSame_RotationSpeed);

		if (m_tEffectParticleDesc.bIsMinMaxSame_RotationSpeed)
			m_tEffectParticleDesc.vMaxRotationSpeed = m_tEffectParticleDesc.vMinRotationSpeed;

		ImGui::Text("Min Rotation Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Rotation_Speed", &m_tEffectParticleDesc.vMinRotationSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Rotation Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Rotation_Speed", &m_tEffectParticleDesc.vMaxRotationSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Rotation Force"))
	{
		ImGui::Text("Min = Max"); ImGui::SameLine();
		ImGui::Checkbox("##Is_MinMaxSame_RotationForce", &m_tEffectParticleDesc.bIsMinMaxSame_RotationForce);

		if (m_tEffectParticleDesc.bIsMinMaxSame_RotationForce)
			m_tEffectParticleDesc.vMaxRotationForce = m_tEffectParticleDesc.vMinRotationForce;

		ImGui::Text("Min Rotation Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Rotation_Force", &m_tEffectParticleDesc.vMinRotationForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Rotation Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Rotation_Force", &m_tEffectParticleDesc.vMaxRotationForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Rotation Limit"))
	{
		ImGui::Text("Min = Max"); ImGui::SameLine();
		ImGui::Checkbox("##Is_MinMaxSame_RotationLimit", &m_tEffectParticleDesc.bIsMinMaxSame_RotationLimit);

		if (m_tEffectParticleDesc.bIsMinMaxSame_RotationLimit)
			m_tEffectParticleDesc.vMaxLimitRotation = m_tEffectParticleDesc.vMinLimitRotation;

		ImGui::Text("Min Limit Rotation"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Min_Limit_Rotation", &m_tEffectParticleDesc.vMinLimitRotation.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Limit Rotation"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Max_Limit_Rotation", &m_tEffectParticleDesc.vMaxLimitRotation.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Rotation_Easing()
{
	if (ImGui::TreeNode("Easing Rotation List"))
	{
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

		if (ImGui::BeginListBox("EasingType_Rotation"))
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

		ImGui::TreePop();
	}

	ImGui::Text("Total Time"); ImGui::SameLine();
	ImGui::DragFloat("##Rotation_Total_Easing_Time", &m_tEffectParticleDesc.fRotationEasingTotalTime, 0.01f, -100.f, 100.f, "%.5f");

	ImGui::Text("Start Rotation"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat3("##Min_Limit_Rotation", &m_tEffectParticleDesc.vMinLimitRotation.x, 0.01f, 0.f, 0.f, "%.5f");

	ImGui::Text("Target Rotation"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat3("##Max_Limit_Rotation", &m_tEffectParticleDesc.vMaxLimitRotation.x, 0.01f, 0.f, 0.f, "%.5f");
}

void CEffect_Rect::Tool_Scale()
{
	if (ImGui::TreeNode("Start Scale"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_StartScale", &m_tEffectParticleDesc.bIsMinMaxSame_StartScale);

		if (m_tEffectParticleDesc.bIsMinMaxSame_StartScale)
			m_tEffectParticleDesc.vMaxStartScale = m_tEffectParticleDesc.vMinStartScale;

		ImGui::Text("Min Start Scale"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Min_Start_Scale", &m_tEffectParticleDesc.vMinStartScale.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Start Scale"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Max_Start_Scale", &m_tEffectParticleDesc.vMaxStartScale.x, 0.1f, 0.f, 0.f, "%.5f");

		if (m_tEffectParticleDesc.bSquareScale)
		{
			m_tEffectParticleDesc.vMinStartScale.y = m_tEffectParticleDesc.vMinStartScale.x;
			m_tEffectParticleDesc.vMaxStartScale.y = m_tEffectParticleDesc.vMaxStartScale.x;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Scale Speed"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ScaleSpeed", &m_tEffectParticleDesc.bIsMinMaxSame_ScaleSpeed);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ScaleSpeed)
			m_tEffectParticleDesc.vMaxScaleSpeed = m_tEffectParticleDesc.vMinScaleSpeed;

		ImGui::Text("Min Scale Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Min_Scale_Speed", &m_tEffectParticleDesc.vMinScaleSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Scale Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Max_Scale_Speed", &m_tEffectParticleDesc.vMaxScaleSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

		if (m_tEffectParticleDesc.bSquareScale)
		{
			m_tEffectParticleDesc.vMinScaleSpeed.y = m_tEffectParticleDesc.vMinScaleSpeed.x;
			m_tEffectParticleDesc.vMaxScaleSpeed.y = m_tEffectParticleDesc.vMaxScaleSpeed.x;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Scale Force"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ScaleForce", &m_tEffectParticleDesc.bIsMinMaxSame_ScaleForce);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ScaleForce)
			m_tEffectParticleDesc.vMaxScaleForce = m_tEffectParticleDesc.vMinScaleForce;

		ImGui::Text("Min Scale Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Min_Scale_Force", &m_tEffectParticleDesc.vMinScaleForce.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Scale Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Max_Scale_Force", &m_tEffectParticleDesc.vMaxScaleForce.x, 0.1f, 0.f, 0.f, "%.5f");

		if (m_tEffectParticleDesc.bSquareScale)
		{
			m_tEffectParticleDesc.vMinScaleForce.y = m_tEffectParticleDesc.vMinScaleForce.x;
			m_tEffectParticleDesc.vMaxScaleForce.y = m_tEffectParticleDesc.vMaxScaleForce.x;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Scale Limit"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ScaleLimit", &m_tEffectParticleDesc.bIsMinMaxSame_ScaleLimit);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ScaleLimit)
			m_tEffectParticleDesc.vMaxLimitScale = m_tEffectParticleDesc.vMinLimitScale;

		ImGui::Text("Min Limit Scale"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Min_Limit_Scale", &m_tEffectParticleDesc.vMinLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Limit Scale"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Max_Limit_Scale", &m_tEffectParticleDesc.vMaxLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");

		if (m_tEffectParticleDesc.bSquareScale)
		{
			m_tEffectParticleDesc.vMinLimitScale.y = m_tEffectParticleDesc.vMinLimitScale.x;
			m_tEffectParticleDesc.vMaxLimitScale.y = m_tEffectParticleDesc.vMaxLimitScale.x;
		}

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Scale_Easing()
{
	if (ImGui::TreeNode("Easing Scale List"))
	{
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

		if (ImGui::BeginListBox("##EasingType_Scale"))
		{
			for (int n(0); n < IM_ARRAYSIZE(EasingItems); n++)
			{
				const bool is_selected((m_tEffectParticleDesc.iScaleEasingType == n));

				if (ImGui::Selectable(EasingItems[n], is_selected))
					m_tEffectParticleDesc.iScaleEasingType = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::TreePop();
	}

	ImGui::Text("Total Time"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat("##Scale_Total_Easing_Time", &m_tEffectParticleDesc.fScaleEasingTotalTime, 0.01f, 0.f, 0.f, "%.5f");

	ImGui::Text("Start Scale"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat2("##Min_Limit_Scale", &m_tEffectParticleDesc.vMinLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");

	ImGui::Text("Target Scale"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat2("##Max_Limit_Scale", &m_tEffectParticleDesc.vMaxLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");
}

void CEffect_Rect::Tool_Sprite()
{
	if (ImGui::TreeNode("Sprite Option"))
	{
		ImGui::Checkbox("Loop Sprite##LoopSprite", &m_tEffectParticleDesc.bLoopSprite);
		ImGui::SameLine();
		ImGui::Checkbox("Stop At End##Stop_At_End_Frame", &m_tEffectParticleDesc.bStopAtEndFrame);
		ImGui::NewLine();

		ImGui::InputInt("NumFramesX", &m_tEffectParticleDesc.iNumFrameX);
		ImGui::InputInt("NumFramesY", &m_tEffectParticleDesc.iNumFrameY);
		ImGui::InputFloat("FrameSpeed", &m_tEffectParticleDesc.fSpriteSpeed);

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Color()
{
	if (ImGui::TreeNode("Start Color"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_StartColor", &m_tEffectParticleDesc.bIsMinMaxSame_StartColor);

		if (m_tEffectParticleDesc.bIsMinMaxSame_StartColor)
			m_tEffectParticleDesc.vMaxStartColor = m_tEffectParticleDesc.vMinStartColor;

		ImGui::Text("Min Start Color"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Min_Start_Color", &m_tEffectParticleDesc.vMinStartColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_Logarithmic);
		if (ImGui::TreeNode("Min Start Color Picker"))
		{
			ImGui::ColorPicker4("##Min_Start_Color", &m_tEffectParticleDesc.vMinStartColor.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);
			ImGui::TreePop();
		}

		ImGui::Text("Max Start Color"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Max_Start_Color", &m_tEffectParticleDesc.vMaxStartColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_Logarithmic);
		if (ImGui::TreeNode("Max Start Color Picker"))
		{
			ImGui::ColorPicker4("##Max_Start_Color", &m_tEffectParticleDesc.vMaxStartColor.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Color Speed"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ColorSpeed", &m_tEffectParticleDesc.bIsMinMaxSame_ColorSpeed);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ColorSpeed)
			m_tEffectParticleDesc.vMaxColorSpeed = m_tEffectParticleDesc.vMinColorSpeed;

		ImGui::Text("Min Color Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Min_Color_Speed", &m_tEffectParticleDesc.vMinColorSpeed.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

		ImGui::Text("Max Color Speed"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Max_Color_Speed", &m_tEffectParticleDesc.vMaxColorSpeed.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Color Force"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ColorForce", &m_tEffectParticleDesc.bIsMinMaxSame_ColorForce);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ColorForce)
			m_tEffectParticleDesc.vMaxColorForce = m_tEffectParticleDesc.vMinColorForce;

		ImGui::Text("Min Color Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Min_Color_Force", &m_tEffectParticleDesc.vMinColorForce.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

		ImGui::Text("Max Color Force"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Max_Color_Force", &m_tEffectParticleDesc.vMaxColorForce.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic);

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Color Limit"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ColorLimit", &m_tEffectParticleDesc.bIsMinMaxSame_ColorLimit);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ColorLimit)
			m_tEffectParticleDesc.vMaxColor = m_tEffectParticleDesc.vMinColor;

		ImGui::Text("Min Limit Color"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Min_Limit_Color", &m_tEffectParticleDesc.vMinColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_Logarithmic);
		if (ImGui::TreeNode("Min Limit Color Picker"))
		{
			ImGui::ColorPicker4("##Min_Limit_Color", &m_tEffectParticleDesc.vMinColor.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);
			ImGui::TreePop();
		}


		ImGui::Text("Max Limit Color"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat4("##Max_Limit_Color", &m_tEffectParticleDesc.vMaxColor.x, 0.01f, 0.f, 0.f, "%.5f", ImGuiSliderFlags_Logarithmic);
		if (ImGui::TreeNode("Max Limit Color Picker"))
		{
			ImGui::ColorPicker4("##Max_Limit_Color", &m_tEffectParticleDesc.vMaxColor.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);
			ImGui::TreePop();
		}

		ImGui::Separator();

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Color_EasingAlpha()
{
	if (ImGui::TreeNode("Easing Alpha"))
	{
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

		if (ImGui::BeginListBox("##EasingType_Alpha"))
		{
			for (int n(0); n < IM_ARRAYSIZE(EasingItems); n++)
			{
				const bool is_selected((m_tEffectParticleDesc.iAlphaEasingType == n));

				if (ImGui::Selectable(EasingItems[n], is_selected))
					m_tEffectParticleDesc.iAlphaEasingType = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::Text("Total Time"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat("##Alpha_Total_Easing_Time", &m_tEffectParticleDesc.fAlphaEasingTotalTime, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("Target Alpha"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat("##Target_Alpha", &m_tEffectParticleDesc.vMinColorSpeed.w, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Texture_Diffuse()
{
	if (ImGui::TreeNode("Diffuse"))
	{
		ImGui::Checkbox("XInverse##Is_XInverse_Diffuse_Texture", &m_tEffectParticleDesc.bDiffuseXInverse);

		ImGui::SameLine(); ImGui::Text(" | "); ImGui::SameLine();

		ImGui::Checkbox("YInverse##Is_YInverse_Diffuse_Texture", &m_tEffectParticleDesc.bDiffuseYInverse);

		ImGui::Text("Index");  ImGui::SetNextItemWidth(100.f);
		ImGui::InputInt("##Diffuse_Index", &m_tEffectParticleDesc.iDiffuseIndex);

		ImGui::Text("Start UV"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Diffuse_Start_UV", &m_tEffectParticleDesc.vDiffuseStartUV.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Speed"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Diffuse_UV_Speed", &m_tEffectParticleDesc.vDiffuseUVSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Force"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Diffuse UV_Force", &m_tEffectParticleDesc.vDiffuseUVForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Max"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Diffuse_UV_Max", &m_tEffectParticleDesc.vDiffuseUVMax.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Texture_Mask()
{
	if (ImGui::TreeNode("Mask"))
	{
		ImGui::Text("Index"); ImGui::SetNextItemWidth(100.f);
		ImGui::InputInt("##Mask_Index", &m_tEffectParticleDesc.iMaskIndex);

		ImGui::Text("Start UV"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Mask_Start_UV", &m_tEffectParticleDesc.vMaskStartUV.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Speed"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Mask_UV_Speed", &m_tEffectParticleDesc.vMaskUVSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Force"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Mask UV_Force", &m_tEffectParticleDesc.vMaskUVForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Max"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Mask_UV_Max", &m_tEffectParticleDesc.vMaskUVMax.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Texture_Noise()
{
	if (ImGui::TreeNode("Noise"))
	{
		ImGui::Text("Index"); ImGui::SetNextItemWidth(100.f);
		ImGui::InputInt("##Noise_Index", &m_tEffectParticleDesc.iNoiseIndex);

		ImGui::Text("Start UV"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Noise_Start_UV", &m_tEffectParticleDesc.vNoiseStartUV.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Speed"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Noise_UV_Speed", &m_tEffectParticleDesc.vNoiseUVSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Force"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Noise UV_Force", &m_tEffectParticleDesc.vNoiseUVForce.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::Text("UV Max"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Noise_UV_Max", &m_tEffectParticleDesc.vNoiseUVMax.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Glow()
{
	if (ImGui::TreeNode("Glow Options"))
	{
		ImGui::Text("Start Glow Color"); ImGui::SetNextItemWidth(150.f);
		ImGui::DragFloat4("##Start_Glow_Color", &m_tEffectParticleDesc.vStartGlowColor.x, 0.01f);
		if (ImGui::TreeNode("Glow Color Picker"))
		{
			ImGui::ColorPicker4("##Start_Glow_Color", &m_tEffectParticleDesc.vStartGlowColor.x, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_InputRGB);
			ImGui::TreePop();
		}

		ImGui::Text("Glow Color Speed "); ImGui::SetNextItemWidth(150.f);
		ImGui::DragFloat4("##Glow_Color_Speed", &m_tEffectParticleDesc.vGlowColorSpeed.x, 0.01f);

		ImGui::Text("Glow Color Force"); ImGui::SetNextItemWidth(150.f);
		ImGui::DragFloat4("##Glow_Color_Force", &m_tEffectParticleDesc.vGlowColorForce.x, 0.01f);

		ImGui::TreePop();
	}
}
#endif // _DEBUG

void CEffect_Rect::OnEventMessage(_uint iArg)
{
#ifdef _DEBUG
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Effect_Particle"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			if (ImGui::Button("Copy##Copy_Particle_Info"))
				GET_SINGLE(CGameManager)->Store_ParticleInfo(m_tEffectParticleDesc);

			ImGui::SameLine();

			if (ImGui::Button("Paste##Paste_Particle_Info"))
			{
				m_tEffectParticleDesc = GET_SINGLE(CGameManager)->Get_StoredParticleInfo();
				ReBake_EditParticle();
			}

			ImGui::Text("Max Instance"); ImGui::SameLine();
			if (ImGui::DragInt("##Max_Instance", &m_tEffectParticleDesc.iMaxInstance, 1, 0, 0, "%d", ImGuiSliderFlags_AlwaysClamp))
			{
				ReBake_EditParticle();
			}
			
			ImGui::Separator();

			ImGui::Checkbox("Is Looping##Is_Looping", &m_tEffectParticleDesc.bLooping);
			
			if (ImGui::CollapsingHeader("Particle Type"))
			{
				const char* items[] = { "None", "Outburst", "Attraction", "Billboard" };

				if (ImGui::BeginListBox("##ParticleType"))
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
			}
			if (ImGui::CollapsingHeader("Transform Type"))
			{
				const char* Transform_items[] = { "Static", "Just Spawn", "Child" };

				if (ImGui::BeginListBox("##TransformType"))
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
			}

			Tool_ToggleOption1("Attraction", "##Is_Attraction", EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction);

			if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Attraction))
			{
				if (ImGui::TreeNode("Attraction##Attraction_Option"))
				{
					ImGui::Text("Goal Position"); ImGui::SetNextItemWidth(300.f);
					ImGui::DragFloat3("##Goal_Position", &m_tEffectParticleDesc.vGoalPosition.x, 0.01f, 0.f, 0.f, "%.5f");

					ImGui::TreePop();
				}
			}

			if (ImGui::CollapsingHeader("Animation Sync"))
			{
				ImGui::Checkbox("Sync Animation##Is_Sync_Animation", &m_tEffectParticleDesc.bSyncAnimation);
				ImGui::SetNextItemWidth(150.f);
				ImGui::InputInt("Sync Animation Key", &m_tEffectParticleDesc.iSyncAnimationKey);
			}

			if (ImGui::CollapsingHeader("Spawn & Life Time"))
				Tool_Spawn_Life_Time();
			
			ImGui::Separator();
			Tool_ToggleOption1("Is Move Look", "##Is_MoveLook", EFFECTPARTICLE_DESC::ParticleOption1::Is_MoveLook);
			ImGui::Separator();

			ImGui::Checkbox("Use Gravity##Use_Gravity", &m_tEffectParticleDesc.bUseGravity);
			if (m_tEffectParticleDesc.bUseGravity)
			{
				if (ImGui::TreeNode("Gravity Options"))
				{
					ImGui::Text("Gravity Force"); ImGui::SetNextItemWidth(300.f);
					ImGui::DragFloat3("##Gravity_Force", &m_tEffectParticleDesc.vGravityForce.x, 0.1f, 0.f, 0.f, "%.5f");

					ImGui::TreePop();
				}
			}

			ImGui::Separator();

			if (ImGui::CollapsingHeader("Boner"))
			{
				Tool_ToggleOption1("Is Boner", "##Is_Boner", EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner);

				if (Check_Option1(EFFECTPARTICLE_DESC::ParticleOption1::Is_Boner))
					Tool_Boner();
				else
				{
					m_pBoneNode.reset();
					m_strBoneName.clear();
				}
			}
			if (ImGui::CollapsingHeader("Position"))
			{
				ImGui::Checkbox("Easing Position##Is_Easing_Position", &m_tEffectParticleDesc.bEasingPosition);
				ImGui::Separator();
				Tool_Position();
			}
			if (!m_tEffectParticleDesc.bEasingPosition)
			{
				if (ImGui::CollapsingHeader("Speed"))
				{
					ImGui::Checkbox("Apply Easing##Is_Easing_Speed", &m_tEffectParticleDesc.bEasingSpeed);

					if (!m_tEffectParticleDesc.bEasingSpeed)
						Tool_Speed();
					else
						Tool_Speed_Easing();
				}
			}
			else
				Tool_Position_Easing();

			if (_int(PARTICLETYPE::OUTBURST) != m_tEffectParticleDesc.iParticleType)
			{
				if (ImGui::CollapsingHeader("Rotation"))
				{
					ImGui::Checkbox("Apply Easing##Is_Easing_Rotation", &m_tEffectParticleDesc.bEasingRotation);

					ImGui::Separator();

					if (!m_tEffectParticleDesc.bEasingRotation)
						Tool_Rotation();
					else
						Tool_Rotation_Easing();
				}
			}
			if (ImGui::CollapsingHeader("Scale"))
			{

#ifdef _JOJO_EFFECT_TOOL_
				
				ImGui::RadioButton("None##Is_None_Scale_Type", &m_iScaleType, m_iScaleType_None);
				ImGui::SameLine();
				ImGui::RadioButton("Square Scale##Is_Square_Scale", &m_iScaleType, m_iScaleType_Square);
				ImGui::SameLine();
				ImGui::RadioButton("Ratio Scale##Is_Ratio_Scale", &m_iScaleType, m_iScaleType_Ratio);
				switch (m_iScaleType)
				{
				case 1:
					m_tEffectParticleDesc.bSquareScale = true;
					m_tEffectParticleDesc.bRatioScale = false;
					break;
				case 2:
					m_tEffectParticleDesc.bSquareScale = false;
					m_tEffectParticleDesc.bRatioScale = true;
					break;
				default:
					m_tEffectParticleDesc.bSquareScale = false;
					m_tEffectParticleDesc.bRatioScale = false;
					break;
				}
#else // _JOJO_EFFECT_TOOL_
				ImGui::Checkbox("Square Scale##Is_Square_Scale", &m_tEffectParticleDesc.bSquareScale);
				ImGui::SameLine();
				ImGui::Checkbox("Ratio Scale##Is_Ratio_Scale", &m_tEffectParticleDesc.bRatioScale);
#endif // _JOJO_EFFECT_TOOL_

				if (m_tEffectParticleDesc.bRatioScale)
				{
					if (ImGui::TreeNode("Y = X * ?"))
					{
						ImGui::Text("Min Ratio"); ImGui::SameLine();
						ImGui::SetNextItemWidth(100.f);
						ImGui::DragFloat("##Min_Y_Scale_Ratio", &m_tEffectParticleDesc.fMinYScaleRatio, 0.1f, 0.f, 0.f, "%.5f");

						ImGui::Text("Max Ratio"); ImGui::SameLine();
						ImGui::SetNextItemWidth(100.f);
						ImGui::DragFloat("##Max_Y_Scale_Ratio", &m_tEffectParticleDesc.fMaxYScaleRatio, 0.1f, 0.f, 0.f, "%.5f");

						ImGui::TreePop();
					}
				}

				ImGui::Checkbox("Apply Easing##Is_Easing_Scale", &m_tEffectParticleDesc.bEasingScale);
				ImGui::Separator();

				if (!m_tEffectParticleDesc.bEasingScale)
					Tool_Scale();
				else
					Tool_Scale_Easing();
			}
			if (ImGui::CollapsingHeader("Colors"))
			{
				if (ImGui::TreeNode("Shader Pass"))
				{
#ifdef _JOJO_EFFECT_TOOL_
					Show_ShaderPasses();
#endif // _JOJO_EFFECT_TOOL_
					ImGui::SetNextItemWidth(100.f);
					ImGui::InputInt("Shader Pass", &m_tEffectParticleDesc.iShaderPassIndex);
					ImGui::TreePop();
				}

				ImGui::Text("Render Group");
				if (ImGui::Button("Non Alpha Effect##RenderGroup_NonAlphaEffect"))
					m_eRenderGroup = RENDERGROUP::RENDER_NONALPHA_EFFECT;

				ImGui::SameLine();

				if (ImGui::Button("Alpha Blend##RenderGroup_AlphaBlend"))
					m_eRenderGroup = RENDERGROUP::RENDER_ALPHABLEND;
				

				if (Is_Sprite())
					Tool_Sprite();

				ImGui::Text("Discard Ratio"); ImGui::SameLine(); ImGui::SetNextItemWidth(100.f);
				ImGui::DragFloat("##Discard_Ratio", &m_tEffectParticleDesc.fDiscardRatio, 0.01f, 0.f, 3.f);
				ImGui::Checkbox("Is Gray Only Use Red##Is_Gray_Only_Use_Red", &m_tEffectParticleDesc.IsGrayOnlyUseRed);
				ImGui::Checkbox("Easing Alpha##Is_Easing_Alpha", &m_tEffectParticleDesc.bEasingAlpha);
				Tool_Color();
				if (m_tEffectParticleDesc.bEasingAlpha)
					Tool_Color_EasingAlpha();
			}
			if (ImGui::CollapsingHeader("Textures"))
			{
				Tool_Texture_Diffuse();
				Tool_Texture_Mask();
				Tool_Texture_Noise();
			}
			if (ImGui::CollapsingHeader("Bloom & Glow"))
			{
				ImGui::Checkbox("Bloom##Bloom", &m_tEffectParticleDesc.bBloom);

				ImGui::SameLine(); ImGui::Text(" | "); ImGui::SameLine();

				ImGui::Checkbox("Glow##Glow", &m_tEffectParticleDesc.bGlow);

				if (m_tEffectParticleDesc.bGlow)
					Tool_Glow();

			}
		}
	}
#endif // _DEBUG
}

void CEffect_Rect::Free()
{
}

void CEffect_Rect::OnChangeAnimationKey(const _uint& In_Key)
{
	if (m_tEffectParticleDesc.iSyncAnimationKey != (_int)In_Key)
		return;

	weak_ptr<CTransform> pPreviewModelTransform = GET_SINGLE(CWindow_AnimationModelView)->Get_PreViewModel().lock()->Get_Transform();
	Reset_Effect(pPreviewModelTransform);
}

#undef PASS_SPRITE_BLACKDISCARD
#undef PASS_ALPHADISCARD
#undef PASS_BLACKDISCARD	
#undef PASS_SPRITE_ALPHADISCARD

#undef PASS_ALPHADISCARD_SOFT
#undef PASS_BLACKDISCARD_SOFT
#undef PASS_SPRITE_ALPHADISCARD_SOFT
#undef PASS_SPRITE_BLACKDISCARD_SOFT

#undef PASS_SPRITE_BLACKDISCARD_SPECIAL	
#undef PASS_ALPHADISCARD_SPECIAL			
#undef PASS_BLACKDISCARD_SPECIAL			
#undef PASS_SPRITE_ALPHADISCARD_SPECIAL

#undef PASS_ALPHADISCARD_SOFT_SPECIAL		
#undef PASS_BLACKDISCARD_SOFT_SPECIAL	
