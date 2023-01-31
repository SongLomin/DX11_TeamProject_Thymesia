#include "stdafx.h"
#include "Interaction_Door.h"

#include "Shader.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Collider.h"
#include "Model.h"
#include "PhysXCollider.h"
#include "Inventory.h"
#include "Item.h"

#include "UI_ItemRequirement.h"


#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"
#include "SMath.h"

GAMECLASS_C(CInteraction_Door);
CLONE_C(CInteraction_Door, CGameObject);

HRESULT CInteraction_Door::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInteraction_Door::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pColliderCom      = Add_Component<CCollider>();
    m_pPhysXColliderCom = Add_Component<CPhysXCollider>();

    m_pShaderCom.lock()->Set_ShaderInfo
    (
        TEXT("Shader_VtxModel"),
        VTXMODEL_DECLARATION::Element,
        VTXMODEL_DECLARATION::iNumElements
    );

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_STATICSHADOWDEPTH, Weak_StaticCast<CGameObject>(m_this));
    m_eInteractionType = INTERACTION_DOOR;

//#ifdef _USE_THREAD_
//    Use_Thread(THREAD_TYPE::PRE_BEFORERENDER);
//#endif // _USE_THREAD_


    return S_OK;
}

HRESULT CInteraction_Door::Start()
{
    __super::Start();

    XMStoreFloat4x4(&m_FirstMatrix, m_pTransformCom.lock()->Get_WorldMatrix());

    return S_OK;
}

void CInteraction_Door::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CInteraction_Door::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom);
}

void CInteraction_Door::Thread_PreBeforeRender(_float fTimeDelta)
{
    __super::Thread_PreBeforeRender(fTimeDelta);
}

HRESULT CInteraction_Door::Render(ID3D11DeviceContext* pDeviceContext)
{
    return __super::Render(pDeviceContext);
}

