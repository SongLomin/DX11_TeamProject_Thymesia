#include "stdafx.h"
#include "Effect_Rect.h"
#include "Client_Components.h"
#include "Window_AnimationModelView.h"
#include "Window_EffectHierarchyView.h"
#include "PreViewAnimationModel.h"
#include "Thread_Manager.h"
#include "Easing_Utillity.h"
#include "Model.h"
#include "BoneNode.h"

#include "Level.h"

#include "GameManager.h"
//#include "update_Particle.cu"
#include <imgui_impl_dx11.h>

static _int g_iThread_Scale = 500;

GAMECLASS_C(CEffect_Rect)
CLONE_C(CEffect_Rect, CGameObject)


//extern void CudaMain_UpdateParticle(float fTimeDelta, _matrix BoneMatrix, PARTICLE_DESC* input, PARTICLE_DESC* output, _int size);

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

	m_tEffectParticleDesc.vMinStartScale  = { 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartScale  = { 1.f, 1.f };
	m_tEffectParticleDesc.vMaxLimitScale  = { 1.f, 1.f };

	m_tEffectParticleDesc.fDiscardRatio   = 0.1f;
	m_tEffectParticleDesc.vMinStartColor  = { 1.f, 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartColor  = { 1.f, 1.f, 1.f, 1.f };

	m_tEffectParticleDesc.vMaxColor       = { 1.f, 1.f, 1.f, 1.f };

	m_tEffectParticleDesc.vDiffuseUVMax   = { 1.f, 1.f };
	m_tEffectParticleDesc.vMaskUVMax      = { 1.f, 1.f };
	m_tEffectParticleDesc.vNoiseUVMax     = { 1.f, 1.f };

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
#ifdef _EFFECT_TOOL_
	Initialize_ParticleShaderInfos();
#endif // _EFFECT_TOOL_

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
	CGameObject::Use_Thread(THREAD_TYPE::PRE_LATETICK);
#endif // _USE_THREAD_

	return S_OK;
}

void CEffect_Rect::Tick(_float fTimeDelta)
{
	CGameObject::CallBack_Tick(fTimeDelta);

	if (m_pVIBuffer.lock()->Get_InstanceCount() != m_tEffectParticleDesc.iMaxInstance)
		Reset_Instance(m_tEffectParticleDesc.iMaxInstance);

	Play(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex));

	/*if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
	{
		g_iThread_Scale = 100;
	}

	if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
	{
		g_iThread_Scale = 300;
	}

	if (KEY_INPUT(KEY::NUM3, KEY_STATE::TAP))
	{
		g_iThread_Scale = 500;
	}

	if (KEY_INPUT(KEY::NUM4, KEY_STATE::TAP))
	{
		g_iThread_Scale = 700;
	}

	if (KEY_INPUT(KEY::NUM5, KEY_STATE::TAP))
	{
		g_iThread_Scale = 1000;
	}

	if (KEY_INPUT(KEY::NUM6, KEY_STATE::TAP))
	{
		g_iThread_Scale = 2000;
	}

	if (KEY_INPUT(KEY::NUM7, KEY_STATE::TAP))
	{
		g_iThread_Scale = 3000;
	}*/
}

void CEffect_Rect::Thread_PreLateTick(_float fTimeDelta)
{
	/*sort(m_tParticleDescs.begin(), m_tParticleDescs.end(), [](PARTICLE_DESC& Left, PARTICLE_DESC& Right) {
		return Left.bEnable < Right.bEnable;
		});*/

	ID3D11DeviceContext* pDeferredContext = GAMEINSTANCE->Get_BeforeRenderContext();

	m_pVIBuffer.lock()->Update(m_tParticleDescs, pDeferredContext, ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType));

	GAMEINSTANCE->Release_BeforeRenderContext(pDeferredContext);
}

void CEffect_Rect::LateTick(_float fTimeDelta)
{
#ifndef _USE_THREAD_
	m_pVIBuffer.lock()->Update(m_tParticleDescs, DEVICECONTEXT, ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType));
#endif // _USE_THREAD_

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

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Boner))
	{
#ifdef _EFFECT_TOOL_
		if (pParentTransform.lock())
		{
			m_pParentModel = pParentTransform.lock()->Get_Owner().lock()->Get_Component<CModel>();
			m_pBoneNode = m_pParentModel.lock()->Find_BoneNode(m_strBoneName);
		}
#else // _EFFECT_TOOL_
		m_pParentModel = pParentTransform.lock()->Get_Owner().lock()->Get_Component<CModel>();
		m_pBoneNode = m_pParentModel.lock()->Find_BoneNode(m_strBoneName);
#endif // _EFFECT_TOOL_
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
		_matrix BoneMatrix(XMMatrixIdentity());
		if (m_pBoneNode.lock())
		{
			_float4x4 TempMat(m_pParentModel.lock()->Get_TransformationMatrix());
			_matrix ModelTranMat(XMLoadFloat4x4(&TempMat));
			BoneMatrix = m_pBoneNode.lock()->Get_CombinedMatrix() * ModelTranMat;

			BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
			BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
			BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);
		}

#ifdef _EFFECT_TOOL_
		if (pParentTransform.lock())
			m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * pParentTransform.lock()->Get_UnScaledWorldMatrix());
#else // _EFFECT_TOOL_
		m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * pParentTransform.lock()->Get_UnScaledWorldMatrix());
#endif // _EFFECT_TOOL_
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
	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Is_Sprite))
	{
		_bool bDiffMask(m_tEffectParticleDesc.byOption6 & _byte(EFFECTPARTICLE_DESC::Option6::Sprite_UseMask));
		m_pShaderCom.lock()->Set_RawValue("g_bUseMask", &bDiffMask, sizeof(_bool));

		m_pShaderCom.lock()->Set_RawValue("g_iNumFrameX", &m_tEffectParticleDesc.iNumFrameX, sizeof(_uint));
		m_pShaderCom.lock()->Set_RawValue("g_iNumFrameY", &m_tEffectParticleDesc.iNumFrameY, sizeof(_uint));
	}
#pragma endregion

#pragma region Billboard
	_vector vCamDir(GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2]);
	m_pShaderCom.lock()->Set_RawValue("g_vCamDirection", &vCamDir, sizeof(_vector));

	_bool isBillboard(m_tEffectParticleDesc.iParticleType == (_int)PARTICLETYPE::BILLBOARD);
	m_pShaderCom.lock()->Set_RawValue("g_bBillboard", &isBillboard, sizeof(_bool));
#pragma endregion

#pragma region Bloom & Glow & ShaderFlag
	_bool bBloom(Check_Option(EFFECTPARTICLE_DESC::Option6::Use_Bloom));
	m_pShaderCom.lock()->Set_RawValue("g_bBloom", &bBloom, sizeof(_bool));

	_bool bGlow(Check_Option(EFFECTPARTICLE_DESC::Option6::Use_Glow));
	m_pShaderCom.lock()->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool));

	_vector vShaderFlag;
	ZeroMemory(&vShaderFlag, sizeof(_vector));
	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_Emissive))
	{
		vShaderFlag = { 0.f, 0.f, 1.f, 0.f };
	}

	m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

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

#pragma region Particle Options
	Out_Json["ParticleOption1"] = m_tEffectParticleDesc.byOption1;
	Out_Json["ParticleOption2"] = m_tEffectParticleDesc.byOption2;
	Out_Json["ParticleOption3"] = m_tEffectParticleDesc.byOption3;
	Out_Json["ParticleOption4"] = m_tEffectParticleDesc.byOption4;
	Out_Json["ParticleOption5"] = m_tEffectParticleDesc.byOption5;
	Out_Json["ParticleOption6"] = m_tEffectParticleDesc.byOption6;
#pragma endregion // Particle Options

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Attraction))
	{
		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction) || Check_Option(EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction))
		{
			Out_Json["Goal_Range"] = m_tEffectParticleDesc.fGoalRange;
		}

		CJson_Utility::Write_Float3(Out_Json["Goal_Position"], m_tEffectParticleDesc.vGoalPosition);
	}

	Out_Json["ShaderPassIndex"] = m_tEffectParticleDesc.iShaderPassIndex;

	Out_Json["Sync_Animation"] = m_tEffectParticleDesc.bSyncAnimation;
	Out_Json["Sync_AnimationKey"] = m_tEffectParticleDesc.iSyncAnimationKey;

#pragma region Life Time
	Out_Json["Init_Time"] = m_tEffectParticleDesc.fInitTime;

	Out_Json["Min_Spawn_Time"] = m_tEffectParticleDesc.fMinSpawnTime;
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnTime))
		Out_Json["Max_Spawn_Time"] = m_tEffectParticleDesc.fMaxSpawnTime;

	Out_Json["Min_Life_Time"] = m_tEffectParticleDesc.fMinLifeTime;
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_LifeTime))
		Out_Json["Max_Life_Time"] = m_tEffectParticleDesc.fMaxLifeTime;
#pragma endregion

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Boner))
	{
		if (!m_strBoneName.empty())
			Out_Json["Bone_Name"] = m_strBoneName;
	}

#pragma region Spawn Position
	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Position"], m_tEffectParticleDesc.vMinSpawnPosition);
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnPos))
		CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);

	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMinSpawnOffsetDirection);
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetDirection))
		CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);

	CJson_Utility::Write_Float3(Out_Json["Min_Spawn_Offset_Range"], m_tEffectParticleDesc.vMinSpawnOffsetRange);
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetRange))
		CJson_Utility::Write_Float3(Out_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);
