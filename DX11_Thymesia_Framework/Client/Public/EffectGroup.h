#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Client)

class CCustomEffectMesh;
class CEffect_Point;
class CEffect_Rect;

class CEffectGroup :
    public CGameObject
{
    GAMECLASS_H(CEffectGroup)
    SHALLOW_COPY(CEffectGroup)
    CLONE_H(CEffectGroup, CGameObject)

public:
    const _char* Get_EffectGroupName() { return m_szEffectGroupName.c_str(); }
    void Set_EffectGroupName(const _char* szName);
    _bool   Is_EffectMesh(const _uint& In_iIndex) const;

    _size_t Get_EffectMeshSize() const { return m_pEffectMeshs.size(); }
    weak_ptr<CCustomEffectMesh> Get_EffectMesh(const _uint& In_iIndex) const;
    vector<string> Get_EffectMeshKeys();

    weak_ptr<CEffect_Rect> Get_Particle(const _uint& In_iIndex) const;
    vector<string> Get_EffectParticleKeys();

    void    Set_TimeScaleLayerForEffects(const _uint& In_iTimeScaleLayer);

protected:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    // For Edit
    void Add_EditEffectMesh(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey);
    void Add_EditParticle(const _char* In_szName, const _char* In_szTextureKey);
    void ReBake_EditParticle();

    void Add_EffectMesh(const _uint iLevelIndex);
    void Add_Particle(const _uint iLevelIndex);


    void Remove_EffectMesh(weak_ptr<CCustomEffectMesh> pEffectMesh);
    void Remove_EffectParticle(weak_ptr<CEffect_Rect> pEffectParticle);
    void Remove_AllEffects();

    void Sync_Animation();
    void Play(_float fTimeDelta);
#ifdef _DEBUG
    void Reset_Effects();
#endif // _DEBUG
    void Reset_Effects(weak_ptr<CTransform> pParentTransformCom);

    _bool Use_EffectGroup(weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer = -1);
    void UnUse_EffectGroup();
    weak_ptr<CEffectGroup> Clone_EffectGroup();

    void Load_FromBinary(const string& In_szPath);
    void Write_FromBinary();
    void Load_EffectJson(const string& In_szPath, const _uint& In_iTimeScaleLayer, const _uint& In_iCreatedLevel);
    void Load_EffectJson(const string& In_szPath, json& In_EffectJson, const _uint& In_iTimeScaleLayer, const _uint& In_iCreatedLevel);
    void Write_EffectJson(json& Out_Json);

private:
    //보유하고 있는 이펙트들이 끝났는지 확인
    _bool Check_Finish();

private:
    string                              m_szPath;
    _uint                               m_iCreatedLevel;
    _uint                               m_iTimeScaleLayer;

    string                              m_szEffectGroupName;
    vector<weak_ptr<CCustomEffectMesh>> m_pEffectMeshs;
    vector<weak_ptr<CEffect_Rect>>      m_pEffectParticles;

    _bool                               m_bUsing = false;

public:
    void OnChangeAnimationKey(const _uint& In_Key);

private:
    void Free();


};

END