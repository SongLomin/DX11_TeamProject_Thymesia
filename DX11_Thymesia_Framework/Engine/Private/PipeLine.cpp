#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix TransformState)
{
	m_bBinded = true;

	if (D3DTS_VIEW == eState)
	{
		m_PreViewMatrix = m_TransformState[D3DTS_VIEW];
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


_float4x4 CPipeLine::Get_PreViewMatrix()
{
	_float4x4 PreViewMat = m_PreViewMatrix;
	//m_PreViewMatrix = m_TransformState[D3DTS_VIEW];

	return PreViewMat;
}

void CPipeLine::Free()
{
}
