#pragma once

namespace Engine
{
	class CTexture;

	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	typedef struct tagKeyData
	{
		_float3	vValue;
		_float	fTime;

	} KEY_DATA;

	typedef struct tagRotationKeyData
	{
		_float4	vValue;
		_float	fTime;
	} ROTATIONKEY_DATA;

	typedef struct tagKeyFrame
	{
		_float3	vScale;
		_float4	vRotation;
		_float3	vPosition;
		_float	fTime;
	} KEYFRAME;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOTLIGHT, TYPE_END };


		_bool	bEnable    = false;
		TYPE	eActorType = TYPE_END;
				 
		_float4 vDirection = { 0.f, 0.f, 0.f, 0.f };
				 
		_float4 vPosition  = { 0.f, 0.f, 0.f, 0.f };
		_float	fRange     = 0.f;
		_float  fIntensity = 0.f;
				 
		_float4 vDiffuse   = { 0.f, 0.f, 0.f, 0.f };
		_float4 vAmbient   = { 0.f, 0.f, 0.f, 0.f };
		_float4 vSpecular  = { 0.f, 0.f, 0.f, 0.f };
				 
		_float4 vLightFlag = { 0.f, 0.f, 0.f, 0.f };

		_float fCutOff = 0.f;
		_float fOuterCutOff = 0.f;
	private:
		_uint iLightIndex = _uint(-1);

	public:
		const _uint Get_LightIndex() const { return iLightIndex; }
		friend class CCustomLight;
	}LIGHTDESC;

	typedef struct tag_MeshVertextPostion
	{
		_float3 vMin;
		_float3 vMax;
		_float3 vCenter;

	} MESH_VTX_INFO;

	// !!! DO NOT ZeroMemory() !!!
	// When created, it is automatically initialized. If you want to reset, call the Reset function. 
	typedef struct tagTextInfo
	{
		tstring	szText;
		_float2	vPosition;
		_float2	vScale;
		_float	fRotation;
		_float4	vColor;
		_bool	bAlways;
		_bool	bCenterAlign;
		RENDERGROUP	eRenderGroup;

		tagTextInfo()
		{
			Reset();
		}

		void Reset()
		{
			ZeroMemory(&vPosition, sizeof(_float2));
			ZeroMemory(&vScale, sizeof(_float2));
			ZeroMemory(&fRotation, sizeof(_float));
			ZeroMemory(&vColor, sizeof(_float4));
			ZeroMemory(&bAlways, sizeof(_bool));
			ZeroMemory(&bCenterAlign, sizeof(_bool));
			eRenderGroup = RENDERGROUP::RENDER_UI;
		}
	} TEXTINFO;

	typedef struct tagRayDesc
	{
		_float4	vOrigin;
		_float3	vDirection;
		float	fLength;
	}RAY;

	typedef struct tagLineIndices32
	{
		unsigned long _1, _2;
	}LINEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short _1, _2;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long _1, _2, _3;
	}FACEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short _1, _2, _3;
	}FACEINDICES16;


	typedef struct tagVertex_Position
	{
		_float3		vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int		iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXPOS_DECLARATION;

	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_DECLARATION;

	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;

	typedef struct tagVertex_Ground_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXGROUND;

	typedef struct ENGINE_DLL tagVertex_Ground_Texture_Declaration
	{
		static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXGROUND_DECLARATION;


	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXCUBETEX_DECLARATION;


	typedef struct tagVertex_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;

		tagVertex_Model()
		{
			ZeroMemory(this, sizeof(tagVertex_Model));
		}

	}VTXMODEL;

	typedef struct ENGINE_DLL tagtagVertex_Model_Declaration
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertex_AnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;

		tagVertex_AnimModel()
		{
			ZeroMemory(this, sizeof(tagVertex_AnimModel));
		}
	}VTXANIM;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXANIM_DECLARATION;

	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXINSTANCE;

	struct VTXCURVE
	{
		XMFLOAT2			vRatio;
	};

	typedef struct tagVertex_Color_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
		XMFLOAT4			vColor;
		XMFLOAT2			vSpriteTexUV;
	}VTXCOLORINSTANCE;

	typedef struct tagVertex_Model_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXMODELINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXPOINT_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Model_Instance_Declaration
	{
		static const unsigned int		iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXMODEL_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Texture_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Texture_Color_Instance_Declaration
	{
		static const unsigned int		iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEXCOLOR_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Texture_Curve_Instance_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEXCURVE_INSTANCE_DECLARATION;


	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int iWinCX, iWinCY;
		HWND	hWnd;
		WINMODE	isWindowMode;
	}GRAPHICDESC;

	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vAlignAxis[3];
		_float3		vCenterAxis[3];
	}OBBDESC;

	typedef struct tagColliderDesc
	{
		_uint			iLayer;
		_float3			vScale;
		_float4			vRotation;
		_float3			vTranslation;
		_float3			vOffset;

		tagColliderDesc() 
		{
			ZeroMemory(this, sizeof(tagColliderDesc));
		}

		tagColliderDesc(const _float3 In_vScale, const _float4 In_vRotation, const _float3 In_vTranslation, const _float3 In_vOffset)
			: vScale(In_vScale), vRotation(In_vRotation), vTranslation(In_vTranslation), vOffset(In_vOffset)
		{
		}

	}COLLIDERDESC;

	struct INSTANCE_MESH_DESC
	{
		_float3         vRotation;
		_float3         vScale;
		_float3			vTarnslation;
		_float			fMaxRange;
		_float3			vCenter;

		void Reset()
		{
			ZeroMemory(this, sizeof(INSTANCE_MESH_DESC));
		}

		_matrix Get_Matrix() const
		{
			_matrix TransformationMatrix;
			_matrix RotationMatrix, ScaleMatrix;
			
			_vector vPitchYawRoll;
			_vector vPosition;

			vPitchYawRoll = XMLoadFloat3(&vRotation);
			vPosition     = XMVectorSetW(XMLoadFloat3(&vTarnslation), 1.f);

			RotationMatrix = XMMatrixRotationRollPitchYawFromVector(vPitchYawRoll);
			ScaleMatrix    = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
			TransformationMatrix = ScaleMatrix * RotationMatrix;
			TransformationMatrix.r[3] = vPosition;

			return TransformationMatrix;
		}
		
		void	Bake_CenterWithMatrix()
		{
			_vector vCenterFromVector = XMLoadFloat3(&vCenter);
			XMStoreFloat3(&vCenter, XMVector3TransformCoord(vCenterFromVector, Get_Matrix()));
		}
	};

	struct PARTICLE_DESC
	{
		_bool			bEnable;
		// _bool			bBillboard;

		_float          fCurrentLifeTime;
		_float			fTargetLifeTime;

		_float			fCurrentSpawnTime;
		_float			fTargetSpawnTime;

		_float4x4		ParentMatrix;
		
		// For. Attraction
		_float3			vTargetLookAt;

		// For. Position
		_float3			vOffsetPosition;
		_float3			vCurrentTranslation;

		_float3			vCurrentSpeedForce;

		_float3			vTargetSpeed;		// is Spawn Position when using easing position
		_float3			vTargetSpeedForce;  // is Goal Position when using easing position

		_float			fGravityAcc;
		_float3			vCurrentGravity;

		// For. Rotation
		_float3         vCurrentRotation;
		_float3			vCurrentRotationForce;

		_float3			vTargetRotationSpeed;
		_float3			vTargetRotationForce;

		// For. Scale
		_float2         vCurrentScale;
		_float2         vCurrentScaleForce;

		_float2			vTargetScaleSpeed;
		_float2			vTargetScaleForce;

		// For. Ratio Scale
		_float			fTargetYScaleRatio;

		// For. Color
		_float4         vCurrentColor;
		_float4         vCurrentColorForce;

		_float4			vTargetColorSpeed;
		_float4			vTargetColorForce;

		// For. Easing Alpha
		_float			fStartAlpha;

		// For. UV
		_float2         vCurrentUV;
		_float2         vCurrentUVForce;

		// For. Sprites
		_float2 vSpriteUV;
		_float fCurrentSpriteTime;

		void Reset()
		{
			ZeroMemory(this, sizeof(PARTICLE_DESC));
		}
	};

	struct PARTICLE_SPAWN
	{
		_float3			vSpawnPosition;
		_float3			vSpawnLook;
		_float			fSpawnTime;
	};

	struct FaderDesc
	{
		LINEAR_TYPE eLinearType;
		FADER_TYPE	eFaderType;
		_float4 vFadeColor;
		_float	fFadeMaxTime;
		_float	fDelayTime;
	};

	typedef struct PhysXColliderDesc
	{
		PhysXColliderDesc()
		{
			ZeroMemory(this, sizeof(PhysXColliderDesc));
		}

		PxConvexMesh* pConvecMesh;
		PxMaterial* pMaterial;
		PHYSXACTOR_TYPE		eActorType;
		XMVECTOR			vPosition;
		XMVECTOR			vAngles;
		PHYSXCOLLIDER_TYPE	eShape;
		_uint				iFilterType;
		XMVECTOR			vScale;
		float				fDensity;
		_bool				bTrigger;

	}PHYSXCOLLIDERDESC;


	typedef struct tag_TrailBuffer
	{
		_float3		vPos_0;
		_float3		vPos_1;
		_uint		iMaxCnt;
	} TRAIL_DESC;

	struct LIFTGAMMAGAIN_DESC
	{
		_float4 vLift;
		_float4 vGamma;
		_float4 vGain;
	};
	
	struct DECAL_DESC
	{
		_float4 vPosition{};
		_float3 vScale{};
		_float4x4 WorldMatrix{};
		std::string strTextureTag;
		_float3 vColor{};

		_float  fTime{};
		_float  fAppearTime{};
		_float	fDisapearTime{};
	};
	
}
