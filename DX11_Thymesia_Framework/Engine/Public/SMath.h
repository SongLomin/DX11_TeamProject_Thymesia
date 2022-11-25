#pragma once

#include "Engine_Defines.h"

#define M_PI		3.14159265358979323846
#define M_PI_FLOAT	3.141592f

namespace Engine
{
	struct MESH_DATA;

	namespace SMath
	{
		XMMATRIX		ENGINE_DLL Get_RotationMatrix(FXMMATRIX Mat);
		XMMATRIX		ENGINE_DLL Get_ScaleMatrix(FXMMATRIX Mat);
		XMMATRIX		ENGINE_DLL Get_PositionMatrix(FXMMATRIX Mat);
		XMMATRIX		ENGINE_DLL Get_MatrixNormalize(FXMMATRIX Mat);
		XMMATRIX		ENGINE_DLL Bake_MatrixNormalizeUseLookVector(FXMVECTOR In_vLook);
		XMMATRIX		ENGINE_DLL Get_RotationQuaternion(FXMMATRIX Mat, FXMVECTOR In_vQauternion);

		XMMATRIX		ENGINE_DLL Set_ScaleMatrix(FXMMATRIX Mat, FXMVECTOR Scale);
		XMMATRIX		ENGINE_DLL Go_Right(FXMMATRIX Mat, float fScaler);
		XMMATRIX		ENGINE_DLL Go_Straight(FXMMATRIX Mat, float fScaler);
		XMMATRIX		ENGINE_DLL Go_Up(FXMMATRIX Mat, float fScaler);
		XMMATRIX		ENGINE_DLL Add_PositionWithRotation(FXMMATRIX Mat, FXMVECTOR vPosition);
		XMMATRIX		ENGINE_DLL LookAt(FXMMATRIX Mat, FXMVECTOR In_vPosition);
		

		XMVECTOR		ENGINE_DLL Get_Scale(FXMMATRIX Mat);
		XMVECTOR		ENGINE_DLL Rotate_PositionPitchYawRoll(FXMVECTOR vPosition, FXMVECTOR vPitchYawRoll);

		XMFLOAT3		ENGINE_DLL Extract_PitchYawRollFromRotationMatrix(FXMMATRIX Mat);

		XMFLOAT3		ENGINE_DLL Add_Float3(const XMFLOAT3& Left, const XMFLOAT3& Right);
		XMFLOAT3		ENGINE_DLL Mul_Float3(const XMFLOAT3& Left, const float& Right);
		void			ENGINE_DLL Add_Float3(XMFLOAT3* InOut_Left, const XMFLOAT3& Right);
		void			ENGINE_DLL Mul_Float3(XMFLOAT3* InOut_Left, const float& Right);
		_bool			ENGINE_DLL Equal_Float3(const XMFLOAT3& Left, const XMFLOAT3& Right);

		XMFLOAT4		ENGINE_DLL Add_Float4(const XMFLOAT4& Left, const XMFLOAT4& Right);
		XMFLOAT4		ENGINE_DLL Mul_Float4(const XMFLOAT4& Left, const float& Right);
		void			ENGINE_DLL Add_Float4(XMFLOAT4* InOut_Left, const XMFLOAT4& Right);
		void			ENGINE_DLL Mul_Float4(XMFLOAT4* InOut_Left, const float& Right);

		float			ENGINE_DLL Lerp(const float& fLeft, const float& fRight, float fRatio);

		int				ENGINE_DLL Random(const int& _iMin, const int& _iMax);
		float			ENGINE_DLL fRandom(const float& _fMin, const float& _fMax);
		XMFLOAT3		ENGINE_DLL vRandom(const XMFLOAT3& _vMin, const XMFLOAT3& _vMax);
		XMFLOAT4		ENGINE_DLL vRandom(const XMFLOAT4& _vMin, const XMFLOAT4& _vMax);
		XMVECTOR		ENGINE_DLL vRandom(const XMVECTOR& _vMin, const XMVECTOR& _vMax);
		
		void			ENGINE_DLL Get_MouseRayInWorldSpace(RAY& Out_Ray, const _uint& In_ViewPortWidth, const _uint& In_ViewPortHeight);
		RAY				ENGINE_DLL Get_MouseRayInWorldSpace(const _uint& In_ViewPortWidth, const _uint& In_ViewPortHeight);

		bool			ENGINE_DLL Is_SphereToRayCollision(const XMFLOAT3& Center, const float& Radius, FXMVECTOR Origin, FXMVECTOR Direction, float& Dist);
		
		void			ENGINE_DLL Set_ClockwiseTriangle(XMFLOAT3* InOut_TrianglePosition);

		void			ENGINE_DLL Convert_PxVec3FromMeshData(PxVec3* In_PxVec3, weak_ptr<MESH_DATA> pMeshData);
		void			ENGINE_DLL Convert_PxVec3FromMeshDataWithTransformMatrix(PxVec3* In_PxVec3, weak_ptr<MESH_DATA> pMeshData, FXMMATRIX In_TransformMatrix);
		PxExtendedVec3	ENGINE_DLL Convert_PxExtendedVec3(FXMVECTOR In_Vector);
		PxVec3			ENGINE_DLL Convert_PxVec3(FXMVECTOR In_Vector);

		_bool			ENGINE_DLL Is_Picked_AbstractTerrain(const RAY& _Ray, _float4* _pOutPos);
		_bool			ENGINE_DLL Is_Picked_AbstractCube(const RAY& _Ray, MESH_VTX_INFO _VtxInfo, _matrix _WorldMatrix, _float* Out_fDist = nullptr);
	}
}