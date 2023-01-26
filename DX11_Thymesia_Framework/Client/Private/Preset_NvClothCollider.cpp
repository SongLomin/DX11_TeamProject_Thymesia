#include "stdafx.h"
#include "Preset_NvClothCollider.h"
#include "CNvClothCollider.h"

void* Preset::NvClothCollider::CorvusSetting(_uint& Out_Count)
{
	CNvClothCollider::NVCLOTH_COLLIDER_DESC* pColliderDesc = DBG_NEW CNvClothCollider::NVCLOTH_COLLIDER_DESC[5];

	pColliderDesc[0].szBoneName = "Bip001-Head";
	pColliderDesc[0].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[0].fScale = 0.2f;

	pColliderDesc[1].szBoneName = "Bip001-Neck";
	pColliderDesc[1].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[1].fScale = 0.1f;

	pColliderDesc[2].szBoneName = "Bip001-Spine";
	pColliderDesc[2].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[2].fScale = 0.15f;

	pColliderDesc[3].szBoneName = "Bip001-Spine1";
	pColliderDesc[3].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[3].fScale = 0.15f;

	pColliderDesc[4].szBoneName = "Bip001-Spine2";
	pColliderDesc[4].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[4].fScale = 0.15f;

	/*pColliderDesc[5].szBoneName = "Bip001-Xtra02";
	pColliderDesc[5].vOffset = _float3(0.f, -0.2f, 0.f);
	pColliderDesc[5].fScale = 0.15f;

	pColliderDesc[6].szBoneName = "Bip001-Xtra02Opp";
	pColliderDesc[6].vOffset = _float3(0.f, -0.2f, 0.f);
	pColliderDesc[6].fScale = 0.15f;*/

	Out_Count = 5;

	return pColliderDesc;
}

void* Preset::NvClothCollider::VergSetting(_uint& Out_Count)
{
	CNvClothCollider::NVCLOTH_COLLIDER_DESC* pColliderDesc = DBG_NEW CNvClothCollider::NVCLOTH_COLLIDER_DESC[5];

	pColliderDesc[0].szBoneName = "Bip001-Head";
	pColliderDesc[0].vOffset = _float3(0.2f, 0.f, 0.f);
	pColliderDesc[0].fScale = 0.15f;

	pColliderDesc[1].szBoneName = "Bip001-Neck";
	pColliderDesc[1].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[1].fScale = 0.2f;

	pColliderDesc[2].szBoneName = "Bip001-Neck";
	pColliderDesc[2].vOffset = _float3(0.1f, 0.f, 0.f);
	pColliderDesc[2].fScale = 0.2f;

	pColliderDesc[3].szBoneName = "Bip001-Spine2";
	pColliderDesc[3].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[3].fScale = 0.3f;

	pColliderDesc[4].szBoneName = "Bip001";
	pColliderDesc[4].vOffset = _float3(0.f, 0.f, 0.f);
	pColliderDesc[4].fScale = 0.3f;

	Out_Count = 5;

	return pColliderDesc;
}