void CInteraction_Door::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_ENTER_SECTION:
        {
            if (!Callback_ActUpdate.empty())
                return;

            if (!(m_ActionFlag & ACTION_FLAG::AUTO) || !(m_ActionFlag & ACTION_FLAG::ROTATION))
                return;

            // Open
            Callback_ActUpdate += bind(&CInteraction_Door::Act_OpenDoor, this, placeholders::_1, placeholders::_2);
            m_pPhysXColliderCom.lock()->Set_Enable(false);
        }
        break;

        case EVENT_TYPE::ON_EXIT_SECTION:
        {
            if (!Callback_ActUpdate.empty())
                return;

            if (!(m_ActionFlag & ACTION_FLAG::AUTO) || (m_ActionFlag & ACTION_FLAG::ROTATION))
                return;

            // Close
            Callback_ActUpdate += bind(&CInteraction_Door::Act_CloseDoor, this, placeholders::_1, placeholders::_2);
        }
        break;

        case EVENT_TYPE::ON_LOCK_SECTION:
        {
            if (!(m_ActionFlag & ACTION_FLAG::LOCK))
                return;

            m_pColliderCom.lock()->Set_Enable(false);
        }
        break;

        case EVENT_TYPE::ON_RESET_OBJ:
        {
            m_pTransformCom.lock()->Set_WorldMatrix(XMLoadFloat4x4(&m_FirstMatrix));

            if (!(m_ActionFlag & ACTION_FLAG::ROTATION))
                m_ActionFlag ^= ACTION_FLAG::ROTATION;

            PxControllerFilters Filters;

            m_pColliderCom.lock()->Set_Enable(true);
            m_pPhysXColliderCom.lock()->Set_Enable(true);

            Callback_ActUpdate.Clear();
        }
        break;

        case EVENT_TYPE::ON_UNLOCK_SECTION:
        {
            if (!(m_ActionFlag & ACTION_FLAG::LOCK))
                return;

            m_pColliderCom.lock()->Set_Enable(true);
        }
        break;

        case EVENT_TYPE::ON_EDITINIT:
        {
            if (0.f == m_pColliderCom.lock()->Get_ColliderDesc().vScale.x)
            {
                _float fDefaultDesc[4] = { 3.f, 0.f, 1.5f, 0.f };
                SetUpColliderDesc(m_pColliderCom, fDefaultDesc, COLLISION_LAYER::TRIGGER);
            }

            if (!m_pDirColliderCom.lock())
            {
                m_pDirColliderCom = Add_Component<CCollider>();

                _float fDefaultDesc[4] = { 1.f, 0.f, 1.5f, 1.f };
                SetUpColliderDesc(m_pDirColliderCom, fDefaultDesc, COLLISION_LAYER::CHECK_DIR);
                m_pDirColliderCom.lock()->Set_Enable(false);
            }
        }
        break;

        case EVENT_TYPE::ON_EDIT_UDATE:
        {
            m_pColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
            m_pDirColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
        }
        break;

        case EVENT_TYPE::ON_EDITDRAW:
        {
            SetUp_Invisibility();

            ImGui::DragFloat("Rotation Speed ", &m_fRotationtSpeed);
            ImGui::DragFloat("Rotation Radian", &m_fRotationtRadian);

            _bool bOpction[] =
            {
                (m_ActionFlag & ACTION_FLAG::ROTATION),
                (m_ActionFlag & ACTION_FLAG::KEY),
                (m_ActionFlag & ACTION_FLAG::AUTO),
                (m_ActionFlag & ACTION_FLAG::OPEN_DIR),
                (m_ActionFlag & ACTION_FLAG::LOCK),
            };

            if (ImGui::Checkbox("ROTATION", &bOpction[0]))
                m_ActionFlag ^= ACTION_FLAG::ROTATION;

            if (ImGui::Checkbox("KEY", &bOpction[1]))
            {
                m_ActionFlag ^= ACTION_FLAG::KEY;

                if (bOpction[2]) m_ActionFlag ^= ACTION_FLAG::AUTO;
                if (bOpction[3]) m_ActionFlag ^= ACTION_FLAG::OPEN_DIR;
            }

            if (ImGui::Checkbox("AUTO", &bOpction[2]))
            {
                m_ActionFlag ^= ACTION_FLAG::AUTO;

                if (bOpction[1]) m_ActionFlag ^= ACTION_FLAG::KEY;
                if (bOpction[3]) m_ActionFlag ^= ACTION_FLAG::OPEN_DIR;
            }

            if (ImGui::Checkbox("OPEN_DIR", &bOpction[3]))
            {
                m_ActionFlag ^= ACTION_FLAG::OPEN_DIR;

                m_pDirColliderCom.lock()->Set_Enable(m_ActionFlag & ACTION_FLAG::OPEN_DIR);

                if (bOpction[1]) m_ActionFlag ^= ACTION_FLAG::KEY;
                if (bOpction[2]) m_ActionFlag ^= ACTION_FLAG::AUTO;
            }

            if (ImGui::Checkbox("LOCK", &bOpction[4]))
            {
                m_ActionFlag ^= ACTION_FLAG::LOCK;
            }

            static const char* szKeyTag[] =
            {
                "GARDEN_KEY",
                "VARG_KEY"
            };

            static _int iSelect_KeyID = 0;

            if (bOpction[1])
            {
                ImGui::Combo("Key ID", &iSelect_KeyID, szKeyTag, IM_ARRAYSIZE(szKeyTag));
                m_iKeyID = (ITEM_NAME)((_uint)ITEM_NAME::GARDEN_KEY + iSelect_KeyID);
            }
            else
            {
                m_iKeyID      = ITEM_NAME::ITEM_NAME_END;
                iSelect_KeyID = 0;
            }

            if (bOpction[2])
            {
                ImGui::InputInt("Section Index", &m_iSectionIndex);
            }
            else
            {
                m_iSectionIndex = -1;
            }

            if (bOpction[3])
            {
                COLLIDERDESC ColliderDesc;
                ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

                ColliderDesc = m_pDirColliderCom.lock()->Get_ColliderDesc();

                _bool bChage = false;

                bChage |= ImGui::DragFloat3("Dir Coll Transform", &ColliderDesc.vTranslation.x);
                bChage |= ImGui::DragFloat("Dir Coll Size", &ColliderDesc.vScale.x);

                if (bChage)
                {
                    m_pDirColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
                }

                ImGui::Separator();
            }

            COLLIDERDESC ColliderDesc;
            ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

            ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();

            _bool bChage = false;

            bChage |= ImGui::DragFloat3("Coll Transform", &ColliderDesc.vTranslation.x);
            bChage |= ImGui::DragFloat ("Coll Size"     , &ColliderDesc.vScale.x);

            if (bChage)
            {
                m_pColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
            }
        }
        break;
    }
}

void CInteraction_Door::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["ActionFlag"]        = m_ActionFlag;
    Out_Json["RotationtRadian"]   = m_fRotationtRadian;
    Out_Json["RotationSpeed"]     = m_fRotationtSpeed;
    Out_Json["KeyID"]             = m_iKeyID;
    Out_Json["SectionIndex"]      = m_iSectionIndex;

    if (m_pColliderCom.lock())
    {
        COLLIDERDESC ColliderDesc;
        ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

        ColliderDesc = m_pColliderCom.lock()->Get_ColliderDesc();

        _float ColliderInfo[4] = 
        {
            ColliderDesc.vScale.x,
            ColliderDesc.vTranslation.x,
            ColliderDesc.vTranslation.y,
            ColliderDesc.vTranslation.z
        };

        Out_Json["ColliderDesc"] = ColliderInfo;
    }

    if (m_pDirColliderCom.lock() && m_ActionFlag & ACTION_FLAG::OPEN_DIR)
    {
        COLLIDERDESC ColliderDesc;
        ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

        ColliderDesc = m_pDirColliderCom.lock()->Get_ColliderDesc();

        _float ColliderInfo[4] =
        {
            ColliderDesc.vScale.x,
            ColliderDesc.vTranslation.x,
            ColliderDesc.vTranslation.y,
            ColliderDesc.vTranslation.z
        };

        Out_Json["DirColliderDesc"] = ColliderInfo;
    }
}

