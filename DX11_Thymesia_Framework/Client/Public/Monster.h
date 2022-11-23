#pragma once
#include "Character.h"

BEGIN(Client)

class CMobWeapon;
class CStatus_Monster;
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
        _uint                m_iAtkCounterGauge;
        //BOSSTYPE             eBossType;
        //TODO 야매에요 밑에꺼 ㅎ 
        _float3            vYame;
    }STATE_LINK_MONSTER_DESC;

public:
    void Set_TargetCharacter(weak_ptr<CCharacter> In_pCharacter);
    weak_ptr<CCharacter> Get_TargetCharacter() const;
    virtual _float  Get_CamOffset() const { return 0.f; }
    _float  Get_TimeAcc() const { return m_fTimeAcc; }

    void Set_DissolveAmount(const _float& In_fAmount) { m_fDissolveAmount = In_fAmount; }

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix) override;

    virtual void Respawn_Monster(_fvector In_vPosition);
    virtual void Release_Monster();

public:
    list<weak_ptr<CMobWeapon>>  Get_Wepons() { return m_pWeapons; }

public:
    virtual void Enable_Weapons(const _bool In_bEnable) override;

protected:
    virtual void SetUp_ShaderResource() override;    

    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;
    //객체의 상태가 활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnEnable(void* _Arg = nullptr) override;
    //객체의 상태가 비활성화 상태로 변경될 때, 호출되는 이벤트입니다.
    virtual void OnDisable() override;
    
private:
    _float      m_fTimeAcc = 0.f;
    _float      m_fDissolveAmount = 0.f;

protected:
    weak_ptr<CPhysXCollider> m_pPhysXColliderCom;


protected:
    list<weak_ptr<CMobWeapon>> m_pWeapons;
    weak_ptr<CStateBase> m_pStandState;

    weak_ptr<CCharacter> m_pTargetCharacter; // = player
    weak_ptr<CTexture>   m_pDissolveTextureCom;
    // MONSTERTYPE       m_eMonType    =     MONSTERTYPE::NMON_END;
    STATE_LINK_MONSTER_DESC      m_tLinkStateDesc;
    MONSTERWEAPONTYPE    m_eMonWeaPonType =     MONSTERWEAPONTYPE::MON_WEAPON_END;
    NORMONSTERIDLETYPE   m_eNorMonIdleType =    NORMONSTERIDLETYPE::IDLEEND;

    weak_ptr<CStatus_Monster>           m_pStatus;

private:
    
    void Free();


};

END