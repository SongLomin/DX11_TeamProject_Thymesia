#pragma once
#include "Attack_Area.h"
//#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CBoneNode;
class CCollider;
class CVIBuffer_Trail;
END

BEGIN(Client)

class CJavelinWeapon :
    public CAttackArea
{
    GAMECLASS_H(CJavelinWeapon);
    CLONE_H(CJavelinWeapon, CGameObject);

public: 
    enum class JAVELIN_STATE
    {
        BIND_HAND,
        THROW,
        STAKE,
        STATE_END
    };

public:
    void  Set_JavelinState(const JAVELIN_STATE In_JavelinState);
    void  Set_RenderCheck(_bool bRenderCheck) { m_bRenderCheck = bRenderCheck; }
    const JAVELIN_STATE Get_JavelinState() const { return m_eCurrentState; }
    _bool Get_RenderCheck() { return m_bRenderCheck; }

protected:// CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

	void SetUp_ShaderResource();

	virtual void OnCollisionEnter(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
	virtual void OnCollisionStay(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;
	virtual void OnCollisionExit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider) override;

public:
    void  Init_Weapon(weak_ptr<CModel> In_pModelCom, weak_ptr<CTransform> In_ParentTransformCom, const string& szTargetNode = "WeaponCase1");
    void  Set_WeaponNum(_int In_WeaponNum) { m_iWeaponNum = In_WeaponNum; }
    _uint Get_WeaponNum() { return m_iWeaponNum; }
    void  Init_Model(const string& strWeaponName, TIMESCALE_LAYER eLayer);
    void  Weapon_BoneChange(weak_ptr<CModel> In_pModelCom, const string& szTargetNode);
   
  
    void  Add_Collider(_fvector In_vOffset, const _float In_fScale, const COLLISION_LAYER In_Layer);
    void  Enable_Weapon();
    void  Disable_Weapon();
    weak_ptr<CCharacter> Get_ParentCharacter();
    void  Set_RenderOnOff(_bool RenderOnOff) { m_bWeaponRenderOnOff = RenderOnOff; }
    void  Set_WeaponDesc(const WEAPON_DESC& In_Weapon);
    void  Set_WeaponDesc(const HIT_TYPE In_eHitType, const _float In_fDamage, const ATTACK_OPTION In_eOptionType = ATTACK_OPTION::OPTION_END);
    void  LookAt_Player();

    void Set_BombTimer();

private:
    void Update_Matrix_Hand();
    void Update_Matrix_Throw(_float fTimeDelta);
    void Update_Matrix_Stake();

public:
    void Activate_ExplosionEffect(weak_ptr<CJavelinWeapon> pJavelinWeapon);
    void Activate_ExplosionEffect();


public:
    FDelegate<weak_ptr<CCollider>> CallBack_Attack;
    FDelegate<weak_ptr<CCollider>> CallBack_AttackOnce;

protected:
    weak_ptr<CModel> m_pCurrentModelCom;
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CModel> m_pStakeModelCom;

    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    
    weak_ptr<CBoneNode> m_pTargetBoneNode;

    weak_ptr<CCharacter> m_pForEffectCharacter; // 가짜 캐릭터

    WEAPON_DESC             m_tWeaponDesc;

    _float4x4				m_WorldMatrix;
    _float4x4               m_TransformationMatrix;
    _float3                 m_vOffset;
    _bool                   m_bWeaponRenderOnOff = true;
    _uint                   m_iNumMeshContainers;
    _uint                   m_iWeaponNum = 0;
    _bool                   m_bRenderCheck = false;
    _bool                   m_bBoneBind = true;

private:
    _float                  m_fBombTimeAcc = 0.f;
    _bool                   m_bBomb = true;

private:
    JAVELIN_STATE           m_eCurrentState = JAVELIN_STATE::STATE_END;
    DECAL_DESC              m_DecalDesc;

private:
    void Free();
};


END