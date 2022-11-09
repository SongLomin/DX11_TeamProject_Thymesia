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
	enum class EFFECTRESOURCE_TYPE
	{
		MESH = 0, PARTICLE, TYPE_END
	};

public:
	virtual HRESULT Initialize() override;
	virtual void	Start() override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Load_Resources();
	void Load_EffectMesh();
	void Load_Particle();
	void Load_EffectGroup();
	void Free();
	FDelegate<const _char*>					CallBack_MeshClick;
	FDelegate<const _char*, const _char*>	CallBack_ParticleClick;
	FDelegate<const _char*>					CallBack_EffectGroupClick;
	FDelegate<const _char*>					CallBack_SpriteImageClick;

private:
	void Load_AllEffectMeshInPath_Recursive(const filesystem::path& In_Path);	
	void Load_Particle_Recursive(const filesystem::path& In_Path);

private:
	vector<string> m_szAnimEffectMeshNames;
	vector<pair<string, string>> m_szParticleMeshNames;
	vector<string> m_szEffectGroupNames;
};
END
