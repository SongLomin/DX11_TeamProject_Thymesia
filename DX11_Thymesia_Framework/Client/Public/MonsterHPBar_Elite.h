#pragma once
#include "MonsterHPBar_Base.h"


BEGIN(Client)

class CMonsterHPBar_Elite : public CMonsterHPBar_Base
{
public:
	GAMECLASS_H(CMonsterHPBar_Elite)
	CLONE_H(CMonsterHPBar_Elite, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual void	Set_ChildPosFromThis();

private:
	weak_ptr<CCustomUI>		m_pEliteBorder;
};
END

