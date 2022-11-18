#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CImGui_Window;
class CCamera_Free;
class CTerrain;

class CImGui_Manager : public CBase
{
public:
	enum class IMGUI_WINDOW_TYPE
	{
		IMGUI_UIVIEW,
		IMGUI_PROTOTYPEVIEW,
		IMGUI_HIERARCHYVIEW,
		IMGUI_COMPONENTVIEW,
		IMGUI_END
	};

	enum class EDITER_TYPE
	{
		SCENE,
		EFFECT,
		TYPE_END
	};

	DECLARE_SINGLETON(CImGui_Manager)

public:
	HRESULT Initialize();
	void	Tick(_float fTimeDelta);
	HRESULT Render();

public:
	void Save_EffectJson();

private:

	void Init_SceneEditer();
	void Init_EffectEditer();

	void Release_CurrentEditer();

	void Write_Json(const string& In_szPath);
	void Load_FromJson(const string& In_szPath);

public:
	vector<shared_ptr<CImGui_Window>> m_arrWindows;

private: /* For Json */
	string m_szJsonPath = "../Bin/LevelData/";
	string m_szCurrentLocalPath;
	json m_CurrentLevelJson;

	EDITER_TYPE m_eCurrentEditerType = EDITER_TYPE::TYPE_END;

private: /* For Effect Editer */
	weak_ptr<CCamera_Free>	m_pEditerCamera;
	weak_ptr<CTerrain>		m_pTerrain;

private:
	void Free();
};

END