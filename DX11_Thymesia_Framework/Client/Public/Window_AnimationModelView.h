#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CPreViewAnimationModel;

class CWindow_AnimationModelView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_AnimationModelView)

public:
	weak_ptr<CPreViewAnimationModel> Get_PreViewModel();
	WEAPON_DESC Get_DebugWeaponDesc() const { return m_tWeaponDesc; }

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	FDelegate<> CallBack_UpdatePreViewModel;

private:
	void Load_PreViewModels();
	void Update_PreViewModel();

private:
	weak_ptr<CPreViewAnimationModel>	m_pPreViewModel;
	vector<string>	m_AllModelKeys;
	_int			m_CurrentModelIndex = 0;

private: /* For Weapon Collider */
	vector<string> m_AllBoneNames;
	_int			m_CurrentBoneIndex = 0;

	WEAPON_DESC		m_tWeaponDesc;
	


public:
	virtual void Free() override;
};
END


