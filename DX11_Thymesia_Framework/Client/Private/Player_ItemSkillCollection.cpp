#include "stdafx.h"
#include "Player_ItemSkillCollection.h"


GAMECLASS_C(CPlayer_ItemSkillCollection)
CLONE_C(CPlayer_ItemSkillCollection, CGameObject);

HRESULT CPlayer_ItemSkillCollection::Initialize_Prototype()
{
    __super::Initialize_Prototype();
 
    return S_OK;
}

HRESULT CPlayer_ItemSkillCollection::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    return S_OK;
}

HRESULT CPlayer_ItemSkillCollection::Start()
{
    __super::Start();

    return S_OK;
}

void CPlayer_ItemSkillCollection::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CPlayer_ItemSkillCollection::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CPlayer_ItemSkillCollection::Render(ID3D11DeviceContext* pDeviceContext)
{



    return S_OK;
}

void CPlayer_ItemSkillCollection::OnEventMessage(_uint iArg)
{

}

void CPlayer_ItemSkillCollection::Free()
{
    __super::Free();
}
