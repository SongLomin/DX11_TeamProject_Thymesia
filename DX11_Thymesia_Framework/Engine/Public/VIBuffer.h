#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
	GAMECLASS_H(CVIBuffer)

public:
	CVIBuffer(const CVIBuffer& rhs);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

protected:
	D3D11_BUFFER_DESC				m_BufferDesc;
	D3D11_SUBRESOURCE_DATA			m_SubResourceData;

protected:
	ComPtr<ID3D11Buffer>			m_pVB;
	ComPtr<ID3D11Buffer>			m_pIB;
	vector<_float3>					m_VertexPositions;
	_uint							m_iStride = 0;
	_uint							m_iNumVertices = 0;
	_uint							m_iNumPrimitive = 0;
	_uint							m_iIndicesStride = 0;
	_uint							m_iNumIndices = 0;
	_uint							m_iNumVertexBuffers = 0;
	DXGI_FORMAT						m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY		m_eToplogy;


protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();



public:
	void Free();
};

END