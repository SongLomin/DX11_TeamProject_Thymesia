#pragma once
#include "ClientComponent.h"
#include "Base.h"



BEGIN(Client)

class CPlayer;
class CCorvus;
class CStatus_Player;


class CClientComponent_Utils : public CBase
{
public:
	static weak_ptr<CPlayer>	ConvertOwnerToPlayer(weak_ptr<CGameObject> pOwner);
	static weak_ptr<CCorvus>	ConvertOwnerToCorvus(weak_ptr<CGameObject> pOwner);
	
};

END