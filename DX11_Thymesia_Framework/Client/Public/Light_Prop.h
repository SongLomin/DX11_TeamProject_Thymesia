#pragma once
#include "Prop.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CPhysXCollider;

END

BEGIN(Client)

class CLight_Prop final :
    public CProp
{
    GAMECLASS_H(CLight_Prop);
    CLONE_H(CLight_Prop, CGameObject);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    virtual HRESULT Render() override;

private:
	LIGHTDESC   m_tLightDesc;
    _int        m_iLightIndex;
	_float      m_fMinLightRange = 0.f;
	_float      m_fMaxLightRange = 0.f;

private:
    weak_ptr<CPhysXCollider> m_pPhysXColliderCom;

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;
    virtual void OnEventMessage(_uint iArg) override;
    void Free();
};

END