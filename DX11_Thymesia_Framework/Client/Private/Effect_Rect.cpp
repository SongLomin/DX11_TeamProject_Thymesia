#include "stdafx.h"
#include "Effect_Rect.h"
#include "Client_Components.h"

GAMECLASS_C(CEffect_Rect)
CLONE_C(CEffect_Rect, CGameObject)

const _char* CEffect_Rect::Get_EffectName() const
{
	return m_szEffectName.c_str();
}

HRESULT CEffect_Rect::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_eRenderGroup = RENDERGROUP::RENDER_ALPHABLEND;

	ZeroMemory(&m_tEffectParticleDesc, sizeof(m_tEffectParticleDesc));
	m_tEffectParticleDesc.vMinStartScale = { 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartScale = { 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxScale = { 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxStartColor = { 1.f, 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMinStartColor = { 1.f, 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vMaxColor = { 1.f, 1.f, 1.f, 1.f };
	m_tEffectParticleDesc.vStartGlowColor = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentUV = { 0.f, 0.f };
	m_vCurrentUVForce = { 0.f, 0.f };
	m_vCurrentGlowColor = { 1.f, 1.f, 1.f, 1.f };
	m_vCurrentGlowColorForce = { 0.f, 0.f, 0.f, 0.f };

	m_bFinish = false;
	m_bStopParticle = false;

	return S_OK;
}


HRESULT CEffect_Rect::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pVIBuffer = Add_Component<CVIBuffer_Rect_Instance>();
	//m_pVIBuffer.lock()->Init_Particle(10);
	//m_pVIBuffer.lock()->Init_Particle(3);

	m_pShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_VtxInstance"), 
		VTXTEXCOLOR_INSTANCE_DECLARATION::Element, 
		VTXTEXCOLOR_INSTANCE_DECLARATION::iNumElements);

	m_pMaskTextureCom = Add_Component<CTexture>();
	m_pMaskTextureCom.lock()->Use_Texture("UVMask");
	m_pColorTextureCom = Add_Component<CTexture>();
	m_pColorTextureCom.lock()->Use_Texture("UVColorDiffuse");
	Set_Enable(false);


	return S_OK;
}

void CEffect_Rect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);



	if (m_pVIBuffer.lock()->Get_InstanceCount() != m_tEffectParticleDesc.iMaxInstance)
	{
		Reset_Instance(m_tEffectParticleDesc.iMaxInstance);
	}

	Play(fTimeDelta * GAMEINSTANCE->Get_TimeScale(m_iTimeScaleLayerIndex));
	m_pVIBuffer.lock()->Update(m_tParticleDescs);
}

void CEffect_Rect::LateTick(_float fTimeDelta)
{
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

	m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());

	m_fCurrentInitTime = m_tEffectParticleDesc.fInitTime;
	m_vCurrentUV = m_tEffectParticleDesc.vStartUV;
	m_vCurrentUVForce = { 0.f, 0.f };
	//루핑일때 생존 시간 정해주자.

	m_vCurrentGlowColor = m_tEffectParticleDesc.vStartGlowColor;
	m_vCurrentGlowColorForce = { 0.f, 0.f, 0.f, 0.f };
	m_fPreFrame = 0.f;
	
	m_pParentTransformCom = pParentTransform;
	
	Update_ParentTransform();

	Reset_ParticleDescs();
}

void CEffect_Rect::SetUp_ShaderResource()
{
	if ((_uint)TRANSFORMTYPE::STATIC != m_tEffectParticleDesc.iFollowTransformType)
	{
		Update_ParentTransform();
	}
	__super::SetUp_ShaderResource();

	m_pMaskTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_tEffectParticleDesc.iUVMaskIndex);
	
	_float2 vZeroUV = { 0.f, 0.f };

	if (m_tEffectParticleDesc.bDiffuseUV)
	{
		m_pShaderCom.lock()->Set_RawValue("g_vDiffuseUV", &m_vCurrentUV, sizeof(_float2));
		m_pShaderCom.lock()->Set_RawValue("g_vMaskUV", &vZeroUV, sizeof(_float2));
	}
	else
	{
		m_pShaderCom.lock()->Set_RawValue("g_vDiffuseUV", &vZeroUV, sizeof(_float2));
		m_pShaderCom.lock()->Set_RawValue("g_vMaskUV", &m_vCurrentUV, sizeof(_float2));
	}
	
	m_pShaderCom.lock()->Set_RawValue("g_iNumFrameX", &m_tEffectParticleDesc.iNumFrameX, sizeof(_uint));
	m_pShaderCom.lock()->Set_RawValue("g_iNumFrameY", &m_tEffectParticleDesc.iNumFrameY, sizeof(_uint));

	_vector vCamDir = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[2];

	m_pShaderCom.lock()->Set_RawValue("g_vCamDirection", &vCamDir, sizeof(_vector));
	_bool isBillboard = m_tEffectParticleDesc.iParticleType == (_int)PARTICLETYPE::BILLBOARD;
	m_pShaderCom.lock()->Set_RawValue("g_bBillboard", &isBillboard, sizeof(_bool));

	m_pShaderCom.lock()->Set_RawValue("g_bBloom", &m_tEffectParticleDesc.bBloom, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_bGlow", &m_tEffectParticleDesc.bGlow, sizeof(_bool));
	m_pShaderCom.lock()->Set_RawValue("g_vGlowColor", &m_vCurrentGlowColor, sizeof(_float4));

	m_pShaderCom.lock()->Set_RawValue("g_fDiscardRatio", &m_tEffectParticleDesc.fDiscardRatio, sizeof(_float));
}