void CInteraction_Door::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_fRotationtRadian = In_Json["RotationtRadian"];
    m_fRotationtSpeed  = In_Json["RotationSpeed"];

    if (In_Json.end() != In_Json.find("ActionFlag"))
    {
        m_ActionFlag = In_Json["ActionFlag"];

        if (m_ActionFlag & ACTION_FLAG::KEY)
        {
            CallBack_Requirement += bind(&CInteraction_Door::Requirement_Key, this, placeholders::_1);
            //Callback_ActFail 바인드하기
        }

        else if (m_ActionFlag & ACTION_FLAG::OPEN_DIR)
        {
            CallBack_Requirement += bind(&CInteraction_Door::Requirement_Dir, this, placeholders::_1);
            //Callback_ActFail 바인드하기
        }
    }

    if (In_Json.end() != In_Json.find("KeyID"))
        m_iKeyID = In_Json["KeyID"];

    if (In_Json.end() != In_Json.find("SectionIndex"))
    {
        m_iSectionIndex = In_Json["SectionIndex"];

        if (0 <= m_iSectionIndex)
            GET_SINGLE(CGameManager)->Registration_Section(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
    }

    if (In_Json.end() != In_Json.find("ColliderDesc"))
    {
        _float fColliderDesc[4];

        for (_uint i = 0; i < 4; ++i)
            fColliderDesc[i] = In_Json["ColliderDesc"][i];

        SetUpColliderDesc(m_pColliderCom, fColliderDesc, COLLISION_LAYER::TRIGGER);
    }
    else
    {
        _float fDefaultDesc[4] = { 3.f, 0.f, 1.5f, 0.f };
        SetUpColliderDesc(m_pColliderCom, fDefaultDesc, COLLISION_LAYER::TRIGGER);
    }

    if ((In_Json.end() != In_Json.find("DirColliderDesc")) && (m_ActionFlag & ACTION_FLAG::OPEN_DIR))
    {
        m_pDirColliderCom = Add_Component<CCollider>();
        
        _float fColliderDesc[4];

        for (_uint i = 0; i < 4; ++i)
            fColliderDesc[i] = In_Json["DirColliderDesc"][i];

        SetUpColliderDesc(m_pDirColliderCom, fColliderDesc, COLLISION_LAYER::CHECK_DIR);
    }

    m_fFirstRadian = SMath::Extract_PitchYawRollFromRotationMatrix(SMath::Get_RotationMatrix(m_pTransformCom.lock()->Get_WorldMatrix())).y;

    if ("" == string(m_pModelCom.lock()->Get_ModelKey()))
        m_pModelCom.lock()->Init_Model("Door01_05", "");

#ifdef _GENERATE_PROP_COLLIDER_
    m_pPhysXColliderCom.lock()->Init_ModelCollider(m_pModelCom.lock()->Get_ModelData(), true);
    PhysXColliderDesc tDesc;
    Preset::PhysXColliderDesc::ConvexStaticPropSetting(tDesc, m_pTransformCom);
    m_pPhysXColliderCom.lock()->CreatePhysXActor(tDesc);
    m_pPhysXColliderCom.lock()->Add_PhysXActorAtSceneWithOption();
#endif // _GENERATE_PROP_COLLIDER_
}

void CInteraction_Door::Act_OpenDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fAddRadian += fTimeDelta * m_fRotationtSpeed;

    //_float fDir = (0.f == m_fRotationtSpeed) ? (1.f) : (m_fRotationtSpeed / abs(m_fRotationtSpeed));
    m_pTransformCom.lock()->Rotation
    (
        XMVectorSet(0.f, 1.f, 0.f, 1.f),
        m_fFirstRadian + m_fAddRadian
    );

    if (m_fRotationtRadian <= fabs(m_fAddRadian))
    {
        m_ActionFlag ^= ACTION_FLAG::ROTATION;
        Out_IsEnd     = true;
        m_fAddRadian  = 0.f;

        m_pPhysXColliderCom.lock()->Set_Enable(true);

        Callback_ActEnd();
    }
}

