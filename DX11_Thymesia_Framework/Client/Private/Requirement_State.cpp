#include "stdafx.h"
#include "Requirement_State.h"
#include "Character.h"
#include "StateBase.h"

void CRequirement_State::Init_Req(weak_ptr<CCharacter> pCharacter, const _uint In_StateIndex)
{
    m_pCharacter = pCharacter;
    m_iStateIndex = In_StateIndex;
}

_bool CRequirement_State::Check_Requirement()
{
    if (!m_pCharacter.lock())
    {
        DEBUG_ASSERT;
    }

    return m_pCharacter.lock()->Get_CurrentStateIndex() != m_iStateIndex;
}

_bool CRequirement_State::Is_Valid()
{
    return m_pCharacter.lock()->Get_CurrentStateIndex() == m_iStateIndex;
}
