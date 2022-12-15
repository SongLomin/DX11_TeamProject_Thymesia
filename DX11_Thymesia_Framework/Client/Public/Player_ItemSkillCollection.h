#pragma once
#include "UI.h"

BEGIN(Client)

class CPlayer_PotionUI;


class CPlayer_ItemSkillCollection final :public CUI
{
	GAMECLASS_H(CPlayer_ItemSkillCollection);
	CLONE_H(CPlayer_ItemSkillCollection, CGameObject);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

public:
	virtual void OnEventMessage(_uint iArg) override;

private:

public:
	void Free();

};

END
