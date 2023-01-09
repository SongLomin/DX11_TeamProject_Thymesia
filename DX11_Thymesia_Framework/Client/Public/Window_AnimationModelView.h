#pragma once
#include "ImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CPreviewAnimationModel;
class CPreview_Prop;
class CWindow_AnimationModelView final
	: public CImGui_Window
{
	DECLARE_SINGLETON(CWindow_AnimationModelView)

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Start() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext) override;


public:
	weak_ptr<CPreviewAnimationModel> Get_PreviewAnimModel();
	weak_ptr< CPreview_Prop> Get_PreviewPropModel();
	ATTACKAREA_DESC Get_DebugWeaponDesc() const { return m_tWeaponDesc; }
	const std::vector<std::string> Get_AllBoneNames() const { return m_AllBoneNames; };

public:
	FDelegate<> CallBack_UpdatePreViewModel;

private:
	void Load_PreViewModels();
	void Update_PreViewModel();

private:
	weak_ptr<CPreviewAnimationModel>	m_pPreviewModel;
	// TODO : comment because explode
	weak_ptr<CPreview_Prop>				m_pPreviewNoAnimModel;
	vector<const string*>	                    m_AllModelKeys;
	vector<const string*>	                    m_AllNoAnimModelKeys;
	_int			                    m_CurrentModelIndex = 0;
	_int			                    m_CurrentNoAnimModelIndex = 0;

private: /* For Weapon Collider */
	vector<string>	                    m_AllBoneNames;
	_int			                    m_CurrentBoneIndex = 0;

	ATTACKAREA_DESC		                    m_tWeaponDesc;

	_float3								m_vOffset{ 0.f, 0.f, 0.f };
	_float								m_fSize = { 1.f };

public:
	void Free();
};
END


