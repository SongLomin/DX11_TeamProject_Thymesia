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
#else
#define DXUT_SetDebugName( pObj, pstrName )
#endif