void CInteraction_Door::Act_CloseDoor(_float fTimeDelta, _bool& Out_IsEnd)
{
    m_fAddRadian += fTimeDelta * m_fRotationtSpeed;

    _float fDir = (0.f == m_fRotationtSpeed) ? (1.f) : (m_fRotationtSpeed / abs(m_fRotationtSpeed));
    m_pTransformCom.lock()->Rotation
    (
        XMVectorSet(0.f, 1.f, 0.f, 1.f),
        m_fFirstRadian + (m_fRotationtRadian * fDir) - m_fAddRadian
    );

    if (m_fRotationtRadian <= fabs(m_fAddRadian))
    {
        m_ActionFlag ^= ACTION_FLAG::ROTATION;
        Out_IsEnd     = true;
        m_fAddRadian  = 0.f;

        Callback_ActEnd();
    }
}

void CInteraction_Door::Act_Interaction()//여기서 Use
{
    if (m_ActionFlag & ACTION_FLAG::ROTATION)
    {
        GAMEINSTANCE->Get_GameObjects<CUI_ItemRequirement>(LEVEL_STATIC).front().lock()->Call_UseItem(m_iKeyID);
        Callback_ActUpdate += bind(&CInteraction_Door::Act_OpenDoor, this, placeholders::_1, placeholders::_2);
        m_pPhysXColliderCom.lock()->Set_Enable(false);

        GAMEINSTANCE->PlaySound3D("EVM_Fantasy_Game_Door_Open.ogg", 5.f, m_pTransformCom.lock()->Get_Position());
    }
    else
    {
        Callback_ActUpdate += bind(&CInteraction_Door::Act_CloseDoor, this, placeholders::_1, placeholders::_2);

        GAMEINSTANCE->PlaySound3D("EVM_Fantasy_Game_Door_Close.ogg", 5.f, m_pTransformCom.lock()->Get_Position());
    }
}

void CInteraction_Door::Requirement_Key(_bool& Out_bRequirement)
{
    if (ITEM_NAME::ITEM_NAME_END == m_iKeyID)
    {
        Out_bRequirement = false;

        return;
    }
    
    // 나중에 인벤토리 컴포넌트 찾아서 검색하기
    weak_ptr<CInventory> pInventory = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CInventory>().lock();
    weak_ptr<CItem>      pItem      = pInventory.lock()->Find_Item(m_iKeyID);

    Out_bRequirement = (nullptr != pItem.lock());

    if (!Out_bRequirement)
    {
        GAMEINSTANCE->PlaySound3D("EVM_Fantasy_Game_Item_Wooden_Chest_Open_or_Close.ogg", 5.f, m_pTransformCom.lock()->Get_Position());
        GAMEINSTANCE->Get_GameObjects<CUI_ItemRequirement>(LEVEL_STATIC).front().lock()->Call_ItemRequireMent(m_iKeyID);
    }
}

void CInteraction_Door::Requirement_Dir(_bool& Out_bRequirement)
{
    if (!(m_ActionFlag & ACTION_FLAG::ROTATION))
    {
        Out_bRequirement = true;

        return;
    }

    _uint iCheckIndex = m_pDirColliderCom.lock()->Get_ColliderIndex();

    auto iter_find = find_if(m_CollisionIndex.begin(), m_CollisionIndex.end(), [&](_uint _iIndex)->_bool
    {
        return (iCheckIndex == _iIndex);
    });

    Out_bRequirement = (m_CollisionIndex.end() != iter_find);

    if (!Out_bRequirement)
    {
        GAMEINSTANCE->Get_GameObjects<CUI_ItemRequirement>(LEVEL_STATIC).front().lock()->Call_RequirementDir();
        GAMEINSTANCE->PlaySound3D("EVM_Fantasy_Game_Item_Wooden_Chest_Open_or_Close.ogg", 5.f, m_pTransformCom.lock()->Get_Position());
    }
}

void CInteraction_Door::SetUpColliderDesc(weak_ptr<CCollider> In_pColldierCom, _float* _pColliderDesc, COLLISION_LAYER _eCollLayer)
{
    COLLIDERDESC ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.iLayer       = (_uint)_eCollLayer; // COLLISION_LAYER::TRIGGER;
    ColliderDesc.vScale       = _float3(_pColliderDesc[0], 0.f, 0.f); 
    ColliderDesc.vTranslation = _float3(_pColliderDesc[1], _pColliderDesc[2], _pColliderDesc[3]);

    In_pColldierCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);
    In_pColldierCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CInteraction_Door::OnDestroy()
{
    __super::OnDestroy();

    GET_SINGLE(CGameManager)->Remove_SectionLight(m_iSectionIndex, Weak_Cast<CGameObject>(m_this));
}

void CInteraction_Door::Free()
{
}
