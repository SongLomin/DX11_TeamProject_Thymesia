#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CEffectGroup;

class CWindow_EffectHierarchyView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_EffectHierarchyView)

public:
	enum class EFFECTRESOURCE_TYPE
	{
		EFFECT,
		SOUND,
		TYPE_END
	};

public:
	const _char* Get_EffectGroupName() const;
	weak_ptr<CEffectGroup> Get_EffectGroup() const { return m_pEffectGroup; }

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;

public:
	void Write_FromBinary();
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	FDelegate<weak_ptr<CEffectGroup>, _uint>	CallBack_SelectEffect;
	FDelegate<const _char*>						CallBack_SelectSound;

public:
	void Call_AddEffectMesh(const _char* In_szModelKey);
	void Call_AddParticle(const _char* In_szName, const _char* In_szTextureKey);
	void Call_LoadEffectGroup(const _char* In_szEffectGroupName);
	void Call_LoadSoundFile(const _char* In_szSoundFileName);
	void Call_RemoveCurrentIndex();

public:
	void Add_EffectMesh_Internal(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey);
	void Add_EffectParticle_Internal(const _char* In_szName, const _char* In_szModelKey);

private:
	void Render_Effect();
	void Render_Sound();


private:
	EFFECTRESOURCE_TYPE		m_eCurrentResourceType = EFFECTRESOURCE_TYPE::TYPE_END;
	vector<string>			m_szEffectMeshNames;
	vector<string>			m_szEffectParticleNames;
	_uint					m_iCurrentEffectIndex = 0;
	_uint					m_iCurrentSoundIndex = 0;

	weak_ptr<CEffectGroup>	m_pEffectGroup;

public:
	void Free();
};
END

