#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Trail)
		CLONE_H(CVIBuffer_Trail, CComponent)

public:
	CVIBuffer_Trail(const CVIBuffer_Trail& rhs);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

private:
	virtual void Free() override;
};

END