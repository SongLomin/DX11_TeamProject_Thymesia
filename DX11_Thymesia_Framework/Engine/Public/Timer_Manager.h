#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer;

class CTimer_Manager final :
	public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)

private:
	typedef struct tag_TimerEvent
	{
		_float	fMaxTime = 0.f;
		_float	fCurTime = 0.f;
		_uint	iEventNum = 0;
		_bool	bLoop = false;
		_bool	bUseTimeScale = false;
		_bool	bAlwaysCall = false;
		weak_ptr<CBase> pInstance;

	}TIMEREVENT;

public:
	void Tick(const _float& _fOriginDeltaTime, const _float& _fTimeScale);

public:
	void Reserve_TimeScaleLayer(_uint iNumLayers);

	HRESULT Add_Timer(_uint eTimer);
	_float Compute_Timer(_uint eTimer);

	HRESULT Add_TimerEvent(_uint _iEventNum, weak_ptr<CBase> _Instance, _float _fTime, _bool _bLoop = false, _bool _bUseTimeScale = false, _bool _bAlwaysCall = false);

	_float Get_TimeScale(_uint In_iIndex);
	void Set_TimeScale(_uint In_iIndex, _float In_fTimeScale);

private:
	map<_uint, shared_ptr<CTimer>>			m_Timers;
	list<tag_TimerEvent>				m_TimerEvents;

	vector<_float>						m_TimeScaleLayers;

private:
	class shared_ptr<CTimer> Find_Timer(_uint eTimer);

protected:
	virtual void Free();

};

END