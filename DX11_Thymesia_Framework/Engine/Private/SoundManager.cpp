#include "SoundManager.h"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"
#include "corecrt_io.h"
#include <tchar.h>
#include "Optimization_Manager.h"
#include "GameInstance.h"
#include "SMath.h"

IMPLEMENT_SINGLETON(CSound_Manager)

void CSound_Manager::Initialize()
{
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, MAX_CHANNEL, FMOD_INIT_NORMAL, NULL);

	//FMOD_System_CreateSound(m_pSystem, "example.mp3", FMOD_3D, 0, &m_pSound);
	
}


void CSound_Manager::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second.pSound);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

_int CSound_Manager::VolumeUp(CHANNELID eID, _float _vol)
{
	if (m_volume < SOUND_MAX) {
		m_volume += _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);

	return 0;
}

_int CSound_Manager::VolumeDown(CHANNELID eID, _float _vol)
{
	if (m_volume > SOUND_MIN) {
		m_volume -= _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_volume);

	return 0;
}

_int CSound_Manager::BGMVolumeUp(_float _vol)
{
	if (m_BGMvolume < SOUND_MAX) {
		m_BGMvolume += _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);

	return 0;
}

void CSound_Manager::Tick()
{
	_matrix CameraWorldMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD);

 	FMOD_VECTOR vListenerPosition = SMath::Convert_FMOD_VECTOR(CameraWorldMatrix.r[3]);
	FMOD_VECTOR vVelocity{};
 	FMOD_VECTOR vListenerLook = SMath::Convert_FMOD_VECTOR(CameraWorldMatrix.r[2]);
 	FMOD_VECTOR vListenerUp = SMath::Convert_FMOD_VECTOR(CameraWorldMatrix.r[1]);

	FMOD_System_Set3DListenerAttributes(m_pSystem, 0, &vListenerPosition, &vVelocity, &vListenerLook, &vListenerUp);
	FMOD_System_Update(m_pSystem);
}

_int CSound_Manager::BGMVolumeDown(_float _vol)
{
	if (m_BGMvolume > SOUND_MIN) {
		m_BGMvolume -= _vol;
	}

	FMOD_Channel_SetVolume(m_pChannelArr[BGM], m_BGMvolume);

	return 0;
}

_int CSound_Manager::Pause(CHANNELID eID)
{
	m_bPause = !m_bPause;
	FMOD_Channel_SetPaused(m_pChannelArr[eID], m_bPause);

	return 0;
}




_uint CSound_Manager::PlaySound3D(const string& In_szSoundKey, _uint _iIndex, _float _vol, _fvector In_WorldPosition)
{
	map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);
	

	if (iter == m_mapSound.end())
		return -1;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[_iIndex], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[_iIndex]);
		if (_vol >= SOUND_MAX)
			_vol = 1.f;
		else if (_vol <= SOUND_MIN)
			_vol = 0.f;

		FMOD_VECTOR vPosition = SMath::Convert_FMOD_VECTOR(In_WorldPosition);
		FMOD_VECTOR vVelocity{};

		FMOD_Channel_Set3DAttributes(m_pChannelArr[_iIndex], &vPosition, &vVelocity);
		FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[_iIndex], 1.0f, 100.0f);
		FMOD_Channel_SetVolume(m_pChannelArr[_iIndex], _vol);

		if ((_uint)ENVIRONMENT == _iIndex)
		{
			FMOD_Channel_SetMode(m_pChannelArr[ENVIRONMENT], FMOD_LOOP_NORMAL);
		}
		
		FMOD_System_Update(m_pSystem);
		return _iIndex;
	}
	//FMOD_System_Update(m_pSystem);

	return -1;
}

_uint CSound_Manager::PlaySound3D(const string& In_szSoundKey, _float _vol, _fvector In_WorldPosition)
{
	map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;


	/*_vector vCamToWorldPosition = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3] - In_WorldPosition;
	
	_float fDistance = XMVectorGetX(XMVector3Length(vCamToWorldPosition));

	const _float fMaxSoundDistance = 10.f;

	fDistance = min(fMaxSoundDistance, max(0.f, fDistance));

	_float fSoundScale = 1.f - (fDistance / fMaxSoundDistance);

	if (fSoundScale < DBL_EPSILON)
	{
		return -1;
	}*/


	FMOD_BOOL bPlay = FALSE;

	_uint iResult = -1;

	for (_int i(1); i < MAX_CHANNEL; i++)
	{
		FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay);

		if (0 == bPlay)
		{
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[i]);
			if (_vol >= SOUND_MAX)
				_vol = 1.f;
			else if (_vol <= SOUND_MIN)
				_vol = 0.f;

			FMOD_VECTOR vPosition = SMath::Convert_FMOD_VECTOR(In_WorldPosition);
			FMOD_VECTOR vVelocity{};

			FMOD_Channel_Set3DAttributes(m_pChannelArr[i], &vPosition, &vVelocity);
			FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[i], 1.0f, 100.0f);
			FMOD_Channel_SetVolume(m_pChannelArr[i], _vol);
			iResult = i;
			break;
		}

	}

	FMOD_System_Update(m_pSystem);

	return iResult;
}

