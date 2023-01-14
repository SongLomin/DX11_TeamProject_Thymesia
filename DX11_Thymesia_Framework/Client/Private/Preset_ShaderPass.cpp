#include "stdafx.h"
#include "Preset_ShaderPass.h"
#include "Engine_Typedef.h"
#include <assimp/material.h>

_uint Preset::ShaderPass::ModelShaderPass(const _flag In_BindTextureFlag, const _bool In_bMasking, const _bool In_bNoneCulling, const _bool In_bDissolve)
{
    _uint iPassResult = 0;

    if (!((1 << aiTextureType_DIFFUSE) & In_BindTextureFlag))
    {
        return -1;
    }

	// Dissolve			OK.
	else if (In_bDissolve)
	{
		return 9;
	}

	// Invisibility		NO.
	// NonCulling		NO.
	// NormalTexture	NO.
	else if (!In_bMasking &&
		!In_bNoneCulling &&
		!((1 << aiTextureType_NORMALS) & In_BindTextureFlag))
	{
		return 0;
	}

	// Invisibility		NO.
	// NonCulling		NO.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (!In_bMasking &&
		!In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 7;
	}

	// Invisibility		NO.
	// NonCulling		OK.
	// NormalTexture	NO.
	else if (!In_bMasking &&
		In_bNoneCulling &&
		!((1 << aiTextureType_NORMALS) & In_BindTextureFlag))
	{
		return 4;
	}

	// Invisibility		NO.
	// NonCulling		OK.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (!In_bMasking &&
		In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 7;
	}

	// Invisibility		OK.
	// NonCulling		NO.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (In_bMasking &&
		!In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 17;
	}

	// Invisibility		OK.
	// NonCulling		OK.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (In_bMasking &&
		In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 17;
	}

	// Invisibility		OK.
	// NonCulling		OK.
	// NormalTexture	NO.
	else if (In_bMasking &&
		In_bNoneCulling &&
		!((1 << aiTextureType_NORMALS) & In_BindTextureFlag))
	{

		return 0;
	}

	// Invisibility		OK.
	// NonCulling		OK.
	// NormalTexture	OK.
	// ORMTexture		NO.
	else if (In_bMasking &&
		In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		!((1 << aiTextureType_SPECULAR) & In_BindTextureFlag))
	{
#ifdef _DEBUG
		//cout << "The correct pass does not define. : " << m_pInstanceModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG

		return 6;
	}

	// Invisibility		OK.
	// NonCulling		NO.
	// NormalTexture	OK.
	// ORMTexture		NO.
	else if (In_bMasking &&
	!In_bNoneCulling &&
	(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
	!((1 << aiTextureType_SPECULAR) & In_BindTextureFlag))
	{
#ifdef _DEBUG
	//cout << "The correct pass does not define. : " << m_pInstanceModelCom.lock()->Get_ModelKey() << endl;
#endif // _DEBUG

		return 16;
	}

	else if (!((1 << aiTextureType_NORMALS) & In_BindTextureFlag))
	{
		return 0;
	}

	


    return iPassResult;
}

_uint Preset::ShaderPass::ModelInstancingShaderPass(const _flag In_BindTextureFlag, const _bool In_bMasking, const _bool In_bNoneCulling, const _bool In_bDissolve)
{
	_uint iPassResult = 0;

	if (!((1 << aiTextureType_DIFFUSE) & In_BindTextureFlag))
	{
		return -1;
	}

	// Dissolve			OK.
	else if (In_bDissolve)
	{
		return 9;
	}

	// Invisibility		NO.
	// NonCulling		NO.
	// NormalTexture	NO.
	else if (!In_bMasking &&
		!In_bNoneCulling &&
		!((1 << aiTextureType_NORMALS) & In_BindTextureFlag))
	{
		return 0;
	}

	// Invisibility		NO.
	// NonCulling		__.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (!In_bMasking &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 8;
	}

	// Invisibility		NO.
	// NonCulling		NO.
	// NormalTexture	OK.
	// ORMTexture		NO.
	else if (!In_bMasking &&
		!In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag)
	{
		return 1;
	}

	// Invisibility		NO.
	// NonCulling		OK.
	// NormalTexture	OK.
	// ORMTexture		NO.
	else if (!In_bMasking &&
		In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag)
	{
		return 5;
	}

	// Invisibility		OK.
	// NonCulling		OK.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (In_bMasking &&
		In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 6;
	}

	// Invisibility		OK.
	// NonCulling		NO.
	// NormalTexture	OK.
	// ORMTexture		OK.
	else if (In_bMasking &&
		!In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		(1 << aiTextureType_SPECULAR) & In_BindTextureFlag)
	{
		return 7;
	}

	// Invisibility		OK.
	// NonCulling		OK.
	// NormalTexture	OK.
	// ORMTexture		NO.
	else if (In_bMasking &&
		In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		!((1 << aiTextureType_SPECULAR) & In_BindTextureFlag))
	{
		return 11;
	}

	// Invisibility		OK.
	// NonCulling		NO.
	// NormalTexture	OK.
	// ORMTexture		NO.
	else if (In_bMasking &&
		!In_bNoneCulling &&
		(1 << aiTextureType_NORMALS) & In_BindTextureFlag &&
		!((1 << aiTextureType_SPECULAR) & In_BindTextureFlag))
	{
		return 10;
	}


	return iPassResult;
}

_uint Preset::ShaderPass::AnimModelPass(const _flag In_BindTextureFlag)
{
    _uint iPassResult = -1;



    return iPassResult;
}
