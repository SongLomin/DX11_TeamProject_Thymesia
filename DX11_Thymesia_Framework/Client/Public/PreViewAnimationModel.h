#pragma once
#include "Character.h"

BEGIN(Client)

class CWeapon;
class CMobWeapon;
class CPreviewAnimationModel final :
    public CCharacter
{
    GAMECLASS_H(CPreviewAnimationModel)
    SHALLOW_COPY(CPreviewAnimationModel)
    CLONE_H(CPreviewAnimationModel, CGameObject)

public:
    weak_ptr<CModel> Get_CurrentModel();

protected:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Thread_PreTick(_float fTimeDelta) override;
    virtual void Thread_PreBeforeRender(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
    virtual HRESULT Render_ShadowDepth(_fmatrix In_LightViewMatrix, _fmatrix In_LightProjMatrix, ID3D11DeviceContext* pDeviceContext) override;

    virtual void SetUp_ShaderResource() override;

public:
    void Init_EditPreviewAnimationModel(const string& In_szModelKey);
    void Change_AnimationFromIndex(const _uint& In_iAnimIndex);
    void Play_Animation(_float fTimeDelta);
    void Add_DebugWeapon(const string& In_szBoneName, const _float3& In_vOffset, const _float In_fSize);
    void Remove_DebugWeapon(const string& In_szBoneName);
    void Clear_DebugWeapon();
    void Clear_ModelWeapon();

    void Bind_KeyFrameEvent(const string& In_szKeyFrameEventFileName);

private:
    void Release_BeforeModel();

private:
    _int   m_iContainerIndex = 0;

    unordered_map<_hashcode, list<weak_ptr<CWeapon>>> m_pDebugWeapons;
    list<weak_ptr<CWeapon>>                 m_pModelWeapons;

private: /* For. NvCloth */
    weak_ptr<CBoneNode>                     m_pTargetBoneNode;
    _float4x4                               m_TransformationMatrix;

private:
    void Free();
};

END
