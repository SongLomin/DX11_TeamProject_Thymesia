#include "SMath.h"
#include <random>
#include "GameInstance.h"
#include "MeshData.h"

XMMATRIX Engine::SMath::Get_RotationMatrix(FXMMATRIX Mat)
{
	XMMATRIX ResultMat = XMMatrixIdentity();

	for (int i(0); i < 3; i++)
		ResultMat.r[i] = XMVector3Normalize(Mat.r[i]);

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Get_ScaleMatrix(FXMMATRIX Mat)
{
	XMMATRIX ResultMat = XMMatrixIdentity();

	for (int i = 0; i < 3; i++)
	{
		ResultMat.r[i].m128_f32[i] = XMVector3Length(Mat.r[i]).m128_f32[0];
	}

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Get_PositionMatrix(FXMMATRIX Mat)
{
	XMMATRIX ResultMat = XMMatrixIdentity();

	ResultMat.r[3] = Mat.r[3];

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Get_MatrixNormalize(FXMMATRIX Mat)
{
	XMMATRIX ResultMat;

	ResultMat.r[0] = XMVector3Normalize(Mat.r[0]);
	ResultMat.r[1] = XMVector3Normalize(Mat.r[1]);
	ResultMat.r[2] = XMVector3Normalize(Mat.r[2]);
	ResultMat.r[3] = Mat.r[3];

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Bake_MatrixNormalizeUseLookVector(FXMVECTOR In_vLook)
{
	_matrix		ResultMat;
	_vector		vLook = XMVector3Normalize(In_vLook);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	ResultMat.r[0] = XMVector3Normalize(vRight);
	ResultMat.r[1] = XMVector3Normalize(vUp);
	ResultMat.r[2] = XMVector3Normalize(vLook);
	ResultMat.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Get_RotationQuaternion(FXMMATRIX Mat, FXMVECTOR In_vQauternion)
{
	_matrix Result = XMMatrixIdentity();

	_vector     vScale = SMath::Get_Scale(Mat);

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	_matrix     RotationMatrix = XMMatrixRotationQuaternion(In_vQauternion);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	vRight = XMVector3Normalize(vRight);
	vUp = XMVector3Normalize(vUp);
	vLook = XMVector3Normalize(vLook);

	Result.r[0] = vRight * vScale.m128_f32[0];
	Result.r[1] = vUp * vScale.m128_f32[1];
	Result.r[2] = vLook * vScale.m128_f32[2];
	Result.r[3] = Mat.r[3];

	return Result;
}

XMVECTOR ENGINE_DLL Engine::SMath::Get_Scale(FXMMATRIX Mat)
{
	XMVECTOR ResultVec;

	ResultVec.m128_f32[0] = XMVectorGetX(XMVector3Length(Mat.r[0]));
	ResultVec.m128_f32[1] = XMVectorGetX(XMVector3Length(Mat.r[1]));
	ResultVec.m128_f32[2] = XMVectorGetX(XMVector3Length(Mat.r[2]));

	return ResultVec;
}

XMVECTOR ENGINE_DLL Engine::SMath::Rotate_PositionPitchYawRoll(FXMVECTOR vPosition, FXMVECTOR vPitchYawRoll)
{
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(vPitchYawRoll.m128_f32[0], vPitchYawRoll.m128_f32[1], vPitchYawRoll.m128_f32[2]);

	return XMVector3TransformCoord(vPosition, RotationMatrix);
}

XMFLOAT3 ENGINE_DLL Engine::SMath::Extract_PitchYawRollFromRotationMatrix(FXMMATRIX Mat)
{
	XMFLOAT4X4 MatFrom4x4;

	XMStoreFloat4x4(&MatFrom4x4, Mat);

	float pitch = DirectX::XMScalarASin(-MatFrom4x4._32);

	DirectX::XMVECTOR from(DirectX::XMVectorSet(MatFrom4x4._12, MatFrom4x4._31, 0.0f, 0.0f));
	DirectX::XMVECTOR to(DirectX::XMVectorSet(MatFrom4x4._22, MatFrom4x4._33, 0.0f, 0.0f));
	DirectX::XMVECTOR res(DirectX::XMVectorATan2(from, to));

	float roll = DirectX::XMVectorGetX(res);
	float yaw = DirectX::XMVectorGetY(res);

	return XMFLOAT3(pitch, yaw, roll);
}

XMFLOAT3 ENGINE_DLL Engine::SMath::Add_Float3(const XMFLOAT3& Left, const XMFLOAT3& Right)
{
	XMFLOAT3 vResult;
	vResult.x = Left.x + Right.x;
	vResult.y = Left.y + Right.y;
	vResult.z = Left.z + Right.z;

	return vResult;
}

XMFLOAT3 ENGINE_DLL Engine::SMath::Mul_Float3(const XMFLOAT3& Left, const float& Right)
{
	XMFLOAT3 vResult;

	vResult.x = Left.x * Right;
	vResult.y = Left.y * Right;
	vResult.z = Left.z * Right;

	return vResult;
}

void ENGINE_DLL Engine::SMath::Add_Float3(XMFLOAT3* InOut_Left, const XMFLOAT3& Right)
{
	InOut_Left->x += Right.x;
	InOut_Left->y += Right.y;
	InOut_Left->z += Right.z;
}

void ENGINE_DLL Engine::SMath::Mul_Float3(XMFLOAT3* InOut_Left, const float& Right)
{
	InOut_Left->x *= Right;
	InOut_Left->y *= Right;
	InOut_Left->z *= Right;
}

_bool ENGINE_DLL Engine::SMath::Equal_Float3(const XMFLOAT3& Left, const XMFLOAT3& Right)
{
	_vector vLeft, vRight;

	vLeft = XMLoadFloat3(&Left);
	vRight = XMLoadFloat3(&Right);

	return XMVector3Equal(vLeft, vRight);
}

XMFLOAT4 ENGINE_DLL Engine::SMath::Add_Float4(const XMFLOAT4& Left, const XMFLOAT4& Right)
{
	XMFLOAT4 vResult;
	vResult.x = Left.x + Right.x;
	vResult.y = Left.y + Right.y;
	vResult.z = Left.z + Right.z;
	vResult.w = Left.w + Right.w;

	return vResult;
}

XMFLOAT4 ENGINE_DLL Engine::SMath::Mul_Float4(const XMFLOAT4& Left, const float& Right)
{
	XMFLOAT4 vResult;

	vResult.x = Left.x * Right;
	vResult.y = Left.y * Right;
	vResult.z = Left.z * Right;
	vResult.w = Left.w * Right;

	return vResult;
}

void ENGINE_DLL Engine::SMath::Add_Float4(XMFLOAT4* InOut_Left, const XMFLOAT4& Right)
{
	InOut_Left->x += Right.x;
	InOut_Left->y += Right.y;
	InOut_Left->z += Right.z;
	InOut_Left->w += Right.w;
}

void ENGINE_DLL Engine::SMath::Mul_Float4(XMFLOAT4* InOut_Left, const float& Right)
{
	InOut_Left->x *= Right;
	InOut_Left->y *= Right;
	InOut_Left->z *= Right;
	InOut_Left->w *= Right;
}

float ENGINE_DLL Engine::SMath::Lerp(const float& fLeft, const float& fRight, float fRatio)
{
	fRatio = max(min(fRatio, 1.f), 0.f);

	return (fLeft * (1.f - fRatio)) + (fRight * (fRatio));
}

int ENGINE_DLL Engine::SMath::Random(const int& _iMin, const int& _iMax)
{
	if (_iMin >= _iMax)
		return _iMin;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> disx(_iMin, _iMax);

	return disx(gen);
}

float ENGINE_DLL Engine::SMath::fRandom(const float& _fMin, const float& _fMax)
{
	if (_fMin >= _fMax)
		return _fMin;

	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> disx(_fMin, _fMax);

	return disx(gen);
}

XMFLOAT3 ENGINE_DLL Engine::SMath::vRandom(const XMFLOAT3& _vMin, const XMFLOAT3& _vMax)
{

	XMFLOAT3 Result;

	Result.x = fRandom(_vMin.x, _vMax.x);
	Result.y = fRandom(_vMin.y, _vMax.y);
	Result.z = fRandom(_vMin.z, _vMax.z);

	if (Result.x == 0.f)
	{
		Result.x = DBL_EPSILON;
	}

	if (Result.y == 0.f)
	{
		Result.y = DBL_EPSILON;
	}

	if (Result.z == 0.f)
	{
		Result.z = DBL_EPSILON;
	}

	return Result;
}

XMFLOAT4 ENGINE_DLL Engine::SMath::vRandom(const XMFLOAT4& _vMin, const XMFLOAT4& _vMax)
{
	XMFLOAT4 Result;

	Result.x = fRandom(_vMin.x, _vMax.x);
	Result.y = fRandom(_vMin.y, _vMax.y);
	Result.z = fRandom(_vMin.z, _vMax.z);
	Result.w = fRandom(_vMin.w, _vMax.w);

	if (Result.x == 0.f)
	{
		Result.x = DBL_EPSILON;
	}

	if (Result.y == 0.f)
	{
		Result.y = DBL_EPSILON;
	}

	if (Result.z == 0.f)
	{
		Result.z = DBL_EPSILON;
	}

	if (Result.w == 0.f)
	{
		Result.w = DBL_EPSILON;
	}

	return Result;
}

XMVECTOR ENGINE_DLL Engine::SMath::vRandom(const XMVECTOR& _vMin, const XMVECTOR& _vMax)
{
	XMVECTOR Result;

	for (_uint i = 0; i < 4; i++)
	{
		Result.m128_f32[i] = fRandom(_vMin.m128_f32[i], _vMax.m128_f32[i]);

		if (Result.m128_f32[i] == 0.f)
		{
			Result.m128_f32[i] = DBL_EPSILON;
		}
	}

	return Result;
}

void ENGINE_DLL Engine::SMath::Get_MouseRayInWorldSpace(RAY& Out_Ray, const _uint& In_ViewPortWidth, const _uint& In_ViewPortHeight)
{
	POINT  ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(GAMEINSTANCE->Get_WindowHandle(), &ptMouse);

	/* 2. ���� �����̽� ���� ���콺 ��ǥ ���ϱ� */
	_vector  vProjPos;

	vProjPos.m128_f32[0] = ptMouse.x / (In_ViewPortWidth * 0.5f) - 1.f;
	vProjPos.m128_f32[1] = ptMouse.y / -(In_ViewPortHeight * 0.5f) + 1.f;
	vProjPos.m128_f32[2] = 0.0f;
	vProjPos.m128_f32[3] = 1.0f;

	/* 3.�佺���̽����� ���콺 ��ǥ�� ������. */
	_vector		vViewPos;

	_matrix		ProjMatrixInv;
	ProjMatrixInv = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);

	vViewPos = XMVector3TransformCoord(vProjPos, ProjMatrixInv);

	/* 4.���콺���̿� ���콺Pos��������.  */
	_vector		vRayDir, vRayPos;

	vRayDir = vViewPos;
	vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	/* 5.����ΰ���. */
	_matrix	ViewMatrixInv;
	ViewMatrixInv = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	XMStoreFloat3(&Out_Ray.vDirection, XMVector3TransformNormal(vRayDir, ViewMatrixInv));
	XMStoreFloat4(&Out_Ray.vOrigin, XMVector3TransformCoord(vRayPos, ViewMatrixInv));

	Out_Ray.fLength = 1000000.0f; //����
}

RAY ENGINE_DLL Engine::SMath::Get_MouseRayInWorldSpace(const _uint& In_ViewPortWidth, const _uint& In_ViewPortHeight)
{
	POINT  ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(GAMEINSTANCE->Get_WindowHandle(), &ptMouse);

	/* 2. ���� �����̽� ���� ���콺 ��ǥ ���ϱ� */
	_vector  vProjPos;

	vProjPos.m128_f32[0] = ptMouse.x / (In_ViewPortWidth * 0.5f) - 1.f;
	vProjPos.m128_f32[1] = ptMouse.y / -(In_ViewPortHeight * 0.5f) + 1.f;
	vProjPos.m128_f32[2] = 0.0f;
	vProjPos.m128_f32[3] = 0.0f;

	/* 3.�佺���̽����� ���콺 ��ǥ�� ������. */
	_vector		vViewPos;

	_matrix		ProjMatrixInv;
	ProjMatrixInv = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);

	vViewPos = XMVector3TransformCoord(vProjPos, ProjMatrixInv);

	/* 4.���콺���̿� ���콺Pos��������.  */
	_vector		vRayDir, vRayPos;

	vRayDir = vViewPos;
	vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	/* 5.����ΰ���. */
	_matrix	ViewMatrixInv;
	ViewMatrixInv = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);

	RAY MouseRay;

	XMStoreFloat3(&MouseRay.vDirection, XMVector3Normalize(XMVector3TransformNormal(vRayDir, ViewMatrixInv)));
	XMStoreFloat4(&MouseRay.vOrigin, XMVector3TransformCoord(vRayPos, ViewMatrixInv));

	MouseRay.fLength = 1000000.0f; //����

	return MouseRay;
}


XMMATRIX ENGINE_DLL Engine::SMath::Set_ScaleMatrix(FXMMATRIX Mat, FXMVECTOR Scale)
{
	XMMATRIX ResultMat;

	ResultMat.r[0] = XMVector3Normalize(Mat.r[0]) * Scale.m128_f32[0];
	ResultMat.r[1] = XMVector3Normalize(Mat.r[1]) * Scale.m128_f32[1];
	ResultMat.r[2] = XMVector3Normalize(Mat.r[2]) * Scale.m128_f32[2];
	ResultMat.r[3] = Mat.r[3];

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Go_Right(FXMMATRIX Mat, float fScaler)
{
	XMMATRIX ResultMat = Mat;

	_vector		vPosition = ResultMat.r[3];
	_vector		vRight = ResultMat.r[0];

	vPosition += XMVector3Normalize(vRight) * fScaler;

	ResultMat.r[3] = vPosition;

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Go_Straight(FXMMATRIX Mat, float fScaler)
{
	XMMATRIX ResultMat = Mat;

	_vector		vPosition = ResultMat.r[3];
	_vector		vLook = ResultMat.r[2];

	vPosition += XMVector3Normalize(vLook) * fScaler;

	ResultMat.r[3] = vPosition;

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Go_Up(FXMMATRIX Mat, float fScaler)
{
	XMMATRIX ResultMat = Mat;

	_vector		vPosition = ResultMat.r[3];
	_vector		vUp = ResultMat.r[1];

	vPosition += XMVector3Normalize(vUp) * fScaler;

	ResultMat.r[3] = vPosition;

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Add_PositionWithRotation(FXMMATRIX Mat, FXMVECTOR vPosition)
{
	_vector vRotatedPosition = XMVector3TransformCoord(vPosition, SMath::Get_RotationMatrix(Mat));
	_matrix ResultMatrix = Mat;

	ResultMatrix.r[3] += vRotatedPosition;

	return ResultMatrix;
}

XMMATRIX ENGINE_DLL Engine::SMath::LookAt(FXMMATRIX Mat, FXMVECTOR In_vPosition)
{
	XMMATRIX ResultMat;
	XMVECTOR vScale = SMath::Get_Scale(Mat);


	_vector		vPosition = Mat.r[3];

	_vector		vLook = XMVector3Normalize(In_vPosition - vPosition);

	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	ResultMat.r[0] = XMVector3Normalize(vRight) * vScale.m128_f32[0];
	ResultMat.r[1] = XMVector3Normalize(vUp) * vScale.m128_f32[1];
	ResultMat.r[2] = XMVector3Normalize(vLook) * vScale.m128_f32[2];
	ResultMat.r[3] = vPosition;

	return ResultMat;
}

XMMATRIX ENGINE_DLL Engine::SMath::Bake_WorldMatrix(const XMFLOAT3& In_vScale, const XMFLOAT3& In_vRot, const XMFLOAT3& In_vPos)
{
	_matrix TransformationMatrix;
	_matrix RotationMatrix, ScaleMatrix;

	_vector vPitchYawRoll;
	_vector vPosition;

	vPitchYawRoll = XMLoadFloat3(&In_vRot);
	vPosition = XMLoadFloat3(&In_vPos);
	vPosition.m128_f32[3] = 1.f;

	RotationMatrix = XMMatrixRotationRollPitchYawFromVector(vPitchYawRoll);
	ScaleMatrix = XMMatrixScaling(In_vScale.x, In_vScale.y, In_vScale.z);
	TransformationMatrix = ScaleMatrix * RotationMatrix;
	TransformationMatrix.r[3] = vPosition;

	return TransformationMatrix;
}

bool ENGINE_DLL Engine::SMath::Is_SphereToRayCollision(const XMFLOAT3& Center, const float& Radius, FXMVECTOR Origin, FXMVECTOR Direction, float& Dist)
{
	XMVECTOR vCenter = XMLoadFloat3(&Center);
	XMVECTOR vRadius = XMVectorReplicatePtr(&Radius);

	// l is the vector from the ray origin to the center of the sphere.
	XMVECTOR l = XMVectorSubtract(vCenter, Origin);

	// s is the projection of the l onto the ray direction.
	XMVECTOR s = XMVector3Dot(l, Direction);

	XMVECTOR l2 = XMVector3Dot(l, l);

	XMVECTOR r2 = XMVectorMultiply(vRadius, vRadius);

	// m2 is squared distance from the center of the sphere to the projection.
	XMVECTOR m2 = XMVectorNegativeMultiplySubtract(s, s, l2);

	XMVECTOR NoIntersection;

	// If the ray origin is outside the sphere and the center of the sphere is
	// behind the ray origin there is no intersection.
	NoIntersection = XMVectorAndInt(XMVectorLess(s, XMVectorZero()), XMVectorGreater(l2, r2));

	// If the squared distance from the center of the sphere to the projection
	// is greater than the radius squared the ray will miss the sphere.
	NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(m2, r2));

	// The ray hits the sphere, compute the nearest intersection point.
	XMVECTOR q = XMVectorSqrt(XMVectorSubtract(r2, m2));
	XMVECTOR t1 = XMVectorSubtract(s, q);
	XMVECTOR t2 = XMVectorAdd(s, q);

	XMVECTOR OriginInside = XMVectorLessOrEqual(l2, r2);
	XMVECTOR t = XMVectorSelect(t1, t2, OriginInside);

	if (XMVector4NotEqualInt(NoIntersection, XMVectorTrueInt()))
	{
		// Store the x-component to *pDist.
		XMStoreFloat(&Dist, t);
		return true;
	}

	Dist = 0.f;
	return false;
}

void ENGINE_DLL Engine::SMath::Set_ClockwiseTriangle(XMFLOAT3* InOut_TrianglePosition)
{
	_vector vPositionFromVector[3];

	for (int i = 0; i < 3; i++)
	{
		vPositionFromVector[i] = XMLoadFloat3(&(InOut_TrianglePosition[i]));
	}


	_vector vAtoB, vAtoC;

	vAtoB = vPositionFromVector[1] - vPositionFromVector[0];
	vAtoC = vPositionFromVector[2] - vPositionFromVector[0];

	_vector vAtoB2D, vAtoC2D, vAtoB2DCross;
	vAtoB2D = vAtoC2D = vAtoB2DCross = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	vAtoB2D.m128_f32[0] = vAtoB.m128_f32[0];
	vAtoB2D.m128_f32[1] = vAtoB.m128_f32[2];

	vAtoC2D.m128_f32[0] = vAtoC.m128_f32[0];
	vAtoC2D.m128_f32[1] = vAtoC.m128_f32[2];

	vAtoB2DCross.m128_f32[0] = -vAtoB2D.m128_f32[1];
	vAtoB2DCross.m128_f32[1] = vAtoB2D.m128_f32[0];

	_float fDot = XMVector2Dot(vAtoB2DCross, vAtoC2D).m128_f32[0];

	// ������ ����� ������ �ݽð� �����̹Ƿ� �ٽ� �ð�������� �����ش�.
	if (fDot > 0.f)
	{
		XMStoreFloat3(&InOut_TrianglePosition[1], vPositionFromVector[2]);
		XMStoreFloat3(&InOut_TrianglePosition[2], vPositionFromVector[1]);
	}


}

_bool ENGINE_DLL Engine::SMath::Is_Picked_AbstractTerrain(const RAY& _Ray, _float4* _pOutPos)
{
	if (0 != isnan(_Ray.vOrigin.x))
		return false;

	_float3 vPos[4] =
	{
		_float3(-99999.f, 0.f,  99999.f),
		_float3(99999.f, 0.f,  99999.f),
		_float3(99999.f, 0.f, -99999.f),
		_float3(-99999.f, 0.f, -99999.f)
	};

	_uint3 iIndex[2] =
	{
		_uint3(0, 1, 2),
		_uint3(0, 2, 3)
	};

	for (_uint i = 0; i < 2; ++i)
	{
		if (0 != isnan(_Ray.vOrigin.x))
			break;

		_vector		vPickedPos;

		_vector	vVec0 = XMLoadFloat3(&vPos[iIndex[i].ix]);
		_vector	vVec1 = XMLoadFloat3(&vPos[iIndex[i].iy]);
		_vector	vVec2 = XMLoadFloat3(&vPos[iIndex[i].iz]);

		_float fDist = 0;
		if (DirectX::TriangleTests::Intersects(XMLoadFloat4(&_Ray.vOrigin), XMLoadFloat3(&_Ray.vDirection), vVec0, vVec1, vVec2, fDist))
		{
			vPickedPos = XMLoadFloat4(&_Ray.vOrigin) + XMVector3Normalize(XMLoadFloat3(&_Ray.vDirection)) * fDist;
			XMStoreFloat4(_pOutPos, vPickedPos);

			return true;
		}
	}

	return false;
}

_bool ENGINE_DLL Engine::SMath::Is_Picked_AbstractCube(const RAY& _Ray, MESH_VTX_INFO _VtxInfo, _matrix _WorldMatrix, _float* Out_pDist)
{
	if (0 != isnan(_Ray.vOrigin.x))
		return false;

	_matrix		WorldMatrixInv = XMMatrixInverse(nullptr, _WorldMatrix);

	_vector vRayPos = XMVector3TransformCoord(XMLoadFloat4(&_Ray.vOrigin), WorldMatrixInv);
	_vector vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&_Ray.vDirection), WorldMatrixInv));

	_float3				vPosition[8];
	_uint3				vIndex[12];

	vPosition[0] = { _VtxInfo.vMin.x, _VtxInfo.vMax.y, _VtxInfo.vMin.z };
	vPosition[1] = { _VtxInfo.vMax.x, _VtxInfo.vMax.y, _VtxInfo.vMin.z };
	vPosition[2] = { _VtxInfo.vMax.x, _VtxInfo.vMin.y, _VtxInfo.vMin.z };
	vPosition[3] = { _VtxInfo.vMin.x, _VtxInfo.vMin.y, _VtxInfo.vMin.z };
	vPosition[4] = { _VtxInfo.vMin.x, _VtxInfo.vMax.y, _VtxInfo.vMax.z };
	vPosition[5] = { _VtxInfo.vMax.x, _VtxInfo.vMax.y, _VtxInfo.vMax.z };
	vPosition[6] = { _VtxInfo.vMax.x, _VtxInfo.vMin.y, _VtxInfo.vMax.z };
	vPosition[7] = { _VtxInfo.vMin.x, _VtxInfo.vMin.y, _VtxInfo.vMax.z };

	vIndex[0] = { 1, 5, 6 };
	vIndex[1] = { 1, 6, 2 };
	vIndex[2] = { 4, 0, 3 };
	vIndex[3] = { 4, 3, 7 };
	vIndex[4] = { 4, 5, 1 };
	vIndex[5] = { 4, 1, 0 };
	vIndex[6] = { 3, 2, 6 };
	vIndex[7] = { 3, 6, 7 };
	vIndex[8] = { 5, 4, 7 };
	vIndex[9] = { 5, 7, 6 };
	vIndex[10] = { 0, 1, 2 };
	vIndex[11] = { 0, 2, 3 };

	_float		fDist;

	for (_uint i = 0; i < 12; ++i)
	{
		_vector	vVec0 = XMLoadFloat3(&vPosition[vIndex[i].ix]);
		_vector	vVec1 = XMLoadFloat3(&vPosition[vIndex[i].iy]);
		_vector	vVec2 = XMLoadFloat3(&vPosition[vIndex[i].iz]);

		if (true == DirectX::TriangleTests::Intersects(vRayPos, vRayDir, vVec0, vVec1, vVec2, fDist))
		{
			if (Out_pDist)
				*Out_pDist = fDist;
			return true;
		}
	}

	return false;
}


void ENGINE_DLL Engine::SMath::Convert_PxVec3FromMeshData(PxVec3* In_PxVec3, weak_ptr<MESH_DATA> pMeshData)
{
	_uint iNumVertices = pMeshData.lock()->iNumVertices;

	MODEL_TYPE eActorType = pMeshData.lock()->eModelType;

	for (_uint i = 0; i < iNumVertices; ++i)
	{
		switch (eActorType)
		{
		case MODEL_TYPE::NONANIM:
			memcpy(&In_PxVec3[i], &pMeshData.lock()->pVertices[i].vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::ANIM:
			memcpy(&In_PxVec3[i], &pMeshData.lock()->pAnimVertices[i].vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::NAVI:
			memcpy(&In_PxVec3[i], &pMeshData.lock()->pPosVertices[i].vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::GROUND:
			memcpy(&In_PxVec3[i], &pMeshData.lock()->pGroundVertices[i].vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::TYPE_END:
			DEBUG_ASSERT;
			break;
		default:
			DEBUG_ASSERT;
			break;
		}
	}

}

void ENGINE_DLL Engine::SMath::Convert_PxVec3FromMeshDataWithTransformMatrix(PxVec3* In_PxVec3, weak_ptr<MESH_DATA> pMeshData, FXMMATRIX In_TransformMatrix)
{
	_uint iNumVertices = pMeshData.lock()->iNumVertices;

	MODEL_TYPE eActorType = pMeshData.lock()->eModelType;

	_vector vPosition;

	for (_uint i = 0; i < iNumVertices; ++i)
	{
		switch (eActorType)
		{
		case MODEL_TYPE::NONANIM:
			vPosition = XMVector3TransformCoord(XMLoadFloat3(&pMeshData.lock()->pVertices[i].vPosition), In_TransformMatrix);
			memcpy(&In_PxVec3[i], &vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::ANIM:
			vPosition = XMVector3TransformCoord(XMLoadFloat3(&pMeshData.lock()->pAnimVertices[i].vPosition), In_TransformMatrix);
			memcpy(&In_PxVec3[i], &vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::NAVI:
			vPosition = XMVector3TransformCoord(XMLoadFloat3(&pMeshData.lock()->pPosVertices[i].vPosition), In_TransformMatrix);
			memcpy(&In_PxVec3[i], &vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::GROUND:
			vPosition = XMVector3TransformCoord(XMLoadFloat3(&pMeshData.lock()->pGroundVertices[i].vPosition), In_TransformMatrix);
			memcpy(&In_PxVec3[i], &vPosition, sizeof(PxVec3));
			break;
		case MODEL_TYPE::TYPE_END:
			DEBUG_ASSERT;
			break;
		default:
			DEBUG_ASSERT;
			break;
		}
	}
}

PxExtendedVec3 ENGINE_DLL Engine::SMath::Convert_PxExtendedVec3(FXMVECTOR In_Vector)
{
	return PxExtendedVec3(In_Vector.m128_f32[0], In_Vector.m128_f32[1], In_Vector.m128_f32[2]);
}

PxVec3 ENGINE_DLL Engine::SMath::Convert_PxVec3(FXMVECTOR In_Vector)
{
	return PxVec3(In_Vector.m128_f32[0], In_Vector.m128_f32[1], In_Vector.m128_f32[2]);
}
