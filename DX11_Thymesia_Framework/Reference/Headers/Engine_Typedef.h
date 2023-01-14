#pragma once

namespace Engine
{
	typedef unsigned char				_ubyte;
	typedef signed char					_byte;

	typedef unsigned short				_ushort;
	typedef signed short				_short;

	typedef unsigned int				_uint;
	typedef signed int					_int;
	typedef size_t						_size_t;
	typedef size_t						_hashcode;

	typedef unsigned long				_ulong;
	typedef unsigned long long			_ulonglong;
	typedef signed long					_long;

	typedef float						_float;
	typedef double						_double;

	typedef bool						_bool;

	typedef wchar_t						_tchar;
	typedef char						_char;

	typedef XMINT2						_int2;

	typedef XMFLOAT2					_float2;
	typedef XMFLOAT3					_float3;
	typedef XMFLOAT4					_float4;
	typedef XMVECTOR					_vector;
	typedef FXMVECTOR					_fvector;

	typedef XMFLOAT4X4					_float4x4;
	typedef XMFLOAT3X3					_float3x3;
	typedef XMMATRIX					_matrix;
	typedef FXMMATRIX					_fmatrix;

	typedef unsigned long long			_flag;
	typedef unsigned long long			_time;



	typedef struct tagInt32
	{
		tagInt32(unsigned int _ix, unsigned int _iy, unsigned int _iz)
			: ix(_ix)
			, iy(_iy)
			, iz(_iz) {	}

		tagInt32()
			: ix(0)
			, iy(0)
			, iz(0) {	}

		unsigned int ix, iy, iz;
	} _uint3;

	typedef XMINT4						_uint4;



	

	
}
