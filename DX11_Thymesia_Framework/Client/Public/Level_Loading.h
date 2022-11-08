#pragma once

/* �δ��� �ڿ���� �ε��ϴ� ���� ȭ�鿡 ������ �����ٲ���. */
/* �δ����� ��� ������� ������?? */
/* �� ����ϸ� ���������� ����.�� */

#include "Client_Defines.h"
#include "ClientLevel.h"

BEGIN(Client)

class CLevel_Loading final : public CClientLevel
{
public:
	CLevel_Loading();
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL eNextLevel);
	virtual void Tick(_float TimeDelta);
	virtual HRESULT Render();

private:
	LEVEL			m_eNextLevel = LEVEL_END;
	shared_ptr<class CLoader>	m_pLoader;

public:
	static shared_ptr<CLevel_Loading> Create(LEVEL eNextLevel);
	void Free();
};

END