void CEffect_Rect::Write_EffectJson(json& Out_Json)
{
	Out_Json["Name"] = m_szEffectName;
	Out_Json["Max_Instance"] = m_tEffectParticleDesc.iMaxInstance;
	Out_Json["Init_Time"] = m_tEffectParticleDesc.fInitTime;
	Out_Json["Is_Looping"] = m_tEffectParticleDesc.bLooping;
	Out_Json["ParticleType"] = m_tEffectParticleDesc.iParticleType;
	Out_Json["Min_Life_Time"] = m_tEffectParticleDesc.fMinLifeTime;
	Out_Json["Sync_Animation"] = m_tEffectParticleDesc.bSyncAnimation;
	Out_Json["Sync_AnimationKey"] = m_tEffectParticleDesc.iSyncAnimationKey;
	Out_Json["Follow_Transform"] = m_tEffectParticleDesc.iFollowTransformType;
	Out_Json["Min_Spawn_Time"] = m_tEffectParticleDesc.fMinSpawnTime;
	Out_Json["Max_Spawn_Time"] = m_tEffectParticleDesc.fMaxSpawnTime;
	Out_Json["Min_Life_Time"] = m_tEffectParticleDesc.fMinLifeTime;
	Out_Json["Max_Life_Time"] = m_tEffectParticleDesc.fMaxLifeTime;
	Out_Json["Is_Rect_Spawn"] = m_tEffectParticleDesc.bRectSpawn;
	Out_Json["ShaderPassIndex"] = m_tEffectParticleDesc.iShaderPassIndex;
	Out_Json["Sprite_Pendulum"] = m_tEffectParticleDesc.bPendulumSprite;
	Out_Json["Sprite_NumFrameX"] = m_tEffectParticleDesc.iNumFrameX;
	Out_Json["Sprite_NumFrameY"] = m_tEffectParticleDesc.iNumFrameY;
	Out_Json["Sprite_FrameSpeed"] = m_tEffectParticleDesc.fSpriteSpeed;

	CJson_Utility::Write_Float3(Out_Json["Min_Start_Position"], m_tEffectParticleDesc.vMinStartPosition);
	CJson_Utility::Write_Float3(Out_Json["Max_Start_Position"], m_tEffectParticleDesc.vMaxStartPosition);
	CJson_Utility::Write_Float3(Out_Json["Min_Offset_Direction"], m_tEffectParticleDesc.vMinOffsetDirection);
	CJson_Utility::Write_Float3(Out_Json["Max_Offset_Direction"], m_tEffectParticleDesc.vMaxOffsetDirection);
	CJson_Utility::Write_Float3(Out_Json["Min_Offset_Range"], m_tEffectParticleDesc.vMinOffsetRange);
	CJson_Utility::Write_Float3(Out_Json["Max_Offset_Range"], m_tEffectParticleDesc.vMaxOffsetRange);
	CJson_Utility::Write_Float3(Out_Json["Min_Speed"], m_tEffectParticleDesc.vMinSpeed);
	CJson_Utility::Write_Float3(Out_Json["Max_Speed"], m_tEffectParticleDesc.vMaxSpeed);
	CJson_Utility::Write_Float3(Out_Json["Min_Force"], m_tEffectParticleDesc.vMinForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Force"], m_tEffectParticleDesc.vMaxForce);
	CJson_Utility::Write_Float3(Out_Json["Min_Start_Position"], m_tEffectParticleDesc.vMinStartPosition);
	Out_Json["Is_Move_Look"] = m_tEffectParticleDesc.bMoveLook;
	CJson_Utility::Write_Float3(Out_Json["Limite_Speed"], m_tEffectParticleDesc.vLimiteSpeed);
	CJson_Utility::Write_Float3(Out_Json["Min_Start_Rotation"], m_tEffectParticleDesc.vMinStartRotation);
	CJson_Utility::Write_Float3(Out_Json["Max_Start_Rotation"], m_tEffectParticleDesc.vMaxStartRotation);
	CJson_Utility::Write_Float3(Out_Json["Rotation_Speed"], m_tEffectParticleDesc.vRotationSpeed);
	CJson_Utility::Write_Float3(Out_Json["Rotation_Force"], m_tEffectParticleDesc.vRotationForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Rotation"], m_tEffectParticleDesc.vMaxRotation);
	CJson_Utility::Write_Float3(Out_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
	CJson_Utility::Write_Float3(Out_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);
	CJson_Utility::Write_Float3(Out_Json["Scale_Speed"], m_tEffectParticleDesc.vScaleSpeed);
	CJson_Utility::Write_Float3(Out_Json["Scale_Force"], m_tEffectParticleDesc.vScaleForce);
	CJson_Utility::Write_Float3(Out_Json["Max_Scale"], m_tEffectParticleDesc.vMaxScale);
	Out_Json["Is_Gray_Only_Use_Red"] = m_tEffectParticleDesc.IsGrayOnlyUseRed;
	CJson_Utility::Write_Float4(Out_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	CJson_Utility::Write_Float4(Out_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);
	CJson_Utility::Write_Float4(Out_Json["Color_Speed"], m_tEffectParticleDesc.vColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Color_Force"], m_tEffectParticleDesc.vColorForce);
	CJson_Utility::Write_Float4(Out_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);
	Out_Json["Discard_Ratio"] = m_tEffectParticleDesc.fDiscardRatio;
	CJson_Utility::Write_Float2(Out_Json["Start_UV"], m_tEffectParticleDesc.vStartUV);
	Out_Json["Is_Diffuse_UV"] = m_tEffectParticleDesc.bDiffuseUV;
	Out_Json["UV_Color_Index"] = m_tEffectParticleDesc.iUVColorIndex;
	Out_Json["UV_Mask_Index"] = m_tEffectParticleDesc.iUVMaskIndex;
	CJson_Utility::Write_Float2(Out_Json["UV_Speed"], m_tEffectParticleDesc.vUVSpeed);
	CJson_Utility::Write_Float2(Out_Json["UV_Force"], m_tEffectParticleDesc.vUVForce);
	CJson_Utility::Write_Float2(Out_Json["UV_Max"], m_tEffectParticleDesc.vUVMax);

	Out_Json["Bloom"] = m_tEffectParticleDesc.bBloom;
	Out_Json["Glow"] = m_tEffectParticleDesc.bGlow;

	CJson_Utility::Write_Float4(Out_Json["Start_Glow_Color"], m_tEffectParticleDesc.vStartGlowColor);
	CJson_Utility::Write_Float4(Out_Json["Glow_Color_Speed"], m_tEffectParticleDesc.vGlowColorSpeed);
	CJson_Utility::Write_Float4(Out_Json["Glow_Color_Force"], m_tEffectParticleDesc.vGlowColorForce);
}

void CEffect_Rect::Load_EffectJson(const json& In_Json, const _uint& In_iTimeScaleLayer)
{
	m_iTimeScaleLayerIndex = In_iTimeScaleLayer;

	m_szEffectName = In_Json["Name"];
	m_tEffectParticleDesc.iMaxInstance = In_Json["Max_Instance"];
	m_tEffectParticleDesc.fInitTime = In_Json["Init_Time"];
	m_tEffectParticleDesc.bLooping = In_Json["Is_Looping"];
	m_tEffectParticleDesc.iParticleType = In_Json["ParticleType"];
	m_tEffectParticleDesc.fMinLifeTime = In_Json["Min_Life_Time"];
	m_tEffectParticleDesc.bSyncAnimation = In_Json["Sync_Animation"];

	if (In_Json.find("Sync_AnimationKey") != In_Json.end())
		m_tEffectParticleDesc.iSyncAnimationKey = In_Json["Sync_AnimationKey"];

	if (In_Json.find("Follow_Transform") != In_Json.end())
		m_tEffectParticleDesc.iFollowTransformType = In_Json["Follow_Transform"];

	m_tEffectParticleDesc.fMinSpawnTime = In_Json["Min_Spawn_Time"];
	m_tEffectParticleDesc.fMaxSpawnTime = In_Json["Max_Spawn_Time"];
	m_tEffectParticleDesc.fMinLifeTime = In_Json["Min_Life_Time"];
	m_tEffectParticleDesc.fMaxLifeTime = In_Json["Max_Life_Time"];
	m_tEffectParticleDesc.bRectSpawn = In_Json["Is_Rect_Spawn"];
	m_tEffectParticleDesc.iShaderPassIndex = In_Json["ShaderPassIndex"];
	m_tEffectParticleDesc.bPendulumSprite = In_Json["Sprite_Pendulum"];
	m_tEffectParticleDesc.iNumFrameX = In_Json["Sprite_NumFrameX"];
	m_tEffectParticleDesc.iNumFrameY = In_Json["Sprite_NumFrameY"];
	m_tEffectParticleDesc.fSpriteSpeed = In_Json["Sprite_FrameSpeed"];
	CJson_Utility::Load_Float3(In_Json["Min_Start_Position"], m_tEffectParticleDesc.vMinStartPosition);
	CJson_Utility::Load_Float3(In_Json["Max_Start_Position"], m_tEffectParticleDesc.vMaxStartPosition);
	CJson_Utility::Load_Float3(In_Json["Min_Offset_Direction"], m_tEffectParticleDesc.vMinOffsetDirection);
	CJson_Utility::Load_Float3(In_Json["Max_Offset_Direction"], m_tEffectParticleDesc.vMaxOffsetDirection);
	CJson_Utility::Load_Float3(In_Json["Min_Offset_Range"], m_tEffectParticleDesc.vMinOffsetRange);
	CJson_Utility::Load_Float3(In_Json["Max_Offset_Range"], m_tEffectParticleDesc.vMaxOffsetRange);
	CJson_Utility::Load_Float3(In_Json["Min_Speed"], m_tEffectParticleDesc.vMinSpeed);
	CJson_Utility::Load_Float3(In_Json["Max_Speed"], m_tEffectParticleDesc.vMaxSpeed);
	CJson_Utility::Load_Float3(In_Json["Min_Force"], m_tEffectParticleDesc.vMinForce);
	CJson_Utility::Load_Float3(In_Json["Max_Force"], m_tEffectParticleDesc.vMaxForce);
	CJson_Utility::Load_Float3(In_Json["Min_Start_Position"], m_tEffectParticleDesc.vMinStartPosition);
	m_tEffectParticleDesc.bMoveLook = In_Json["Is_Move_Look"];
	CJson_Utility::Load_Float3(In_Json["Limite_Speed"], m_tEffectParticleDesc.vLimiteSpeed);
	CJson_Utility::Load_Float3(In_Json["Min_Start_Rotation"], m_tEffectParticleDesc.vMinStartRotation);
	CJson_Utility::Load_Float3(In_Json["Max_Start_Rotation"], m_tEffectParticleDesc.vMaxStartRotation);
	CJson_Utility::Load_Float3(In_Json["Rotation_Speed"], m_tEffectParticleDesc.vRotationSpeed);
	CJson_Utility::Load_Float3(In_Json["Rotation_Force"], m_tEffectParticleDesc.vRotationForce);
	CJson_Utility::Load_Float3(In_Json["Max_Rotation"], m_tEffectParticleDesc.vMaxRotation);
	CJson_Utility::Load_Float3(In_Json["Min_Start_Scale"], m_tEffectParticleDesc.vMinStartScale);
	CJson_Utility::Load_Float3(In_Json["Max_Start_Scale"], m_tEffectParticleDesc.vMaxStartScale);
	CJson_Utility::Load_Float3(In_Json["Scale_Speed"], m_tEffectParticleDesc.vScaleSpeed);
	CJson_Utility::Load_Float3(In_Json["Scale_Force"], m_tEffectParticleDesc.vScaleForce);
	CJson_Utility::Load_Float3(In_Json["Max_Scale"], m_tEffectParticleDesc.vMaxScale);

	if (In_Json.find("Is_Gray_Only_Use_Red") != In_Json.end())
		m_tEffectParticleDesc.IsGrayOnlyUseRed = In_Json["Is_Gray_Only_Use_Red"];

	CJson_Utility::Load_Float4(In_Json["Min_Start_Color"], m_tEffectParticleDesc.vMinStartColor);
	CJson_Utility::Load_Float4(In_Json["Max_Start_Color"], m_tEffectParticleDesc.vMaxStartColor);
	CJson_Utility::Load_Float4(In_Json["Color_Speed"], m_tEffectParticleDesc.vColorSpeed);
	CJson_Utility::Load_Float4(In_Json["Color_Force"], m_tEffectParticleDesc.vColorForce);
	CJson_Utility::Load_Float4(In_Json["Max_Color"], m_tEffectParticleDesc.vMaxColor);

	if (In_Json.find("Discard_Ratio") != In_Json.end())
		m_tEffectParticleDesc.fDiscardRatio = In_Json["Discard_Ratio"];

	CJson_Utility::Load_Float2(In_Json["Start_UV"], m_tEffectParticleDesc.vStartUV);

	if(In_Json.find("Is_Diffuse_UV") != In_Json.end())
		m_tEffectParticleDesc.bDiffuseUV = In_Json["Is_Diffuse_UV"];

	m_tEffectParticleDesc.iUVColorIndex = In_Json["UV_Color_Index"];
	m_tEffectParticleDesc.iUVMaskIndex = In_Json["UV_Mask_Index"];

	CJson_Utility::Load_Float2(In_Json["UV_Speed"], m_tEffectParticleDesc.vUVSpeed);
	CJson_Utility::Load_Float2(In_Json["UV_Force"], m_tEffectParticleDesc.vUVForce);
	CJson_Utility::Load_Float2(In_Json["UV_Max"], m_tEffectParticleDesc.vUVMax);

	if (In_Json.find("Bloom") != In_Json.end())
		m_tEffectParticleDesc.bBloom = In_Json["Bloom"];

	if (In_Json.find("Glow") != In_Json.end())
		m_tEffectParticleDesc.bGlow = In_Json["Glow"];

	if (In_Json.find("Start_Glow_Color") != In_Json.end())
	{
		CJson_Utility::Load_Float4(In_Json["Start_Glow_Color"], m_tEffectParticleDesc.vStartGlowColor);
		CJson_Utility::Load_Float4(In_Json["Glow_Color_Speed"], m_tEffectParticleDesc.vGlowColorSpeed);
		CJson_Utility::Load_Float4(In_Json["Glow_Color_Force"], m_tEffectParticleDesc.vGlowColorForce);
	}

	m_pTextureCom.lock()->Use_Texture(m_szEffectName.c_str());
	
	Reset_Instance(m_tEffectParticleDesc.iMaxInstance);
}

void CEffect_Rect::Play(_float fTimeDelta)
{
	if (m_fCurrentInitTime > 0.f)
	{
		m_fCurrentInitTime -= fTimeDelta;
		return;
	}

	_float fFrameTime = 1.f / 144.f;
	_int	iTickCount = 0;
	//iTickCount = fTimeDelta / fFrameTime;
	//m_fPreFrame += fTimeDelta - (iTickCount * fTimeDelta / fFrameTime);
	m_fPreFrame += fTimeDelta;

	while(m_fPreFrame >= fFrameTime)
	{
		++iTickCount;
		m_fPreFrame -= fFrameTime;
	}

	for (_int i = 0; i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{
		if (!m_tParticleDescs[i].bEnable)
		{
			if (m_tParticleDescs[i].fCurrentSpawnTime > m_tParticleDescs[i].fTargetSpawnTime)
			{
				m_tParticleDescs[i].bEnable = true;

				if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType
					&& m_pParentTransformCom.lock())
				{
					_float3 ParentPosition;
					XMStoreFloat3(&ParentPosition, m_pParentTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));

					SMath::Add_Float3(&m_tParticleDescs[i].vCurrentTarnslation, ParentPosition);
				}
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
					m_tParticleDescs[i].fCurrentSpawnTime = -999999.f;
				}
				continue;
			}

			m_tParticleDescs[i].fCurrentLifeTime += fTimeDelta;
		}


		for (_int x = 0; x < iTickCount; ++x)
		{
			Update_ParticleRotation(i, fFrameTime);
			Update_ParticlePosition(i, fFrameTime);
			Update_ParticleScale(i, fFrameTime);
			Update_ParticleColor(i, fFrameTime);
			Update_ParticleSpriteFrame(i, fFrameTime);
		}
				
		
		
	}

	for (_int x = 0; x < iTickCount; ++x)
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

	m_tParticleDescs = vector<PARTICLE_DESC>(In_ParticleCount, PARTICLE_DESC());
	m_tOriginalParticleDescs = vector<PARTICLE_DESC>(In_ParticleCount, PARTICLE_DESC());

	m_pVIBuffer.lock()->Init_Particle(In_ParticleCount);
	Generate_RandomOriginalParticleDesc();
	Reset_ParticleDescs();
}

void CEffect_Rect::Reset_ParticleDescs()
{
	for (_int i = 0; i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{
		Reset_ParticleDesc((_uint)i);
	}
}

void CEffect_Rect::Reset_ParticleDesc(const _uint& In_iIndex)
{
	memcpy(&m_tParticleDescs[In_iIndex], &m_tOriginalParticleDescs[In_iIndex], sizeof(PARTICLE_DESC));
	SMath::Add_Float3(&m_tParticleDescs[In_iIndex].vCurrentTarnslation, m_tParticleDescs[In_iIndex].vOffsetPosition);
}

void CEffect_Rect::Generate_RandomOriginalParticleDesc()
{
	for (_int i = 0; i < m_tEffectParticleDesc.iMaxInstance; ++i)
	{
		m_tOriginalParticleDescs[i].Reset();

		m_tOriginalParticleDescs[i].fTargetLifeTime =
			SMath::fRandom(m_tEffectParticleDesc.fMinLifeTime, m_tEffectParticleDesc.fMaxLifeTime);

		m_tOriginalParticleDescs[i].fTargetSpawnTime =
			SMath::fRandom(m_tEffectParticleDesc.fMinSpawnTime, m_tEffectParticleDesc.fMaxSpawnTime);


		/*m_tOriginalParticleDescs[i].vCurrentTarnslation =
			SMath::vRandom(m_tEffectParticleDesc.vMinStartPosition, m_tEffectParticleDesc.vMaxStartPosition);*/

		_float3 vRandomDir = SMath::vRandom(m_tEffectParticleDesc.vMinOffsetDirection, m_tEffectParticleDesc.vMaxOffsetDirection);
		_vector vRandomDirFromVector = XMLoadFloat3(&vRandomDir);
		vRandomDirFromVector = XMVector3Normalize(vRandomDirFromVector);

		_float3 vRandomScalar = SMath::vRandom(m_tEffectParticleDesc.vMinOffsetRange, m_tEffectParticleDesc.vMaxOffsetRange);
		_vector vRandomScalarFromVector = XMLoadFloat3(&vRandomScalar);

		_matrix RotationMatrix = SMath::Bake_MatrixNormalizeUseLookVector(vRandomDirFromVector);
		RotationMatrix = SMath::Go_Right(RotationMatrix, vRandomScalarFromVector.m128_f32[0]);
		RotationMatrix = SMath::Go_Up(RotationMatrix, vRandomScalarFromVector.m128_f32[1]);
		RotationMatrix = SMath::Go_Straight(RotationMatrix, vRandomScalarFromVector.m128_f32[2]);

		XMStoreFloat3(&m_tOriginalParticleDescs[i].vCurrentTarnslation, RotationMatrix.r[3]);

		m_tOriginalParticleDescs[i].vOffsetPosition = SMath::vRandom(m_tEffectParticleDesc.vMinStartPosition, m_tEffectParticleDesc.vMaxStartPosition);

		m_tOriginalParticleDescs[i].vTargetSpeed =
			SMath::vRandom(m_tEffectParticleDesc.vMinSpeed, m_tEffectParticleDesc.vMaxSpeed);

		m_tOriginalParticleDescs[i].vTargetForce =
			SMath::vRandom(m_tEffectParticleDesc.vMinForce, m_tEffectParticleDesc.vMaxForce);

		m_tOriginalParticleDescs[i].vCurrentRotation =
			SMath::vRandom(m_tEffectParticleDesc.vMinStartRotation, m_tEffectParticleDesc.vMaxStartRotation);

		m_tOriginalParticleDescs[i].vCurrentScale =
			SMath::vRandom(m_tEffectParticleDesc.vMinStartScale, m_tEffectParticleDesc.vMaxStartScale);

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

		

		m_tOriginalParticleDescs[i].vCurrentUV = m_tEffectParticleDesc.vStartUV;

		m_tOriginalParticleDescs[i].vCurrentScale.x = max(0.00001f, m_tOriginalParticleDescs[i].vCurrentScale.x);
		m_tOriginalParticleDescs[i].vCurrentScale.y = max(0.00001f, m_tOriginalParticleDescs[i].vCurrentScale.y);
		m_tOriginalParticleDescs[i].vCurrentScale.z = max(0.00001f, m_tOriginalParticleDescs[i].vCurrentScale.z);

	}
}

_bool CEffect_Rect::Check_DisableAllParticle()
{
	if (m_tEffectParticleDesc.bLooping && !m_bStopParticle)
	{
		return false;
	}

	if (m_fCurrentInitTime > 0.f)
	{
		return false;
	}
	
	if ((_uint)TIMESCALE_LAYER::EDITER == m_iTimeScaleLayerIndex)
	{
		return false;
	}

	for (auto& elem : m_tParticleDescs)
	{
		if (elem.bEnable || elem.fCurrentSpawnTime >= 0.f)
			return false;
	}

	return true;
}

void CEffect_Rect::Update_ParticlePosition(const _uint& i, _float fTimeDelta)
{

	_float3 vMove = SMath::Mul_Float3(m_tParticleDescs[i].vTargetSpeed, fTimeDelta);
	m_tParticleDescs[i].vCurrentForce =
		SMath::Add_Float3(m_tParticleDescs[i].vCurrentForce, SMath::Mul_Float3(m_tParticleDescs[i].vTargetForce, fTimeDelta));

	vMove = SMath::Add_Float3(vMove, m_tParticleDescs[i].vCurrentForce);

	vMove.x = min(m_tEffectParticleDesc.vLimiteSpeed.x, vMove.x);
	vMove.y = min(m_tEffectParticleDesc.vLimiteSpeed.y, vMove.y);
	vMove.z = min(m_tEffectParticleDesc.vLimiteSpeed.z, vMove.z);

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
			_vector vCurrentPosition = XMLoadFloat3(&m_tParticleDescs[i].vCurrentTarnslation);
			vCurrentPosition += vRotatedPosition;
			XMStoreFloat3(&m_tParticleDescs[i].vCurrentTarnslation, vCurrentPosition);
		}

		else
		{
			_vector vMovePosition = XMLoadFloat3(&vMove);
			_vector vRotatedPosition = XMVector3TransformCoord(vMovePosition, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_tParticleDescs[i].vCurrentRotation)));
			_vector vCurrentPosition = XMLoadFloat3(&m_tParticleDescs[i].vCurrentTarnslation);
			vCurrentPosition += vRotatedPosition;
			XMStoreFloat3(&m_tParticleDescs[i].vCurrentTarnslation, vCurrentPosition);
		}

		
	}

	else
	{
		m_tParticleDescs[i].vCurrentTarnslation =
			SMath::Add_Float3(m_tParticleDescs[i].vCurrentTarnslation, vMove);
	}

	
}

