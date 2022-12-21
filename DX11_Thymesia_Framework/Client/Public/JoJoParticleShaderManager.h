#pragma once
#include "Base.h"
#include "Client_Defines.h"
#ifdef _EFFECT_TOOL_
BEGIN(Client)
class CJoJoParticleShaderManager :
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
#endif // _EFFECT_TOOL_
