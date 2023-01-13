#pragma once

namespace Preset
{
	namespace ShaderPass
	{
		_uint ModelShaderPass(const _flag In_BindTextureFlag, const _bool In_bMasking, const _bool In_bNoneCulling, const _bool In_bDissolve);
		_uint ModelInstancingShaderPass(const _flag In_BindTextureFlag, const _bool In_bMasking, const _bool In_bNoneCulling, const _bool In_bDissolve);
		_uint AnimModelPass(const _flag In_BindTextureFlag);
	}
}