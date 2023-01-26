#include "stdafx.h"
#include "Window_EffectResourceView.h"
#include "GameInstance.h"
#include <imgui_impl_win32.h>
#include "ClientLevel.h"

IMPLEMENT_SINGLETON(CWindow_EffectResourceView)

HRESULT CWindow_EffectResourceView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("Resource##Effect_Resource_View", ImVec2(150.f, 500.f), window_flags);

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

HRESULT CWindow_EffectResourceView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("CWindow_EffectResourceView", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Mesh"))
        {
			static ImGuiTextFilter EffectMeshFilter;
			ImGui::Text("Search"); ImGui::SameLine();
			EffectMeshFilter.Draw("##EffectMeshSearchFilter", 340.f);

            for (int i(0); i < m_szAnimEffectMeshNames.size(); ++i)
            {
                auto MeshKit = m_szAnimEffectMeshNames.at(i);

                if (EffectMeshFilter.PassFilter(MeshKit.c_str()))
                {
                    std::string label = MeshKit + "##" + std::to_string(i);

					if (ImGui::Selectable(label.c_str()))
					{
						CallBack_MeshClick(MeshKit.c_str());
					}
                }
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Particle"))
        {
			static ImGuiTextFilter ParticleFilter;
			ImGui::Text("Search"); ImGui::SameLine();
            ParticleFilter.Draw("##ParticleSearchFilter", 340.f);

			for (int i(0); i < m_szParticleMeshNames.size(); ++i)
			{
				auto ParticleKit = m_szParticleMeshNames.at(i);

				if (ParticleFilter.PassFilter(ParticleKit.first.c_str()))
				{
					std::string label = ParticleKit.first + "##" + std::to_string(i);

					if (ImGui::Selectable(label.c_str()))
					{
                        CallBack_ParticleClick(ParticleKit.first.c_str(), ParticleKit.second.c_str());
					}
				}
			}

            //for (auto& elem : m_szParticleMeshNames)
            //{
            //    if (ImGui::Selectable(elem.first.c_str()))
            //    {
            //        CallBack_ParticleClick(elem.first.c_str(), elem.second.c_str());
            //    }
            //}


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Effect_Group"))
        {
			static ImGuiTextFilter EffectGroupFilter;
			ImGui::Text("Search"); ImGui::SameLine();
            EffectGroupFilter.Draw("##EffectGroupSearchFilter", 340.f);

			for (int i(0); i < m_szEffectGroupNames.size(); ++i)
			{
				auto EffectGroupKit = m_szEffectGroupNames.at(i);

				if (EffectGroupFilter.PassFilter(EffectGroupKit.c_str()))
				{
					std::string label = EffectGroupKit + "##" + std::to_string(i);

					if (ImGui::Selectable(label.c_str()))
					{
                        CallBack_EffectGroupClick(EffectGroupKit.c_str());
					}
				}
			}

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("SoundFile"))
        {
            static ImGuiTextFilter EffectGroupFilter;
            ImGui::Text("Search"); ImGui::SameLine();
            EffectGroupFilter.Draw("##SoundFileSearchFilter", 340.f);

            for (int i(0); i < m_szSoundNames.size(); ++i)
            {
                auto EffectGroupKit = m_szSoundNames.at(i);

                if (EffectGroupFilter.PassFilter(EffectGroupKit->c_str()))
                {
                    std::string label = *EffectGroupKit + "##" + std::to_string(i);

                    if (ImGui::Selectable(label.c_str()))
                    {
                        CallBack_SoundFileClick(EffectGroupKit->c_str());
                    }
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
    Load_Sound();
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

void CWindow_EffectResourceView::Load_Sound()
{
    m_szSoundNames.clear();

    m_szSoundNames = GAMEINSTANCE->Get_AllSoundNames();

    std::sort(m_szSoundNames.begin(), m_szSoundNames.end());
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

void CWindow_EffectResourceView::Free()
{
}
