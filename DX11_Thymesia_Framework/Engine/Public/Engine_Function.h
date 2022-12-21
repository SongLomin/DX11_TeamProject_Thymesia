#pragma once

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}

	/*template<typename T>
	void Safe_Release(T& Temp)
	{
		if (Temp)
		{
			Temp->Free();
			Temp = nullptr;
		}
	}*/

	/*template<typename T>
	unsigned long Safe_Release(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)
		{
			dwRefCnt = Temp->Release();

			if(0 == dwRefCnt)
				Temp = nullptr;
		}

		return dwRefCnt;
	}*/

	/*template<typename T>
	unsigned long Safe_AddRef(T& Temp)
	{
		unsigned long dwRefCnt = 0;

		if (Temp)	
			dwRefCnt = Temp->AddRef();		

		return dwRefCnt;
	}*/


	template<typename T>
	void Safe_Delete_Array(T& Temp)
	{
		if (Temp)
		{
			delete[] Temp;
			Temp = nullptr;
		}
	}

	template<typename T>
	std::ostream& write_typed_data(std::ostream& stream, const T& value) {
		return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}
	template<typename T>
	std::istream& read_typed_data(std::istream& stream, T& value) {
		return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
	}

}

#ifdef _DEBUG

inline void DXUT_SetDebugName(_In_ ID3D11DeviceChild* pObj,
	_In_z_ const CHAR* pstrName)
{
	if (pObj)
		pObj->SetPrivateData(WKPDID_D3DDebugObjectName,
			(UINT)strlen(pstrName), pstrName);
}

inline void Print_Vector(XMVECTOR In_vector)
{
	std::cout << In_vector.m128_f32[0] << ", " << In_vector.m128_f32[1] << ", " << In_vector.m128_f32[2] << ", " << In_vector.m128_f32[3] << std::endl;
}

inline void Print_Float4(XMFLOAT4 In_Float4)
{
	Print_Vector(XMLoadFloat4(&In_Float4));
}

inline void Print_Float3(XMFLOAT3 In_Float3)
{
	Print_Vector(XMLoadFloat3(&In_Float3));
}


#else
#define DXUT_SetDebugName( pObj, pstrName )
#define Print_Vector(Do_Nothing)
#define Print_Float4(Do_Nothing)
#define Print_Float3(Do_Nothing)
#endif