#pragma once
#include "Client_Defines.h"

BEGIN(Client)
class CDynamic_Piece;
END


namespace Preset
{
	namespace Dynamic_Prop
	{
		void GateWay_DynamicPropSetting(const string& In_szModelKey, const _uint In_iCreatedLevel, list<weak_ptr<Client::CDynamic_Piece>>& Out_Pieces);
		void Wagon3_DynamicPropSetting(const _uint In_iCreatedLevel, list<weak_ptr<Client::CDynamic_Piece>>& Out_Pieces);
		void Fence_16a_DynamicPropSetting(const _uint In_iCreatedLevel, list<weak_ptr<Client::CDynamic_Piece>>& Out_Pieces);
		void ColumnDamage08_DynamicPropSetting(const _uint In_iCreatedLevel, list<weak_ptr<Client::CDynamic_Piece>>& Out_Pieces);
	}
}