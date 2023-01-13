#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CLoader final : public CBase
{
public:
	CLoader();
	virtual ~CLoader();

public:
	CRITICAL_SECTION Get_CS() const {
		return m_CriticalSection;
	}

	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	_bool is_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

public:
	HRESULT Initialize(LEVEL eNextLevel);
	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForLobby();
	HRESULT Loading_ForGamePlayLevel();
	HRESULT Loading_ForStage2Level();
	HRESULT Loading_ForStage3Level();
	HRESULT Loading_ForHomeLevel();
	HRESULT Loading_ForEditLevel();
	HRESULT Loading_ForTestLevel();

private:
	void	Loading_ForEffectGroup(const char* In_Path, const _uint& In_LevelIndex);
	void	Load_AllDiffuseTexture();
	void	Load_AllMaskMap();
	void	Load_AllNoiseTexture();
	void	Load_AllParticleTexture();
	void	Load_AllParticleInPath_Recursive(const filesystem::path& In_Path);
	void	Load_AllEffectMesh();
	void	Load_AllEffectMeshInPath_Recursive(const filesystem::path& In_Path);
	void	Load_AllNaviMesh();

	void	Load_AllMeshes(const filesystem::path& In_Path, MODEL_TYPE In_eModelType, const MEMORY_TYPE& In_eMemoryType, _matrix TransformationMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f), const char* In_extansion = ".bin");
	//void	Load_AllMeshesInPath_Recursive(const filesystem::path& In_Path, MODEL_TYPE In_eModelType, const MEMORY_TYPE& In_eMemoryType, _matrix TransformationMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f), const char* In_extansion = ".bin");
	void	Load_AllTexture(const filesystem::path& In_Path, const MEMORY_TYPE& In_eMemoryType);

	void	Create_GameObjectFromJson(const string& In_szJsonPath, const LEVEL& In_eLevel);

	void	Load_UIResource();
	void	Load_CorvusModel();
	void	Load_NormalMobModel();
	void    Load_RareMobModel();
	void    Load_EliteMobModel();
	void	Load_BossMobModel();
	void	Load_AllMapModel();

	void	Load_Model_UseThread(const _char* sKey, const _char* sModelFilePath, MODEL_TYPE eModelType, _fmatrix In_TransformMatrix, MEMORY_TYPE eMemType, _bool Is_bAnimZero = false);

private:
	LEVEL					m_eNextLevel = LEVEL_END;

private:
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_isFinished = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");

public:
	static shared_ptr<CLoader> Create(LEVEL eNextLevel);
	void Free();
};

END
