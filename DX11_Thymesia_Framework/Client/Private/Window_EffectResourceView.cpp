#include "stdafx.h"
#include "Window_EffectResourceView.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CWindow_EffectResourceView)

HRESULT CWindow_EffectResourceView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("EffectResource_View", ImVec2(150.f, 500.f), window_flags);


    Load_Resources();


    return S_OK;
}

void CWindow_EffectResourceView::Start()
{
}

void CWindow_EffectResourceView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

HRESULT CWindow_EffectResourceView::Render()
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Mesh"))
        {
            for (auto& elem : m_szAnimEffectMeshNames)
            {
                if (ImGui::Selectable(elem.c_str()))
                {
                    CallBack_MeshClick(elem.c_str());
                }
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Particle"))
        {
            for (auto& elem : m_szParticleMeshNames)
            {
                if (ImGui::Selectable(elem.first.c_str()))
                {
                    CallBack_ParticleClick(elem.first.c_str(), elem.second.c_str());
                }
            }


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Effect_Group"))
        {
            for (auto& elem : m_szEffectGroupNames)
            {
                if (ImGui::Selectable(elem.c_str()))
                {
                    CallBack_EffectGroupClick(elem.c_str());
                }
            }


            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    __super::End();

    return S_OK;
}

void CWindow_EffectResourceView::Load_Resources()
{
    Load_EffectMesh();
    Load_Particle();
    Load_EffectGroup();
}

void CWindow_EffectResourceView::Load_EffectMesh()
{
    m_szAnimEffectMeshNames.clear();

    fs::directory_iterator itr("..\\Bin\\Resources\\Meshes\\EffectMesh");


    while (itr != fs::end(itr)) {
        const fs::directory_entry& entry = *itr;

        Load_AllEffectMeshInPath_Recursive(entry);
        

        itr++;
    }
}

void CWindow_EffectResourceView::Load_Particle()
{
    m_szParticleMeshNames.clear();

    fs::directory_iterator itr("..\\Bin\\Resources\\Textures\\Effect");
    tstring szPath;
    tstring szFileName;

    while (itr != fs::end(itr)) {
        const fs::directory_entry& entry = *itr;
        szFileName = entry.path().filename().wstring();

        #ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
        #endif

        Load_Particle_Recursive(entry.path());

        itr++;
    }

}

void CWindow_EffectResourceView::Load_EffectGroup()
{
    m_szEffectGroupNames.clear();

    fs::directory_iterator itr("..\\Bin\\EffectData");
    string szPath;
    string szFileName;

    while (itr != fs::end(itr)) {
        const fs::directory_entry& entry = *itr;
        szFileName = entry.path().filename().string();

        #ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
        #endif

        m_szEffectGroupNames.push_back(szFileName);
        itr++;
    }

}

void CWindow_EffectResourceView::Free()
{
}

void CWindow_EffectResourceView::Load_AllEffectMeshInPath_Recursive(const filesystem::path& In_Path)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	string szFileName;
	string szExtension;
	fs::directory_iterator itr(In_Path);
	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		Load_AllEffectMeshInPath_Recursive(entry.path());

		szFileName = entry.path().filename().string().c_str();
		szFileName = szFileName.substr(0, szFileName.size() - 4);

		 if (strcmp(entry.path().extension().string().c_str(), ".bin") == 0)
		 {
			#ifdef _DEBUG_COUT_
		cout << szFileName << endl;
#endif

            // TODO : ! control effect mesh initial size here !
			GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(),
                MODEL_TYPE::NONANIM,
                XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));

			m_szAnimEffectMeshNames.push_back(szFileName);
		 }


		///*else*/ if (strcmp(entry.path().extension().string().c_str(), ".fbx") == 0)
		//{
#ifdef _DEBUG_COUT_
		//cout << szFileName << endl;
#endif
		//    GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(),
  //              MODEL_TYPE::NONANIM,
  //              XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
		//    m_szAnimEffectMeshNames.push_back(szFileName);
		//}

		//else if (strcmp(entry.path().extension().string().c_str(), ".FBX") == 0)
		//{
 #ifdef _DEBUG_COUT_
		//cout << szFileName << endl;
#endif
		//    GAMEINSTANCE->Load_Model(szFileName.c_str(), entry.path().string().c_str(),
  //              MODEL_TYPE::NONANIM,
  //              XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
		//    m_szAnimEffectMeshNames.push_back(szFileName);
		//}

		itr++;
	}
}


void CWindow_EffectResourceView::Load_Particle_Recursive(const filesystem::path& In_Path)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	tstring szPath;
	string szFileName;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;

		szFileName = entry.path().filename().string();

#ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
#endif

		Load_Particle_Recursive(entry.path());

		szPath = entry.path().wstring();
		GAMEINSTANCE->Load_Textures(szFileName.c_str(), szPath.c_str());

		m_szParticleMeshNames.emplace_back(entry.path().filename().string(), szFileName);

		itr++;
	}

}
