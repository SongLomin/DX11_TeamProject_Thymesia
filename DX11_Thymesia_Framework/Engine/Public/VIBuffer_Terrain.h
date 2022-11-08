#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Terrain)
	CLONE_H(CVIBuffer_Terrain, CComponent)

public:
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;

public:
	void Free();
};

END