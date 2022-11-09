#pragma once
#include "Prop.h"
#include "Client_Defines.h"

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

public:
    virtual void Write_Json(json& Out_Json) override;
    virtual void Load_FromJson(const json& In_Json) override;
    virtual void OnEventMessage(_uint iArg) override;
    void Free();
};

END