#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CTransform;
class CBoneNode;

class ENGINE_DLL CVIBuffer_Curve : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Curve)
	CLONE_H(CVIBuffer_Curve, CComponent)

public:
	CVIBuffer_Curve(const CVIBuffer_Curve& rhs);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
	//void Update(_fvector In_vLeft, _fvector In_vLeftDir, _fvector In_vRight, _fvector in_vRightDir);


protected:
	ComPtr<ID3D11Buffer>        m_pVBInstance;
	_uint						m_iInstanceStride = 0;
	_uint						m_iNumInstance = 0;

private:
	void Free();
};

END