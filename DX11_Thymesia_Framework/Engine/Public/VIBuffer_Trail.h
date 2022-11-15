#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CTransform;
class CBoneNode;
class MODEL_DATA;

class ENGINE_DLL CVIBuffer_Trail : public CVIBuffer
{
	GAMECLASS_H(CVIBuffer_Trail)
	CLONE_H(CVIBuffer_Trail, CComponent)

public:
	CVIBuffer_Trail(const CVIBuffer_Trail& rhs);

public:
	typedef struct tag_TrailBuffer
	{
		_float3		vPos_0;
		_float3		vPos_1;
		_uint		iMaxCnt;
	} TRAIL_DESC;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	void			Update(_float _fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);

private:
	_uint			m_iVtxCnt	= 0;
	_float			m_fAccTime	= 0.f;
	TRAIL_DESC		m_pTrailDesc;

private:
	void Free();
};

END