#pragma endregion // Spawn Position

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Gravity))
		CJson_Utility::Write_Float3(Out_Json["Gravity_Force"], m_tEffectParticleDesc.vGravityForce);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed))
	{
		CJson_Utility::Write_Float3(Out_Json["Min_Speed"], m_tEffectParticleDesc.vMinStartSpeed);
		CJson_Utility::Write_Float3(Out_Json["Max_Speed"], m_tEffectParticleDesc.vMaxStartSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
	{
		CJson_Utility::Write_Float3(Out_Json["Min_Speed_Force"], m_tEffectParticleDesc.vMinSpeedForce);
		CJson_Utility::Write_Float3(Out_Json["Max_Speed_Force"], m_tEffectParticleDesc.vMaxSpeedForce);
	}

	CJson_Utility::Write_Float3(Out_Json["Min_Limit_Speed"], m_tEffectParticleDesc.vMinLimitSpeed);
	CJson_Utility::Write_Float3(Out_Json["Max_Limit_Speed"], m_tEffectParticleDesc.vMaxLimitSpeed);

#pragma region Rotation
	CJson_Utility::Write_Float3(Out_Json["Min_Start_Rotation"], m_tEffectParticleDesc.vMinStartRotation);
	CJson_Utility::Write_Float3(Out_Json["Max_Start_Rotation"], m_tEffectParticleDesc.vMaxStartRotation);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed))
	{
		CJson_Utility::Write_Float3(Out_Json["Min_Rotation_Speed"], m_tEffectParticleDesc.vMinRotationSpeed);
		CJson_Utility::Write_Float3(Out_Json["Max_Rotation_Speed"], m_tEffectParticleDesc.vMaxRotationSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
	{
		CJson_Utility::Write_Float3(Out_Json["Min_Rotation_Force"], m_tEffectParticleDesc.vMinRotationForce);
		CJson_Utility::Write_Float3(Out_Json["Max_Rotation_Force"], m_tEffectParticleDesc.vMaxRotationForce);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationLimit))
	{
		CJson_Utility::Write_Float3(Out_Json["Min_Rotation"], m_tEffectParticleDesc.vMinLimitRotation);
		CJson_Utility::Write_Float3(Out_Json["Max_Rotation"], m_tEffectParticleDesc.vMaxLimitRotation);
	}
#pragma endregion

#pragma region Scale
	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Ratio_Scale))
	{
		Out_Json["Min_Y_Scale_Ratio"] = m_tEffectParticleDesc.fMinYScaleRatio;
		Out_Json["Max_Y_Scale_Ratio"] = m_tEffectParticleDesc.fMaxYScaleRatio;
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Easing_Scale))
	{
		Out_Json["Scale_Easing_Type"] = m_tEffectParticleDesc.iScaleEasingType;
		Out_Json["Scale_Easing_Total_Time"] = m_tEffectParticleDesc.fScaleEasingTotalTime;

		CJson_Utility::Write_Float2(Out_Json["Easing_Start_Scale"], m_tEffectParticleDesc.vEasingStartScale);
		CJson_Utility::Write_Float2(Out_Json["Easing_Target_Scale"], m_tEffectParticleDesc.vEasingTargetScale);
	}
	else
	{
		CJson_Utility::Write_Float2(Out_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
		CJson_Utility::Write_Float2(Out_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleSpeed))
		{
			CJson_Utility::Write_Float2(Out_Json["Min_Scale_Speed"], m_tEffectParticleDesc.vMinScaleSpeed);
			CJson_Utility::Write_Float2(Out_Json["Max_Scale_Speed"], m_tEffectParticleDesc.vMaxScaleSpeed);
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleForce))
		{
			CJson_Utility::Write_Float2(Out_Json["Min_Scale_Force"], m_tEffectParticleDesc.vMinScaleForce);
			CJson_Utility::Write_Float2(Out_Json["Max_Scale_Force"], m_tEffectParticleDesc.vMaxScaleForce);
		}
	}
	CJson_Utility::Write_Float2(Out_Json["Min_Scale"], m_tEffectParticleDesc.vMinLimitScale);
	CJson_Utility::Write_Float2(Out_Json["Max_Scale"], m_tEffectParticleDesc.vMaxLimitScale);
#pragma endregion

#pragma region Color
	Out_Json["Render_Group"] = m_eRenderGroup;

	Out_Json["Discard_Ratio"] = m_tEffectParticleDesc.fDiscardRatio;

	CJson_Utility::Write_Float4(Out_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	CJson_Utility::Write_Float4(Out_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorSpeed))
	{
		CJson_Utility::Write_Float4(Out_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
		CJson_Utility::Write_Float4(Out_Json["Max_Color_Speed"], m_tEffectParticleDesc.vMaxColorSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorForce))
	{
		CJson_Utility::Write_Float4(Out_Json["Min_Color_Force"], m_tEffectParticleDesc.vMinColorForce);
		CJson_Utility::Write_Float4(Out_Json["Max_Color_Force"], m_tEffectParticleDesc.vMaxColorForce);
	}
	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Easing_Alpha))
	{
		Out_Json["Alpha_Easing_Type"] = m_tEffectParticleDesc.iAlphaEasingType;
		Out_Json["Alpha_Easing_Total_Time"] = m_tEffectParticleDesc.fAlphaEasingTotalTime;

		CJson_Utility::Write_Float4(Out_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
	}

	CJson_Utility::Write_Float4(Out_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);
#pragma endregion

#pragma region Texture
	Out_Json["Diffuse_XInverse"] = m_tEffectParticleDesc.bDiffuseXInverse;
	Out_Json["Diffuse_YInverse"] = m_tEffectParticleDesc.bDiffuseYInverse;

#pragma region Diffuse
	Out_Json["UV_Diffuse_Index"] = m_tEffectParticleDesc.iDiffuseIndex;

	CJson_Utility::Write_Float2(Out_Json["Diffuse_Start_UV"], m_tEffectParticleDesc.vDiffuseStartUV);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseSpeed))
		CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Speed"], m_tEffectParticleDesc.vDiffuseUVSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseForce))
		CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Force"], m_tEffectParticleDesc.vDiffuseUVForce);

	CJson_Utility::Write_Float2(Out_Json["Diffuse_UV_Max"], m_tEffectParticleDesc.vDiffuseUVMax);
#pragma endregion
#pragma region Mask
	Out_Json["UV_Mask_Index"] = m_tEffectParticleDesc.iMaskIndex;

	CJson_Utility::Write_Float2(Out_Json["Mask_Start_UV"], m_tEffectParticleDesc.vMaskStartUV);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskSpeed))
		CJson_Utility::Write_Float2(Out_Json["Mask_UV_Speed"], m_tEffectParticleDesc.vMaskUVSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskForce))
		CJson_Utility::Write_Float2(Out_Json["Mask_UV_Force"], m_tEffectParticleDesc.vMaskUVForce);

	CJson_Utility::Write_Float2(Out_Json["Mask_UV_Max"], m_tEffectParticleDesc.vMaskUVMax);
#pragma endregion
#pragma region Noise
	Out_Json["UV_Noise_Index"] = m_tEffectParticleDesc.iNoiseIndex;

	CJson_Utility::Write_Float2(Out_Json["Noise_Start_UV"], m_tEffectParticleDesc.vNoiseStartUV);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseSpeed))
		CJson_Utility::Write_Float2(Out_Json["Noise_UV_Speed"], m_tEffectParticleDesc.vNoiseUVSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseForce))
		CJson_Utility::Write_Float2(Out_Json["Noise_UV_Force"], m_tEffectParticleDesc.vNoiseUVForce);

	CJson_Utility::Write_Float2(Out_Json["Noise_UV_Max"], m_tEffectParticleDesc.vNoiseUVMax);
#pragma endregion
#pragma endregion

#pragma region Bloom & Glow
	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_Glow))
	{
		CJson_Utility::Write_Float4(Out_Json["Start_Glow_Color"], m_tEffectParticleDesc.vStartGlowColor);

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowSpeed))
			CJson_Utility::Write_Float4(Out_Json["Glow_Color_Speed"], m_tEffectParticleDesc.vGlowColorSpeed);

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowForce))
			CJson_Utility::Write_Float4(Out_Json["Glow_Color_Force"], m_tEffectParticleDesc.vGlowColorForce);
	}
#pragma endregion

#pragma region For. Sprite
	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Is_Sprite))
	{
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
	m_tEffectParticleDesc.iMaxInstance = In_Json["Max_Instance"];
	m_tEffectParticleDesc.bLooping = In_Json["Is_Looping"];

	m_tEffectParticleDesc.iParticleType = In_Json["ParticleType"];
	m_tEffectParticleDesc.iFollowTransformType = In_Json["Follow_Transform"];

#pragma region Particle Options

	if (In_Json.find("ParticleOption1") != In_Json.end())
		m_tEffectParticleDesc.byOption1 = In_Json["ParticleOption1"];
#ifdef NDEBUG
	else
		DEBUG_ASSERT;
#endif // NDEBUG

	if (In_Json.find("ParticleOption2") != In_Json.end())
		m_tEffectParticleDesc.byOption2 = In_Json["ParticleOption2"];
#ifdef NDEBUG
	else
		DEBUG_ASSERT;
#endif // NDEBUG

	if (In_Json.find("ParticleOption3") != In_Json.end())
		m_tEffectParticleDesc.byOption3 = In_Json["ParticleOption3"];
#ifdef NDEBUG
	else
		DEBUG_ASSERT;
#endif // NDEBUG

	if (In_Json.find("ParticleOption4") != In_Json.end())
		m_tEffectParticleDesc.byOption4 = In_Json["ParticleOption4"];
#ifdef NDEBUG
	else
		DEBUG_ASSERT;
#endif // NDEBUG

	if (In_Json.find("ParticleOption5") != In_Json.end())
		m_tEffectParticleDesc.byOption5 = In_Json["ParticleOption5"];
#ifdef NDEBUG
	else
		DEBUG_ASSERT;
#endif // NDEBUG

	if (In_Json.find("ParticleOption6") != In_Json.end())
		m_tEffectParticleDesc.byOption6 = In_Json["ParticleOption6"];
#ifdef NDEBUG
	else
		DEBUG_ASSERT;
#endif // NDEBUG

#pragma endregion // Particle Options

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Attraction))
	{
		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction) || Check_Option(EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction))
		{
			m_tEffectParticleDesc.fGoalRange = In_Json["Goal_Range"];
		}

		if (In_Json.find("Goal_Position") != In_Json.end())
		{
			CJson_Utility::Load_Float3(In_Json["Goal_Position"], m_tEffectParticleDesc.vGoalPosition);
		}
	}

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

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnTime))
	{
		if (In_Json.find("Max_Spawn_Time") != In_Json.end())
			m_tEffectParticleDesc.fMaxSpawnTime = In_Json["Max_Spawn_Time"];
	}

#pragma endregion // Spawn Time

#pragma region Life Time

	if (In_Json.find("Min_Life_Time") != In_Json.end())
		m_tEffectParticleDesc.fMinLifeTime = In_Json["Min_Life_Time"];

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_LifeTime))
	{
		if (In_Json.find("Max_Life_Time") != In_Json.end())
			m_tEffectParticleDesc.fMaxLifeTime = In_Json["Max_Life_Time"];
	}

#ifdef _EFFECT_TOOL_
	//if (m_tEffectParticleDesc.fMinLifeTime > 900.f)
	//	assert(0);
#endif // _EFFECT_TOOL_

#pragma endregion // Life Time

#pragma region Boner

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Boner))
	{
		if (In_Json.find("Bone_Name") != In_Json.end())
			m_strBoneName = In_Json["Bone_Name"];

		if (m_strBoneName.empty())
		{
			goto JumpBoner;
		}

#ifdef _EFFECT_TOOL_
		if ((_uint)LEVEL_EDIT == m_CreatedLevel)
		{
			m_pParentTransformCom = GET_SINGLE(CWindow_AnimationModelView)->Get_PreviewAnimModel().lock()->Get_Component<CTransform>().lock();
			m_pBoneNode = GET_SINGLE(CWindow_AnimationModelView)->Get_PreviewAnimModel().lock()->Get_CurrentModel().lock()->Find_BoneNode(m_strBoneName);
		}
#endif // _EFFECT_TOOL_
	}

#pragma endregion // Boner

#pragma region Spawn Position
	JumpBoner :
	if (In_Json.find("Min_Spawn_Position") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Position"], m_tEffectParticleDesc.vMinSpawnPosition);

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnPos))
	{
		if (In_Json.find("Max_Spawn_Position") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Spawn_Position"], m_tEffectParticleDesc.vMaxSpawnPosition);
	}

#pragma endregion  // Spawn Position

#pragma region Spawn Offset Direction

	if (In_Json.find("Min_Spawn_Offset_Direction") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMinSpawnOffsetDirection);


	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetDirection))
	{
		if (In_Json.find("Max_Spawn_Offset_Direction") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Direction"], m_tEffectParticleDesc.vMaxSpawnOffsetDirection);
	}

#pragma endregion // Spawn Offset Direction

#pragma region Spawn Offset Range

	if (In_Json.find("Min_Spawn_Offset_Range") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Spawn_Offset_Range"], m_tEffectParticleDesc.vMinSpawnOffsetRange);


	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetRange))
	{
		if (In_Json.find("Max_Spawn_Offset_Range") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Spawn_Offset_Range"], m_tEffectParticleDesc.vMaxSpawnOffsetRange);
	}

#pragma endregion // Spawn Offset Range

#pragma region Gravity

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Gravity))
	{
		if (In_Json.find("Gravity_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Gravity_Force"], m_tEffectParticleDesc.vGravityForce);
	}

