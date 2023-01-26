#include "stdafx.h"
#include "Requirement_PlayerStatusMana.h"
#include "Status_Player.h"
#include "GameManager.h"

HRESULT CRequirement_PlayerStatusMana::Initialize(void* pArg)
{
    m_fRequireMana = 0.f;
    m_bIsValid = true;

    return S_OK;
}

_bool CRequirement_PlayerStatusMana::Check_Requirement()
{
    if (!m_pStatus_Player.lock())
    {
        m_pStatus_Player = GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status();
    }

    return  m_pStatus_Player.lock()->Get_CurrentMP() >= m_fRequireMana;
}

_bool CRequirement_PlayerStatusMana::Is_Valid()
{
    return m_bIsValid;
}

void CRequirement_PlayerStatusMana::Init_Req(_float fRequireMana)
{
    m_fRequireMana = fRequireMana;
}
