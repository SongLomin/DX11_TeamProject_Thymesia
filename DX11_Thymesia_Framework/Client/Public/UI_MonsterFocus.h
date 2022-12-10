#pragma once
#include "CustomUI.h"


BEGIN(Client)

class CMonster;

class CEasingComponent_Alpha;

class CUI_MonsterFocus : public CCustomUI
{
	friend class CPreset_UIDesc;
public:
	GAMECLASS_H(CUI_MonsterFocus)
	CLONE_H(CUI_MonsterFocus, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void					Call_FocusInTarget();
	void					Call_GroggyStart();
	
	


	void					Call_FocusOutTarget();	
	void					Bind_Callback();

private:
	virtual void			SetUp_Component() override;

private:
	void					FollowTargetBone();

private:
	void					OnEnable(void* pArg) override;

private:
	string					m_strTargetMonsterBone;
	weak_ptr<CMonster>		m_pTargetMonster;
	weak_ptr<CEasingComponent_Alpha>	m_pEasingAlphaCom;

};

END