#pragma endregion // Gravity


	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed))
	{
		if (In_Json.find("Min_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Speed"], m_tEffectParticleDesc.vMinStartSpeed);

		if (In_Json.find("Max_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Speed"], m_tEffectParticleDesc.vMaxStartSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
	{
		if (In_Json.find("Min_Speed_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Speed_Force"], m_tEffectParticleDesc.vMinSpeedForce);

		if (In_Json.find("Max_Speed_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Speed_Force"], m_tEffectParticleDesc.vMaxSpeedForce);
	}

	if (In_Json.find("Min_Limit_Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Limit_Speed"], m_tEffectParticleDesc.vMinLimitSpeed);


	if (In_Json.find("Max_Limit_Speed") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Limit_Speed"], m_tEffectParticleDesc.vMaxLimitSpeed);


#pragma region Rotation
	if (In_Json.find("Min_Start_Rotation") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Min_Start_Rotation"], m_tEffectParticleDesc.vMinStartRotation);
	if (In_Json.find("Max_Start_Rotation") != In_Json.end())
		CJson_Utility::Load_Float3(In_Json["Max_Start_Rotation"], m_tEffectParticleDesc.vMaxStartRotation);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed))
	{
		if (In_Json.find("Min_Rotation_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Rotation_Speed"], m_tEffectParticleDesc.vMinRotationSpeed);
		if (In_Json.find("Max_Rotation_Speed") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Rotation_Speed"], m_tEffectParticleDesc.vMaxRotationSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
	{
		if (In_Json.find("Min_Rotation_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Rotation_Force"], m_tEffectParticleDesc.vMinRotationForce);
		if (In_Json.find("Max_Rotation_Force") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Rotation_Force"], m_tEffectParticleDesc.vMaxRotationForce);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationLimit))
	{
		if (In_Json.find("Min_Rotation") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Min_Rotation"], m_tEffectParticleDesc.vMinLimitRotation);

		if (In_Json.find("Max_Rotation") != In_Json.end())
			CJson_Utility::Load_Float3(In_Json["Max_Rotation"], m_tEffectParticleDesc.vMaxLimitRotation);
	}

#pragma endregion

#pragma region Scale

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Ratio_Scale))
	{
		m_tEffectParticleDesc.fMinYScaleRatio = In_Json["Min_Y_Scale_Ratio"];
		m_tEffectParticleDesc.fMaxYScaleRatio = In_Json["Max_Y_Scale_Ratio"];
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Easing_Scale))
	{
		if (In_Json.find("Scale_Easing_Type") != In_Json.end())
			m_tEffectParticleDesc.iScaleEasingType = In_Json["Scale_Easing_Type"];
		if (In_Json.find("Scale_Easing_Total_Time") != In_Json.end())
			m_tEffectParticleDesc.fScaleEasingTotalTime = In_Json["Scale_Easing_Total_Time"];

		if (In_Json.find("Easing_Start_Scale") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Easing_Start_Scale"], m_tEffectParticleDesc.vEasingStartScale);
		if (In_Json.find("Easing_Target_Scale") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Easing_Target_Scale"], m_tEffectParticleDesc.vEasingTargetScale);
	}
	else
	{
		if (In_Json.find("Min_Start_Scale") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
		if (In_Json.find("Max_Start_Scale") != In_Json.end())
			CJson_Utility::Load_Float2(In_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleSpeed))
		{
			if (In_Json.find("Min_Scale_Speed") != In_Json.end())
				CJson_Utility::Load_Float2(In_Json["Min_Scale_Speed"], m_tEffectParticleDesc.vMinScaleSpeed);
			if (In_Json.find("Max_Scale_Speed") != In_Json.end())
				CJson_Utility::Load_Float2(In_Json["Max_Scale_Speed"], m_tEffectParticleDesc.vMaxScaleSpeed);
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleForce))
		{
			if (In_Json.find("Min_Scale_Force") != In_Json.end())
				CJson_Utility::Load_Float2(In_Json["Min_Scale_Force"], m_tEffectParticleDesc.vMinScaleForce);
			if (In_Json.find("Max_Scale_Force") != In_Json.end())
				CJson_Utility::Load_Float2(In_Json["Max_Scale_Force"], m_tEffectParticleDesc.vMaxScaleForce);
		}
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

	if (In_Json.find("Min_Start_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	if (In_Json.find("Max_Start_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorSpeed))
	{
		if (In_Json.find("Min_Color_Speed") != In_Json.end())
			CJson_Utility::Load_Float4(In_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
		if (In_Json.find("Max_Color_Speed") != In_Json.end())
			CJson_Utility::Load_Float4(In_Json["Max_Color_Speed"], m_tEffectParticleDesc.vMaxColorSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorForce))
	{
		if (In_Json.find("Min_Color_Force") != In_Json.end())
			CJson_Utility::Load_Float4(In_Json["Min_Color_Force"], m_tEffectParticleDesc.vMinColorForce);
		if (In_Json.find("Max_Color_Force") != In_Json.end())
			CJson_Utility::Load_Float4(In_Json["Max_Color_Force"], m_tEffectParticleDesc.vMaxColorForce);
	}

	if (In_Json.find("Min_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Min_Color"], m_tEffectParticleDesc.vMinColor);
	if (In_Json.find("Max_Color") != In_Json.end())
		CJson_Utility::Load_Float4(In_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Easing_Alpha))
	{
		if (In_Json.find("Alpha_Easing_Type") != In_Json.end())
			m_tEffectParticleDesc.iAlphaEasingType = In_Json["Alpha_Easing_Type"];

		if (In_Json.find("Alpha_Easing_Total_Time") != In_Json.end())
			m_tEffectParticleDesc.fAlphaEasingTotalTime = In_Json["Alpha_Easing_Total_Time"];

		if (In_Json.find("Min_Color_Speed") != In_Json.end())
			CJson_Utility::Load_Float4(In_Json["Min_Color_Speed"], m_tEffectParticleDesc.vMinColorSpeed);
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

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseSpeed))
		CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Speed"], m_tEffectParticleDesc.vDiffuseUVSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseForce))
		CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Force"], m_tEffectParticleDesc.vDiffuseUVForce);

	CJson_Utility::Load_Float2(In_Json["Diffuse_UV_Max"], m_tEffectParticleDesc.vDiffuseUVMax);
#pragma endregion
#pragma region Mask
	m_tEffectParticleDesc.iMaskIndex = In_Json["UV_Mask_Index"];
	CJson_Utility::Load_Float2(In_Json["Mask_Start_UV"], m_tEffectParticleDesc.vMaskStartUV);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskSpeed))
		CJson_Utility::Load_Float2(In_Json["Mask_UV_Speed"], m_tEffectParticleDesc.vMaskUVSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskForce))
		CJson_Utility::Load_Float2(In_Json["Mask_UV_Force"], m_tEffectParticleDesc.vMaskUVForce);

	CJson_Utility::Load_Float2(In_Json["Mask_UV_Max"], m_tEffectParticleDesc.vMaskUVMax);
#pragma endregion
#pragma region Noise
	m_tEffectParticleDesc.iNoiseIndex = In_Json["UV_Noise_Index"];
	CJson_Utility::Load_Float2(In_Json["Noise_Start_UV"], m_tEffectParticleDesc.vNoiseStartUV);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseSpeed))
		CJson_Utility::Load_Float2(In_Json["Noise_UV_Speed"], m_tEffectParticleDesc.vNoiseUVSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseForce))
		CJson_Utility::Load_Float2(In_Json["Noise_UV_Force"], m_tEffectParticleDesc.vNoiseUVForce);

	CJson_Utility::Load_Float2(In_Json["Noise_UV_Max"], m_tEffectParticleDesc.vNoiseUVMax);
#pragma endregion
#pragma endregion

#pragma region Bloom & Glow
	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_Glow))
	{
		CJson_Utility::Load_Float4(In_Json["Start_Glow_Color"], m_tEffectParticleDesc.vStartGlowColor);

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowSpeed))
			CJson_Utility::Load_Float4(In_Json["Glow_Color_Speed"], m_tEffectParticleDesc.vGlowColorSpeed);

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowForce))
			CJson_Utility::Load_Float4(In_Json["Glow_Color_Force"], m_tEffectParticleDesc.vGlowColorForce);
	}
#pragma endregion

#pragma region For. Sprite
	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Is_Sprite))
	{
		m_tEffectParticleDesc.iNumFrameX = In_Json["Sprite_NumFrameX"];
		m_tEffectParticleDesc.iNumFrameY = In_Json["Sprite_NumFrameY"];
		m_tEffectParticleDesc.fSpriteSpeed = In_Json["Sprite_FrameSpeed"];
	}
#pragma endregion

	m_pTextureCom.lock()->Use_Texture(m_szEffectName.c_str());
	Reset_Instance(m_tEffectParticleDesc.iMaxInstance);
}


__global__ void EKernel(int size)
{

}

