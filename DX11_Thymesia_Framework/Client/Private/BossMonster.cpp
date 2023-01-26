#include "stdafx.h"
#include "BossMonster.h"
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
#include "Status_Boss.h"
#include "MonsterHPBar_Boss.h"
#include "Collider.h"


GAMECLASS_C(CBossMonster);
CLONE_C(CBossMonster, CGameObject);

void CBossMonster::Set_TargetCharacter(weak_ptr<CCharacter> In_pCharacter)
{
}

weak_ptr<CCharacter> CBossMonster::Get_TargetCharacter() const
{
    return weak_ptr<CCharacter>();
}

HRESULT CBossMonster::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}



HRESULT CBossMonster::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pStatus = Add_Component<CStatus_Boss>(pArg);

    return S_OK;
}

HRESULT CBossMonster::Start()
{
    __super::Start();
    
    Bind_HPBar();

    return S_OK;
}

void CBossMonster::Bind_HPBar()
{
    //UI ÀçÈ°¿ë
    m_pHPBar = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CMonsterHPBar_Boss>(LEVEL_STATIC);

    if (!m_pHPBar.lock())
    {
        m_pHPBar = GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Boss>(LEVEL_STATIC);
    }
    m_pHPBar.lock()->Set_Target(m_this);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, m_pHPBar);
}

void CBossMonster::Release_Monster()
{
    __super::Release_Monster();
     GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::BOSSMONSTER, m_thisToGameObject);
}


weak_ptr<CMonsterHPBar_Boss>  CBossMonster::Get_HPBar()
{
    return Weak_StaticCast<CMonsterHPBar_Boss>(m_pHPBar);
}

void CBossMonster::Tick(_float fTimeDelta)
{

    __super::Tick(fTimeDelta);
}




void CBossMonster::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CBossMonster::Before_Render(_float fTimeDelta)
{

    __super::Before_Render(fTimeDelta);
}

HRESULT CBossMonster::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);


    return S_OK;
}

HRESULT CBossMonster::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
    __super::Render_ShadowDepth(In_LightViewMatrix, In_LightProjMatrix, pDeviceContext);

    return S_OK;
}



void CBossMonster::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    
}

void CBossMonster::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pMyCollider, pOtherCollider);


}

void CBossMonster::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pMyCollider, pOtherCollider);

   
}

void CBossMonster::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CBossMonster::OnEnable(void* _Arg)
{
    __super::OnEnable(_Arg);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BOSSMONSTER, Weak_Cast<CGameObject>(m_this));

}

void CBossMonster::OnDisable()
{
    __super::OnDisable();


}





void CBossMonster::Free()
{
}
