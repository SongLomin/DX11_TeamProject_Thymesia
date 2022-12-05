#ifdef _JOJO_EFFECT_TOOL_
#include "stdafx.h"
#include "JoJoParticleShaderManager.h"

IMPLEMENT_SINGLETON(CJoJoParticleShaderManager)

void CJoJoParticleShaderManager::Initialize()
{
	m_ParticleShaderInfos.reserve(16);

#define MACRO(variable_name, pass_num, rect_type, discard_type, soft, special)\
	JJ_PARTICLE_SHADER_INFO variable_name;\
	ZeroMemory(&variable_name, sizeof(JJ_PARTICLE_SHADER_INFO));\
	variable_name.iShaderPassNumber = pass_num;\
	variable_name.eRectType         = JJ_PARTICLE_SHADER_INFO::PARTICLE_RECT_TYPE::rect_type;\
	variable_name.eDiscardType      = JJ_PARTICLE_SHADER_INFO::PARTICLE_DISCARD_TYPE::discard_type;\
	variable_name.bSoftRendering    = soft;\
	variable_name.bSpecialRendering = special;\
	m_ParticleShaderInfos.emplace_back(variable_name);

	MACRO(tInfo0, 0, SPRITE, DISCARD_BLACK, false, false);
	MACRO(tInfo1, 1, DEFAULT, DISCARD_ALPHA, false, false);
	MACRO(tInfo2, 2, DEFAULT, DISCARD_BLACK, false, false);
	MACRO(tInfo3, 3, SPRITE, DISCARD_ALPHA, false, false);
	MACRO(tInfo4, 4, DEFAULT, DISCARD_ALPHA, true, false);
	MACRO(tInfo5, 5, DEFAULT, DISCARD_BLACK, true, false);
	MACRO(tInfo6, 6, SPRITE, DISCARD_ALPHA, true, false);
	MACRO(tInfo7, 7, SPRITE, DISCARD_BLACK, true, false);

	MACRO(tInfo8, 8, SPRITE, DISCARD_BLACK, false, true);
	MACRO(tInfo9, 9, DEFAULT, DISCARD_ALPHA, false, true);
	MACRO(tInfo10, 10, DEFAULT, DISCARD_BLACK, false, true);
	MACRO(tInfo11, 11, SPRITE, DISCARD_ALPHA, false, true);
	MACRO(tInfo12, 12, DEFAULT, DISCARD_ALPHA, true, true);
	MACRO(tInfo13, 13, DEFAULT, DISCARD_BLACK, true, true);
	MACRO(tInfo14, 14, SPRITE, DISCARD_ALPHA, true, true);
	MACRO(tInfo15, 15, SPRITE, DISCARD_BLACK, true, true);

#undef MACRO
}

void CJoJoParticleShaderManager::Free()
{
	m_ParticleShaderInfos.clear();
}

#endif // _JOJO_EFFECT_TOOL_