__host__ void CEffect_Rect::Play(_float fTimeDelta)
{
	if (0.f < m_fCurrentInitTime)
	{
		m_fCurrentInitTime -= fTimeDelta;
		return;
	}

	_int	iTickCount(0);
	m_fPreFrame += fTimeDelta;

	while (HZ_144 <= m_fPreFrame)
	{
		++iTickCount;
		m_fPreFrame -= HZ_144;
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


	

	if (m_tEffectParticleDesc.iMaxInstance < g_iThread_Scale * 2)
	{
		Play_WithIndex(0, m_tEffectParticleDesc.iMaxInstance, BoneMatrix, fTimeDelta, iTickCount);
	}
	else
	{
		_int iJob_Size = m_tEffectParticleDesc.iMaxInstance / g_iThread_Scale;
		_int iExtra = m_tEffectParticleDesc.iMaxInstance % g_iThread_Scale;


		_int iStartIndex;
		_int iEndIndex;
		for (_int i = 0; i < iJob_Size; ++i)
		{
			iStartIndex = i * g_iThread_Scale;
			iEndIndex = (i + 1) * g_iThread_Scale + ((i + 1) == iJob_Size ? iExtra : 0);

			// 마지막 쓰레드면 여분까지 전부 플레이한다.
			GET_SINGLE(CThread_Manager)->Enqueue_Job(bind(&CEffect_Rect::Play_WithIndex, 
				this, iStartIndex, iEndIndex,
				BoneMatrix, fTimeDelta, iTickCount));
		}
	}

	

	


	//for (_int i(0); i < m_tEffectParticleDesc.iMaxInstance; ++i)
	//{

	//	if (!m_tParticleDescs[i].bEnable)
	//	{

	//		if (m_tParticleDescs[i].fCurrentSpawnTime > m_tParticleDescs[i].fTargetSpawnTime)
	//		{
	//			// 파티클 생성 시점

	//			m_tParticleDescs[i].bEnable = true;

	//			if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
	//				XMStoreFloat4x4(&m_tParticleDescs[i].ParentMatrix, BoneMatrix * m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
	//		}
	//		else
	//		{
	//			m_tParticleDescs[i].fCurrentSpawnTime += fTimeDelta;
	//			continue;
	//		}
	//	}

	//	else
	//	{
	//		if (m_tParticleDescs[i].fCurrentLifeTime > m_tParticleDescs[i].fTargetLifeTime)
	//		{
	//			if (m_tEffectParticleDesc.bLooping && !m_bStopParticle)
	//			{
	//				Reset_ParticleDesc((_uint)i);
	//			}
	//			else
	//			{
	//				m_tParticleDescs[i].bEnable = false;
	//				m_tParticleDescs[i].fCurrentSpawnTime = -1.f * FLT_MAX;
	//			}

	//			continue;
	//		}

	//		m_tParticleDescs[i].fCurrentLifeTime += fTimeDelta;
	//	}


	//	

	//	for (_int x(0); x < iTickCount; ++x)
	//	{
	//		Play_Internal(i, HZ_144, BoneMatrix);
	//	}
	//}

	

	//CudaMain_UpdateParticleTEST(HZ_144);
	

	/*kernel_UpdateParticle<<<1, m_tEffectParticleDesc.iMaxInstance>>>(HZ_144, BoneMatrix, 
		pInputParticleDescToCuda, pOutputParticleDescToCuda, m_tEffectParticleDesc.iMaxInstance);*/
	//EKernel<<<1, 10>>>(10);

	//cudaMemcpy(&m_tParticleDescs[0], pOutputParticleDescToCuda, Size, cudaMemcpyDeviceToHost);

	

	for (_int x(0); x < iTickCount; ++x)
	{
		Update_ParticleUV(HZ_144);

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_Glow))
			Update_ParticleGlowColor(HZ_144);
	}
}

void CEffect_Rect::Play_WithIndex(_int iStart_Index, _int iEnd_Index, _matrix BoneMatrix, _float fTimeDelta, _int iTickCount)
{
	if (iEnd_Index > m_tParticleDescs.size())
		return;

	for (_int i(iStart_Index); i < iEnd_Index; ++i)
	{

		if (!m_tParticleDescs[i].bEnable)
		{

			if (m_tParticleDescs[i].fCurrentSpawnTime > m_tParticleDescs[i].fTargetSpawnTime)
			{
				// 파티클 생성 시점

				m_tParticleDescs[i].bEnable = true;

				if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
					XMStoreFloat4x4(&m_tParticleDescs[i].ParentMatrix, BoneMatrix * m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
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
					Reset_ParticleDesc((_uint)i);
				}
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
		{
			Play_Internal(i, HZ_144, BoneMatrix);
		}
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
#ifdef _EFFECT_TOOL_
	if (0 == m_tParticleDescs.size() || 0 == m_tOriginalParticleDescs.size())
		return;
#endif // _EFFECT_TOOL_

	memcpy(&m_tParticleDescs[In_iIndex], &m_tOriginalParticleDescs[In_iIndex], sizeof(PARTICLE_DESC));

	SMath::Add_Float3(&m_tParticleDescs[In_iIndex].vCurrentTranslation, m_tParticleDescs[In_iIndex].vOffsetPosition);

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Attraction))
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


		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_LifeTime))
			m_tOriginalParticleDescs[i].fTargetLifeTime = SMath::fRandom(m_tEffectParticleDesc.fMinLifeTime, m_tEffectParticleDesc.fMaxLifeTime);
		else
			m_tOriginalParticleDescs[i].fTargetLifeTime = m_tEffectParticleDesc.fMinLifeTime;

		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnTime))
			m_tOriginalParticleDescs[i].fTargetSpawnTime = SMath::fRandom(m_tEffectParticleDesc.fMinSpawnTime, m_tEffectParticleDesc.fMaxSpawnTime);
		else
			m_tOriginalParticleDescs[i].fTargetSpawnTime = m_tEffectParticleDesc.fMinSpawnTime;

		_float3 vRandomDir;
		ZeroMemory(&vRandomDir, sizeof(_float3));
		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetDirection))
			vRandomDir = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnOffsetDirection, m_tEffectParticleDesc.vMaxSpawnOffsetDirection);
		else
			vRandomDir = m_tEffectParticleDesc.vMinSpawnOffsetDirection;

		_vector vRandomDirFromVector(XMLoadFloat3(&vRandomDir));
		vRandomDirFromVector = XMVector3Normalize(vRandomDirFromVector);

		_float3 vRandomScalar;
		ZeroMemory(&vRandomScalar, sizeof(_float3));
		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetRange))
			vRandomScalar = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnOffsetRange, m_tEffectParticleDesc.vMaxSpawnOffsetRange);
		else
			vRandomScalar = m_tEffectParticleDesc.vMinSpawnOffsetRange;

		_vector vRandomScalarFromVector(XMLoadFloat3(&vRandomScalar));

		_matrix RotationMatrix(SMath::Bake_MatrixNormalizeUseLookVector(vRandomDirFromVector));
		RotationMatrix = SMath::Go_Right(RotationMatrix, XMVectorGetX(vRandomScalarFromVector));
		RotationMatrix = SMath::Go_Up(RotationMatrix, XMVectorGetY(vRandomScalarFromVector));
		RotationMatrix = SMath::Go_Straight(RotationMatrix, XMVectorGetZ(vRandomScalarFromVector));

		XMStoreFloat3(&m_tOriginalParticleDescs[i].vCurrentTranslation, RotationMatrix.r[3]);

		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnPos))
			m_tOriginalParticleDescs[i].vOffsetPosition = SMath::vRandom(m_tEffectParticleDesc.vMinSpawnPosition, m_tEffectParticleDesc.vMaxSpawnPosition);
		else
			m_tOriginalParticleDescs[i].vOffsetPosition = m_tEffectParticleDesc.vMinSpawnPosition;

		m_tOriginalParticleDescs[i].vCurrentRotation = SMath::vRandom(m_tEffectParticleDesc.vMinStartRotation, m_tEffectParticleDesc.vMaxStartRotation);

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed))
			m_tOriginalParticleDescs[i].vTargetRotationSpeed = SMath::vRandom(m_tEffectParticleDesc.vMinRotationSpeed, m_tEffectParticleDesc.vMaxRotationSpeed);

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
			m_tOriginalParticleDescs[i].vTargetRotationForce = SMath::vRandom(m_tEffectParticleDesc.vMinRotationForce, m_tEffectParticleDesc.vMaxRotationForce);

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed))
			m_tOriginalParticleDescs[i].vTargetSpeed = SMath::vRandom(m_tEffectParticleDesc.vMinStartSpeed, m_tEffectParticleDesc.vMaxStartSpeed);

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
			m_tOriginalParticleDescs[i].vTargetSpeedForce = SMath::vRandom(m_tEffectParticleDesc.vMinSpeedForce, m_tEffectParticleDesc.vMaxSpeedForce);

		XMStoreFloat2(&m_tOriginalParticleDescs[i].vCurrentScale, SMath::vRandom(XMLoadFloat2(&m_tEffectParticleDesc.vMinStartScale), XMLoadFloat2(&m_tEffectParticleDesc.vMaxStartScale)));

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleSpeed))
			XMStoreFloat2(&m_tOriginalParticleDescs[i].vTargetScaleSpeed, SMath::vRandom(XMLoadFloat2(&m_tEffectParticleDesc.vMinScaleSpeed), XMLoadFloat2(&m_tEffectParticleDesc.vMaxScaleSpeed)));

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleForce))
			XMStoreFloat2(&m_tOriginalParticleDescs[i].vTargetScaleForce, SMath::vRandom(XMLoadFloat2(&m_tEffectParticleDesc.vMinScaleForce), XMLoadFloat2(&m_tEffectParticleDesc.vMaxScaleForce)));

		m_tOriginalParticleDescs[i].vCurrentColor = SMath::vRandom(m_tEffectParticleDesc.vMinStartColor, m_tEffectParticleDesc.vMaxStartColor);

		if (Check_Option(EFFECTPARTICLE_DESC::Option4::Easing_Alpha))
			m_tOriginalParticleDescs[i].fStartAlpha = m_tOriginalParticleDescs[i].vCurrentColor.w;

		if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorSpeed))
			m_tOriginalParticleDescs[i].vTargetColorSpeed = SMath::vRandom(m_tEffectParticleDesc.vMinColorSpeed, m_tEffectParticleDesc.vMaxColorSpeed);

		if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorForce))
			m_tOriginalParticleDescs[i].vTargetColorForce = SMath::vRandom(m_tEffectParticleDesc.vMinColorForce, m_tEffectParticleDesc.vMaxColorForce);


		m_tOriginalParticleDescs[i].vCurrentUV = m_tEffectParticleDesc.vMaskStartUV;


		m_tOriginalParticleDescs[i].vCurrentScale.x = max(FLT_MIN, m_tOriginalParticleDescs[i].vCurrentScale.x);

		m_tOriginalParticleDescs[i].vCurrentScale.y = max(FLT_MIN, m_tOriginalParticleDescs[i].vCurrentScale.y);


		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Ratio_Scale))
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

	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Is_Sprite))
		Update_ParticleSpriteFrame(i, fTimeDelta);
}

__host__ void CEffect_Rect::Update_ParticlePosition(const _uint& i, _float fTimeDelta, _matrix BoneMatrix)
{
	if (m_pBoneNode.lock() && (_int)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType)
	{
#ifdef _EFFECT_TOOL_
		if (m_pParentTransformCom.lock())
			m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
#else // _EFFECT_TOOL_
		m_pTransformCom.lock()->Set_WorldMatrix(BoneMatrix * m_pParentTransformCom.lock()->Get_WorldMatrix());
#endif // _EFFECT_TOOL_
	}

	_float3 vMove;
	ZeroMemory(&vMove, sizeof(_float3));

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed))
		vMove = SMath::Mul_Float3(m_tParticleDescs[i].vTargetSpeed, fTimeDelta);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
	{

		m_tParticleDescs[i].vCurrentSpeedForce = SMath::Add_Float3(m_tParticleDescs[i].vCurrentSpeedForce, SMath::Mul_Float3(m_tParticleDescs[i].vTargetSpeedForce, fTimeDelta));

		vMove = SMath::Add_Float3(vMove, m_tParticleDescs[i].vCurrentSpeedForce);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed) || Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
	{
		vMove.x = max(m_tEffectParticleDesc.vMinLimitSpeed.x, min(m_tEffectParticleDesc.vMaxLimitSpeed.x, vMove.x));
		vMove.y = max(m_tEffectParticleDesc.vMinLimitSpeed.y, min(m_tEffectParticleDesc.vMaxLimitSpeed.y, vMove.y));
		vMove.z = max(m_tEffectParticleDesc.vMinLimitSpeed.z, min(m_tEffectParticleDesc.vMaxLimitSpeed.z, vMove.z));
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Attraction))
	{
		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction))
		{
			if (SMath::Is_InRange(m_tParticleDescs[i].vCurrentTranslation, m_tEffectParticleDesc.vGoalPosition, m_tEffectParticleDesc.fGoalRange))
			{
				return;
			}
		}
		else if (Check_Option(EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction))
		{
			if (SMath::Is_InRange(m_tParticleDescs[i].vCurrentTranslation, m_tEffectParticleDesc.vGoalPosition, m_tEffectParticleDesc.fGoalRange))
			{
				m_tParticleDescs[i].vCurrentScale = { 0.f, 0.f };
			}
		}

		m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, SMath::Mul_Float3(m_tParticleDescs[i].vTargetLookAt, vMove.z));
	}
	else
	{
		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_MoveLook))
		{
			_vector vMovePosition(XMLoadFloat3(&vMove));

			_vector vRotatedPosition(XMVector3TransformCoord(vMovePosition, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation))));

			_vector vCurrentPosition(XMLoadFloat3(&m_tParticleDescs[i].vCurrentTranslation));

			vCurrentPosition += vRotatedPosition;

			XMStoreFloat3(&m_tParticleDescs[i].vCurrentTranslation, vCurrentPosition);
		}
		else
		{
			m_tParticleDescs[i].vCurrentTranslation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentTranslation, vMove);
		}
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Gravity))
	{
		m_tParticleDescs[i].vCurrentGravity.x += m_tEffectParticleDesc.vGravityForce.x * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta);
		m_tParticleDescs[i].vCurrentGravity.y += m_tEffectParticleDesc.vGravityForce.y * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta);
		m_tParticleDescs[i].vCurrentGravity.z += m_tEffectParticleDesc.vGravityForce.z * fTimeDelta * (m_tParticleDescs[i].fCurrentLifeTime * 2.f + fTimeDelta);
	}
}

