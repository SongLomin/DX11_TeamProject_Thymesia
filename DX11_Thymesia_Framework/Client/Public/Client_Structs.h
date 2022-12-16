#pragma once
namespace Client
{
	struct EFFECTMESH_DESC
	{
#pragma region Tool Options
		// TODO : for imgui movements -> remove after finishing mesh effect
		_bool		bOnFocus;
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
		_bool		bSyncStartPositionToController;
		_bool		bSyncStartRotationToController;

		_bool		bSyncStartScale;
		_bool		bSyncScaleSpeed;
		_bool		bSyncScaleForce;
		_bool		bSyncMinScale;
		_bool		bSyncMaxScale;
#pragma endregion

#pragma region Datas
#pragma region Life Time
		_float		fInitTime;
		_float		fLifeTime;
#pragma endregion

#pragma region Animation Sync
		_bool		bSyncAnimation;
		_int		iSyncAnimationKey = -1;
#pragma endregion

		_bool		bFollowTransform;
		_bool		bBillBoard;

		_bool		bBoner;

		_float3		vStartPosition;


#pragma region Speed
		_float3		vSpeed;
		_float3		vForce;
		_float3		vMinSpeed;
		_float3		vMaxSpeed;
#pragma endregion

#pragma region Rotation
		_float3		vStartRotation;
		_float3		vRotationSpeed;
		_float3		vRotationForce;
		_float3		vMaxRotation;
#pragma endregion

#pragma region Scale
		_float3		vStartScale;
		_float3		vScaleSpeed;
		_float3		vScaleForce;
		_float3		vMinScale;
		_float3		vMaxScale;
#pragma endregion

		_int		iShaderPassIndex;

#pragma region Dissolve
		_bool bApplyDissolve;

		_int iDissolveAppearShaderPassIndex;
		_int iDissolveDisappearShaderPassIndex;

		_int iDissolveTextureIndex;

		_float4 vDissolveGradiationStartColor;
		_float4 vDissolveGradiationGoalColor;

		_float	fDissolveGradiationDistance;

		_float	fDissolveAppearSpeed;

		_float  fDissolveDisappearTime;
		_float	fDissolveDisappearSpeed;

		// For. Directional Dissolve
		_float3 vDissolveStartPosition;
		_float3 vDissolveDirection;

#pragma endregion

#pragma region Colors
		_float		fDiscardRatio;

		_float4		vStartColor;
		_float4		vColorSpeed;
		_float4		vColorForce;
		_float4		vMinColor;
		_float4		vMaxColor;
#pragma endregion

		_bool		bDynamicNoiseOption;
		/**
		* x : Diffuse | y : Mask | z : Noise | w : Dissolve
		*/
		_float4		vWrapWeight;

#pragma region Diffuse
		_int		iDiffuseTextureIndex;
		_bool		bDiffuseWrap;

		_float2		vDiffuseStartUV;
		_float2		vDiffuseUVSpeed;
		_float2		vDiffuseUVForce;
		_float2		vDiffuseUVMin;
		_float2		vDiffuseUVMax;
#pragma endregion

#pragma region Noise
		_int		iNoiseTextureIndex;
		_bool		bNoiseWrap;

		_float2		vNoiseStartUV;
		_float2		vNoiseUVSpeed;
		_float2		vNoiseUVForce;
		_float2		vNoiseUVMin;
		_float2		vNoiseUVMax;
#pragma endregion

#pragma region Mask
		_int		iMaskTextureIndex;
		_bool		bMaskWrap;

		_float2		vMaskStartUV;
		_float2		vMaskUVSpeed;
		_float2		vMaskUVForce;
		_float2		vMaskUVMin;
		_float2		vMaskUVMax;
#pragma endregion

#pragma region Bloom & Glow
		_bool		bBloom;
		_bool		bGlow;
		_bool		bDistortion;

		_float4		vStartGlowColor;
		_float4		vGlowColorSpeed;
		_float4		vGlowColorForce;
#pragma endregion

#pragma region Collider
		_bool		bCollider;
		_bool		bWeaponSyncTransform;
		_float		fWeaponLifeTime;
		_float		fWeaponScale;
		_int		iHitType;
		_int		iOptionType;
		_float      fDamage;
		_float3		vWeaponOffset;
		_float		fHitFreq;
#pragma endregion
	};

	struct EFFECTPARTICLE_DESC
	{
		_int		iMaxInstance;

