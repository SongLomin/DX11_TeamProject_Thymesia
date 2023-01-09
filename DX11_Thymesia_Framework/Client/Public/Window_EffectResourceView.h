#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_EffectResourceView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_EffectResourceView)


public:
	vector<string>& Get_AllEffectGroupNames() { return m_szEffectGroupNames; }

public:
	virtual HRESULT Initialize() override;
	virtual void	Start() override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;
	
public:
	void Load_Resources();
	void Load_EffectMesh();
	void Load_Particle();
	void Load_EffectGroup();
	void Load_Sound();

public:
	FDelegate<const _char*>					CallBack_MeshClick;
	FDelegate<const _char*, const _char*>	CallBack_ParticleClick;
	FDelegate<const _char*>					CallBack_EffectGroupClick;
	FDelegate<const _char*>					CallBack_SpriteImageClick;
	FDelegate<const _char*>					CallBack_SoundFileClick;

private:
	void Load_AllEffectMeshInPath_Recursive(const filesystem::path& In_Path);	
	void Load_Particle_Recursive(const filesystem::path& In_Path);

private:
	vector<string> m_szAnimEffectMeshNames;
	vector<pair<string, string>> m_szParticleMeshNames;
	vector<string> m_szEffectGroupNames;
	vector<const string*> m_szSoundNames;

private:
	void Free();
};
END
