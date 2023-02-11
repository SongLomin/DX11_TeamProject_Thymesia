#pragma once
#include "FMOD/fmod.h"
#include "Base.h"

BEGIN(Engine)

#define MAX_CHANNEL 1024
static const _float Sound_Min_Distance = 30.f;
static const _float Sound_Max_Distance = 80.f;


class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

private:
	struct SOUND_DESC
	{
		string		szFileName;
		string		szFilePath;
		FMOD_SOUND* pSound{};

		SOUND_DESC(const string& In_szFileName, const string& In_szFilePath, FMOD_SOUND* In_pSound)
			: szFileName(In_szFileName), szFilePath(In_szFilePath), pSound(In_pSound)
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
	_uint PlaySound3D(const string& In_szSoundKey, _float _vol, _fvector In_WorldPosition, const _float In_fMinDistance = Sound_Min_Distance, const _float In_fMaxDistance = Sound_Max_Distance);
	// Play sound without perspective. (e.g. environmental sound, UI sound) with index.
	_uint PlaySound2D(const string& In_szSoundKey, _uint _iIndex, _float _vol);
	// Play sound without perspective. (e.g. environmental sound, UI sound)
	_uint PlaySound2D(const string& In_szSoundKey, _float _vol, const _bool isLoop = false);
	void PlayBGM(const string& In_szSoundKey, _float _vol, const _float In_fFadeSound = 0.f);
	void StopBGM();
	void StopSound(_uint _iChannelIndex);
	void StopAll();
	

private:
	_bool	Is_PlayingChannel(FMOD_CHANNEL* pChannel);
	void	Update_BGMVolume(_float fTimeDelta);

public:
	vector<const string*> Get_AllSoundNames();

public:
	//void LoadSoundFilesFromPath_Legacy();
	void LoadSoundFilesFromPath(const string& In_szFile);
	void LoadSoundFile(const string& In_szFilePath, string* In_szFile = nullptr);

public: /* Captured_Resource */
	void Write_JsonUsingResource(json& Out_Json);


private:
	_float m_fEffectvolume = SOUND_DEFAULT;
	_float m_fBGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

private: /* BGM Fader */
	_float	m_fStartFadeTime = 0.f;
	_float	m_fCurrentFadeTime = 0.f;
	string	m_szNextBGM;
	_float	m_fPreBGMvolume = 0.f;
	_float	m_fNextBGMvolume = 0.f;

	// 0: Fade out, 1: Fade in, 2: Fade End 
	_ushort	m_FaderState = 0;


private:
	// 사운드 리소스 정보를 갖는 객체 
	unordered_map<_hashcode, SOUND_DESC> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	CHANNEL_DESC m_pChannelArr[MAX_CHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem{};
	_bool		m_bPause = false;

private:
	unordered_map<_hashcode, string*> m_UsedSounds;

public:
	virtual void OnDestroy() override;
	void Free();
};

END