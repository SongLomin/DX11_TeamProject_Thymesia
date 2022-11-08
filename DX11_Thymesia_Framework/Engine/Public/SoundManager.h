#pragma once
#include "FMOD/fmod.h"
#include "Base.h"

BEGIN(Engine)

#define MAX_CHANNEL 512

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

public:
	void Initialize();
	void Release();

public:
	_int  VolumeUp(CHANNELID eID, _float _vol);
	_int  VolumeDown(CHANNELID eID, _float _vol);
	_int  BGMVolumeUp(_float _vol);
	_int  BGMVolumeDown(_float _vol);
	_int  Pause(CHANNELID eID);
	_uint PlaySound(const TCHAR* pSoundKey, _uint _iIndex, _float _vol);
	_uint PlaySound(const TCHAR* pSoundKey, _float _vol);
	void PlayBGM(const TCHAR* pSoundKey, _float _vol);
	void StopSound(_uint _iChannelIndex);
	void StopAll();

private:
	void LoadSoundFile();

private:
	_float m_volume = SOUND_DEFAULT;
	_float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;



private:
	// 사운드 리소스 정보를 갖는 객체 
	map<tstring, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAX_CHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;
	_bool		m_bPause = false;

public:
	virtual void OnDestroy() override;
	void Free();
};

END