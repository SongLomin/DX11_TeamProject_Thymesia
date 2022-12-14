#pragma once
#include "CustomUI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CUI_T_Dissolve : public CCustomUI
{
public:
	GAMECLASS_H(CUI_T_Dissolve);
	CLONE_H(CUI_T_Dissolve, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);



private:


};


END

    