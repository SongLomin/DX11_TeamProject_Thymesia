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

public:
	void Tick(_float fTimeDelta);
	// void Set_OwnerTransform(shared_ptr<CTransform> pTransform);

private:
	_bool m_bTrailOn;
	_uint m_iVtxCount;
	_float4 m_vLocalSwordLow;
	_float4 m_vLocalSwordHigh;
	// class weak_ptr<CTransform> m_pOwnerTransform;
	// class weak_ptr<CHierarchyNode>

private:
	// void Free();
};

END