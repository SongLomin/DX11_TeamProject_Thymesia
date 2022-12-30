#pragma once
#include "Player.h"

BEGIN(Client)

class CCamera_Target;
class CTalent_Effect;
class CTalent_Sword;
class CPlayerSkill_System;


class CCorvus : 
    public CPlayer
{
    GAMECLASS_H(CCorvus)
    CLONE_H(CCorvus, CGameObject)

public:
    virtual HRESULT Initialize_Prototype()         override;
    virtual HRESULT Initialize(void* pArg)         override;
    virtual HRESULT Start()                        override;
    virtual void Tick(_float fTimeDelta)           override;
    virtual void LateTick(_float fTimeDelta)       override;
    virtual void Thread_PreBeforeRender(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta)  override;
    virtual void Thread_PreLateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

private:
    virtual void OnEventMessage(_uint iArg) override;
    void Debug_KeyInput(_float fTimeDelta);
    void Ready_Weapon();
    void Ready_States();
    void Ready_Skills();


    virtual void SetUp_ShaderResource() override;
    virtual void Move_RootMotion_Internal() override;
    virtual void OnBattleEnd() override;
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;
    void Free();

public:
    void       Test_BindSkill();
    virtual void       Save_ClientComponentData() override;

private:
    void        WriteTalentFromJson(json& Out_Json);
    void        LoadTalentFromJson(const json& In_Json);


private:
    LIGHTDESC   m_LightDesc;


private://Moon SkillSytem RequireMent;
    virtual void SetUp_Requirement();

    //TODO¿”Ω√
private:
    weak_ptr<CTalent_Sword>  m_pSword;
    _uint                    m_iContainerIndex = 0;
    
    weak_ptr<CPlayerSkill_System> m_pSkillSystem;
private:
    weak_ptr<CCamera_Target> m_pCamera;
    weak_ptr<CTransform> m_pCameraTransform;

  
};

END