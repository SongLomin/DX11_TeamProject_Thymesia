#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformState)
{
	if (D3DTS_VIEW == eState)
	{
		m_PreViewMatrix = m_TransformState[D3DTS_VIEW];
		_vector PreCamQuaternion = XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_TransformState[eState]));
	}
	XMStoreFloat4x4(&m_TransformState[eState], TransformState);
}

void CPipeLine::Tick()
{
	_matrix		ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW]));
	XMStoreFloat4(&m_vCamPosition, ViewMatrixInv.r[3]);


	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_VIEW], XMMatrixTranspose(XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW])));
	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_PROJ], XMMatrixTranspose(XMLoadFloat4x4(&m_TransformState[D3DTS_PROJ])));
}

_vector CPipeLine::Get_LinearVelocity()
{
	_vector vLinearVelocity = XMLoadFloat4(&m_vCamPosition) - XMLoadFloat4(&m_vPreCamPosition);
	m_vPreCamPosition = m_vCamPosition;
	//아예 이전 카메라 속도를 저장해줌
	return vLinearVelocity;
}

_float4x4 CPipeLine::Get_PreViewMatrix()
{
	_float4x4 PreViewMat = m_PreViewMatrix;
	//m_PreViewMatrix = m_TransformState[D3DTS_VIEW];

	return PreViewMat;
}

_vector CPipeLine::Get_AngularVelocity()
{
	_vector vCurCamQuaternion = XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW]));
	_vector vPreCamQuaternion = XMLoadFloat4(&m_vPreCamQuaternion);
	_vector vCamRotDiff = vCurCamQuaternion - vPreCamQuaternion;
	if (XMVector4Dot(vCurCamQuaternion, vPreCamQuaternion).m128_f32[0] < 0.0)
		vCamRotDiff *= -1.f;

	_vector vCamQuatConj = XMQuaternionConjugate(vCurCamQuaternion);
	_vector vAngularVelocity = (vCamRotDiff * 2.0) * vCamQuatConj;
	vAngularVelocity.m128_f32[3] = 0.f;
	//이전 프레임에서의 카메라 쿼터니언을 저장한다.
	XMStoreFloat4(&m_vPreCamQuaternion, vCurCamQuaternion);

	return vAngularVelocity;
}

void CPipeLine::Free()
{
}
