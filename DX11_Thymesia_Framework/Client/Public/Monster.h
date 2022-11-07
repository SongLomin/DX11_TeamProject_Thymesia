#pragma once
#include "Character.h"

BEGIN(Client)

class CMonsterWeapon;

class CMonster :
    public CCharacter
{
    GAMECLASS_H(CMonster);
    CLONE_H(CMonster, CGameObject);

public:
    void Set_TargetCharacter(weak_ptr<CCharacter> In_pCharacter);
    weak_ptr<CCharacter> Get_TargetCharacter() const;
    virtual _float  Get_CamOffset() const { return 0.f; }
    _float  Get_TimeAcc() const { return m_fTimeAcc; }

    void Set_DissolveAmount(const _float& In_fAmount) { m_fDissolveAmount = In_fAmount; }

public:
    // CGameObject��(��) ���� ��ӵ�
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix) override;

    virtual void Respawn_Monster(_fvector In_vPosition);
    virtual void Release_Monster();

protected:
    virtual void SetUp_ShaderResource() override;    

    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;
    //��ü�� ���°� Ȱ��ȭ ���·� ����� ��, ȣ��Ǵ� �̺�Ʈ�Դϴ�.
    virtual void OnEnable(void* _Arg = nullptr) override;
    //��ü�� ���°� ��Ȱ��ȭ ���·� ����� ��, ȣ��Ǵ� �̺�Ʈ�Դϴ�.
    virtual void OnDisable() override;
    
private:
    _float      m_fTimeAcc = 0.f;
    _float      m_fDissolveAmount = 0.f;

protected:
    list<weak_ptr<CMonsterWeapon>> m_pWeapons;
    weak_ptr<CCharacter> m_pTargetCharacter;
    weak_ptr<CTexture>  m_pDissolveTextureCom;

private:
    
    virtual void Free() override;


};

END