void CEffect_Rect::Update_ParticleRotation(const _uint& i, _float fTimeDelta)
{

	if ((_int)PARTICLETYPE::OUTBURST == m_tEffectParticleDesc.iParticleType)
	{
		_vector vUp = XMLoadFloat3(&m_tOriginalParticleDescs[i].vCurrentTarnslation); //커런트 포지션
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
		_float3 vRotation = SMath::Mul_Float3(m_tEffectParticleDesc.vRotationSpeed, fTimeDelta);
		m_tParticleDescs[i].vCurrentRotationForce =
			SMath::Add_Float3(m_tParticleDescs[i].vCurrentRotationForce, SMath::Mul_Float3(m_tEffectParticleDesc.vRotationForce, fTimeDelta));

		vRotation = SMath::Add_Float3(vRotation, m_tParticleDescs[i].vCurrentRotationForce);

		m_tParticleDescs[i].vCurrentRotation = SMath::Add_Float3(m_tParticleDescs[i].vCurrentRotation, vRotation);
	}
}

void CEffect_Rect::Update_ParticleScale(const _uint& i, _float fTimeDelta)
{
	_float3 vScale = SMath::Mul_Float3(m_tEffectParticleDesc.vScaleSpeed, fTimeDelta);

	m_tParticleDescs[i].vCurrentScaleForce =
		SMath::Add_Float3(m_tParticleDescs[i].vCurrentScaleForce, SMath::Mul_Float3(m_tEffectParticleDesc.vScaleForce, fTimeDelta));

	SMath::Add_Float3(&m_tParticleDescs[i].vCurrentScale, m_tParticleDescs[i].vCurrentScaleForce);

	m_tParticleDescs[i].vCurrentScale = SMath::Add_Float3(m_tParticleDescs[i].vCurrentScale, vScale);

	m_tParticleDescs[i].vCurrentScale.x = max(0.00001f, min(m_tEffectParticleDesc.vMaxScale.x, m_tParticleDescs[i].vCurrentScale.x));
	m_tParticleDescs[i].vCurrentScale.y = max(0.00001f, min(m_tEffectParticleDesc.vMaxScale.y, m_tParticleDescs[i].vCurrentScale.y));
	m_tParticleDescs[i].vCurrentScale.z = max(0.00001f, min(m_tEffectParticleDesc.vMaxScale.z, m_tParticleDescs[i].vCurrentScale.z));
}

