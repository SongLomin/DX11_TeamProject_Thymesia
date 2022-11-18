#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_DynamicCube final :
    public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_DynamicCube)
	CLONE_H(CVIBuffer_DynamicCube, CComponent)

public:
	virtual HRESULT Initialize_Prototype	();
	virtual HRESULT Initialize				(void* _pArg)	override;
	virtual void	Start					()				override;

public:
	void			Update(MESH_VTX_INFO _VtxInfo, _fmatrix _WorldMatrix);

public:
	void Free();
};

END
