#pragma once
#include "NorMonsterStateBase.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class CNorMonState_Run :
	public CNorMonsterStateBase
{
	GAMECLASS_H(CNorMonState_Run);
	CLONE_H(CNorMonState_Run, CComponent)
		SHALLOW_COPY(CNorMonState_Run)

public:
	void  Set_ClosePlayer(_bool ClosePlayer) { m_bClosePlayer = ClosePlayer; }
	void  Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE);
	void  Set_GardnerAtkIndex(_uint GardnerAtkIndex) {	m_iGardnerAtkIndex = GardnerAtkIndex;}
	void  Set_GardnerPlusAtkIndex(_uint GardnerAtkIndex) { m_iGardnerAtkIndex += GardnerAtkIndex; }
	void  Set_RunCheck(_bool RunCheck) { m_bRunCheck = RunCheck; }


protected:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

protected:
	virtual void OnStateStart(const _float& In_fAnimationBlendTime) override;
	virtual void OnStateEnd() override;
	virtual _bool Check_AndChangeNextState() override;


private:
	_float m_fMaxSpeed = 7.f;
	_float m_fCurrentSpeed = 3.6f;
	_float m_fAccel = 0.f;
	_bool  m_bRunCheck = false; // 아이들에서 런으로 들어오고 8방향중 한방향으로 움직이고 다시아이들로가고 다시런가고 그다음에는 공격하게끔짬
	// 8방향 가기전에는 펄스 가고난다음에는 트루로바꿔줌 
	_uint  m_iGardnerAtkIndex = 0;


	//TODO 야매
	_bool m_bClosePlayer = true;
private:
	virtual void OnEventMessage(_uint iArg) override;
	void Call_AnimationEnd(_uint iEndAnimIndex);
protected:
	virtual void OnDestroy() override;
	void Free();

};

END
