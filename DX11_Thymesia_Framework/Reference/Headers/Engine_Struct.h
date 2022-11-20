#pragma once

namespace Engine
{
	class CTexture;

	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	typedef struct tagKeyData
	{
		XMFLOAT3	vValue;
		float		fTime;

	} KEY_DATA;

	typedef struct tagRotationKeyData
	{
		XMFLOAT4	vValue;
		float		fTime;

	} ROTATIONKEY_DATA;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		float			fTime;
	} KEYFRAME;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_HALFPOINT, TYPE_END };


		bool			bEnable;
		TYPE			eActorType = TYPE_END;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		XMFLOAT4		vLightFlag;

	private:
		_uint			iLightIndex;

	public:
		const _uint Get_LightIndex() const { return iLightIndex; }

		friend class CCustomLight;

	}LIGHTDESC;

	typedef struct tag_MeshVertextPostion
	{
		_float3         vMin;
		_float3         vMax;
		_float3         vCenter;

	} MESH_VTX_INFO;

	typedef struct tagTextInfo
	{
		tstring		szText;
		XMFLOAT2	vPosition;
		XMFLOAT2	vScale;
		float		fRotation;
		XMFLOAT4	vColor;
		bool		bAlways;
		bool		bCenterAlign;
	} TEXTINFO;

	typedef struct tagRayDesc
	{
		XMFLOAT4	vOrigin;
		XMFLOAT3	vDirection;
		float		fLength;
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
		XMFLOAT3		vPosition;
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

		tagColliderDesc() {}

		tagColliderDesc(const _float3& In_vScale, const _float4& In_vRotation, const _float3& In_vTranslation)
			: vScale(In_vScale), vRotation(In_vRotation), vTranslation(In_vTranslation)
		{
		}

	}COLLIDERDESC;

	struct INSTANCE_MESH_DESC
	{
		_float3         vRotation;
		_float3         vScale;
		_float3			vTarnslation;

		void Reset()
		{
			ZeroMemory(this, sizeof(INSTANCE_MESH_DESC));
		}
	};

	struct PARTICLE_DESC
	{
		_bool			bEnable;
		_bool			bBillboard;

		_float          fCurrentLifeTime;
		_float			fTargetLifeTime;

		_float			fCurrentSpawnTime;
		_float			fTargetSpawnTime;

		// For. Position
		_float3			vOffsetPosition;
		_float3			vCurrentTranslation;

		_float3			vCurrentSpeedForce;

		_float3			vTargetSpeed;		// is Spawn Position when using easing position
		_float3			vTargetSpeedForce;  // is Goal Position when using easing position

		// For. Rotation
		_float3         vCurrentRotation;
		_float3			vCurrentRotationForce;

		// For. Scale
		_float3         vCurrentScale;
		_float3         vCurrentScaleForce;

		// For. Color
		_float4         vCurrentColor;
		_float4         vCurrentColorForce;

		// For. UV
		_float2         vCurrentUV;
		_float2         vCurrentUVForce;

		// For. Sprites
		_float2 vSpriteUV;
		_float fCurrentSpriteTime;
		// For Sprites - Pendulum Effect
		_bool bFramePlayBackward; // default is false : Frame plays forward

		void Reset()
		{
			ZeroMemory(this, sizeof(PARTICLE_DESC));
		}
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
}