void CEffect_Rect::Update_ParticleRotation(const _uint& i, _float fTimeDelta)
{
	_float3 vRotation;
	ZeroMemory(&vRotation, sizeof(_float3));

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed) || Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
	{
		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed))
			vRotation = SMath::Mul_Float3(m_tParticleDescs[i].vTargetRotationSpeed, fTimeDelta);

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
		{
			m_tParticleDescs[i].vCurrentRotationForce = SMath::Add_Float3(m_tParticleDescs[i].vCurrentRotationForce, SMath::Mul_Float3(m_tParticleDescs[i].vTargetRotationForce, fTimeDelta));

			vRotation = SMath::Add_Float3(vRotation, m_tParticleDescs[i].vCurrentRotationForce);
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationLimit))
		{
			vRotation.x = max(m_tEffectParticleDesc.vMinLimitRotation.x, min(m_tEffectParticleDesc.vMaxLimitRotation.x, vRotation.x));
			vRotation.y = max(m_tEffectParticleDesc.vMinLimitRotation.y, min(m_tEffectParticleDesc.vMaxLimitRotation.y, vRotation.y));
			vRotation.z = max(m_tEffectParticleDesc.vMinLimitRotation.z, min(m_tEffectParticleDesc.vMaxLimitRotation.z, vRotation.z));
		}
	}

	m_tParticleDescs[i].vCurrentRotation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentRotation, vRotation);
}

void CEffect_Rect::Update_ParticleScale(const _uint& i, _float fTimeDelta)
{
	_float2 vScale;
	ZeroMemory(&vScale, sizeof(_float2));

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Easing_Scale))
	{
		_float fElapsedTime(m_tParticleDescs[i].fCurrentLifeTime);

		if (0.f > fElapsedTime)
			return;

		Apply_Easing
		(
			vScale
			, (EASING_TYPE)m_tEffectParticleDesc.iScaleEasingType
			, XMLoadFloat2(&m_tEffectParticleDesc.vEasingStartScale)
			, XMLoadFloat2(&m_tEffectParticleDesc.vEasingTargetScale)
			, fElapsedTime
			, m_tEffectParticleDesc.fScaleEasingTotalTime
		);

		m_tParticleDescs[i].vCurrentScale = vScale;

	}
	else
	{
		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleSpeed))
			vScale = SMath::Mul_Float2(m_tParticleDescs[i].vTargetScaleSpeed, fTimeDelta);


		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleForce))
		{
			m_tParticleDescs[i].vCurrentScaleForce = SMath::Add_Float2(m_tParticleDescs[i].vCurrentScaleForce, SMath::Mul_Float2(m_tParticleDescs[i].vTargetScaleForce, fTimeDelta));

			SMath::Add_Float2(&m_tParticleDescs[i].vCurrentScale, m_tParticleDescs[i].vCurrentScaleForce);
		}

		m_tParticleDescs[i].vCurrentScale = SMath::Add_Float2(m_tParticleDescs[i].vCurrentScale, vScale);

	}

	m_tParticleDescs[i].vCurrentScale.x = max(m_tEffectParticleDesc.vMinLimitScale.x, min(m_tEffectParticleDesc.vMaxLimitScale.x, m_tParticleDescs[i].vCurrentScale.x));

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Square_Scale))
		m_tParticleDescs[i].vCurrentScale.y = m_tParticleDescs[i].vCurrentScale.x;
	else if (Check_Option(EFFECTPARTICLE_DESC::Option3::Ratio_Scale))
		m_tParticleDescs[i].vCurrentScale.y = m_tParticleDescs[i].vCurrentScale.x * m_tParticleDescs[i].fTargetYScaleRatio;
	else
		m_tParticleDescs[i].vCurrentScale.y = max(m_tEffectParticleDesc.vMinLimitScale.y, min(m_tEffectParticleDesc.vMaxLimitScale.y, m_tParticleDescs[i].vCurrentScale.y));
}

void CEffect_Rect::Update_ParticleUV(_float fTimeDelta)
{
#pragma region Diffuse
	_vector vDiffuseCurrentUV(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseSpeed) || Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseForce))
	{
		_vector vDiffuseUVSpeed(XMVectorSet(0.f, 0.f, 0.f, 0.f));

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseSpeed))
			vDiffuseUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vDiffuseUVSpeed) * fTimeDelta;

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseForce))
		{
			m_vDiffuseCurrentUVForce.x += m_tEffectParticleDesc.vDiffuseUVForce.x * fTimeDelta;
			m_vDiffuseCurrentUVForce.y += m_tEffectParticleDesc.vDiffuseUVForce.y * fTimeDelta;
		}

		_vector vDiffuseMoveUV = vDiffuseUVSpeed + XMLoadFloat2(&m_vDiffuseCurrentUVForce);
		vDiffuseCurrentUV = XMLoadFloat2(&m_vDiffuseCurrentUV);
		vDiffuseCurrentUV += vDiffuseMoveUV;
	}

	vDiffuseCurrentUV = XMVectorSetX(vDiffuseCurrentUV, min(m_tEffectParticleDesc.vDiffuseUVMax.x, XMVectorGetX(vDiffuseCurrentUV)));
	vDiffuseCurrentUV = XMVectorSetY(vDiffuseCurrentUV, min(m_tEffectParticleDesc.vDiffuseUVMax.y, XMVectorGetY(vDiffuseCurrentUV)));
	XMStoreFloat2(&m_vDiffuseCurrentUV, vDiffuseCurrentUV);
#pragma endregion

#pragma region Mask
	_vector vMaskCurrentUV(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskSpeed) || Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskForce))
	{
		_vector vMaskUVSpeed(XMVectorSet(0.f, 0.f, 0.f, 0.f));

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskSpeed))
			vMaskUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vMaskUVSpeed) * fTimeDelta;

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskForce))
		{
			m_vMaskCurrentUVForce.x += m_tEffectParticleDesc.vMaskUVForce.x * fTimeDelta;
			m_vMaskCurrentUVForce.y += m_tEffectParticleDesc.vMaskUVForce.y * fTimeDelta;
		}

		_vector vMaskMoveUV = vMaskUVSpeed + XMLoadFloat2(&m_vMaskCurrentUVForce);
		vMaskCurrentUV = XMLoadFloat2(&m_vMaskCurrentUV);
		vMaskCurrentUV += vMaskMoveUV;
	}

	vMaskCurrentUV = XMVectorSetX(vMaskCurrentUV, min(m_tEffectParticleDesc.vMaskUVMax.x, XMVectorGetX(vMaskCurrentUV)));
	vMaskCurrentUV = XMVectorSetY(vMaskCurrentUV, min(m_tEffectParticleDesc.vMaskUVMax.y, XMVectorGetY(vMaskCurrentUV)));
	XMStoreFloat2(&m_vMaskCurrentUV, vMaskCurrentUV);
#pragma endregion

#pragma region Noise
	_vector vNoiseCurrentUV(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseSpeed) || Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseForce))
	{
		_vector vNoiseUVSpeed(XMVectorSet(0.f, 0.f, 0.f, 0.f));

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseSpeed))
			vNoiseUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vNoiseUVSpeed) * fTimeDelta;

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseForce))
		{
			m_vNoiseCurrentUVForce.x += m_tEffectParticleDesc.vNoiseUVForce.x * fTimeDelta;
			m_vNoiseCurrentUVForce.y += m_tEffectParticleDesc.vNoiseUVForce.y * fTimeDelta;
		}

		_vector vNoiseMoveUV = vNoiseUVSpeed + XMLoadFloat2(&m_vNoiseCurrentUVForce);
		vNoiseCurrentUV = XMLoadFloat2(&m_vNoiseCurrentUV);
		vNoiseCurrentUV += vNoiseMoveUV;
	}

	vNoiseCurrentUV = XMVectorSetX(vNoiseCurrentUV, min(m_tEffectParticleDesc.vNoiseUVMax.x, XMVectorGetX(vNoiseCurrentUV)));
	vNoiseCurrentUV = XMVectorSetY(vNoiseCurrentUV, min(m_tEffectParticleDesc.vNoiseUVMax.y, XMVectorGetY(vNoiseCurrentUV)));
	XMStoreFloat2(&m_vNoiseCurrentUV, vNoiseCurrentUV);
#pragma endregion
}

void CEffect_Rect::Update_ParticleColor(const _uint& i, _float fTimeDelta)
{
	_vector vColor;
	ZeroMemory(&vColor, sizeof(_vector));


	_float fAlpha(0.f);
	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Easing_Alpha))
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

	_float4 vApplyColorSpeed;
	ZeroMemory(&vApplyColorSpeed, sizeof(_float4));

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorSpeed))
	{
		vApplyColorSpeed = SMath::Mul_Float4(m_tParticleDescs[i].vTargetColorSpeed, fTimeDelta);

		vColor = XMLoadFloat4(&vApplyColorSpeed);
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorForce))
	{
		SMath::Add_Float4(&m_tParticleDescs[i].vCurrentColorForce, SMath::Mul_Float4(m_tParticleDescs[i].vTargetColorForce, fTimeDelta));
	}

	_float4 float4Color;
	ZeroMemory(&float4Color, sizeof(_float4));
	XMStoreFloat4(&float4Color, vColor);

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorForce))
	{
		_float4 vApplyColorForce = SMath::Add_Float4(float4Color, m_tParticleDescs[i].vCurrentColorForce);

		vColor = XMLoadFloat4(&vApplyColorForce);
		XMStoreFloat4(&float4Color, vColor);
	}

	_float4 vApplyColor = SMath::Add_Float4(float4Color, m_tParticleDescs[i].vCurrentColor);

	vColor = XMLoadFloat4(&vApplyColor);

	vColor = XMVectorSetX(vColor, max(m_tEffectParticleDesc.vMinColor.x, min(m_tEffectParticleDesc.vMaxColor.x, XMVectorGetX(vColor))));
	vColor = XMVectorSetY(vColor, max(m_tEffectParticleDesc.vMinColor.y, min(m_tEffectParticleDesc.vMaxColor.y, XMVectorGetY(vColor))));
	vColor = XMVectorSetZ(vColor, max(m_tEffectParticleDesc.vMinColor.z, min(m_tEffectParticleDesc.vMaxColor.z, XMVectorGetZ(vColor))));
	vColor = XMVectorSetW(vColor, max(m_tEffectParticleDesc.vMinColor.w, min(m_tEffectParticleDesc.vMaxColor.w, XMVectorGetW(vColor))));

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Easing_Alpha))
		vColor = XMVectorSetW(vColor, fAlpha);

	XMStoreFloat4(&m_tParticleDescs[i].vCurrentColor, vColor);
}

