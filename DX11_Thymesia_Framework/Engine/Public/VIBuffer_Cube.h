#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Cube)
	CLONE_H(CVIBuffer_Cube, CComponent)
	SHALLOW_COPY(CVIBuffer_Cube)

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

private:
	void Free();
};

END