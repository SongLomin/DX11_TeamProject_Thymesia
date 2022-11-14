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
    virtual HRESULT Render() override;
    
public:
    _vector Get_WorldPosition();
 

public:
    void Call_NextAnimationKey(const _uint& In_iKeyIndex);

protected:
    virtual void SetUp_ShaderResource();

protected:
    weak_ptr<CModel> m_pModelCom;
    weak_ptr<CShader> m_pShaderCom;
    weak_ptr<CRenderer> m_pRendererCom;
    weak_ptr<CRigidBody> m_pRigidBodyCom;

    list<pair<string, _uint>>   m_EffectIndexList;

protected:
    RENDERGROUP m_eRenderGroup = RENDERGROUP::RENDER_NONALPHABLEND;
    _uint       m_iPassIndex = 0;
    _float3     m_vMoveDir;
    _float      m_fSpeed = 0.4f;

public:
    // 전투가 끝나면 호출됩니다. 주로 승리 연출
    virtual void OnBattleEnd() {};

protected:
    virtual void OnEnable(void* pArg) override;
    virtual void OnDisable() override;

private:
    void Free();

};


END