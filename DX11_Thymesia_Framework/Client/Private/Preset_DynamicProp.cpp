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
