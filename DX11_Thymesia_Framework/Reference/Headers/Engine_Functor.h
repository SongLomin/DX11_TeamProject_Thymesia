#pragma once

namespace Engine
{

	class CTag_Finder
	{
	private:
		const TCHAR*	m_pStr;

	public:
		CTag_Finder(const TCHAR* pStr)
			: m_pStr(pStr)
		{

		}

	public:
		template<typename T>
		bool	operator()(T& Pair)
		{
			if (!lstrcmp(m_pStr, Pair.first.c_str()))
				return true;

			return false;
		}
	};

	class CTag_Finder_c_str
	{
	private:
		const char* m_pStr;

	public:
		CTag_Finder_c_str(const char* pStr)
			: m_pStr(pStr)
		{

		}

	public:
		template<typename T>
		bool	operator()(T& Pair)
		{
			if (!strcmp(m_pStr, Pair.first.c_str()))
				return true;

			return false;
		}
	};

}

