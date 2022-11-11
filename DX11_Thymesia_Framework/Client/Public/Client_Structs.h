#pragma once

namespace Client
{
	struct EFFECTMESH_DESC
	{
#pragma region Tool Options
		// TODO : for imgui movements
		_bool		bOnFocus;
#pragma endregion

#pragma region Datas
		_float		fInitTime;
		_float		fLifeTime;

		_bool		bSyncAnimation;
		_int		iSyncAnimationKey = -1;

		_bool		bFollowTransform;
		_bool		bBillBoard;

		_float3		vStartPosition;

		_float3		vSpeed;
		_float3		vForce;
		_float3		vMaxSpeed;

		_float3		vStartRotation;
		_float3		vRotationSpeed;
		_float3		vRotationForce;
		_float3		vMaxRotation;

		_float3		vStartScale;
		_float3		vScaleSpeed;
		_float3		vScaleForce;
		_float3		vMaxScale;

		_int		iShaderPassIndex;

		_float		fDiscardRatio;

		_float4		vStartColor;
		_float4		vColorSpeed;
		_float4		vColorForce;
		_float4		vMaxColor;

		_int		iUVDiffuseIndex;
		_int		iUVMaskIndex;
		_int		iUVNoiseIndex;

		// TODO : bDynamicNoiseOption temporary for test
		_bool		bDynamicNoiseOption;
		/**
		* x : Diffuse | y : Mask | z : Noise | w : None
		*/
		_float4		vWrapWeight;

		_float2		vStartUV;
		_float2		vUVSpeed;
		_float2		vUVForce;
		_float2		vUVMax;

		_bool		bBloom;
		_bool		bGlow;
		_bool		bDistortion;

		_float4		vStartGlowColor;
		_float4		vGlowColorSpeed;
		_float4		vGlowColorForce;

		_bool		bCollider;
		_bool		bWeaponSyncTransform;
		_float		fWeaponLifeTime;
		_float		fWeaponScale;
		_int		iHitType;
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
		_bool		bSyncAnimation;
		_int		iFollowTransformType;
		_int		iSyncAnimationKey = -1;
		_bool		bRectSpawn;

		_float		fInitTime;

		_float		fMinSpawnTime;
		_float		fMaxSpawnTime;


		_float		fMinLifeTime;
		_float		fMaxLifeTime;

		_float3		vMinStartPosition;
		_float3		vMaxStartPosition;

		_float3		vMinOffsetDirection;
		_float3		vMaxOffsetDirection;
		_float3		vMinOffsetRange;
		_float3		vMaxOffsetRange;

		_float3		vMinSpeed;
		_float3		vMaxSpeed;

		_float3		vMinForce;
		_float3		vMaxForce;

		_bool		bMoveLook;

		_float3		vLimiteSpeed;

		_float3		vMinStartRotation;
		_float3		vMaxStartRotation;

		_float3		vRotationSpeed;
		_float3		vRotationForce;
		_float3		vMaxRotation;


		_float3		vMinStartScale;
		_float3		vMaxStartScale;

		_float3		vScaleSpeed;
		_float3		vScaleForce;
		_float3		vMaxScale;

		_bool		IsGrayOnlyUseRed;

		_float4		vMinStartColor;
		_float4		vMaxStartColor;
		_float4		vColorSpeed;
		_float4		vColorForce;
		_float4		vMaxColor;

		_float fDiscardRatio;

		_bool		bDiffuseUV;
		_float2		vStartUV;
		_int		iUVMaskIndex;
		_float2		vUVSpeed;
		_float2		vUVForce;
		_float2		vUVMax;

		_int		iUVColorIndex;

		_bool		bBloom;
		_bool		bGlow;
		_float4		vStartGlowColor;
		_float4		vGlowColorSpeed;
		_float4		vGlowColorForce;

		_int	iShaderPassIndex;

		// for sprite images
		_bool		bPendulumSprite;
		_int		iNumFrameX;
		_int		iNumFrameY;
		_float2		fCurFrame;
		_float		fSpriteSpeed;
	};

	struct WEAPON_DESC
	{
		_float		fHitFreq;
		_float		fWeaponScale;
		_int		iHitType;
		_float      fDamage;
		_float3		vWeaponOffset;
	};

	struct KEYFRAME_EVENT
	{
		list<_hashcode> EffectGroups;
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