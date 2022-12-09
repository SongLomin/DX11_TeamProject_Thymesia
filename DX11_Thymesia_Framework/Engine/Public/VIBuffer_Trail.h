#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CTransform;
class CBoneNode;
struct MODEL_DATA;

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

public:
	void Update(_float _fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);
	void Tick(_float fTimeDelta);
	
public:
	void Reset_Points(weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);


protected:
	_bool m_bTrailOn;
	_uint m_iVtxCnt;
	_uint m_iNumVertices;
	_float4 m_vLocalSwordLow;
	_float4 m_vLocalSwordHigh;
	TRAIL_DESC m_tTrailDesc;
	
	_uint m_iLerpPointNum = 12;
	_int m_iCatMullRomIndex[4];

private:
	 void Free();
};

END