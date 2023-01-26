#pragma once
#include "FMOD/fmod.h"
#include "Base.h"

BEGIN(Engine)

#define MAX_CHANNEL 1024

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

private:
	struct SOUND_DESC
	{
		string		szFileName;
		FMOD_SOUND* pSound{};

		SOUND_DESC(const string& In_szFileName, FMOD_SOUND* In_pSound)
			: szFileName(In_szFileName), pSound(In_pSound)
		{
		}
	};

	struct CHANNEL_DESC
	{
		_bool			is3DSound{};
		_float3			vWorldPosition{};
		FMOD_CHANNEL*	pChannel{};
	};

public:
	void Initialize();
	void Tick();

	void Release();

public:
	void Update_VolumeScale(const _float In_VolumeScale);
	void Update_BGMVolumeScale(const _float In_VolumeScale);

	_int  Pause(CHANNELID eID);
	// Play sound with attenuation rate based on perspective with index.
	_uint PlaySound3D(const string& In_szSoundKey, _uint _iIndex, _float _vol, _fvector In_WorldPosition);
	// Play sound with attenuation rate based on perspective.
	_uint PlaySound3D(const string& In_szSoundKey, _float _vol, _fvector In_WorldPosition);
	// Play sound without perspective. (e.g. environmental sound, UI sound) with index.
	_uint PlaySound2D(const string& In_szSoundKey, _uint _iIndex, _float _vol);
	// Play sound without perspective. (e.g. environmental sound, UI sound)
	_uint PlaySound2D(const string& In_szSoundKey, _float _vol, const _bool isLoop = false);
	void PlayBGM(const string& In_szSoundKey, _float _vol);
	void StopBGM();
	void StopSound(_uint _iChannelIndex);
	void StopAll();
	

private:
	_bool	Is_PlayingChannel(FMOD_CHANNEL* pChannel);

public:
	vector<const string*> Get_AllSoundNames();

public:
	//void LoadSoundFile_Legacy();
	void LoadSoundFile(const string& In_szFile);

private:
	_float m_fEffectvolume = SOUND_DEFAULT;
	_float m_fBGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;



private:
	// 사운드 리소스 정보를 갖는 객체 
	map<_hashcode, SOUND_DESC> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	CHANNEL_DESC m_pChannelArr[MAX_CHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem{};
	_bool		m_bPause = false;

public:
	virtual void OnDestroy() override;
	void Free();
};

END