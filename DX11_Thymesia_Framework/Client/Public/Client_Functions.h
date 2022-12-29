#pragma once


namespace Client
{
	template <typename TemplateDelegate>
	void	Safe_DelegateClear(TemplateDelegate& Deleagate)
	{
		if (!Deleagate.empty())
		{
			Deleagate.Clear();
		}
	}

}