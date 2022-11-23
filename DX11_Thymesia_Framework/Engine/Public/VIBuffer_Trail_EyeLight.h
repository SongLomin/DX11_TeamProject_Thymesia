#pragma once
#include "VIBuffer_Trail.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail_EyeLight: public CVIBuffer_Trail
{
	GAMECLASS_H(CVIBuffer_Trail_EyeLight)
	CLONE_H(CVIBuffer_Trail_EyeLight, CComponent)

public:
	CVIBuffer_Trail_EyeLight(const CVIBuffer_Trail_EyeLight& rhs);

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Start() override;

public:
	void Update(_float _fTimeDelta, weak_ptr <CTransform> _pOwnerTransform, weak_ptr<CBoneNode> _pOwnerBoneNode, weak_ptr<MODEL_DATA> _pOwnerModel_Data);


private:
	 void Free();
};

END