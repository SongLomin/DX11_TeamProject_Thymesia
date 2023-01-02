#include "stdafx.h"
#include "ClientComponent_Utils.h"
#include "Player.h"
#include "Corvus.h"
#include "Engine_Defines.h"
#include "GameObject.h"
#include "Client_Defines.h"
#include "Base.h"
#include "Component.h"
#include "ClientComponent.h"


weak_ptr<CPlayer> CClientComponent_Utils::ConvertOwnerToPlayer(weak_ptr<CGameObject> pOwner)
{
    return Weak_StaticCast<CPlayer>(pOwner);
}

weak_ptr<CCorvus> CClientComponent_Utils::ConvertOwnerToCorvus(weak_ptr<CGameObject> pOwner)
{
    return Weak_StaticCast<CCorvus>(pOwner);
}
