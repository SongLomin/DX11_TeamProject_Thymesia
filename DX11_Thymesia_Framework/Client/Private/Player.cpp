#include "stdafx.h"
#include "Player.h"
#include "GameInstance.h"
#include "GameManager.h"
#include "Client_Components.h"
#include "Collider.h"
#include "Monster.h"
#include "PlayerStateBase.h"
#include "Attack_Area.h"
#include "PhysXCollider.h"
#include "PhysXController.h"
#include "Client_Presets.h"
#include "CorvusStates/Talent_Effect.h"
#include "Inventory.h"

GAMECLASS_C(CPlayer);
CLONE_C(CPlayer, CGameObject);

HRESULT CPlayer::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_thisToPlayer = Weak_StaticCast<CPlayer>(m_this);

    m_pTransformCom.lock()->Add_Position(XMVectorSet(20.f, 5.f, 20.f, 0.f));


    m_pHitColliderCom = Add_Component<CCollider>();


    COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.5f, 0.f, 0.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.x * 0.5f, 0.f);
    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::PLAYER;

    m_pHitColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);


  

    m_pModelCom.lock()->Init_Model("Corvus", "", (_uint)TIMESCALE_LAYER::PLAYER);

    //Preset::PhysXColliderDesc::PlayerBodyTriggerSetting(tPhysxColliderDesc, m_pTransformCom);
    //m_pPhysXTriggerColliderCom = Add_Component<CPhysXCollider>(&tPhysxColliderDesc);
    //m_pPhysXTriggerColliderCom.lock()->Add_PhysXActorAtScene();

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::PLAYER, m_thisToPlayer);
    m_eAttackCollisionLayer = COLLISION_LAYER::PLAYER_ATTACK;

    m_pPhysXControllerCom.lock()->Init_Controller(Preset::PhysXControllerDesc::PlayerSetting(m_pTransformCom),
        (_uint)PHYSX_COLLISION_LAYER::PLAYER);
    //m_pPhysXControllerCom.lock()->Get_Controller()->setPosition

    m_pInventory = Add_Component<CInventory>();

    return S_OK;
}

HRESULT CPlayer::Start()
{
    __super::Start();
    

    return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    //if (m_bIsFocused)
    //    Look_At_Mosnter();

}

void CPlayer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, m_thisToPlayer);
}

HRESULT CPlayer::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);
    _float fDissolveAmount(-1.1f);
    m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float));

    return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
    CallBack_Bind_SRV(m_pShaderCom, "");
    //플레이어 그림자는 위 callback이 안불려서 이전 그림자 렌더했던 월드를 쓰고 있었음

    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");

    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));
     

    _uint iNumMeshContainers(m_pModelCom.lock()->Get_NumMeshContainers());
    for (_uint i(0); i < iNumMeshContainers; ++i)
    {
        if (4 == i || 5 == i || 8 == i || 9 == i || 10 == i || 11 == i || 12 == i || 13 == i)
            continue;

        m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 1, "g_Bones", pDeviceContext);
    }

    return S_OK;
}

void CPlayer::Bind_TalentEffects(weak_ptr<CTalent_Effect> pTalentEffect)
{
    for(auto& iter : m_pTalent_Effects)
    { 
            //이미 같은 탤런트가 들어와있다면 나가리.
            if (iter.lock()->Check_Requirement(m_thisToPlayer) == pTalentEffect.lock()->Check_Requirement(m_thisToPlayer))
            {
                return;
            }
    }
    m_pTalent_Effects.push_back(pTalentEffect);
    pTalentEffect.lock()->Bind_Talent_Effect(m_thisToPlayer);
}

void CPlayer::UnBind_TalentEffects(weak_ptr<CTalent_Effect> pTalentEffect)
{
    for (auto iter = m_pTalent_Effects.begin(); iter != m_pTalent_Effects.end();)
    {
        if ((*iter).lock() == pTalentEffect.lock())
        {
            (*iter).lock()->UnBind_Talent_Effect(m_thisToPlayer);
            m_pTalent_Effects.erase(iter);
            break;
        }
        else
        {
            iter++;
        }
    }
}

_flag CPlayer::Check_RequirementForTalentEffects()
{

    for (auto& elem : m_pTalent_Effects)
    {
        m_iBindedTalentEffectes |= elem.lock()->Check_Requirement(m_thisToPlayer);
    }

    return m_iBindedTalentEffectes;
}


void CPlayer::Focus_Monster(weak_ptr<CGameObject> In_pMonster)
{
    if (!In_pMonster.lock())
        return;

    m_bIsFocused = true;
    m_pMonster = In_pMonster;
    m_pMonsterTransform = In_pMonster.lock()->Get_Component<CTransform>();
}

void CPlayer::Release_Focus()
{
    m_bIsFocused = false;
    m_pMonster = weak_ptr<CGameObject>();
    m_pMonsterTransform = weak_ptr<CTransform>();
}


void CPlayer::Set_TargetMonster(weak_ptr<CMonster> In_pMonster)
{
    m_pTargetMonster = In_pMonster;
}

weak_ptr<CMonster> CPlayer::Get_TargetMonster() const
{
    return m_pTargetMonster;
}

void CPlayer::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    _vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

    m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));

   
}

void CPlayer::SetUp_Requirement()
{
}

void CPlayer::OnBattleEnd()
{
    __super::OnBattleEnd();
}

void CPlayer::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    auto iter = Out_Json["Component"].find("Model");
    Out_Json["Component"].erase(iter);
}

void CPlayer::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    _float4x4 WorldMatrix;
    ZeroMemory(&WorldMatrix, sizeof(_float4x4));
    CJson_Utility::Load_JsonFloat4x4(In_Json["Component"]["Transform"], WorldMatrix);

    PxControllerFilters Filters;
    m_pPhysXControllerCom.lock()->Set_Position
    (
        XMVectorSet(WorldMatrix._41, WorldMatrix._42, WorldMatrix._43, 1.f),
        0.f,
        Filters
    );
}

void CPlayer::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    m_pHitColliderCom.lock()->Set_Enable(true);

    if (m_pHitColliderCom.lock())
        m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

}

void CPlayer::OnDisable()
{
    __super::OnDisable();

    m_pHitColliderCom.lock()->Set_Enable(false);

    Set_TargetMonster(weak_ptr<CMonster>());
}

void CPlayer::Call_WeaponFirstAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::Call_WeaponFirstAttack(pMyCollider, pOtherCollider);

    if (m_pCurState.lock())
    {
        Weak_Cast<CPlayerStateBase>(m_pCurState).lock()->OnWeaponFirstAttack(pMyCollider, pOtherCollider);
        m_fNearSearchDelay = 2.f;
    }

}

void CPlayer::Call_WeaponAttack(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::Call_WeaponAttack(pMyCollider, pOtherCollider);

    if (m_pCurState.lock())
    {
        Weak_Cast<CPlayerStateBase>(m_pCurState).lock()->OnWeaponAttack(pMyCollider, pOtherCollider);
        m_fNearSearchDelay = 2.f;
    }
}

void CPlayer::Set_CollisionObjectFlag(const _flag In_CollisionObjectFlags, const _bool In_bEnable)
{
    if (In_bEnable)
    {
        m_CollisionObjectFlags |= In_CollisionObjectFlags;
    }
    else
    {
        m_CollisionObjectFlags &= ~In_CollisionObjectFlags;
    }
}


void CPlayer::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pMyCollider, pOtherCollider);

    

}

void CPlayer::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CPlayer::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pMyCollider, pOtherCollider);
}

void CPlayer::Free()
{
}
