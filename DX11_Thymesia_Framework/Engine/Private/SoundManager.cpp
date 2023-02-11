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

void CSound_Manager::Tick()
{
	_matrix CameraWorldMatrix = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD);

	FMOD_VECTOR vListenerPosition = SMath::Convert_FMOD_VECTOR(CameraWorldMatrix.r[3]);
	FMOD_VECTOR vVelocity{};
	FMOD_VECTOR vListenerLook = SMath::Convert_FMOD_VECTOR(CameraWorldMatrix.r[2]);
	FMOD_VECTOR vListenerUp = SMath::Convert_FMOD_VECTOR(CameraWorldMatrix.r[1]);


	FMOD_System_Set3DListenerAttributes(m_pSystem, 0, &vListenerPosition, &vVelocity, &vListenerLook, &vListenerUp);


	for (_uint i = 0; i < MAX_CHANNEL; ++i)
	{
		if (!m_pChannelArr[i].is3DSound)
		{
			/*_vector ChannelPostion = XMLoadFloat3(&m_pChannelArr[i].vWorldPosition);
			ChannelPostion -= CameraWorldMatrix.r[3];
			FMOD_VECTOR vChannelPositionToFMOD = SMath::Convert_FMOD_VECTOR(ChannelPostion);
			FMOD_Channel_Set3DAttributes(m_pChannelArr[i].pChannel, &vChannelPositionToFMOD, &vVelocity);*/

			FMOD_Channel_Set3DAttributes(m_pChannelArr[i].pChannel, &vListenerPosition, &vVelocity);
		}
	}

	if (m_FaderState != 2)
	{
		Update_BGMVolume(GAMEINSTANCE->Get_DeltaTime());
	}


	FMOD_System_Update(m_pSystem);


}



void CSound_Manager::Update_VolumeScale(const _float In_VolumeScale)
{
	_float CurrentVolume;

	for (_uint i = 0; i < MAX_CHANNEL; ++i)
	{
		if (!m_pChannelArr[i].pChannel)
			continue;

		if (BGM == i)
		{
			continue;
		}

		FMOD_Channel_GetVolume(m_pChannelArr[i].pChannel, &CurrentVolume);
		CurrentVolume /= m_fEffectvolume;
		FMOD_Channel_SetVolume(m_pChannelArr[i].pChannel, CurrentVolume * In_VolumeScale);
	}

	m_fEffectvolume = In_VolumeScale;
}

void CSound_Manager::Update_BGMVolumeScale(const _float In_VolumeScale)
{
	if (!m_pChannelArr[BGM].pChannel)
		return;

	_float CurrentVolume;

	FMOD_Channel_GetVolume(m_pChannelArr[BGM].pChannel, &CurrentVolume);
	CurrentVolume /= m_fBGMvolume;
	// FMOD_Channel_SetVolume(m_pChannelArr[BGM].pChannel, CurrentVolume * In_VolumeScale);
	//m_fBGMvolume = In_VolumeScale;
	FMOD_Channel_SetVolume(m_pChannelArr[BGM].pChannel, 0.f);
	m_fBGMvolume = 0.f;
}

_int CSound_Manager::Pause(CHANNELID eID)
{
	m_bPause = !m_bPause;
	FMOD_Channel_SetPaused(m_pChannelArr[eID].pChannel, m_bPause);

	return 0;
}




_uint CSound_Manager::PlaySound3D(const string& In_szSoundKey, _uint _iIndex, _float _vol, _fvector In_WorldPosition)
{
	unordered_map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;

	m_UsedSounds[HashFromKey] = &iter->second.szFilePath;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[_iIndex].pChannel);

	if (_vol <= 0.f)
		_vol = 0.f;

	m_pChannelArr[_iIndex].is3DSound = true;
	XMStoreFloat3(&m_pChannelArr[_iIndex].vWorldPosition, In_WorldPosition);

	FMOD_VECTOR vPosition = SMath::Convert_FMOD_VECTOR(In_WorldPosition);
	FMOD_VECTOR vVelocity{};

	FMOD_Channel_Set3DAttributes(m_pChannelArr[_iIndex].pChannel, &vPosition, &vVelocity);
	FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[_iIndex].pChannel, Sound_Min_Distance, Sound_Max_Distance);
	FMOD_Channel_SetVolume(m_pChannelArr[_iIndex].pChannel, _vol * m_fEffectvolume * 0.25f);

	return _iIndex;
}