void CEffect_Rect::Update_ParticleUV(_float fTimeDelta)
{
	_vector vUVSpeed = XMLoadFloat2(&m_tEffectParticleDesc.vUVSpeed) * fTimeDelta;
	m_vCurrentUVForce.x += m_tEffectParticleDesc.vUVForce.x * fTimeDelta;
	m_vCurrentUVForce.y += m_tEffectParticleDesc.vUVForce.y * fTimeDelta;

	_vector vMoveUV = vUVSpeed + XMLoadFloat2(&m_vCurrentUVForce);
	_vector vCurrentUV = XMLoadFloat2(&m_vCurrentUV);
	vCurrentUV += vMoveUV;

	vCurrentUV.m128_f32[0] = min(m_tEffectParticleDesc.vUVMax.x, vCurrentUV.m128_f32[0]);
	vCurrentUV.m128_f32[1] = min(m_tEffectParticleDesc.vUVMax.y, vCurrentUV.m128_f32[1]);

	XMStoreFloat2(&m_vCurrentUV, vCurrentUV);
}

void CEffect_Rect::Update_ParticleColor(const _uint& i, _float fTimeDelta)
{
	_float4 vColor;

	if (m_tEffectParticleDesc.IsGrayOnlyUseRed)
	{
		vColor.x = m_tEffectParticleDesc.vColorSpeed.x * fTimeDelta;
		vColor.w = m_tEffectParticleDesc.vColorSpeed.w * fTimeDelta;
		m_tParticleDescs[i].vCurrentColorForce.x += m_tEffectParticleDesc.vColorForce.x * fTimeDelta;
		m_tParticleDescs[i].vCurrentColorForce.w += m_tEffectParticleDesc.vColorForce.w * fTimeDelta;
		vColor.x += m_tParticleDescs[i].vCurrentColorForce.x;
		vColor.w += m_tParticleDescs[i].vCurrentColorForce.w;
		vColor.x += m_tParticleDescs[i].vCurrentColor.x;
		vColor.w += m_tParticleDescs[i].vCurrentColor.w;

		vColor.y = vColor.x;
		vColor.z = vColor.x;

		vColor.x = min(m_tEffectParticleDesc.vMaxColor.x, vColor.x);
		vColor.y = min(m_tEffectParticleDesc.vMaxColor.y, vColor.y);
		vColor.z = min(m_tEffectParticleDesc.vMaxColor.z, vColor.z);
		vColor.w = min(m_tEffectParticleDesc.vMaxColor.w, vColor.w);
	}

	else
	{
		vColor = SMath::Mul_Float4(m_tEffectParticleDesc.vColorSpeed, fTimeDelta);
		SMath::Add_Float4(&m_tParticleDescs[i].vCurrentColorForce, SMath::Mul_Float4(m_tEffectParticleDesc.vColorForce, fTimeDelta));

		SMath::Add_Float4(&vColor, m_tParticleDescs[i].vCurrentColorForce);
		SMath::Add_Float4(&vColor, m_tParticleDescs[i].vCurrentColor);

		vColor.x = min(m_tEffectParticleDesc.vMaxColor.x, vColor.x);
		vColor.y = min(m_tEffectParticleDesc.vMaxColor.y, vColor.y);
		vColor.z = min(m_tEffectParticleDesc.vMaxColor.z, vColor.z);
		vColor.w = min(m_tEffectParticleDesc.vMaxColor.w, vColor.w);
	}

	

	m_tParticleDescs[i].vCurrentColor = vColor;
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
	m_tParticleDescs[i].fCurrentSpriteTime += fTimeDelta;
	if (m_tEffectParticleDesc.fSpriteSpeed <= m_tParticleDescs[i].fCurrentSpriteTime)
	{
		m_tParticleDescs[i].fCurrentSpriteTime = 0.f;


		if (!m_tEffectParticleDesc.bPendulumSprite)
		{
			m_tParticleDescs[i].vSpriteUV.x += (1.f / m_tEffectParticleDesc.iNumFrameX);

			if ((1.f - 1.f / m_tEffectParticleDesc.iNumFrameX) <= m_tParticleDescs[i].vSpriteUV.x &&
				(1.f - 1.f / m_tEffectParticleDesc.iNumFrameY) <= m_tParticleDescs[i].vSpriteUV.y)
			{
				ZeroMemory(&m_tParticleDescs[i].vSpriteUV, sizeof(_float2));
				return;
			}

			if ((1.f - 1.f / m_tEffectParticleDesc.iNumFrameX) <= m_tParticleDescs[i].vSpriteUV.x)
			{
				m_tParticleDescs[i].vSpriteUV.x = 0.f;
				m_tParticleDescs[i].vSpriteUV.y += (1.f / m_tEffectParticleDesc.iNumFrameY);
				return;
			}
		}
		else
		{
			if (!m_tParticleDescs[i].bFramePlayBackward)
			{
				m_tParticleDescs[i].vSpriteUV.x += (1.f / m_tEffectParticleDesc.iNumFrameX);

				if ((1.f - 1.f / m_tEffectParticleDesc.iNumFrameX) <= m_tParticleDescs[i].vSpriteUV.x &&
					(1.f - 1.f / m_tEffectParticleDesc.iNumFrameY) <= m_tParticleDescs[i].vSpriteUV.y)
				{
					m_tParticleDescs[i].vSpriteUV.x = 1.f - 1.f / m_tEffectParticleDesc.iNumFrameX;
					m_tParticleDescs[i].vSpriteUV.y = 1.f - 1.f / m_tEffectParticleDesc.iNumFrameY;
					m_tParticleDescs[i].bFramePlayBackward = true;
					return;
				}

				if ((1.f - 1.f / m_tEffectParticleDesc.iNumFrameX) <= m_tParticleDescs[i].vSpriteUV.x)
				{
					m_tParticleDescs[i].vSpriteUV.x = 0.f;
					m_tParticleDescs[i].vSpriteUV.y += (1.f / m_tEffectParticleDesc.iNumFrameY);
					return;
				}
			}
			else
			{
				m_tParticleDescs[i].vSpriteUV.x -= (1.f / m_tEffectParticleDesc.iNumFrameX);

				if (0.f >= m_tParticleDescs[i].vSpriteUV.x && 0.f >= m_tParticleDescs[i].vSpriteUV.y)
				{
					m_tParticleDescs[i].bFramePlayBackward = false;
					return;
				}

				if (0.f >= m_tParticleDescs[i].vSpriteUV.x)
				{
					m_tParticleDescs[i].vSpriteUV.x = 1.f - 1.f / m_tEffectParticleDesc.iNumFrameX;
					m_tParticleDescs[i].vSpriteUV.y -= (1.f / m_tEffectParticleDesc.iNumFrameY);
					return;
				}
			}
		}
	}
}

