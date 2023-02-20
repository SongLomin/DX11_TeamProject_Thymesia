#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
class CRenderer;
class CRigidBody;


END

BEGIN(Client)
class CRequirementBase;

class CRequirementChecker;

class CActor :
    public CGameObject
{
    GAMECLASS_H(CActor)
    CLONE_H(CActor, CGameObject)

public:
    // CGameObject을(를) 통해 상속됨
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual void Before_Render(_float fTimeDelta) override;
    virtual void Thread_PreRender(_float fTimeDelta) override;
    virtual void Thread_PreLateTick(_float fTimeDelta) override;
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
    _vector Get_WorldPosition();

    const std::string Get_KeyEventName()  const { return m_strKeyEventName; }
    void Bind_KeyEvent(const char* szKeyEventName);
    void Unbind_KeyEvent(const char* szKeyEventName);

public:
    void Call_NextAnimationKey(const _uint& In_iKeyIndex);
    weak_ptr<CModel>   Get_Model() { return m_pModelCom; }
    weak_ptr<CRequirementChecker> Get_Requirement(const string& In_szCheckerKey);

protected:
    virtual void SetUp_ShaderResource();

protected:
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    unordered_map<_hashcode, shared_ptr<CRequirementChecker>> m_pRequirementChecker;

    list<pair<string, _uint>>   m_EffectIndexList;

protected:
    RENDERGROUP m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;
    _uint       m_iPassIndex = 0;
    _float3     m_vMoveDir;
    _float      m_fSpeed = 0.4f;

    _bool       m_bRendering = true;

    _float      m_fCullingRange = 5.f;

private:
    string m_strKeyEventName;

public:
    // 전투가 끝나면 호출됩니다. 주로 승리 연출
    virtual void OnBattleEnd() {};

protected:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;
    virtual void OnDestroy() override;

private:
    void Free();

};


END