void CEffect_Rect::Update_ParticleGlowColor(_float fTimeDelta)
{
	_float4 vColor;
	ZeroMemory(&vColor, sizeof(_float4));

	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowSpeed))
		vColor = SMath::Mul_Float4(m_tEffectParticleDesc.vGlowColorSpeed, fTimeDelta);

	if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowForce))
	{
		SMath::Add_Float4(&m_vCurrentGlowColorForce, SMath::Mul_Float4(m_tEffectParticleDesc.vGlowColorForce, fTimeDelta));
		SMath::Add_Float4(&vColor, m_vCurrentGlowColorForce);
	}

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
				if (Check_Option(EFFECTPARTICLE_DESC::Option6::Loop_Sprite))
				{
					ZeroMemory(&m_tParticleDescs[i].vSpriteUV, sizeof(_float2));
					return;
				}
				else
				{
					if (Check_Option(EFFECTPARTICLE_DESC::Option6::Sprite_StopAtEnd))
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
			m_pTransformCom.lock()->Set_WorldMatrix(m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
		if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
			m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());
		else if ((_int)TRANSFORMTYPE::CHILD == m_tEffectParticleDesc.iFollowTransformType)
			m_pTransformCom.lock()->Set_WorldMatrix(m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
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
	_float& fTarget
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

const _bool CEffect_Rect::Check_Option(const EFFECTPARTICLE_DESC::Option1 eOption) const
{
	return (m_tEffectParticleDesc.byOption1 & (_ubyte)eOption) ? true : false;
}

const _bool CEffect_Rect::Check_Option(const EFFECTPARTICLE_DESC::Option2 eOption) const
{
	return (m_tEffectParticleDesc.byOption2 & (_ubyte)eOption) ? true : false;
}

const _bool CEffect_Rect::Check_Option(const EFFECTPARTICLE_DESC::Option3 eOption) const
{
	return (m_tEffectParticleDesc.byOption3 & (_ubyte)eOption) ? true : false;
}

const _bool CEffect_Rect::Check_Option(const EFFECTPARTICLE_DESC::Option4 eOption) const
{
	return (m_tEffectParticleDesc.byOption4 & (_ubyte)eOption) ? true : false;
}

const _bool CEffect_Rect::Check_Option(const EFFECTPARTICLE_DESC::Option5 eOption) const
{
	return (m_tEffectParticleDesc.byOption5 & (_ubyte)eOption) ? true : false;
}

const _bool CEffect_Rect::Check_Option(const EFFECTPARTICLE_DESC::Option6 eOption) const
{
	return (m_tEffectParticleDesc.byOption6 & (_ubyte)eOption) ? true : false;
}

#ifdef _EFFECT_TOOL_
void CEffect_Rect::TurnOn_Option(const EFFECTPARTICLE_DESC::Option1 eOption)
{
	m_tEffectParticleDesc.byOption1 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option(const EFFECTPARTICLE_DESC::Option1 eOption)
{
	m_tEffectParticleDesc.byOption1 &= ~(_ubyte)eOption;
}

void CEffect_Rect::TurnOn_Option(const EFFECTPARTICLE_DESC::Option2 eOption)
{
	m_tEffectParticleDesc.byOption2 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option(const EFFECTPARTICLE_DESC::Option2 eOption)
{
	m_tEffectParticleDesc.byOption2 &= ~(_ubyte)eOption;
}

void CEffect_Rect::TurnOn_Option(const EFFECTPARTICLE_DESC::Option3 eOption)
{
	m_tEffectParticleDesc.byOption3 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option(const EFFECTPARTICLE_DESC::Option3 eOption)
{
	m_tEffectParticleDesc.byOption3 &= ~(_ubyte)eOption;
}

void CEffect_Rect::TurnOn_Option(const EFFECTPARTICLE_DESC::Option4 eOption)
{
	m_tEffectParticleDesc.byOption4 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option(const EFFECTPARTICLE_DESC::Option4 eOption)
{
	m_tEffectParticleDesc.byOption4 &= ~(_ubyte)eOption;
}

void CEffect_Rect::TurnOn_Option(const EFFECTPARTICLE_DESC::Option5 eOption)
{
	m_tEffectParticleDesc.byOption5 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option(const EFFECTPARTICLE_DESC::Option5 eOption)
{
	m_tEffectParticleDesc.byOption5 &= ~(_ubyte)eOption;
}

void CEffect_Rect::TurnOn_Option(const EFFECTPARTICLE_DESC::Option6 eOption)
{
	m_tEffectParticleDesc.byOption6 |= (_ubyte)eOption;
}

void CEffect_Rect::TurnOff_Option(const EFFECTPARTICLE_DESC::Option6 eOption)
{
	m_tEffectParticleDesc.byOption6 &= ~(_ubyte)eOption;
}

void CEffect_Rect::Tool_ToggleOption(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::Option1 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}

void CEffect_Rect::Tool_ToggleOption(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::Option2 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}

void CEffect_Rect::Tool_ToggleOption(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::Option3 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}

void CEffect_Rect::Tool_ToggleOption(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::Option4 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}

void CEffect_Rect::Tool_ToggleOption(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::Option5 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}

void CEffect_Rect::Tool_ToggleOption(const char* szOptionName, const char* szOptionButtonName, const EFFECTPARTICLE_DESC::Option6 eOption)
{
	ImGuiColorEditFlags byButtonFlags(ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	if (Check_Option(eOption))
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 0.f, 1.f, 0.f, 1.f }, byButtonFlags))
			TurnOff_Option(eOption);
	}
	else
	{
		if (ImGui::ColorButton(szOptionButtonName, ImVec4{ 1.f, 0.f, 0.f, 1.f }, byButtonFlags))
			TurnOn_Option(eOption);
	}

	ImGui::SameLine();
	ImGui::Text(szOptionName);
}

#endif // _EFFECT_TOOL_

#ifdef _EFFECT_TOOL_
void CEffect_Rect::Show_ShaderPasses()
{
	for (auto elem : m_ParticleShaderInfos)
	{
		ImGui::Text("[%d]", elem.iShaderPassNumber); ImGui::SameLine();
		switch (elem.eRectType)
		{
		case PARTICLE_SHADER_INFO::PARTICLE_RECT_TYPE::DEFAULT:
			ImGui::TextColored(ImVec4{ 0.0705f, 0.f, 0.6510f, 1.f }, "Default");
			break;
		case PARTICLE_SHADER_INFO::PARTICLE_RECT_TYPE::SPRITE:
			ImGui::TextColored(ImVec4{ 0.6510f, 0.1490f, 0.1490f, 1.f }, "Sprite");
			break;
		default:
			assert(0);
		}

		ImGui::SameLine();

		switch (elem.eDiscardType)
		{
		case PARTICLE_SHADER_INFO::PARTICLE_DISCARD_TYPE::DISCARD_ALPHA:
			ImGui::TextColored(ImVec4{ 0.3333f, 0.3333f, 0.3333f, 1.f }, "Alpha Discard");
			break;
		case PARTICLE_SHADER_INFO::PARTICLE_DISCARD_TYPE::DISCARD_BLACK:
			ImGui::TextColored(ImVec4{ 0.f, 0.f, 0.f, 1.f }, "Black Discard");
			break;
		default:
			assert(0);
		}

		switch (elem.bSoftRendering)
		{
		case true:
			ImGui::SameLine();
			ImGui::TextColored(ImVec4{ 0.f, 0.8f, 0.f, 1.f }, "Soft");
			break;
		case false:
			break;
		}

		switch (elem.bSpecialRendering)
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

void CEffect_Rect::Tool_Spawn_Life_Time()
{
	ImGui::Text("Init Time"); ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##Init_Time", &m_tEffectParticleDesc.fInitTime, 0.1f);

	Tool_ToggleOption("Min Max Spawn Time", "##Is_MinMaxSpawnTime", EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnTime);
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnTime))
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

	Tool_ToggleOption("Min Max Life Time", "##Is_MinMaxLifeTime", EFFECTPARTICLE_DESC::Option1::Use_MinMax_LifeTime);
	if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_LifeTime))
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
	weak_ptr<CPreviewAnimationModel> pPreviewModel(GET_SINGLE(CWindow_AnimationModelView)->Get_PreviewAnimModel());

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
		static ImGuiTextFilter BoneFilter;
		ImGui::Text("Search"); ImGui::SameLine();
		BoneFilter.Draw("##Bone_Search_Bar", 250.f);
		ImGui::BeginChild("##Bone_List_Particle", ImVec2(250.f, 150.f));
		for (_int n(0); n < m_AllBoneNames.size(); n++)
		{
			auto BoneKit = m_AllBoneNames.at(n);
			const _bool is_selected = (m_iCurrentBoneIndex == n);
			if (BoneFilter.PassFilter(BoneKit.c_str()))
			{
				std::string label = BoneKit + "##" + std::to_string(n);

				if (ImGui::Selectable(label.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					m_iCurrentBoneIndex = n;

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						m_strBoneName = m_AllBoneNames[m_iCurrentBoneIndex];
					}
				}
			}
		}
		ImGui::EndChild();
	}

	if (ImGui::Button("Bind##Bind_To_Bone"))
	{
		m_pBoneNode = m_pParentModel.lock()->Find_BoneNode(m_strBoneName);
		if (nullptr == m_pBoneNode.lock())
		{
			MSG_BOX("Invalid Bone Name!");
		}
	}

	if (m_pBoneNode.lock())
	{
		ImGui::Text("Binded to [ "); ImGui::SameLine();
		ImGui::Text(m_pBoneNode.lock()->Get_Name()); ImGui::SameLine();
		ImGui::Text(" ]");
	}
}

void CEffect_Rect::Tool_Attraction()
{
	if (ImGui::TreeNode("Attraction##Attraction_Option"))
	{
		ImGui::Text("At Goal...");
		Tool_ToggleOption("Stop", "##Stop_At_GoalAttraction", EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction);
		Tool_ToggleOption("Kill", "##Kill_At_GoalAttraction", EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction);

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction))
		{
			TurnOff_Option(EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction);
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction))
		{
			TurnOff_Option(EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction);
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Stop_At_GoalAttraction) || Check_Option(EFFECTPARTICLE_DESC::Option3::Kill_At_GoalAttraction))
		{
			ImGui::Text("Goal Range"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat("##Goal_Range", &m_tEffectParticleDesc.fGoalRange, 0.01f, 0.f, 0.f, "%.5f");
		}

		ImGui::Text("Goal Position"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat3("##Goal_Position", &m_tEffectParticleDesc.vGoalPosition.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Position()
{
	if (ImGui::TreeNode("Spawn Position"))
	{
		Tool_ToggleOption("Min Max", "##Is_MinMaxSpawnPos", EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnPos);

		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnPos))
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
		Tool_ToggleOption("Min Max", "##Is_MinMaxSpawnOffsetDirection", EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetDirection);

		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetDirection))
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
		Tool_ToggleOption("Min Max", "##Is_MinMaxSpawnOffsetRange", EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetRange);

		if (Check_Option(EFFECTPARTICLE_DESC::Option1::Use_MinMax_SpawnOffsetRange))
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

void CEffect_Rect::Tool_Speed()
{
	Tool_ToggleOption("Use Speed", "##Use_Speed", EFFECTPARTICLE_DESC::Option2::Use_Speed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed))
	{
		if (ImGui::TreeNode("Start Speed"))
		{
			ImGui::Checkbox("Min = Max##Is_MinMaxSame_StartSpeed", &m_tEffectParticleDesc.bIsMinMaxSame_StartSpeed);

			if (m_tEffectParticleDesc.bIsMinMaxSame_StartSpeed)
				m_tEffectParticleDesc.vMaxStartSpeed = m_tEffectParticleDesc.vMinStartSpeed;

			ImGui::Text("Min Start Speed"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Min_Speed", &m_tEffectParticleDesc.vMinStartSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

			ImGui::Text("Max Start Speed"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat3("##Max_Speed", &m_tEffectParticleDesc.vMaxStartSpeed.x, 0.01f, 0.f, 0.f, "%.5f");

			ImGui::TreePop();
		}
	}

	Tool_ToggleOption("Use Force", "##Use_Force", EFFECTPARTICLE_DESC::Option2::Use_Force);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
	{
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
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Speed) || Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Force))
	{
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
}

void CEffect_Rect::Tool_Speed_Easing()
{
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

	Tool_ToggleOption("Use Rotation Speed", "##Use_Rotation_Speed", EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed))
	{
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
	}

	Tool_ToggleOption("Use Rotation Force", "##Use_Rotation_Force", EFFECTPARTICLE_DESC::Option2::Use_RotationForce);

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
	{
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
	}

	if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationSpeed) || Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationForce))
	{
		Tool_ToggleOption("Use Rotation Limit", "##Use_RotationLimit", EFFECTPARTICLE_DESC::Option2::Use_RotationLimit);

		if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_RotationLimit))
		{
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
	}
}

