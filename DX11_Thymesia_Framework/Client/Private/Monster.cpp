#include "stdafx.h"
#include "Monster.h"
#include "Collider.h"
#include "Shader.h"
#include "StateBase.h"
#include "GameObject.h"
#include "GameManager.h"
#include "Model.h"
#include "Status.h"
#include "Player.h"
#include "Texture.h"
#include "Client_Presets.h"
#include "PhysXCollider.h"
#include "Status_Monster.h"
#include "MobWeapon.h"

GAMECLASS_C(CMonster);
CLONE_C(CMonster, CGameObject);

void CMonster::Set_TargetCharacter(weak_ptr<CCharacter> In_pCharacter)
{

}

weak_ptr<CCharacter> CMonster::Get_TargetCharacter() const
{
    return weak_ptr<CCharacter>();
}

HRESULT CMonster::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	__super::Initialize(pArg);

    m_pStatus = Add_Component<CStatus_Monster>(pArg);

    m_pHitColliderCom = Add_Component<CCollider>();

    COLLIDERDESC			ColliderDesc;
    ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));

    ColliderDesc.vScale = _float3(1.8f, 1.f, 1.f);
    ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
    ColliderDesc.vTranslation = _float3(0.f, ColliderDesc.vScale.y * 0.5f, 0.f);
    ColliderDesc.iLayer = (_uint)COLLISION_LAYER::MONSTER;

    m_pHitColliderCom.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

    m_eAttackCollisionLayer = COLLISION_LAYER::MONSTER_ATTACK;

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::MONSTER, Weak_Cast<CGameObject>(m_this));

    m_pDissolveTextureCom = Add_Component<CTexture>();
    m_pDissolveTextureCom.lock()->Use_Texture("Dissolve_1");

	return S_OK;
}

HRESULT CMonster::Start()
{
    __super::Start();


    return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
    m_fTimeAcc += fTimeDelta * GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::MONSTER);

    __super::Tick(fTimeDelta);

    m_pCurState.lock()->Tick(fTimeDelta);
    m_pHitColliderCom.lock()->Update(m_pTransformCom.lock()->Get_WorldMatrix());
    m_pStatus.lock()->Tick(fTimeDelta);


}

void CMonster::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    m_pCurState.lock()->LateTick(fTimeDelta);
    m_pStatus.lock()->LateTick(fTimeDelta);

    GAMEINSTANCE->Add_RenderGroup(RENDERGROUP::RENDER_SHADOWDEPTH, Weak_Cast<CGameObject>(m_this));

    //m_pPhysXColliderCom.lock()->Synchronize_Collider(m_pTransformCom, XMVectorSet(0.f, 1.5f, 0.f, 1.f));
}

void CMonster::Before_Render(_float fTimeDelta)
{
    //m_pPhysXColliderCom.lock()->Synchronize_Transform(m_pTransformCom, XMVectorSet(0.f, -1.5f, 0.f, 1.f));

    __super::Before_Render(fTimeDelta);
}

HRESULT CMonster::Render()
{
    _float3 vDissolveDir = { 0.f,1.f,0.f };

    m_pDissolveTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", 0);


    m_pShaderCom.lock()->Set_RawValue("g_vDissolveDir", &vDissolveDir, sizeof(_float3));
    m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float));
 
    __super::Render();


#ifdef _DEBUG
#endif // _DEBUG

    return S_OK;
}

HRESULT CMonster::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    CallBack_Bind_SRV(m_pShaderCom, "");
    m_pTransformCom.lock()->Set_ShaderResource(m_pShaderCom, "g_WorldMatrix");
    m_pShaderCom.lock()->Set_RawValue("g_ViewMatrix", (void*)&In_LightViewMatrix, sizeof(_float4x4));
    m_pShaderCom.lock()->Set_RawValue("g_ProjMatrix", (void*)&In_LightProjMatrix, sizeof(_float4x4));

    m_pShaderCom.lock()->Set_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float));

    _uint iNumMeshContainers = m_pModelCom.lock()->Get_NumMeshContainers();
    for (_uint i = 0; i < iNumMeshContainers; ++i)
    {
        //m_pModelCom.lock()->Bind_SRV(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);
        /*if (FAILED(m_pModelCom->Bind_SRV(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
            return E_FAIL;*/

            //m_pShaderCom.lock()->Begin(m_iPassIndex);

        m_pModelCom.lock()->Render_AnimModel(i, m_pShaderCom, 1, "g_Bones");
        //m_pModelCom.lock()->Render_Mesh(i);
    }

    return S_OK;
}


