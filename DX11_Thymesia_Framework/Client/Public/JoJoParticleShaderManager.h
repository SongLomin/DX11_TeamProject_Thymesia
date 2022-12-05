#pragma once
#ifdef _JOJO_EFFECT_TOOL_
#include "Base.h"
#include "Client_Defines.h"
BEGIN(Client)
class CJoJoParticleShaderManager final :
    public CBase
{
    DECLARE_SINGLETON(CJoJoParticleShaderManager)

public:
    void Initialize();

private:
    void Free();

public:

    std::vector<JJ_PARTICLE_SHADER_INFO> m_ParticleShaderInfos;
};
END
#endif // _JOJO_EFFECT_TOOL_