void CEffect_Rect::Tool_Rotation_Easing()
{
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

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Square_Scale))
		{
			m_tEffectParticleDesc.vMinStartScale.y = m_tEffectParticleDesc.vMinStartScale.x;
			m_tEffectParticleDesc.vMaxStartScale.y = m_tEffectParticleDesc.vMaxStartScale.x;
		}

		ImGui::TreePop();
	}

	Tool_ToggleOption("Use Scale Speed", "##Use_ScaleSpeed", EFFECTPARTICLE_DESC::Option3::Use_ScaleSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleSpeed))
	{
		if (ImGui::TreeNode("Scale Speed"))
		{
			ImGui::Checkbox("Min = Max##Is_MinMaxSame_ScaleSpeed", &m_tEffectParticleDesc.bIsMinMaxSame_ScaleSpeed);

			if (m_tEffectParticleDesc.bIsMinMaxSame_ScaleSpeed)
				m_tEffectParticleDesc.vMaxScaleSpeed = m_tEffectParticleDesc.vMinScaleSpeed;

			ImGui::Text("Min Scale Speed"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat2("##Min_Scale_Speed", &m_tEffectParticleDesc.vMinScaleSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

			ImGui::Text("Max Scale Speed"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat2("##Max_Scale_Speed", &m_tEffectParticleDesc.vMaxScaleSpeed.x, 0.1f, 0.f, 0.f, "%.5f");

			if (Check_Option(EFFECTPARTICLE_DESC::Option3::Square_Scale))
			{
				m_tEffectParticleDesc.vMinScaleSpeed.y = m_tEffectParticleDesc.vMinScaleSpeed.x;
				m_tEffectParticleDesc.vMaxScaleSpeed.y = m_tEffectParticleDesc.vMaxScaleSpeed.x;
			}

			ImGui::TreePop();
		}
	}

	Tool_ToggleOption("Use Scale Force", "##Use_ScaleForce", EFFECTPARTICLE_DESC::Option3::Use_ScaleForce);

	if (Check_Option(EFFECTPARTICLE_DESC::Option3::Use_ScaleForce))
	{
		if (ImGui::TreeNode("Scale Force"))
		{
			ImGui::Checkbox("Min = Max##Is_MinMaxSame_ScaleForce", &m_tEffectParticleDesc.bIsMinMaxSame_ScaleForce);

			if (m_tEffectParticleDesc.bIsMinMaxSame_ScaleForce)
				m_tEffectParticleDesc.vMaxScaleForce = m_tEffectParticleDesc.vMinScaleForce;

			ImGui::Text("Min Scale Force"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat2("##Min_Scale_Force", &m_tEffectParticleDesc.vMinScaleForce.x, 0.1f, 0.f, 0.f, "%.5f");

			ImGui::Text("Max Scale Force"); ImGui::SetNextItemWidth(300.f);
			ImGui::DragFloat2("##Max_Scale_Force", &m_tEffectParticleDesc.vMaxScaleForce.x, 0.1f, 0.f, 0.f, "%.5f");

			if (Check_Option(EFFECTPARTICLE_DESC::Option3::Square_Scale))
			{
				m_tEffectParticleDesc.vMinScaleForce.y = m_tEffectParticleDesc.vMinScaleForce.x;
				m_tEffectParticleDesc.vMaxScaleForce.y = m_tEffectParticleDesc.vMaxScaleForce.x;
			}

			ImGui::TreePop();
		}
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

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Square_Scale))
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
	ImGui::DragFloat2("##Easing_Start_Scale", &m_tEffectParticleDesc.vEasingStartScale.x, 0.1f, 0.f, 0.f, "%.5f");

	ImGui::Text("Target Scale"); ImGui::SetNextItemWidth(300.f);
	ImGui::DragFloat2("##Easing_Target_Scale", &m_tEffectParticleDesc.vEasingTargetScale.x, 0.1f, 0.f, 0.f, "%.5f");

	if (ImGui::TreeNode("Scale Limit"))
	{
		ImGui::Checkbox("Min = Max##Is_MinMaxSame_ScaleLimit", &m_tEffectParticleDesc.bIsMinMaxSame_ScaleLimit);

		if (m_tEffectParticleDesc.bIsMinMaxSame_ScaleLimit)
			m_tEffectParticleDesc.vMaxLimitScale = m_tEffectParticleDesc.vMinLimitScale;

		ImGui::Text("Min Limit Scale"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Min_Limit_Scale", &m_tEffectParticleDesc.vMinLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");

		ImGui::Text("Max Limit Scale"); ImGui::SetNextItemWidth(300.f);
		ImGui::DragFloat2("##Max_Limit_Scale", &m_tEffectParticleDesc.vMaxLimitScale.x, 0.1f, 0.f, 0.f, "%.5f");

		if (Check_Option(EFFECTPARTICLE_DESC::Option3::Square_Scale))
		{
			m_tEffectParticleDesc.vMinLimitScale.y = m_tEffectParticleDesc.vMinLimitScale.x;
			m_tEffectParticleDesc.vMaxLimitScale.y = m_tEffectParticleDesc.vMaxLimitScale.x;
		}

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Sprite()
{
	if (ImGui::TreeNode("Sprite Option"))
	{
		Tool_ToggleOption("On : Diffuse, Off : Mask", "#Sprite_DiffMask", EFFECTPARTICLE_DESC::Option6::Sprite_UseMask);
		Tool_ToggleOption("Loop Sprite", "##Loop_Sprite", EFFECTPARTICLE_DESC::Option6::Loop_Sprite);
		Tool_ToggleOption("Stop at End", "##Sprite_StopAtEnd", EFFECTPARTICLE_DESC::Option6::Sprite_StopAtEnd);
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

	Tool_ToggleOption("Use Color Speed", "##Use_ColorSpeed", EFFECTPARTICLE_DESC::Option4::Use_ColorSpeed);

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorSpeed))
	{
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
	}

	Tool_ToggleOption("Use Color Force", "##Use_ColorForce", EFFECTPARTICLE_DESC::Option4::Use_ColorForce);

	if (Check_Option(EFFECTPARTICLE_DESC::Option4::Use_ColorForce))
	{
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

		Tool_ToggleOption("Use UV Speed", "##Use_DiffuseSpeed", EFFECTPARTICLE_DESC::Option5::Use_DiffuseSpeed);
		Tool_ToggleOption("Use UV Force", "##Use_DiffuseForce", EFFECTPARTICLE_DESC::Option5::Use_DiffuseForce);

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseSpeed))
		{
			ImGui::Text("UV Speed"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat2("##Diffuse_UV_Speed", &m_tEffectParticleDesc.vDiffuseUVSpeed.x, 0.01f, 0.f, 0.f, "%.5f");
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_DiffuseForce))
		{
			ImGui::Text("UV Force"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat2("##Diffuse UV_Force", &m_tEffectParticleDesc.vDiffuseUVForce.x, 0.01f, 0.f, 0.f, "%.5f");
		}

		ImGui::Text("UV Max"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Diffuse_UV_Max", &m_tEffectParticleDesc.vDiffuseUVMax.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Texture_Mask()
{
	if (ImGui::TreeNode("Mask"))
	{
		ImGui::Text("Index");  ImGui::SetNextItemWidth(100.f);
		ImGui::InputInt("##Mask_Index", &m_tEffectParticleDesc.iMaskIndex);

		ImGui::Text("Start UV"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Mask_Start_UV", &m_tEffectParticleDesc.vMaskStartUV.x, 0.01f, 0.f, 0.f, "%.5f");

		Tool_ToggleOption("Use UV Speed", "##Use_MaskSpeed", EFFECTPARTICLE_DESC::Option5::Use_MaskSpeed);
		Tool_ToggleOption("Use UV Force", "##Use_MaskForce", EFFECTPARTICLE_DESC::Option5::Use_MaskForce);

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskSpeed))
		{
			ImGui::Text("UV Speed"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat2("##Mask_UV_Speed", &m_tEffectParticleDesc.vMaskUVSpeed.x, 0.01f, 0.f, 0.f, "%.5f");
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_MaskForce))
		{
			ImGui::Text("UV Force"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat2("##Mask UV_Force", &m_tEffectParticleDesc.vMaskUVForce.x, 0.01f, 0.f, 0.f, "%.5f");
		}

		ImGui::Text("UV Max"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Mask_UV_Max", &m_tEffectParticleDesc.vMaskUVMax.x, 0.01f, 0.f, 0.f, "%.5f");

		ImGui::TreePop();
	}
}

void CEffect_Rect::Tool_Texture_Noise()
{
	if (ImGui::TreeNode("Noise"))
	{
		ImGui::Text("Index");  ImGui::SetNextItemWidth(100.f);
		ImGui::InputInt("##Noise_Index", &m_tEffectParticleDesc.iNoiseIndex);

		ImGui::Text("Start UV"); ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat2("##Noise_Start_UV", &m_tEffectParticleDesc.vNoiseStartUV.x, 0.01f, 0.f, 0.f, "%.5f");

		Tool_ToggleOption("Use UV Speed", "##Use_NoiseSpeed", EFFECTPARTICLE_DESC::Option5::Use_NoiseSpeed);
		Tool_ToggleOption("Use UV Force", "##Use_NoiseForce", EFFECTPARTICLE_DESC::Option5::Use_NoiseForce);

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseSpeed))
		{
			ImGui::Text("UV Speed"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat2("##Noise_UV_Speed", &m_tEffectParticleDesc.vNoiseUVSpeed.x, 0.01f, 0.f, 0.f, "%.5f");
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option5::Use_NoiseForce))
		{
			ImGui::Text("UV Force"); ImGui::SetNextItemWidth(200.f);
			ImGui::DragFloat2("##Noise UV_Force", &m_tEffectParticleDesc.vNoiseUVForce.x, 0.01f, 0.f, 0.f, "%.5f");
		}

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

		Tool_ToggleOption("Use Glow Speed", "##Use_GlowSpeed", EFFECTPARTICLE_DESC::Option6::Use_GlowSpeed);
		Tool_ToggleOption("Use Glow Force", "##Use_GlowForce", EFFECTPARTICLE_DESC::Option6::Use_GlowForce);

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowSpeed))
		{
			ImGui::Text("Glow Color Speed "); ImGui::SetNextItemWidth(150.f);
			ImGui::DragFloat4("##Glow_Color_Speed", &m_tEffectParticleDesc.vGlowColorSpeed.x, 0.01f);
		}

		if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_GlowForce))
		{
			ImGui::Text("Glow Color Force"); ImGui::SetNextItemWidth(150.f);
			ImGui::DragFloat4("##Glow_Color_Force", &m_tEffectParticleDesc.vGlowColorForce.x, 0.01f);
		}

		ImGui::TreePop();
	}
}
#endif // _EFFECT_TOOL_

