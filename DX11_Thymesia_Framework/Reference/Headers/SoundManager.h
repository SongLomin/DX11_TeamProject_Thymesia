#pragma once
#include "FMOD/fmod.h"
#include "Base.h"

BEGIN(Engine)

#define MAX_CHANNEL 512

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

public:
	void Initialize();
	void Release();

public:
	_int  VolumeUp(CHANNELID eID, _float _vol);
	_int  VolumeDown(CHANNELID eID, _float _vol);
	_int  BGMVolumeUp(_float _vol);
	_int  BGMVolumeDown(_float _vol);
	_int  Pause(CHANNELID eID);
	_uint PlaySound(const string& In_szSoundKey, _uint _iIndex, _float _vol);
	_uint PlaySound(const string& In_szSoundKey, _float _vol);
	void PlayBGM(const string& In_szSoundKey, _float _vol);
	void StopSound(_uint _iChannelIndex);
	void StopAll();

	vector<const string*> Get_AllSoundNames();

public:
	//void LoadSoundFile_Legacy();
	void LoadSoundFile(const string& In_szFile);

private:
	_float m_volume = SOUND_DEFAULT;
	_float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;



private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<_hashcode, SOUND_DESC> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAX_CHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;
	_bool		m_bPause = false;

public:
	virtual void OnDestroy() override;
	void Free();
};

END