_uint CSound_Manager::PlaySound3D(const string& In_szSoundKey, _float _vol, _fvector In_WorldPosition, const _float In_fMinDistance, const _float In_fMaxDistance)
{
	unordered_map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;

	m_UsedSounds[HashFromKey] = &iter->second.szFilePath;

	/*_vector vCamToWorldPosition = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3] - In_WorldPosition;

	_float fDistance = XMVectorGetX(XMVector3Length(vCamToWorldPosition));

	const _float fMaxSoundDistance = 10.f;

	fDistance = min(fMaxSoundDistance, max(0.f, fDistance));

	_float fSoundScale = 1.f - (fDistance / fMaxSoundDistance);

	if (fSoundScale < DBL_EPSILON)
	{
		return -1;
	}*/


	_uint iResult = -1;

	for (_int i(1); i < MAX_CHANNEL; i++)
	{

		if (!Is_PlayingChannel(m_pChannelArr[i].pChannel))
		{
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[i].pChannel);

			if (_vol <= 0.f)
				_vol = 0.f;

			m_pChannelArr[i].is3DSound = true;
			XMStoreFloat3(&m_pChannelArr[i].vWorldPosition, In_WorldPosition);

			FMOD_VECTOR vPosition = SMath::Convert_FMOD_VECTOR(In_WorldPosition);
			FMOD_VECTOR vVelocity{};

			FMOD_Channel_Set3DAttributes(m_pChannelArr[i].pChannel, &vPosition, &vVelocity);
			FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[i].pChannel, In_fMinDistance, In_fMaxDistance);
			FMOD_Channel_SetVolume(m_pChannelArr[i].pChannel, _vol * m_fEffectvolume * 0.25f);
			iResult = i;
			break;
		}

	}


	return iResult;
}

_uint CSound_Manager::PlaySound2D(const string& In_szSoundKey, _uint _iIndex, _float _vol)
{
	unordered_map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;

	m_UsedSounds[HashFromKey] = &iter->second.szFilePath;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[_iIndex].pChannel);

	if (_vol <= 0.f)
		_vol = 0.f;

	m_pChannelArr[_iIndex].is3DSound = false;

	FMOD_Channel_SetVolume(m_pChannelArr[_iIndex].pChannel, _vol * m_fEffectvolume * 0.25f);
	FMOD_System_Update(m_pSystem);
	return _iIndex;
}

_uint CSound_Manager::PlaySound2D(const string& In_szSoundKey, _float _vol, const _bool isLoop)
{
	unordered_map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);


	if (iter == m_mapSound.end())
		return -1;

	m_UsedSounds[HashFromKey] = &iter->second.szFilePath;

	/*_vector vCamToWorldPosition = GAMEINSTANCE->Get_Transform(CPipeLine::D3DTS_WORLD).r[3] - In_WorldPosition;

	_float fDistance = XMVectorGetX(XMVector3Length(vCamToWorldPosition));

	const _float fMaxSoundDistance = 10.f;

	fDistance = min(fMaxSoundDistance, max(0.f, fDistance));

	_float fSoundScale = 1.f - (fDistance / fMaxSoundDistance);

	if (fSoundScale < DBL_EPSILON)
	{
		return -1;
	}*/


	_uint iResult = -1;

	for (_int i(1); i < MAX_CHANNEL; i++)
	{
		if (!Is_PlayingChannel(m_pChannelArr[i].pChannel))
		{
			FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[i].pChannel);

			if (_vol <= 0.f)
				_vol = 0.f;

			m_pChannelArr[i].is3DSound = false;

			FMOD_Channel_SetVolume(m_pChannelArr[i].pChannel, _vol * m_fEffectvolume * 0.25f);

			if (isLoop)
			{
				FMOD_Channel_SetMode(m_pChannelArr[i].pChannel, FMOD_LOOP_NORMAL);
			}
			iResult = i;

			break;
		}

	}

	return iResult;
}

void CSound_Manager::PlayBGM(const string& In_szSoundKey, _float _vol, const _float In_fFadeSound)
{
	return;

	unordered_map<_hashcode, SOUND_DESC>::iterator iter;

	_hashcode HashFromKey = hash<string>()(In_szSoundKey);

	iter = m_mapSound.find(HashFromKey);

	if (iter == m_mapSound.end())
		return;

	m_UsedSounds[HashFromKey] = &iter->second.szFilePath;

	if (In_fFadeSound <= DBL_EPSILON)
	{
		FMOD_Channel_Stop(m_pChannelArr[BGM].pChannel);
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[BGM].pChannel);
		FMOD_Channel_SetMode(m_pChannelArr[BGM].pChannel, FMOD_LOOP_NORMAL);
		//FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[BGM], 999999.f, 999999.f);

		m_pChannelArr[BGM].is3DSound = false;

		FMOD_VECTOR vPosition{};
		FMOD_VECTOR vVelocity{};

		FMOD_Channel_Set3DAttributes(m_pChannelArr[BGM].pChannel, &vPosition, &vVelocity);
		FMOD_Channel_Set3DDopplerLevel(m_pChannelArr[BGM].pChannel, 0);
		//FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_CHANNEL_IGNORELISTENER);
		//FMOD_Channel_Set3DMinMaxDistance(m_pChannelArr[BGM], 999999.f, 999999.f);

		if (_vol <= 0.f)
			_vol = 0.f;

		FMOD_Channel_SetVolume(m_pChannelArr[BGM].pChannel, _vol * m_fBGMvolume * 0.25f);
	}
	else
	{
		m_szNextBGM = In_szSoundKey;
		m_fNextBGMvolume = _vol * m_fBGMvolume * 0.25f;
		m_fStartFadeTime = In_fFadeSound;
		m_fCurrentFadeTime = 0.f;
		m_FaderState = 0;
		FMOD_Channel_GetVolume(m_pChannelArr[BGM].pChannel, &m_fPreBGMvolume);
	}
}

