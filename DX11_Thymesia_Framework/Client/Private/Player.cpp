#include "stdafx.h"
#include "Player.h"
#include "GameInstance.h"
#include "GameManager.h"
#include "Client_Components.h"
#include "Collider.h"
#include "Monster.h"
#include "PlayerStateBase.h"
#include "Attack_Area.h"

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

    m_pHitColliderCom = Add_Component<CCollider>();

    COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.5f, 0.f, 0.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.x * 0.5f, 0.f);
    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::PLAYER;

    m_pHitColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);


    m_pRigidBodyColliderCom = Add_Component<CCollider>();

    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::PLAYER_RIGIDBODY;

    m_pRigidBodyColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);


    Set_OwnerForMyComponents();

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::PLAYER, Cast<CGameObject>(m_this));

    m_eAttackCollisionLayer = COLLISION_LAYER::PLAYER_ATTACK;

    return S_OK;
}

HRESULT CPlayer::Start()
{

    __super::Start();

#pragma region DummyMonsterSetting

    Search_NearTargetMonster(100.f);

#pragma endregion
    

    

    return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);


    if(m_pCurState.lock())
        m_pCurState.lock()->Tick(fTimeDelta);



    if(m_pHitColliderCom.lock())
        m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    if(m_pRigidBodyColliderCom.lock())
        m_pRigidBodyColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

}

void CPlayer::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_pCurState.lock())
        m_pCurState.lock()->LateTick(fTimeDelta);

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, Weak_Cast<CGameObject>(m_this));

    m_bWarning = false;
}

HRESULT CPlayer::Render()
{
    __super::Render();

    _float	fDissolveAmount = -1.1f;
    m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float));

    return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    CallBack_Bind_SRV(m_pShaderCom, "");

    

    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 1, "g_Bones");
    }

    return S_OK;
}



void CPlayer::Search_NearTargetMonster(_float fTimeDelta)
{
    if (m_fNearSearchDelay > 0.f)
    {
        m_fNearSearchDelay -= fTimeDelta;
        return;
    }

    //가장 가까운 몬스터 탐색
    Forced_SearchNearTargetMonster();
}

void CPlayer::Forced_SearchNearTargetMonster()
{
    m_fNearSearchDelay = 2.f;

    list<weak_ptr<CGameObject>> pMonsters = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::MONSTER);

    if (pMonsters.empty())
    {
        return;
    }

    _vector vMyPosition = Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
    _vector vMonsterPosition;
    weak_ptr<CGameObject> pTargetMonster = pMonsters.front();
    _float fMinDistance = 99999.f;
    _float fDistance = 0.f;

    for (auto& elem : pMonsters)
    {
        if (!elem.lock())
        {
            continue;
        }

        vMonsterPosition = elem.lock()->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION);
        fDistance = XMVector3Length(vMonsterPosition - vMyPosition).m128_f32[0];

        if (fDistance < fMinDistance)
        {
            pTargetMonster = elem;
            fMinDistance = fDistance;
        }
    }

    Set_TargetMonster(Weak_Cast<CMonster>(pTargetMonster));
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

    _vector vLightFlag = { 1.f, 0.f, 0.f, 0.f };

    m_pShaderCom.lock()->Set_RawValue("g_vLightFlag", &vLightFlag, sizeof(_vector));

}

void CPlayer::OnBattleEnd()
{
    __super::OnBattleEnd();
}

void CPlayer::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);

    m_pHitColliderCom.lock()->Set_Enable(true);
    m_pRigidBodyColliderCom.lock()->Set_Enable(true);

    if (m_pHitColliderCom.lock())
        m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());

    if (m_pRigidBodyColliderCom.lock())
        m_pRigidBodyColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
}

void CPlayer::OnDisable()
{
    __super::OnDisable();

    m_pHitColliderCom.lock()->Set_Enable(false);
    m_pRigidBodyColliderCom.lock()->Set_Enable(false);
    Set_TargetMonster(weak_ptr<CMonster>());
}

void CPlayer::Call_WeaponFirstAttack(weak_ptr<CCollider> pOtherCollider)
{
    __super::Call_WeaponFirstAttack(pOtherCollider);

    if (m_pCurState.lock())
    {
        Weak_Cast<CPlayerStateBase>(m_pCurState).lock()->OnWeaponFirstAttack(pOtherCollider);
        m_fNearSearchDelay = 2.f;
    }

}

void CPlayer::Call_WeaponAttack(weak_ptr<CCollider> pOtherCollider)
{
    __super::Call_WeaponAttack(pOtherCollider);

    if (m_pCurState.lock())
    {
        Weak_Cast<CPlayerStateBase>(m_pCurState).lock()->OnWeaponAttack(pOtherCollider);
        m_fNearSearchDelay = 2.f;
    }
}

void CPlayer::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pOtherCollider);

    

}

void CPlayer::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pOtherCollider);

    if ((_uint)COLLISION_LAYER::MONSTER_ATTACK == pOtherCollider.lock()->Get_CollisionLayer())
    {
        weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

        if (pAttackArea.lock())
        {
            if (HIT_TYPE::WARNING == pAttackArea.lock()->Get_HitType())
            {
                m_bWarning = true;
            }
        }
    }

}

void CPlayer::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pOtherCollider);
}

void CPlayer::Free()
{
}