void CEffect_Rect::Update_ParentTransform()
{
	if (m_pParentTransformCom.lock())
	{
		if ((_int)PARTICLETYPE::NONE == m_tEffectParticleDesc.iParticleType ||
			(_int)PARTICLETYPE::OUTBURST == m_tEffectParticleDesc.iParticleType)
		{
			//_matrix RotationMatrix = SMath::Get_RotationMatrix(UnscaledMatrix);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_RIGHT, RotationMatrix.r[0]);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_UP, RotationMatrix.r[1]);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_LOOK, RotationMatrix.r[2]);

			m_pTransformCom.lock()->Set_WorldMatrix(m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix());
		}

		else if ((_int)PARTICLETYPE::BILLBOARD == m_tEffectParticleDesc.iParticleType)
		{
			m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, m_pParentTransformCom.lock()->Get_UnScaledWorldMatrix().r[3]);
		}

		if ((_int)TRANSFORMTYPE::JUSTSPAWN == m_tEffectParticleDesc.iFollowTransformType)
		{
			//_matrix RotationMatrix = SMath::Get_RotationMatrix(UnscaledMatrix);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_RIGHT, RotationMatrix.r[0]);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_UP, RotationMatrix.r[1]);
			//m_pTransformCom.lock()->Set_State(CTransform::STATE_LOOK, RotationMatrix.r[2]);

			m_pTransformCom.lock()->Set_WorldMatrix(XMMatrixIdentity());
		}

	}

}

