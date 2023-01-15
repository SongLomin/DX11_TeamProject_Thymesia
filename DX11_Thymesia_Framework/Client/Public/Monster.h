#pragma once
#include "Character.h"

BEGIN(Client)

class CActorDecor;
class CMobWeapon;
class CJavelinWeapon;
class CStatus_Monster;
class CMonsterHPBar_Base;
#define INIT_STATE(STATE) Weak_StaticCast<STATE>(m_pComponents.find(typeid(STATE).hash_code())->second.front()).lock()->Init_Desc(&m_tLinkStateDesc);

class CMonster :
    public CCharacter
{
    GAMECLASS_H(CMonster);
    CLONE_H(CMonster, CGameObject);

public:
    typedef struct tagMonsterStateLinkDesc
    {
        MONSTERTYPE          eMonType;
        NORMONSTERIDLETYPE   eNorMonIdleType;
        _float4              m_fStartPositon;
        BOSSSTARTTYPE        eBossStartType;
        _int                 iSectionIndex;
        _bool                bPatrol;

        void Reset()
        {
            ZeroMemory(this, sizeof(tagMonsterStateLinkDesc));
        }

    } STATE_LINK_MONSTER_DESC;

public:
    void  Set_TargetCharacter(weak_ptr<CCharacter> In_pCharacter);
    void  Set_LinkStateDesc(const STATE_LINK_MONSTER_DESC& In_tDesc) { m_tLinkStateDesc = In_tDesc; };
    void  Set_BossExecutionStartOnOff(_bool BossExecutionStartOnOff) { m_bBossExecutionStartOnOff = BossExecutionStartOnOff; }
    _bool Get_BossExecutionStartOnOff() { return m_bBossExecutionStartOnOff; }
    _bool Get_EliteExecutionStartOnOff() { return m_bEliteExecutionStartOnOff; }
    void  Set_EliteExecutionStartOnOff(_bool EliteExecutionStartOnOff) { m_bBossExecutionStartOnOff = EliteExecutionStartOnOff; }
    _bool Get_ArmorMonStunCheck() { return m_bArmorMonStunCheck; }
    void  Set_ArmorMonStunCheck(_bool ArmorMonStunCheck) { m_bArmorMonStunCheck = ArmorMonStunCheck; }
    weak_ptr<CCharacter> Get_TargetCharacter() const;
    virtual _float  Get_CamOffset() const { return 0.f; }
    _float  Get_TimeAcc() const { return m_fTimeAcc; }

    MONSTERTYPE Get_MonsterType();

    weak_ptr<CMonsterHPBar_Base>    Get_HPBar() { return m_pHPBar; }

    const STATE_LINK_MONSTER_DESC& Get_LinkStateDesc() { return m_tLinkStateDesc; }


    void Set_LightDesc(const LIGHTDESC& In_LightDesc)
    {
        m_LightDesc = In_LightDesc;
    }
    LIGHTDESC Get_LightDesc()
    {
        return m_LightDesc;
    }

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext) override;

    virtual void Respawn_Monster(_fvector In_vPosition);
    virtual void Release_Monster();

public:
    list<weak_ptr<CMobWeapon>>  Get_Weapons() { return m_pWeapons; }
 

public:
    virtual void Enable_Weapons(const _bool In_bEnable) override;

    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;

    virtual void Init_Desc();
    virtual void OnEventMessage(_uint iArg)override;

protected:
    virtual void SetUp_ShaderResource() override;    

    virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

    virtual void OnEnable(void* _Arg = nullptr) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

    virtual void CollsionContent(_float fScale = 1.8f);

protected://From Moon
    virtual void    Bind_HPBar();


private:
    _float      m_fTimeAcc = 0.f;
protected:
    _bool       m_bBossExecutionStartOnOff = false;
    _bool       m_bEliteExecutionStartOnOff = false;
    _bool       m_bArmorMonStunCheck = false;

protected:
    weak_ptr<CPhysXCollider> m_pPhysXColliderCom;

protected:
    list<weak_ptr<CMobWeapon>> m_pWeapons;
    list<weak_ptr<CActorDecor>> m_pActorDecor;
    weak_ptr<CStateBase> m_pStandState;

    weak_ptr<CCharacter> m_pTargetCharacter; // = player
    weak_ptr<CTexture>   m_pDissolveTextureCom;
    STATE_LINK_MONSTER_DESC      m_tLinkStateDesc;
    MONSTERWEAPONTYPE    m_eMonWeaPonType =     MONSTERWEAPONTYPE::MON_WEAPON_END;
    NORMONSTERIDLETYPE   m_eNorMonIdleType =    NORMONSTERIDLETYPE::IDLEEND;
    BOSSSTARTTYPE        m_eBossStartType = BOSSSTARTTYPE::BOSSSTARTEND;
    weak_ptr<CMonsterHPBar_Base>   m_pHPBar;
    

protected:
    LIGHTDESC   m_LightDesc;

private:
    
    void Free();


};

END