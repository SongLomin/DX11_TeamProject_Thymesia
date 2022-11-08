#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Rect)
	CLONE_H(CVIBuffer_Rect, CComponent)

public:
	CVIBuffer_Rect(const CVIBuffer_Rect& rhs);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

private:
	void Free();
};

END