void CSound_Manager::Update_BGMVolume(_float fTimeDelta)
{
	_ushort PreFaderState = m_FaderState;

	m_fCurrentFadeTime += fTimeDelta;

	if (m_fCurrentFadeTime < m_fStartFadeTime * 0.5f)
	{
		m_FaderState = 0;
	}
	else if (m_fCurrentFadeTime < m_fStartFadeTime)
	{
		m_FaderState = 1;
	}
	else
	{
		m_FaderState = 2;
	}

	// State Start
	if (PreFaderState != m_FaderState)
	{
		switch (m_FaderState)
		{


		case 1:
		{
			unordered_map<_hashcode, SOUND_DESC>::iterator iter;
			_hashcode HashFromKey = hash<string>()(m_szNextBGM);

			iter = m_mapSound.find(HashFromKey);

			if (iter != m_mapSound.end())
			{
				FMOD_Channel_Stop(m_pChannelArr[BGM].pChannel);
				FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second.pSound, FALSE, &m_pChannelArr[BGM].pChannel);
				FMOD_Channel_SetMode(m_pChannelArr[BGM].pChannel, FMOD_LOOP_NORMAL);
				m_pChannelArr[BGM].is3DSound = false;

				FMOD_VECTOR vPosition{};
				FMOD_VECTOR vVelocity{};

				FMOD_Channel_Set3DAttributes(m_pChannelArr[BGM].pChannel, &vPosition, &vVelocity);
				FMOD_Channel_Set3DDopplerLevel(m_pChannelArr[BGM].pChannel, 0);
				FMOD_Channel_SetVolume(m_pChannelArr[BGM].pChannel, 0.f);
			}
		}
		break;

		default:
			break;
		}
	}

	// State Update
	switch (m_FaderState)
	{
	case 0:
	{
		_float fTargetTime = m_fStartFadeTime * 0.5f;
		_float fRatio = (fTargetTime - m_fCurrentFadeTime) / fTargetTime;
		FMOD_Channel_SetVolume(m_pChannelArr[BGM].pChannel, fRatio * m_fPreBGMvolume);
	}
	break;

	case 1:
	{
		_float fOffsetTime = m_fStartFadeTime * 0.5f;
		_float fRatio = (m_fCurrentFadeTime - fOffsetTime) / (m_fStartFadeTime - fOffsetTime);
		FMOD_Channel_SetVolume(m_pChannelArr[BGM].pChannel, fRatio * m_fNextBGMvolume);
	}
	break;

	case 2:
	{

	}
	break;

	default:
		break;
	}
}


void CSound_Manager::StopBGM()
{
	if (!m_pChannelArr[BGM].pChannel)
		return;

	FMOD_Channel_Stop(m_pChannelArr[BGM].pChannel);
}

void CSound_Manager::StopSound(_uint _iChannelIndex)
{
	if (_iChannelIndex == -1)
		return;

	FMOD_Channel_Stop(m_pChannelArr[_iChannelIndex].pChannel);
}

void CSound_Manager::StopAll()
{
	for (_uint i(0); i < (_uint)MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i].pChannel);
}

//void CSound_Manager::LoadSoundFilesFromPath_Legacy()
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

void CSound_Manager::LoadSoundFilesFromPath(const string& In_szFile)
{
	filesystem::path SoundFilePath(In_szFile);

	filesystem::directory_iterator itr(SoundFilePath);

	string szFileName;
	string szFilePath;

	while (itr != filesystem::end(itr))
	{
		const filesystem::directory_entry& entry = *itr;

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, entry.path().string().c_str(), FMOD_3D, 0, &pSound);

		szFileName = entry.path().filename().string();
		szFilePath = entry.path().string();

		LoadSoundFile(szFilePath, &szFileName);

		++itr;
	}

}

void CSound_Manager::LoadSoundFile(const string& In_szFilePath, string* In_szFile)
{
	FMOD_SOUND* pSound = nullptr;

	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, In_szFilePath.c_str(), FMOD_3D, 0, &pSound);
	string szFileName;
	if (FMOD_OK == eRes)
	{
		FMOD_Sound_Set3DMinMaxDistance(pSound, 1.0f, 100.0f);

		if (!In_szFile)
		{
			szFileName = filesystem::path(In_szFilePath).filename().string();
			In_szFile = &szFileName;
		}

		m_mapSound.emplace(hash<string>()(*In_szFile), SOUND_DESC(*In_szFile, In_szFilePath, pSound));
	}

}

void CSound_Manager::Write_JsonUsingResource(json& Out_Json)
{
	//Out_Json["Sounds"] = {};

	for (auto& elem : m_UsedSounds)
	{
		Out_Json["Sounds"].emplace_back(elem.second->c_str());
	}
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

_bool CSound_Manager::Is_PlayingChannel(FMOD_CHANNEL* pChannel)
{
	FMOD_RESULT FResult;
	FMOD_BOOL bPlay = FALSE;

	FResult = FMOD_Channel_IsPlaying(pChannel, &bPlay);

	return FMOD_OK == FResult && bPlay;
}

