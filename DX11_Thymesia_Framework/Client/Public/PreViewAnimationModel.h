#pragma once
#include "Character.h"

BEGIN(Client)

class CLuxiya;
class CWeapon;
class CMobWeapon;
class CPreViewAnimationModel :
    public CCharacter
{
    GAMECLASS_H(CPreViewAnimationModel)
    SHALLOW_COPY(CPreViewAnimationModel)
    CLONE_H(CPreViewAnimationModel, CGameObject)

public:
    weak_ptr<CModel> Get_CurrentModel();

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Custom_Thread0(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext) override;

    virtual void SetUp_ShaderResource() override;

public:
    void Init_EditPreViewAnimationModel(const string& In_szModelKey);
    void Change_AnimationFromIndex(const _uint& In_iAnimIndex);
    void Play_Animation(_float fTimeDelta);
    void Add_DebugWeapon(const string& In_szBoneName);
    void Set_WeaponDesc(const _float& In_fScale, const _float3& In_vOffset, const _float& In_fDamage, const HIT_TYPE& In_eHitType);
    void Clear_DebugWeapon();
    void Clear_ModelWeapon();

private:
    void Release_BeforeModel();

private:
    unordered_map<string, weak_ptr<CModel>> m_pModelComs;
    weak_ptr<CModel>                        m_pCurrentModelCom;

private:
    list<weak_ptr<CWeapon>>                 m_pDebugWeapons;
    list<weak_ptr<CWeapon>>                 m_pModelWeapons;

private:
    void Free();

};

END