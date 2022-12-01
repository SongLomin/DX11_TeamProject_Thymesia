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
		* x : Diffuse | y : Mask | z : Noise | w : None
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

		_int		iShaderPassIndex;

#pragma region Animation Sync
		_bool		bSyncAnimation;
		_int		iSyncAnimationKey = -1;
#pragma endregion
#pragma region Life Time
		_float		fInitTime;

		_float		fMinSpawnTime; // becomse spawn time if bIsSpawnList is true
		_float		fMaxSpawnTime;

		_float		fMinLifeTime;
		_float		fMaxLifeTime;
#pragma endregion

		// if true, particle position is according to bone
		_bool		bBoner;

#pragma region Spawn Position
		
		_bool		bIsMinMaxSame_SpawnPosition; // TODO : for tool -> erase
		_float3		vMinSpawnPosition; // becomes spawn position if bIsSpawnList is true
		_float3		vMaxSpawnPosition;

		_bool		bIsMinMaxSame_OffsetDirection; // TODO : for tool -> erase
		_float3		vMinSpawnOffsetDirection;
		_float3		vMaxSpawnOffsetDirection;

		_bool		bIsMinMaxSame_OffsetRange; // TODO : for tool -> erase
		_float3		vMinSpawnOffsetRange;
		_float3		vMaxSpawnOffsetRange;
#pragma endregion

		// particle moves toward look
		_bool		bMoveLook;

		// For. Gravity
		_bool		bUseGravity;
		_float3		vGravityForce;

#pragma region Easing Position
		_bool		bEasingPosition;
#pragma endregion

#pragma region Speed
		_bool		bEasingSpeed;
		_int		iSpeedEasingType;			// --> is position easing type when EasingPosition is true.
		_float		fSpeedEasingTotalTime;		// --> is position easing total time when EasingPosition is true. 

		_bool		bIsMinMaxSame_StartSpeed; // TODO : for tool -> erase
		_float3		vMinSpeed;					// --> is min goal offset position when easing position is true.
		_float3		vMaxSpeed;					// --> is max goal offset position when easing position is true.

		_bool		bIsMinMaxSame_SpeedForce; // TODO : for tool -> erase
		_float3		vMinSpeedForce;
		_float3		vMaxSpeedForce;

		_bool		bIsMinMaxSame_SpeedLimit; // TODO : for tool -> erase
		_float3		vMinLimitSpeed;
		_float3		vMaxLimitSpeed;
#pragma endregion
#pragma region Rotation
		_bool		bEasingRotation;
		_int		iRotationEasingType;

		_float		fRotationEasingTotalTime;

		_bool		bIsMinMaxSame_StartRotation; // TODO : for tool -> erase
		_float3		vMinStartRotation; // // becomes spawn look if bIsSpawnList is true
		_float3		vMaxStartRotation;

		_bool		bIsMinMaxSame_RotationSpeed; // TODO : for tool -> erase
		_float3		vMinRotationSpeed;
		_float3		vMaxRotationSpeed;

		_bool		bIsMinMaxSame_RotationForce; // TODO : for tool -> erase
		_float3		vMinRotationForce;
		_float3		vMaxRotationForce;

		_bool		bIsMinMaxSame_RotationLimit; // TODO : for tool -> erase
		_float3		vMinLimitRotation;
		_float3		vMaxLimitRotation;
#pragma endregion
#pragma region Scale
		_bool		bEasingScale;
		_int		iScaleEasingType;

		_float		fScaleEasingTotalTime;

		_bool		bIsMinMaxSame_StartScale; // TODO : for tool -> erase
		_float3		vMinStartScale;
		_float3		vMaxStartScale;

		_bool		bIsMinMaxSame_ScaleSpeed; // TODO : for tool -> erase
		_float3		vMinScaleSpeed;
		_float3		vMaxScaleSpeed;

		_bool		bIsMinMaxSame_ScaleForce; // TODO : for tool -> erase
		_float3		vMinScaleForce;
		_float3		vMaxScaleForce;

		_bool		bIsMinMaxSame_ScaleLimit; // TODO : for tool -> erase
		_float3		vMinLimitScale;
		_float3		vMaxLimitScale;
#pragma endregion
#pragma region Color
		_float		fDiscardRatio;
		_bool		IsGrayOnlyUseRed;// when random colors, grayscales

		_bool		bIsMinMaxSame_StartColor; // TODO : for tool -> erase
		_float4		vMinStartColor;
		_float4		vMaxStartColor;

		_bool		bIsMinMaxSame_ColorSpeed; // TODO : for tool -> erase
		_float4		vMinColorSpeed;
		_float4		vMaxColorSpeed;

		_bool		bIsMinMaxSame_ColorForce; // TODO : for tool -> erase
		_float4		vMinColorForce;
		_float4		vMaxColorForce;

		_bool		bIsMinMaxSame_ColorLimit; // TODO : for tool -> erase
		_float4		vMinColor;
		_float4		vMaxColor;
#pragma endregion
#pragma region Texture
#pragma region Diffuse
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

	struct CINEMA_DESC
	{
		_float3		vPitchYawRoll;
		_float		fDistance;
		_float		fTime;
		INTERPOLATION eInterpolationType;
		_float3		vOffset;
		
	};

}