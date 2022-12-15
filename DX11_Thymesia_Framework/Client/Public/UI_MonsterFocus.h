#pragma once
#include "CustomUI.h"


BEGIN(Engine)
class CBoneNode;
class CModel;
END

BEGIN(Client)

class CMonster;

class CEasingComponent_Alpha;
class CUI_MonsterFocus : public CCustomUI
{
	friend class CPreset_UIDesc;
public:
	GAMECLASS_H(CUI_MonsterFocus)
	CLONE_H(CUI_MonsterFocus, CGameObject)

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Start();
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);

private:
	virtual void			SetUp_Component() override;

public:
	void					Bind_Callback();
	void					Bind_Callback_LockOnTriger(weak_ptr<CMonster> pTargetMonster);

	void					Call_FocusInTarget();
	void					Call_FocusOutTarget();	

	//타겟팅 직후 바인딩
	void					Call_ChangeState(weak_ptr<class CStateBase> pChangedState);
	

protected:
	virtual HRESULT SetUp_ShaderResource() override;

private:
	weak_ptr<class CBoneNode>	FindTargetBone(weak_ptr<class CModel> pTargetModel);
	void					FollowTargetBone();
	_bool					Compare_StateFlag(weak_ptr<CStateBase> pStateBase, STATE_FLAG Checkflag);
		
private:
	void					OnEnable(void* pArg) override;
	
private:
	_float4					m_vColor;
	weak_ptr<CMonster>		m_pTargetMonster;
	weak_ptr<CEasingComponent_Alpha>	m_pEasingAlphaCom;
	weak_ptr<class CBoneNode>		m_pBoneNode;
};

END