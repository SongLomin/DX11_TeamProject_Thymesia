#include "PipeLine.h"

IMPLEMENT_SINGLETON(CPipeLine)

void CPipeLine::Tick()
{
	_matrix		ViewMatrixInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW]));
	XMStoreFloat4(&m_vCamPosition, ViewMatrixInv.r[3]);


	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_VIEW], XMMatrixTranspose(XMLoadFloat4x4(&m_TransformState[D3DTS_VIEW])));
	XMStoreFloat4x4(&m_TransformState_TP[D3DTS_PROJ], XMMatrixTranspose(XMLoadFloat4x4(&m_TransformState[D3DTS_PROJ])));
}

void CPipeLine::Free()
{
}
