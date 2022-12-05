#pragma once
#include "CustomUI.h"

BEGIN(Engine)

class CTexture;
class CShader;
END

BEGIN(Client)

class CUI_EvolveMenu_Level_BG : public CCustomUI
{
public:
	GAMECLASS_H(CUI_EvolveMenu_Level_BG)
	CLONE_H(CUI_EvolveMenu_Level_BG, CGameObject)

public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT SetUp_ShaderResource() override;

};

END