void CMonster::Respawn_Monster(_fvector In_vPosition)
{
    //m_pStatus.lock()->Set_FullHP();
    m_pTransformCom.lock()->Set_State(CTransform::STATE_TRANSLATION, In_vPosition);
    
    m_pTransformCom.lock()->LookAt2D(
        GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()
        ->Get_Component<CTransform>().lock()->Get_State(CTransform::STATE_TRANSLATION));

    GET_SINGLE(CGameManager)->Use_EffectGroup("MonsterSpawn", m_pTransformCom);
    m_fTimeAcc = 0.f;
    m_fDissolveAmount = 0.f;
    Set_Enable(true);
}

void CMonster::Release_Monster()
{
    GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::MONSTER, Weak_Cast<CGameObject>(m_this));
    m_pHitColliderCom.lock()->Set_Enable(false);
}

void CMonster::Enable_Weapons(const _bool In_bEnable)
{
    for (auto& elem : m_pWeapons)
    {
        In_bEnable ?
            elem.lock()->Enable_Weapon() :
            elem.lock()->Disable_Weapon();
    }
}

void  CMonster::Write_Json(json& Out_Json)
{
    __super::Write_Json(Out_Json);

    Out_Json["MonsterDesc"]["MonsterType"]      = m_tLinkStateDesc.eMonType;
    Out_Json["MonsterDesc"]["IdleType_Monster"] = m_tLinkStateDesc.eNorMonIdleType;
    Out_Json["MonsterDesc"]["IdleType_Boss"]    = m_tLinkStateDesc.eBossStartType;
}

void  CMonster::Load_FromJson(const json& In_Json)
{
    __super::Load_FromJson(In_Json);

    m_tLinkStateDesc.Reset();

    m_tLinkStateDesc.eMonType           = In_Json["MonsterDesc"]["MonsterType"];
    m_tLinkStateDesc.eNorMonIdleType    = In_Json["MonsterDesc"]["IdleType_Monster"];
    m_tLinkStateDesc.eBossStartType     = In_Json["MonsterDesc"]["IdleType_Boss"];
    XMStoreFloat4(&m_tLinkStateDesc.m_fStartPositon, m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION));

    Init_Desc();
}



void CMonster::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    _vector vShaderFlag = { 0.f, 0.f, 0.f, 0.f };

    m_pShaderCom.lock()->Set_RawValue("g_vShaderFlag", &vShaderFlag, sizeof(_vector));


    /*const LIGHTDESC& LightDesc = GAMEINSTANCE->Get_LightDesc(2);

    m_pShaderCom.lock()->Set_RawValue("g_vCamPosition", &GAMEINSTANCE->Get_CamPosition(), sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightDir", (void*)&LightDesc.vDirection, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightPos", (void*)&LightDesc.vPosition, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_fRange", (void*)&LightDesc.fRange, sizeof(_float));

    m_pShaderCom.lock()->Set_RawValue("g_vLightDiffuse", (void*)&LightDesc.vDiffuse, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightAmbient", (void*)&LightDesc.vAmbient, sizeof(_float4));
    m_pShaderCom.lock()->Set_RawValue("g_vLightSpecular", (void*)&LightDesc.vSpecular, sizeof(_float4));*/
}

void CMonster::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pMyCollider, pOtherCollider);


}

void CMonster::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pMyCollider, pOtherCollider);

}

void CMonster::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pMyCollider, pOtherCollider);


}

void CMonster::OnEnable(void* _Arg)
{
    __super::OnEnable(_Arg);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::MONSTER, Weak_Cast<CGameObject>(m_this));
    m_pHitColliderCom.lock()->Set_Enable(true);
    
}

void CMonster::OnDisable()
{
    __super::OnDisable();

    
}

void CMonster::Free()
{
}
