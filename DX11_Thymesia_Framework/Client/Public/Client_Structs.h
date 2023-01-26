#pragma once
namespace Client
{
	struct EFFECTMESH_DESC
	{
#pragma region Tool Options
		// TODO : for imgui movements -> remove after finishing mesh effect
#ifdef  _DEBUG
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
#endif // _DEBUG
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

		_bool		bBoner;

		_float3		vStartPosition;


#pragma region Speed
		_bool		bMoveByMesh = false;

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
		_bool		bWeaponSyncTransformToEffect;
		_bool		bOnCollision_DisableEffectGroup;
		_bool		bOnCollision_UseEffectGroup;
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

		_float3		vGoalPosition;
		_float		fGoalRange;

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

		_ubyte		byOption1 = 0;
		enum class Option1
		{
			Is_Attraction                     = 0b0000'0001
			, Use_MinMax_SpawnTime            = 0b0000'0010
			, Use_MinMax_LifeTime             = 0b0000'0100
			, Is_Boner                        = 0b0000'1000
			, Use_MinMax_SpawnPos             = 0b0001'0000
			, Use_MinMax_SpawnOffsetDirection = 0b0010'0000
			, Use_MinMax_SpawnOffsetRange     = 0b0100'0000
			, Is_MoveLook                     = 0b1000'0000
			, Option_Spawn_END
		};

		// For. Gravity
		_float3		vGravityForce;

#pragma region Speed
		_float3		vMinStartSpeed;
		_float3		vMaxStartSpeed;

		_float3		vMinSpeedForce;
		_float3		vMaxSpeedForce;

		_float3		vMinLimitSpeed;
		_float3		vMaxLimitSpeed;

#pragma endregion
#pragma region Rotation
		_float3		vMinStartRotation;
		_float3		vMaxStartRotation;

		_float3		vMinRotationSpeed;
		_float3		vMaxRotationSpeed;

		_float3		vMinRotationForce;
		_float3		vMaxRotationForce;

		_float3		vMinLimitRotation;
		_float3		vMaxLimitRotation;
#pragma endregion

		_ubyte		byOption2 = 0;
		enum class Option2
		{
			Use_Gravity              = 0b0000'0001
			, Use_Speed              = 0b0000'0010
			, Use_Force              = 0b0000'0100
			, Use_SpeedMaxLimit      = 0b0000'1000
			, Use_RotationSpeed      = 0b0001'0000
			, Use_RotationForce      = 0b0010'0000
			, Use_RotationLimit	     = 0b0100'0000
			, Option_SpeedRotation_END
		};

#pragma region Scale
		// For. Ratio Scale
		_float		fMinYScaleRatio;
		_float		fMaxYScaleRatio;

		_int		iScaleEasingType;
		_float		fScaleEasingTotalTime;

		_float2		vEasingStartScale;
		_float2		vEasingTargetScale;

		_float2		vMinStartScale;
		_float2		vMaxStartScale;

		_float2		vMinScaleSpeed;
		_float2		vMaxScaleSpeed;

		_float2		vMinScaleForce;
		_float2		vMaxScaleForce;

		_float2		vMinLimitScale;
		_float2		vMaxLimitScale;

		_ubyte		byOption3 = 0;
		enum class Option3
		{
			Square_Scale			 = 0b0000'0001
			, Ratio_Scale			 = 0b0000'0010
			, Easing_Scale			 = 0b0000'0100
			, Use_ScaleSpeed		 = 0b0000'1000
			, Use_ScaleForce		 = 0b0001'0000
			, Stop_At_GoalAttraction = 0b0010'0000
			, Kill_At_GoalAttraction = 0b0100'0000
			, Option_Scale_END
		};
#pragma endregion
#pragma region Color
		_float		fDiscardRatio;

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

		_ubyte		byOption4 = 0;
		enum class Option4
		{
			// Is_Gray          = 0b0000'0001
			Easing_Alpha   = 0b0000'0010
			, Use_ColorSpeed = 0b0000'0100
			, Use_ColorForce = 0b0000'1000
			, Use_Emissive = 0b0001'0000
			, Option_Color_END
		};
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

		_ubyte byOption5;
		enum class Option5
		{
			Use_DiffuseSpeed   = 0b0000'0001
			, Use_DiffuseForce = 0b0000'0010
			, Use_MaskSpeed    = 0b0000'0100
			, Use_MaskForce    = 0b0000'1000
			, Use_NoiseSpeed   = 0b0001'0000
			, Use_NoiseForce   = 0b0010'0000
			, Option_Texture_END
		};
#pragma endregion
#pragma region Bloom & Glow
		_float4		vStartGlowColor;
		_float4		vGlowColorSpeed;
		_float4		vGlowColorForce;

		_ubyte byOption6;
		enum class Option6
		{
			Use_Bloom         = 0b0000'0001
			, Use_Glow        = 0b0000'0010
			, Use_GlowSpeed   = 0b0000'0100
			, Use_GlowForce   = 0b0000'1000
			, Is_Sprite       = 0b0001'0000
			, Loop_Sprite     = 0b0010'0000
			, Sprite_StopAtEnd= 0b0100'0000
			, Sprite_UseMask = 0b1000'0000
			, Option_Glite_END
		};
#pragma endregion
#pragma region For. Sprite
		_int		iNumFrameX;
		_int		iNumFrameY;
		_float		fSpriteSpeed;
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
		_uint		iTimeScaleLayer = 0;
	};

	struct WEAPON_DESC
	{
		_int		iHitType;
		_int		iOptionType;
		_float      fDamage;
	};

	struct KEYSOUND_DESC
	{
		string		szSoundFileName;
		_float		fVolume;
	};

	struct KEYFRAME_EVENT
	{
		list<_hashcode> EffectGroups;
		list<_bool>		Enable_Weapon; // Enable은 한개지만 리스트가 비어있으면 이벤트가 없다는 것을 알려준다.
		list<KEYSOUND_DESC> Sounds;
		vector<KEYSOUND_DESC> RandomSounds;
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

	enum class ATTACKAREA_FLAG
	{
		FOLLOW_TRANSFORM = (1 << 0),
		FOLLOW_EFFECTMESH = (1 << 1),
		DISABLE_EFFECTGROUP = (1 << 2),
		ONCOLLISION_EFFECTGROUP = (1 << 3),
	};

	struct EFFECT_ATTACKAREA_DESC
	{
		_flag AttackAreaFlag;
		string szOnCollisionEffectGroupName;

		void Reset()
		{
			ZeroMemory(&AttackAreaFlag, sizeof(_flag));
			szOnCollisionEffectGroupName.clear();
		}
	};

	struct WATERWAVE_DESC
	{
		_float2		vPosition{};
		_float		fTimeAcc{};
		_float		fVibrationScale{};
		_float		fFreq{};
		_float		fSpeed{};
		_float2		vPadding{};
	};
}
