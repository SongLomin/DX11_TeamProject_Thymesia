#include "stdafx.h"
#include "Interaction_Item.h"

#include "Model.h"
#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Inventory.h"

#include "GameInstance.h"
#include "ClientLevel.h"
#include "GameManager.h"
#include "imgui.h"

GAMECLASS_C(CInteraction_Item);
CLONE_C(CInteraction_Item, CGameObject);

HRESULT CInteraction_Item::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Item::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom = Add_Component<CCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    _float fDefaultDesc[4] = { 1.f, 0.f, 0.f, 0.f };
    SetUpColliderDesc(fDefaultDesc);

    m_eInteractionType = INTERACTIONTYPE::INTERACTION_ITEM;

    return S_OK;
}

HRESULT CInteraction_Item::Start()
{
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
    m_iEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("ItemEffect", m_pTransformCom, (_uint)TIMESCALE_LAYER::NONE);

    return __super::Start();
}

void CInteraction_Item::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Item::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CInteraction_Item::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
}

void CInteraction_Item::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            static const char* szItemList[] = 
            {
                "BASIL",
                "THYME",
                "CINNAMON",

                "GARDEN_KEY",
                "VARG_KEY",

                "MEMORY01",

                "ITEM_NAME_END"
            };

            static _int iSelect_item      = 0;
            static _int iSelect_ItemIndex = 0;

            ImGui::Combo("Item", &iSelect_item, szItemList, IM_ARRAYSIZE(szItemList));

            auto ItemInfo = m_Items.begin();

            if (ImGui::BeginListBox("##Item", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (_uint i = 0; i < m_Items.size(); ++i)
                {
                    const bool is_selected = (iSelect_ItemIndex == i);

                    if (ImGui::Selectable(szItemList[(_uint)(*ItemInfo)], is_selected))
                    {
                        iSelect_ItemIndex = i;
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();

                    ItemInfo++;
                }

                ImGui::EndListBox();
            }

            if (ImGui::Button("Add Item"))
            {
                m_Items.push_back((ITEM_NAME)iSelect_item);
            }
        }
        break;
    }
}


void CInteraction_Item::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["ItemCnt"] = (_uint)m_Items.size();

    _int iIndex = 0;
    for (auto elem : m_Items)
    {
        Out_Json["Item_Info"][iIndex] = (_uint)elem;
        ++iIndex;
    }
}

void CInteraction_Item::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);
 
    _uint iCnt = In_Json["ItemCnt"];

    for (_uint i = 0; i < iCnt; ++i)
    {
        m_Items.push_back((ITEM_NAME)In_Json["Item_Info"][i]);
    }
}

void CInteraction_Item::Act_Interaction()
{
    weak_ptr<CGameObject> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

    if (!pPlayer.lock())
        return;

    for (auto elem : m_Items)
        pPlayer.lock()->Get_Component<CInventory>().lock()->Push_Item(elem);

    GAMEINSTANCE->PlaySound2D("EVM_ItemPickUp.ogg", 3.f);
    m_pColliderCom.lock()->Set_Enable(false);
    Set_Dead();
}

void CInteraction_Item::SetUpColliderDesc(_float* _pColliderDesc)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f);
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_Item::OnDestroy()
{
    GET_SINGLE(CGameManager)->UnUse_EffectGroup("ItemEffect", m_iEffectIndex);
}

void CInteraction_Item::Free()
{
}
