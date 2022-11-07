#pragma once
#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CPlayer;
class CMonster;

class CCamera_Target final : public CCamera
{
	GAMECLASS_H(CCamera_Target);
	CLONE_H(CCamera_Target, CGameObject);

	enum class CAMERA_STATE
	{
		LOOKMONSTER,
		INTERPOLATE,
		LOOKPLAYER,
		STATE_END
	};

public:


public:
	CCamera_Target(const CCamera_Target& rhs);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_Target();
	
	void Add_Shaking(const SHAKE_DIRECTION& In_eState, const _float& In_fPower, const _float& In_fTime);

	void Cinema_LuxiyaUltimate();
	void Cinema_BattleEnd();
	void Cinema_LuxiyaStartBattle();
	void Cinema_BossStartBattle();

protected:
	virtual HRESULT Bind_PipeLine() override;

private:
	void Init_TargetJustPlayer();

	void Bake_TargetCamera(_float fTimeDelta);
	void Bake_TargetWithMonster(_float fTimeDelta);
	void Bake_TargetJustPlayer(_float fTimeDelta);

	void Tick_Shaking(_float fTimeDelta);

	void Interpolate_Camera(_float fTimeDelta);
	void StaticMove_Camera(_float fTimeDelta);

	void Interpolate_Shaking(_float fTimeDelta);

	void Update_Cinema(_float fTimeDelta);

private:
	weak_ptr<CPlayer>		m_pCurrentPlayer;
	weak_ptr<CTransform>	m_pCurrentPlayerTransformCom;

	weak_ptr<CMonster>		m_pLookTargetMonster;
	weak_ptr<CTransform>	m_pLookTargetMonsterTransformCom;

	_float3					m_vTargetPosition;
	_float4					m_vTargetQuaternion;
	
	_float4x4				m_MatResult;

	_float					m_fXAxisRadian = 0.f;
	_float					m_fYAxisRadian = 0.f;
	_float					m_fTargetPlayerCamLerpTime = 0.f;

	_float					m_fCurFreqTime = 0.f;
	_float					m_fDistance = 0.f;

	CAMERA_STATE			m_eCameraState = CAMERA_STATE::STATE_END;
	_bool					m_isMonsterTarget = false;
	_bool					m_bFirst = true;

private: /* For. Shaking */
	_float					m_fLoopShakeTime = 0.05f;
	SHAKE_DIRECTION			m_eCurrentShakeDir = SHAKE_DIRECTION::DIRECTION_END;
	_float					m_fCurrentShakeTime = 0.f;
	_float					m_fMaxShakeTime = 0.f;
	_float					m_fShakePower = 0.f;
	_float					m_bReverseShake = false;
	_float					m_fShakeRatio = 0.f;

	_float3					m_vShakePosition;

private: /* For. Cinema */
	weak_ptr<CTransform>	m_pCinemaTransformCom;
	vector<CINEMA_DESC>*	m_pCurrentCinemaKeys = nullptr;
	_uint					m_iCurrentKeyIndex = 0;
	_float					m_fCurrentTime = 0.f;
	_bool					m_bCinema = false;

	vector<CINEMA_DESC>		m_tLuxiyaUltimateKeys;
	vector<CINEMA_DESC>		m_tLuxiyaBornKeys;
	vector<CINEMA_DESC>		m_tBattleEndCinemaKeys;
	vector<CINEMA_DESC>		m_tBossStartCinemaKeys;

private:
	virtual void OnLevelExit() override;
	virtual void Free() override;
};

END