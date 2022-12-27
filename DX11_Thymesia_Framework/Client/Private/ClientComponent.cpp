#include "stdafx.h"
#include "ClientComponent.h"
#include "Component.h"

GAMECLASS_C(CClientComponent);

void CClientComponent::Write_Json(json& Out_Json)
{
	__super::Write_Json(Out_Json);
}

void CClientComponent::Load_FromJson(const json& In_Json)
{
	__super::Load_FromJson(In_Json);

}
