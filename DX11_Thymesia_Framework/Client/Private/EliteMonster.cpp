#include "stdafx.h"
#include "EliteMonster.h"
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
#include "MonsterHPBar_Base.h"
#include "MonsterHPBar_Elite.h"

GAMECLASS_C(CEliteMonster);
CLONE_C(CEliteMonster, CGameObject);

void CEliteMonster::Set_TargetCharacter(weak_ptr<CCharacter> In_pCharacter)
{
}

weak_ptr<CCharacter> CEliteMonster::Get_TargetCharacter() const
{
    return weak_ptr<CCharacter>();
}

HRESULT CEliteMonster::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CEliteMonster::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    //m_pStatus = Add_Component<CStatus_Boss>(pArg);

    LIGHTDESC LightDesc;

    LightDesc.eActorType = LIGHTDESC::TYPE_SPOTLIGHT;
    LightDesc.vDiffuse = { 1.f,0.95f,0.8f,1.f };
    LightDesc.vSpecular = { 1.f,0.95f,0.8f,1.f };
    LightDesc.vAmbient = { 1.f,0.95f,0.8f,1.f };
    LightDesc.fIntensity = 0.4f;
    LightDesc.fRange = 3.f;
    LightDesc.fCutOff = cosf(XMConvertToRadians(30.f));
    LightDesc.fOuterCutOff = cosf(XMConvertToRadians(40.f));
    _vector vPlayerPos = m_pTransformCom.lock()->Get_Position();
    _vector vPlayerLook = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);

    _vector vLightPos = vPlayerPos + XMVectorSet(0.f, 1.5f, 0.f, 0.f);
    _vector vLightLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);
    XMStoreFloat4(&LightDesc.vPosition, vLightPos);
    XMStoreFloat4(&LightDesc.vDirection, vLightLook);

    LightDesc.bEnable = true;

    m_LightDesc = GAMEINSTANCE->Add_Light(LightDesc);

    return S_OK;
}

HRESULT CEliteMonster::Start()
{
    __super::Start();

    Bind_HPBar();


    //
// m_pHPBar = GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Boss>(LEVEL_STATIC);
// m_pHPBar.lock()->Set_Owner(Weak_Cast<CMonster>(m_this));
//
// GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, m_pHPBar);


    return S_OK;
}



void CEliteMonster::Tick(_float fTimeDelta)
{

    __super::Tick(fTimeDelta);

    _vector vLightPos = m_pTransformCom.lock()->Get_Position() + XMVectorSet(0.f, 1.5f, 0.f, 0.f);

    XMStoreFloat4(&m_LightDesc.vPosition, vLightPos);

    GAMEINSTANCE->Set_LightDesc(m_LightDesc);
}


void CEliteMonster::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CEliteMonster::Before_Render(_float fTimeDelta)
{

    __super::Before_Render(fTimeDelta);
}

HRESULT CEliteMonster::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);


    return S_OK;
}

HRESULT CEliteMonster::Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext)
{
    __super::Render_ShadowDepth(In_LightViewMatrix, In_LightProjMatrix, pDeviceContext);

    return S_OK;
}

void CEliteMonster::Bind_HPBar()
{
    //UI 재활용
    m_pHPBar = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CMonsterHPBar_Elite>(LEVEL_STATIC);
    if (!m_pHPBar.lock())
    {
        m_pHPBar = GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Elite>(LEVEL_STATIC);
    }
    m_pHPBar.lock()->Set_Target(m_this);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, m_pHPBar);
}

void CEliteMonster::Release_Monster()
{
    __super::Release_Monster();
    GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::ELITEMONSTER, m_thisToGameObject);
}


//void   CEliteMonster::Bind_HPBar()
//{
//    //UI 재활용
//    //m_pHPBar = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CMonsterHPBar_Elite>(LEVEL_STATIC);
//
//    //if (!m_pHPBar.lock())
//    //    m_pHPBar = GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Elite>(LEVEL_STATIC);
//
//    //m_pHPBar.lock()->Set_Target(m_this);
//
//    //GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, m_pHPBar);
//
//    //m_pHPBar.lock()->Set_Enable(false);
//}


void CEliteMonster::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();


}

void CEliteMonster::OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionEnter(pMyCollider, pOtherCollider);


}

void CEliteMonster::OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionStay(pMyCollider, pOtherCollider);


}

void CEliteMonster::OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider)
{
    __super::OnCollisionExit(pMyCollider, pOtherCollider);

}

void CEliteMonster::OnEnable(void* _Arg)
{
    __super::OnEnable(_Arg);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::ELITEMONSTER, Weak_Cast<CGameObject>(m_this));


}

void CEliteMonster::OnDisable()
{
    __super::OnDisable();


}


void CEliteMonster::Free()
{
}