		_bool		bLooping;

		_int		iParticleType;
		_int		iFollowTransformType;

		// _bool		bAttraction;
		_float3		vGoalPosition;

		_int		iShaderPassIndex;

#pragma region Animation Sync
		_bool		bSyncAnimation;
		_int		iSyncAnimationKey = -1;
#pragma endregion

#pragma region Life Time
		_float		fInitTime;

		_float		fMinSpawnTime;
		_float		fMaxSpawnTime;

		_float		fMinLifeTime;
		_float		fMaxLifeTime;
#pragma endregion

		// if true, particle position is according to bone
		// _bool		bBoner;

#ifdef _DEBUG
		_bool		bIsMinMaxSame_StartSpeed;
		_bool		bIsMinMaxSame_SpeedForce;
		_bool		bIsMinMaxSame_SpeedLimit;

		_bool		bIsMinMaxSame_StartRotation;
		_bool		bIsMinMaxSame_RotationSpeed;
		_bool		bIsMinMaxSame_RotationForce;
		_bool		bIsMinMaxSame_RotationLimit;

		_bool		bIsMinMaxSame_StartScale;
		_bool		bIsMinMaxSame_ScaleSpeed;
		_bool		bIsMinMaxSame_ScaleForce;
		_bool		bIsMinMaxSame_ScaleLimit;

		_bool		bIsMinMaxSame_StartColor;
		_bool		bIsMinMaxSame_ColorSpeed;
		_bool		bIsMinMaxSame_ColorForce;
		_bool		bIsMinMaxSame_ColorLimit;
#endif // _DEBUG

#pragma region Spawn Position
		_float3		vMinSpawnPosition;
		_float3		vMaxSpawnPosition;

		_float3		vMinSpawnOffsetDirection;
		_float3		vMaxSpawnOffsetDirection;

		_float3		vMinSpawnOffsetRange;
		_float3		vMaxSpawnOffsetRange;
#pragma endregion

		// particle moves toward look
		// _bool		bMoveLook;

		_ubyte		byParticleOption1 = 0;
		enum class ParticleOption1
		{
			Is_Attraction                     = 0b0000'0001
			, Use_MinMax_SpawnTime            = 0b0000'0010
			, Use_MinMax_LifeTime             = 0b0000'0100
			, Is_Boner                        = 0b0000'1000
			, Use_MinMax_SpawnPos             = 0b0001'0000
			, Use_MinMax_SpawnOffsetDirection = 0b0010'0000
			, Use_MinMax_SpawnOffsetRange     = 0b0100'0000
			, Is_MoveLook                     = 0b1000'0000
			, ParticleOption1_END
		};

		// For. Gravity
		_float3		vGravityForce;

#pragma region Easing Position
		_bool		bEasingPosition;
#pragma endregion

#pragma region Speed
		_bool		bEasingSpeed;
		_int		iSpeedEasingType;			// --> is position easing type when EasingPosition is true.
		_float		fSpeedEasingTotalTime;		// --> is position easing total time when EasingPosition is true. 

		_float3		vMinStartSpeed;				// --> is min goal offset position when easing position is true.
		_float3		vMaxStartSpeed;				// --> is max goal offset position when easing position is true.

		_float3		vMinSpeedForce;
		_float3		vMaxSpeedForce;

		_float3		vMinLimitSpeed;
		_float3		vMaxLimitSpeed;

#pragma endregion
#pragma region Rotation
		_bool		bEasingRotation;
		_int		iRotationEasingType;
		_float		fRotationEasingTotalTime;

		_float3		vMinStartRotation;
		_float3		vMaxStartRotation;

		_float3		vMinRotationSpeed;
		_float3		vMaxRotationSpeed;

		_float3		vMinRotationForce;
		_float3		vMaxRotationForce;

		_float3		vMinLimitRotation;
		_float3		vMaxLimitRotation;
#pragma endregion

		_ubyte		byParticleOption2 = 0;
		enum class ParticleOption2
		{
			Use_Gravity         = 0b0000'0001
			, Is_EasingPosition = 0b0000'0010
			, Is_EasingSpeed    = 0b0000'0100
			, Use_Speed         = 0b0000'1000
			, Use_Force         = 0b0001'0000
			, Is_EasingRotation = 0b0010'0000
			, Use_RotationSpeed = 0b0100'0000
			, Use_RotationForce = 0b1000'0000
			, ParticleOption2_END
		};

#pragma region Scale
		_bool		bSquareScale;
		_bool		bRatioScale;