void CEffect_Rect::OnEventMessage(_uint iArg)
{
#ifdef _EFFECT_TOOL_
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Effect_Particle"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			if (ImGui::Button("Copy##Copy_Particle_Info"))
			{
				GET_SINGLE(CGameManager)->Store_ParticleInfo(m_tEffectParticleDesc, m_strBoneName);
			}

			ImGui::SameLine();

			if (ImGui::Button("Paste##Paste_Particle_Info"))
			{
				if (0 < m_tEffectParticleDesc.iMaxInstance)
				{
					m_tEffectParticleDesc = GET_SINGLE(CGameManager)->Get_StoredParticleInfo();
					m_strBoneName = GET_SINGLE(CGameManager)->Get_BoneName();

					ReBake_EditParticle();
				}
			}

			if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
			{
				if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
				{
					if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
					{
						if (0 < m_tEffectParticleDesc.iMaxInstance)
						{
							m_tEffectParticleDesc = GET_SINGLE(CGameManager)->Get_StoredParticleInfo();
							m_strBoneName = GET_SINGLE(CGameManager)->Get_BoneName();

							ReBake_EditParticle();
						}
					}
				}
			}


			ImGui::Text("Max Instance"); ImGui::SameLine();
			if (ImGui::DragInt("##Max_Instance", &m_tEffectParticleDesc.iMaxInstance, 1, 0, 0, "%d", ImGuiSliderFlags_AlwaysClamp))
				ReBake_EditParticle();

			ImGui::Separator();

			ImGui::Checkbox("Is Looping##Is_Looping", &m_tEffectParticleDesc.bLooping);

			if (ImGui::CollapsingHeader("Particle Type"))
			{
				const char* items[] = { "None", "Outburst_NoUse", "Attraction", "Billboard" };

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

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();
				}
			}

			Tool_ToggleOption("Attraction", "##Is_Attraction", EFFECTPARTICLE_DESC::Option1::Is_Attraction);

			if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Attraction))
			{
				Tool_Attraction();
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
			Tool_ToggleOption("Is Move Look", "##Is_MoveLook", EFFECTPARTICLE_DESC::Option1::Is_MoveLook);
			ImGui::Separator();

			Tool_ToggleOption("Use Gravity", "##Use_Gravity", EFFECTPARTICLE_DESC::Option2::Use_Gravity);

			if (Check_Option(EFFECTPARTICLE_DESC::Option2::Use_Gravity))
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
				Tool_ToggleOption("Is Boner", "##Is_Boner", EFFECTPARTICLE_DESC::Option1::Is_Boner);

				if (Check_Option(EFFECTPARTICLE_DESC::Option1::Is_Boner))
					Tool_Boner();
				else
				{
					m_pBoneNode.reset();
					m_strBoneName.clear();
				}
			}

			if (ImGui::CollapsingHeader("Position"))
				Tool_Position();

			if (ImGui::CollapsingHeader("Speed"))
				Tool_Speed();

			if (ImGui::CollapsingHeader("Rotation"))
				Tool_Rotation();

			if (ImGui::CollapsingHeader("Scale"))
			{
				Tool_ToggleOption("Square Scale", "##Square_Scale", EFFECTPARTICLE_DESC::Option3::Square_Scale);
				Tool_ToggleOption("Ratio Scale", "##Ratio_Scale", EFFECTPARTICLE_DESC::Option3::Ratio_Scale);

				if (Check_Option(EFFECTPARTICLE_DESC::Option3::Ratio_Scale))
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

				Tool_ToggleOption("Easing Scale", "##Easing_Scale", EFFECTPARTICLE_DESC::Option3::Easing_Scale);
				ImGui::Separator();

				if (!Check_Option(EFFECTPARTICLE_DESC::Option3::Easing_Scale))
					Tool_Scale();
				else
					Tool_Scale_Easing();
			}
			if (ImGui::CollapsingHeader("Colors"))
			{
				if (ImGui::TreeNode("Shader Pass"))
				{
#ifdef _EFFECT_TOOL_
					Show_ShaderPasses();
#endif // _EFFECT_TOOL_
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

				Tool_ToggleOption("Use Sprite", "##Is_Sprite", EFFECTPARTICLE_DESC::Option6::Is_Sprite);

				if (Check_Option(EFFECTPARTICLE_DESC::Option6::Is_Sprite))
					Tool_Sprite();

				ImGui::Text("Discard Ratio"); ImGui::SameLine(); ImGui::SetNextItemWidth(100.f);
				ImGui::DragFloat("##Discard_Ratio", &m_tEffectParticleDesc.fDiscardRatio, 0.01f, 0.f, 3.f);

				Tool_ToggleOption("Easing Alpha", "Easing_Alpha", EFFECTPARTICLE_DESC::Option4::Easing_Alpha);

				Tool_Color();

				if (Check_Option(EFFECTPARTICLE_DESC::Option4::Easing_Alpha))
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
				Tool_ToggleOption("Use Bloom", "##Use_Bloom", EFFECTPARTICLE_DESC::Option6::Use_Bloom);
				Tool_ToggleOption("Use Glow", "##Use_Glow", EFFECTPARTICLE_DESC::Option6::Use_Glow);
				Tool_ToggleOption("Use Emissive", "##Use_Emissive", EFFECTPARTICLE_DESC::Option4::Use_Emissive);

				if (Check_Option(EFFECTPARTICLE_DESC::Option6::Use_Glow))
					Tool_Glow();

			}
		}
	}
#endif // _EFFECT_TOOL_
}

void CEffect_Rect::Free()
{
}

#ifdef _EFFECT_TOOL_
void CEffect_Rect::Initialize_ParticleShaderInfos()
{
	m_ParticleShaderInfos.reserve(16);

#define REGISGER_PARTICLE_SHADER(variable_name, pass_num, rect_type, discard_type, soft, special)\
	PARTICLE_SHADER_INFO variable_name;\
	ZeroMemory(&variable_name, sizeof(PARTICLE_SHADER_INFO));\
	variable_name.iShaderPassNumber = pass_num;\
	variable_name.eRectType         = PARTICLE_SHADER_INFO::PARTICLE_RECT_TYPE::rect_type;\
	variable_name.eDiscardType      = PARTICLE_SHADER_INFO::PARTICLE_DISCARD_TYPE::discard_type;\
	variable_name.bSoftRendering    = soft;\
	variable_name.bSpecialRendering = special;\
	m_ParticleShaderInfos.emplace_back(variable_name);

	REGISGER_PARTICLE_SHADER(tInfo0, 0, SPRITE, DISCARD_BLACK, false, false);
	REGISGER_PARTICLE_SHADER(tInfo1, 1, DEFAULT, DISCARD_ALPHA, false, false);
	REGISGER_PARTICLE_SHADER(tInfo2, 2, DEFAULT, DISCARD_BLACK, false, false);
	REGISGER_PARTICLE_SHADER(tInfo3, 3, SPRITE, DISCARD_ALPHA, false, false);
	REGISGER_PARTICLE_SHADER(tInfo4, 4, DEFAULT, DISCARD_ALPHA, true, false);
	REGISGER_PARTICLE_SHADER(tInfo5, 5, DEFAULT, DISCARD_BLACK, true, false);
	REGISGER_PARTICLE_SHADER(tInfo6, 6, SPRITE, DISCARD_ALPHA, true, false);
	REGISGER_PARTICLE_SHADER(tInfo7, 7, SPRITE, DISCARD_BLACK, true, false);

	REGISGER_PARTICLE_SHADER(tInfo8, 8, SPRITE, DISCARD_BLACK, false, true);
	REGISGER_PARTICLE_SHADER(tInfo9, 9, DEFAULT, DISCARD_ALPHA, false, true);
	REGISGER_PARTICLE_SHADER(tInfo10, 10, DEFAULT, DISCARD_BLACK, false, true);
	REGISGER_PARTICLE_SHADER(tInfo11, 11, SPRITE, DISCARD_ALPHA, false, true);
	REGISGER_PARTICLE_SHADER(tInfo12, 12, DEFAULT, DISCARD_ALPHA, true, true);
	REGISGER_PARTICLE_SHADER(tInfo13, 13, DEFAULT, DISCARD_BLACK, true, true);
	REGISGER_PARTICLE_SHADER(tInfo14, 14, SPRITE, DISCARD_ALPHA, true, true);
	REGISGER_PARTICLE_SHADER(tInfo15, 15, SPRITE, DISCARD_BLACK, true, true);

#undef REGISGER_PARTICLE_SHADER
}
#endif // _EFFECT_TOOL_

void CEffect_Rect::OnChangeAnimationKey(const _uint& In_Key)
{
	if (m_tEffectParticleDesc.iSyncAnimationKey != (_int)In_Key)
		return;

	weak_ptr<CTransform> pPreviewModelTransform = GET_SINGLE(CWindow_AnimationModelView)->Get_PreviewAnimModel().lock()->Get_Transform();
	Reset_Effect(pPreviewModelTransform);
}