void CEffect_Rect::OnEventMessage(_uint iArg)
{
	if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
	{
		if (ImGui::CollapsingHeader("Effect_Particle"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("Max Instance");
			ImGui::SameLine();
			ImGui::DragInt("##Max_Instance", &m_tEffectParticleDesc.iMaxInstance, 0.1f);

			ImGui::Text("Is Looping");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Looping", &m_tEffectParticleDesc.bLooping);
			ImGui::Separator();


			ImGui::Text("Pass 0 : Default");
			ImGui::Text("Pass 1 : Sprite Image");
			ImGui::InputInt("Shader Pass", &m_tEffectParticleDesc.iShaderPassIndex);

			if (1 == m_tEffectParticleDesc.iShaderPassIndex)
			{
				ImGui::Text("Pendulum Effect");
				ImGui::SameLine();
				ImGui::Checkbox("##Pendulum_Effect", &m_tEffectParticleDesc.bPendulumSprite);
				ImGui::Separator();

				ImGui::InputInt("NumFramesX", &m_tEffectParticleDesc.iNumFrameX);
				ImGui::InputInt("NumFramesY", &m_tEffectParticleDesc.iNumFrameY);
				ImGui::InputFloat("FrameSpeed", &m_tEffectParticleDesc.fSpriteSpeed);
			}

			const char* items[] = { "None", "Outburst", "Billboard"};
			
			if (ImGui::BeginListBox("ParticleType"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (m_tEffectParticleDesc.iParticleType == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						m_tEffectParticleDesc.iParticleType = n;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			const char* Transform_items[] = { "Static", "Just Spawn", "Child" };

			if (ImGui::BeginListBox("Transform Type"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
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
			ImGui::Separator();

			ImGui::Text("Is Sync Animation");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Sync_Animation", &m_tEffectParticleDesc.bSyncAnimation);

			ImGui::InputInt("Sync Animation Key", &m_tEffectParticleDesc.iSyncAnimationKey);
			ImGui::Separator();

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

			/*ImGui::Text("Is Rect Spawn");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Rect_Spawn", &m_tEffectParticleDesc.bRectSpawn);
			ImGui::Separator();*/

			ImGui::Text("Min Start Position");
			ImGui::DragFloat3("##Min_Start_Position", &m_tEffectParticleDesc.vMinStartPosition.x, 0.1f);

			ImGui::Text("Max Start Position");
			ImGui::DragFloat3("##Max_Start_Position", &m_tEffectParticleDesc.vMaxStartPosition.x, 0.1f);

			ImGui::Text("Min Offset Direction");
			ImGui::DragFloat3("##Min_Offset_Direction", &m_tEffectParticleDesc.vMinOffsetDirection.x, 0.1f);

			ImGui::Text("Max Offset Direction");
			ImGui::DragFloat3("##Max_Offset_Direction", &m_tEffectParticleDesc.vMaxOffsetDirection.x, 0.1f);

			ImGui::Text("Min Offset Range");
			ImGui::DragFloat3("##Min_Offset_Range", &m_tEffectParticleDesc.vMinOffsetRange.x, 0.1f);

			ImGui::Text("Max Offset Range");
			ImGui::DragFloat3("##Max_Offset_Range", &m_tEffectParticleDesc.vMaxOffsetRange.x, 0.1f);
			ImGui::Separator();

			ImGui::Text("Min Speed");
			ImGui::DragFloat3("##Min_Speed", &m_tEffectParticleDesc.vMinSpeed.x, 0.1f);

			ImGui::Text("Max Speed");
			ImGui::DragFloat3("##Max_Speed", &m_tEffectParticleDesc.vMaxSpeed.x, 0.1f);

			ImGui::Text("Min Force");
			ImGui::DragFloat3("##Min_Force", &m_tEffectParticleDesc.vMinForce.x, 0.1f);

			ImGui::Text("Max Force");
			ImGui::DragFloat3("##Max_Force", &m_tEffectParticleDesc.vMaxForce.x, 0.1f);

			ImGui::Text("Is Move Look");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Move_Look", &m_tEffectParticleDesc.bMoveLook);

			ImGui::Text("Limite Speed");
			ImGui::DragFloat3("##Limite_Speed", &m_tEffectParticleDesc.vLimiteSpeed.x, 0.1f);
			ImGui::Separator();

			ImGui::Text("Min Start Rotation");
			ImGui::DragFloat3("##Min_Start_Rotation", &m_tEffectParticleDesc.vMinStartRotation.x, 0.1f);

			ImGui::Text("Max Start Rotation");
			ImGui::DragFloat3("##Max_Start_Rotation", &m_tEffectParticleDesc.vMaxStartRotation.x, 0.1f);

			ImGui::Text("Rotation Speed");
			ImGui::DragFloat3("##Rotation_Speed", &m_tEffectParticleDesc.vRotationSpeed.x, 0.1f);

			ImGui::Text("Rotation Force");
			ImGui::DragFloat3("##Rotation_Force", &m_tEffectParticleDesc.vRotationForce.x, 0.1f);

			ImGui::Text("Max Rotation");
			ImGui::DragFloat3("##Max_Rotation", &m_tEffectParticleDesc.vMaxRotation.x, 0.1f);
			ImGui::Separator();

			ImGui::Text("Min Start Scale");
			ImGui::DragFloat3("##Min_Start_Scale", &m_tEffectParticleDesc.vMinStartScale.x, 0.1f);

			ImGui::Text("Max Start Scale");
			ImGui::DragFloat3("##Max_Start_Scale", &m_tEffectParticleDesc.vMaxStartScale.x, 0.1f);

			ImGui::Text("Scale Speed");
			ImGui::DragFloat3("##Scale_Speed", &m_tEffectParticleDesc.vScaleSpeed.x, 0.1f);

			ImGui::Text("Scale Force");
			ImGui::DragFloat3("##Scale_Force", &m_tEffectParticleDesc.vScaleForce.x, 0.1f);

			ImGui::Text("Max Scale");
			ImGui::DragFloat3("##Max_Scale", &m_tEffectParticleDesc.vMaxScale.x, 0.1f);
			ImGui::Separator();

			ImGui::Text("Is Diffuse UV");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Diffuse_UV", &m_tEffectParticleDesc.bDiffuseUV);

			ImGui::InputInt("UV Color Map Index", &m_tEffectParticleDesc.iUVColorIndex);

			ImGui::Text("Is Gray Only Use Red");
			ImGui::SameLine();
			ImGui::Checkbox("##Is_Gray_Only_Use_Red", &m_tEffectParticleDesc.IsGrayOnlyUseRed);

			ImGui::Text("Min Start Color");
			ImGui::DragFloat4("##Min_Start_Color", &m_tEffectParticleDesc.vMinStartColor.x, 0.01f);

			ImGui::Text("Max Start Color");
			ImGui::DragFloat4("##Max_Start_Color", &m_tEffectParticleDesc.vMaxStartColor.x, 0.01f);

			ImGui::Text("Color Speed");
			ImGui::DragFloat4("##Color_Speed", &m_tEffectParticleDesc.vColorSpeed.x, 0.01f);

			ImGui::Text("Color Force");
			ImGui::DragFloat4("##Color_Force", &m_tEffectParticleDesc.vColorForce.x, 0.01f);

			ImGui::Text("Max Color");
			ImGui::DragFloat4("##Max_Color", &m_tEffectParticleDesc.vMaxColor.x, 0.01f);
			ImGui::Separator();

			
			ImGui::Text("Discard Ratio");
			ImGui::DragFloat("##Discard_Ratio", &m_tEffectParticleDesc.fDiscardRatio, 0.01f, 0.f, 1.f);
			ImGui::Separator();

			
			ImGui::Text("Start UV");
			ImGui::DragFloat2("##Start_UV", &m_tEffectParticleDesc.vStartUV.x, 0.1f);

			ImGui::InputInt("UV Mask Index", &m_tEffectParticleDesc.iUVMaskIndex);

			ImGui::Text("UV Speed");
			ImGui::DragFloat2("##UV_Speed", &m_tEffectParticleDesc.vUVSpeed.x, 0.1f);

			ImGui::Text("UV Force");
			ImGui::DragFloat2("##UV_Force", &m_tEffectParticleDesc.vUVForce.x, 0.1f);

			ImGui::Text("UV Max");
			ImGui::DragFloat2("##UV_Max", &m_tEffectParticleDesc.vUVMax.x, 0.1f);
			ImGui::Separator();

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
