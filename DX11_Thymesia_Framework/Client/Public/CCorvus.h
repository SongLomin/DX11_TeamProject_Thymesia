#pragma once
#include "Player.h"

BEGIN(Client)

class CCamera_Target;

class CCorvus : 
    public CPlayer
{
    GAMECLASS_H(CCorvus);
    CLONE_H(CCorvus, CGameObject);


//TODO : UI테스트용 임시 구조체입니다. 나중에 수민형이랑 연계하면 
//TODO : 지워라 문광현 지워라 문광현 지워라 문광현 지워라 문광현 지워라 문광현 지워라 문광현
public:
    enum T_PotionType{POTION_DEFAULT, POTION_BUFF, POTION_IMMEDIATE, POTION_END};
    typedef struct tagTestPlayerDesc
    {
        _float      fAtkDmg;
        _float      fClowDmg;
        
        _float      fMaxHp;
        _float      fCurrentHp;

        _float      fMaxMp;
        _float      fCurrentMp;
        
        _uint       iMaxPotion;
        _uint       iCurrentPotion;
        T_PotionType ePotionType;
        
        _uint       iFeatherMax;
        _uint       iFeatherCurrent;

    }T_PLAYER_DESC;
    
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual HRESULT Render() override;
    virtual void SetUp_ShaderResource() override;

private:
    weak_ptr<CCamera_Target> m_pCamera;
    weak_ptr<CTransform> m_pCameraTransform;

protected:
    virtual void OnCollisionEnter(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionStay(weak_ptr<CCollider> pOtherCollider) override;
    virtual void OnCollisionExit(weak_ptr<CCollider> pOtherCollider) override;

private: // called every Tick
    void RootMove();

private:
    virtual void OnBattleEnd() override;
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

    void Free();

};

END