_uint CSound_Manager::PlaySound2D(const string& In_szSoundKey, _uint _iIndex, _float _vol)
{
	map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[_iIndex], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[_iIndex]);
		if (_vol >= SOUND_MAX)
			_vol = 1.f;
		else if (_vol <= SOUND_MIN)
			_vol = 0.f;

		FMOD_Channel_SetVolume(m_pChannelArr[_iIndex], _vol);

		if ((_uint)ENVIRONMENT == _iIndex)
		{
			FMOD_Channel_SetMode(m_pChannelArr[ENVIRONMENT], FMOD_LOOP_NORMAL);
		}

		FMOD_System_Update(m_pSystem);
		return _iIndex;
	}
	//FMOD_System_Update(m_pSystem);

	return -1;
}

_uint CSound_Manager::PlaySound2D(const string& In_szSoundKey, _float _vol)
{
	map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;


	/*_vector vCamToWorldPosition = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3] - In_WorldPosition;

	_float fDistance = XMVectorGetX(XMVector3Length(vCamToWorldPosition));

	const _float fMaxSoundDistance = 10.f;

	fDistance = min(fMaxSoundDistance, max(0.f, fDistance));

	_float fSoundScale = 1.f - (fDistance / fMaxSoundDistance);

	if (fSoundScale < DBL_EPSILON)
	{
		return -1;
	}*/


	FMOD_BOOL bPlay = FALSE;

	_uint iResult = -1;

	for (_int i(1); i < MAX_CHANNEL; i++)
	{
		FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay);

		if (0 == bPlay)
		{
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[i]);
			if (_vol >= SOUND_MAX)
				_vol = 1.f;
			else if (_vol <= SOUND_MIN)
				_vol = 0.f;

			FMOD_Channel_SetVolume(m_pChannelArr[i], _vol);
			iResult = i;
			break;
		}

	}

	FMOD_System_Update(m_pSystem);

	return iResult;
}

void CSound_Manager::PlayBGM(const string& In_szSoundKey, _float _vol)
{
	map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);

	if (iter == m_mapSound.end())
		return;

	FMOD_Channel_Stop(m_pChannelArr[BGM]);
	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);

	if (_vol >= SOUND_MAX)
		_vol = 1.f;
	else if (_vol <= SOUND_MIN)
		_vol = 0.f;


	m_BGMvolume = _vol;
	FMOD_Channel_SetVolume(m_pChannelArr[BGM], _vol);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::StopSound(_uint _iChannelIndex)
{
	if (_iChannelIndex == -1)
		return;

	FMOD_Channel_Stop(m_pChannelArr[_iChannelIndex]);
}

void CSound_Manager::StopAll()
{
	for (_uint i(0); i < (_uint)MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

//void CSound_Manager::LoadSoundFile_Legacy()
//{
//	_tfinddata64_t fd;
//	__int64 handle = _tfindfirst64(L"../Bin/Sound/*.*", &fd);
//	if (handle == -1 || handle == 0)
//		return;
//
//	_int iResult(0);
//
//	char szCurPath[128] = "../Bin/Sound/";
//	char szFullPath[128] = "";
//	char szFilename[MAX_PATH];
//	while (iResult != -1)
//	{
//		WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFilename, sizeof(szFilename), NULL, NULL);
//		strcpy_s(szFullPath, szCurPath);
//		strcat_s(szFullPath, szFilename);
//		FMOD_SOUND* pSound = nullptr;
//
//		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
//		if (FMOD_OK == eRes)
//		{
//			_int iLength = (_int)strlen(szFilename) + 1;
//
//			TCHAR* pSoundKey = new TCHAR[iLength];
//			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
//			MultiByteToWideChar(CP_ACP, 0, szFilename, iLength, pSoundKey, iLength);
//
//			m_mapSound.emplace(pSoundKey, pSound);
//
//			delete[] pSoundKey;
//		}
//		iResult = _tfindnext64(handle, &fd);
//	}
//	FMOD_System_Update(m_pSystem);
//	_findclose(handle);
//}

void CSound_Manager::LoadSoundFile(const string& In_szFile)
{
	filesystem::path SoundFilePath(In_szFile);

	filesystem::directory_iterator itr(SoundFilePath);

	while (itr != filesystem::end(itr))
	{
		const filesystem::directory_entry& entry = *itr;

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, entry.path().string().c_str(), FMOD_3D, 0, &pSound);

		if (FMOD_OK == eRes)
		{
			FMOD_Sound_Set3DMinMaxDistance(pSound, 1.0f, 100.0f);

			string szFileName = entry.path().filename().string();

			m_mapSound.emplace(hash<string>()(szFileName), SOUND_DESC(szFileName, pSound));
		}

		++itr;
	}

	FMOD_System_Update(m_pSystem);
}

vector<const string*> CSound_Manager::Get_AllSoundNames()
{
	vector<const string*> SoundNames;

	SoundNames.reserve(m_mapSound.size());

	for (auto& elem : m_mapSound)
	{
		SoundNames.push_back(&elem.second.szFileName);
	}

	return SoundNames;
}

void CSound_Manager::OnDestroy()
{
	Release();
}

void CSound_Manager::Free()
{

}