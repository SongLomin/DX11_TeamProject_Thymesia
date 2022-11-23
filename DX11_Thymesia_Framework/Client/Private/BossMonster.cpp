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

   

    return S_OK;
}

HRESULT CBossMonster::Start()
{
    __super::Start();


    return S_OK;
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

HRESULT CBossMonster::Render()
{
    __super::Render();


    return S_OK;
}

HRESULT CBossMonster::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix)
{
    __super::Render_ShadowDepth(In_LightViewMatrix, In_LightProjMatrix);

    return S_OK;
}



void CBossMonster::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    
}

void CBossMonster::OnCollisionEnter(weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pOtherCollider);


}

void CBossMonster::OnCollisionStay(weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pOtherCollider);

   
}

void CBossMonster::OnCollisionExit(weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pOtherCollider);


}

void CBossMonster::OnEnable(void* _Arg)
{
    __super::OnEnable(_Arg);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::MONSTER, Weak_Cast<CGameObject>(m_this));
    m_pHitColliderCom.lock()->Set_Enable(true);
    m_pRigidBodyColliderCom.lock()->Set_Enable(true);

}

void CBossMonster::OnDisable()
{
    __super::OnDisable();


}

void CBossMonster::Free()
{
}
