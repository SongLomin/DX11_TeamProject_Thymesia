#pragma once
#include "UI.h"


BEGIN(Client)

class CCustomUI;
class CEasingComponent_Float;

class CHUD_Player_Memory final : public CUI
{
	GAMECLASS_H(CHUD_Player_Memory);
	CLONE_H(CHUD_Player_Memory, CGameObject);
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);
private:
	virtual void Bind_Player();
public:
	virtual void OnEventMessage(_uint iArg) override;


protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	weak_ptr<CCustomUI>		m_pBG;
	weak_ptr<CCustomUI>		m_pIcon;
	weak_ptr<CCustomUI>		m_pDecoration;

private:
	_float					m_fLerpMemory;
	_float					m_fDifference;


private:
	weak_ptr<CEasingComponent_Float> m_pEasingComFloat;

public:
	void	Call_UpdateMemory();

	void	Call_ChangeMemory(_uint iMemory);
public:
	void Free();
};

END

