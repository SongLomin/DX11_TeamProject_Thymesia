#pragma once
#include "Component.h"

BEGIN(Engine)

class CModel;
class CChannel;
struct ANIMATION_DATA;

class ENGINE_DLL CAnimation :
	public CComponent
{
	GAMECLASS_H(CAnimation);
	CLONE_H(CAnimation, CComponent);

public:
	_uint Get_CurrentChannelKeyIndex() const;
	_uint Get_MaxChannelKeyIndex() const;
	_float Get_TimeAcc() const { return m_fTimeAcc; }
	_float Get_Duration() const { return m_fDuration; }
	_float Get_AbsoluteTimeAcc() const { return m_fTimeAcc / m_fTickPerSecond; }
	_float Get_AbsoluteDuration() const { return m_fDuration / m_fTickPerSecond; }
	const _char* Get_Name() const { return m_szName.c_str(); }

	void Set_TimeAcc(const _float& In_fTimeAcc) { m_fTimeAcc = In_fTimeAcc; }
	void Set_AbsoluteTimeAcc(const _float& In_fTimeAcc) { m_fTimeAcc = In_fTimeAcc * m_fTickPerSecond; }

protected:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Start() override;

public:
	void Init_Animation(weak_ptr<ANIMATION_DATA> pAnimData, weak_ptr<CModel> pModel, _uint iTimeScaleLayer);
	void Update_TransformationMatrices(_float fTimeDelta);
	void Blend_Animation(_float fMaxBlendTime, _float fRatio);
	void Reset_Animaition();
	void Set_StartAnimationKey(_uint iKeyIndex);
	void Set_AnimationSpeed(_float fAnimationSpeed);
	
public:
	FDelegate<const _uint&> CallBack_NextChannelKey;

private:
	weak_ptr<ANIMATION_DATA>	m_pAnimData;
	string						m_szName;
	vector<weak_ptr<CChannel>>	m_Channels;
	weak_ptr<CModel>			m_pModel;

	_float						m_fDuration = 0.f;
	_float						m_fTickPerSecond = 0.f;
	_float						m_fTimeAcc = 0.f;
	_bool						m_isFinished = false;
	_uint						m_iMaxkeyFrame = 0;

	/* For AnimationSpeed */
	_uint						m_iTimeScaleLayer = 0;
	_float						m_fAnimationSpeed = 1.f;

	_int						m_iPreChannelKey = -1;

	GETSET(_float, fAnimRatio)

private:
	virtual void OnDestroy() override;
	void Free();
};

END