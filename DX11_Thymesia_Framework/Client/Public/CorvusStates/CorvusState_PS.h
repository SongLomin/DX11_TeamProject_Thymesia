#pragma once
#include  "CorvusStates/CorvusStateBase.h"

BEGIN(Client)
class CCorvusState_PS abstract :
    public CCorvusStateBase
{
	GAMECLASS_H(CCorvusState_PS);

public:
	void Call_AnimationEnd(_uint iEndAnimIndex);
	virtual void Call_NextKeyFrame(const _uint& In_KeyIndex) = 0;

protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual void OnEventMessage(weak_ptr<CBase> pArg) override;
	virtual void OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage) override;
	virtual _bool Check_AndChangeNextState() override;

	void Set_WeaponRender(const _bool bRender);
	void TurnOn_Effect(const std::string& szPlagueWeaponName);
	void TurnOff_Effect(const std::string& szPlagueWeaponName);

	void Free();

protected:
	weak_ptr<CGameObject> m_pTargetObject;
	weak_ptr<CAnimation> m_pThisAnimationCom;
};
END
