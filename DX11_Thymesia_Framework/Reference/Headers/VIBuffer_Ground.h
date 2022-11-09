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

public:
	_bool			Get_VertexPos(_uint _iIndex, _float3* _pOut);

public:
	void			Update(_vector _vMousePos, _float _fRadious, _float _fPower, _uint _iMode);
	_bool			Compute_MousePos(RAY _Ray, _matrix _WorldMatrix, _float3* pOut);
	//HRESULT			Init_Mesh(shared_ptr<MESH_DATA> tMeshData, weak_ptr<CModel> pModel);

private:
	_ulong					m_iNumVerticesX, m_iNumVerticesZ;
	vector<_uint3>			m_Indices;
	vector<VTXNORTEX>		m_VertexInfo;

private:
	void Free();
};

END