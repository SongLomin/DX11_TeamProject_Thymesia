#include "stdafx.h"
#include "Preset_DynamicProp.h"
#include "Dynamic_Piece.h"

void Preset::Dynamic_Prop::GateWay_DynamicPropSetting(const string& In_szModelKey, const _uint In_iCreatedLevel, list<weak_ptr<CDynamic_Piece>>& Out_Pieces)
{
	_hashcode ModelKeyToHash = hash<string>()(In_szModelKey);

	if (hash<string>()("Wagon3_Origin") == ModelKeyToHash)
	{
		Wagon3_DynamicPropSetting(In_iCreatedLevel, Out_Pieces);
	}

    else if (hash<string>()("Fence_16a_Origin") == ModelKeyToHash)
    {
        Fence_16a_DynamicPropSetting(In_iCreatedLevel, Out_Pieces);
    }

    else if (hash<string>()("ColumnDamage08_Origin") == ModelKeyToHash)
    {
        ColumnDamage08_DynamicPropSetting(In_iCreatedLevel, Out_Pieces);
    }

}

void Preset::Dynamic_Prop::Wagon3_DynamicPropSetting(const _uint In_iCreatedLevel, list<weak_ptr<CDynamic_Piece>>& Out_Pieces)
{
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_1"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_2"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_3"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_4"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_5"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_6"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_7"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_8"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_9"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_10"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_11"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_12"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_13"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_14"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_15"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_16"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_17"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_18"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_19"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_20"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_21"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_22"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_23"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Wagon3_24"));
}

void Preset::Dynamic_Prop::Fence_16a_DynamicPropSetting(const _uint In_iCreatedLevel, list<weak_ptr<Client::CDynamic_Piece>>& Out_Pieces)
{
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Fence_16a_1"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Fence_16a_2"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"Fence_16a_3"));
}

void Preset::Dynamic_Prop::ColumnDamage08_DynamicPropSetting(const _uint In_iCreatedLevel, list<weak_ptr<Client::CDynamic_Piece>>& Out_Pieces)
{
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_1"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_2"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_3"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_4"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_5"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_6"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_7"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_8"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_9"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_10"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_11"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_12"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_13"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_14"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_15"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_16"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_17"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_18"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_19"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_20"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_21"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_22"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_23"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_24"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_25"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_26"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_27"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_28"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_29"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_30"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_31"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_32"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_33"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_34"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_35"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_36"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_37"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_38"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_39"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_40"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_41"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_42"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_43"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_44"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_45"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_46"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_47"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_48"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_49"));
    Out_Pieces.push_back(GAMEINSTANCE->Add_GameObject<CDynamic_Piece>(In_iCreatedLevel, (void*)"ColumnDamage08_50"));
}
