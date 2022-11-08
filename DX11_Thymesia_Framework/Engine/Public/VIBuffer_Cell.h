#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cell : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Cell)
		CLONE_H(CVIBuffer_Cell, CComponent)

public:
	CVIBuffer_Cell(const CVIBuffer_Cell& rhs);

public:
	_float3 Get_CellPosition(const _uint& In_iIndex) const {
		return m_vVerticePositions[In_iIndex];
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;
	void Init_VIBufferCell(const _float3* pVerticesPos);

private:
	_float3	m_vVerticePositions[3];

public:
	void Free();
};

END