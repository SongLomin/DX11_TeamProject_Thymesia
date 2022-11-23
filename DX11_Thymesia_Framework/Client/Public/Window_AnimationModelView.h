#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CPreViewAnimationModel;
class CPreView_Prop;

class CWindow_AnimationModelView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_AnimationModelView)

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	weak_ptr<CPreViewAnimationModel> Get_PreViewModel();
	WEAPON_DESC Get_DebugWeaponDesc() const { return m_tWeaponDesc; }
	const std::vector<std::string> Get_AllBoneNames() const { return m_AllBoneNames; };

public:
	FDelegate<> CallBack_UpdatePreViewModel;

private:
	void Load_PreViewModels();
	void Update_PreViewModel();

private:
	weak_ptr<CPreViewAnimationModel>	m_pPreViewModel;
	weak_ptr<CPreView_Prop>				m_pPreViewNoAnimModel;
	vector<string>	                    m_AllModelKeys;
	vector<string>	                    m_AllNoAnimModelKeys;
	_int			                    m_CurrentModelIndex = 0;
	_int			                    m_CurrentNoAnimModelIndex = 0;

private: /* For Weapon Collider */
	vector<string>	                    m_AllBoneNames;
	_int			                    m_CurrentBoneIndex = 0;

	WEAPON_DESC		                    m_tWeaponDesc;



public:
	void Free();
};
END


