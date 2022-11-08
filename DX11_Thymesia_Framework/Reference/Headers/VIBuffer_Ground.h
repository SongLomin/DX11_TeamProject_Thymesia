#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Ground final : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Ground)
	CLONE_H(CVIBuffer_Ground, CComponent)

public:
	CVIBuffer_Ground(const CVIBuffer_Ground& rhs);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;

private:
	void Free();
};

END