		// For. Ratio Scale
		_float		fMinYScaleRatio;
		_float		fMaxYScaleRatio;

		_bool		bEasingScale;
		_int		iScaleEasingType;

		_float		fScaleEasingTotalTime;

		_float2		vMinStartScale;
		_float2		vMaxStartScale;

		_float2		vMinScaleSpeed;
		_float2		vMaxScaleSpeed;

		_float2		vMinScaleForce;
		_float2		vMaxScaleForce;

		_float2		vMinLimitScale;
		_float2		vMaxLimitScale;
#pragma endregion
#pragma region Color
		_float		fDiscardRatio;
		_bool		IsGrayOnlyUseRed;// when random colors, grayscales

		_bool		bEasingAlpha;
		_int		iAlphaEasingType;

		_float		fAlphaEasingTotalTime;

		_float4		vMinStartColor;
		_float4		vMaxStartColor;

		_float4		vMinColorSpeed;		// w is TargetAlpha when Easing Alpha
		_float4		vMaxColorSpeed;

		_float4		vMinColorForce;
		_float4		vMaxColorForce;

		_float4		vMinColor;
		_float4		vMaxColor;
#pragma endregion
#pragma region Texture
#pragma region Diffuse
		_bool		bDiffuseXInverse;
		_bool		bDiffuseYInverse;

		_int		iDiffuseIndex;
		_float2		vDiffuseStartUV;
		_float2		vDiffuseUVSpeed;
		_float2		vDiffuseUVForce;
		_float2		vDiffuseUVMax;
#pragma endregion
#pragma region Mask
		_int		iMaskIndex;
		_float2		vMaskStartUV;
		_float2		vMaskUVSpeed;
		_float2		vMaskUVForce;
		_float2		vMaskUVMax;
#pragma endregion
#pragma region Noise
		_int		iNoiseIndex;
		_float2		vNoiseStartUV;
		_float2		vNoiseUVSpeed;
		_float2		vNoiseUVForce;
		_float2		vNoiseUVMax;
#pragma endregion
#pragma endregion
#pragma region Bloom & Glow
		_bool		bBloom;
		_bool		bGlow;
		_float4		vStartGlowColor;
		_float4		vGlowColorSpeed;
		_float4		vGlowColorForce;
#pragma endregion
#pragma region For. Sprite
		_bool		bLoopSprite;
		_int		iNumFrameX;
		_int		iNumFrameY;
		_float		fSpriteSpeed;
		_bool		bStopAtEndFrame;
#pragma endregion
	};

	struct ATTACKAREA_DESC
	{
		_float		fHitFreq;
		_float		fWeaponScale;
		_int		iHitType;
		_int		iOptionType;
		_float      fDamage;
		_float3		vWeaponOffset;
	};

	struct WEAPON_DESC
	{
		_int		iHitType;
		_int		iOptionType;
		_float      fDamage;
	};

	struct KEYFRAME_EVENT
	{
		list<_hashcode> EffectGroups;
		list<_bool>		Enable_Weapon; // Enable은 한개지만 리스트가 비어있으면 이벤트가 없다는 것을 알려준다.
	};

	struct DISSOLVE_DESC
	{
		_float fAmount;
		_float3 vDirection;
		_float4	vGlowColor;
		_float3 vStartPos;
		_bool bGlow;
		_bool bBloom;
	};

	struct CINEMA_DESC
	{
		_float3		vPitchYawRoll;
		_float		fDistance;
		_float		fTime;
		INTERPOLATION eInterpolationType;
		_float3		vOffset;
		
	};

	// For. JoJo Particle Tool [Shader]
#ifdef _JOJO_EFFECT_TOOL_
	typedef struct tagJoJoParticleShaderInfo
	{
		enum class PARTICLE_RECT_TYPE		{ DEFAULT, SPRITE, TYPE_END };
		enum class PARTICLE_DISCARD_TYPE	{ DISCARD_ALPHA, DISCARD_BLACK, TYPE_END };

		_uint iShaderPassNumber;
		PARTICLE_RECT_TYPE		eRectType;
		PARTICLE_DISCARD_TYPE	eDiscardType;
		_bool bSoftRendering;
		_bool bSpecialRendering;
	}JJ_PARTICLE_SHADER_INFO;
#endif // _JOJO_EFFECT_TOOL_
}