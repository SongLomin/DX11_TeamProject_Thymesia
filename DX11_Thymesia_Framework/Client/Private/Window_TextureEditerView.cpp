#include "stdafx.h"
#include "Window_TextureEditerView.h"
#include "ImGui_Manager.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include <imgui_impl_win32.h>
#include "MainApp.h"


IMPLEMENT_SINGLETON(CWindow_TextureEditerView)

HRESULT CWindow_TextureEditerView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;
    
    m_bEnable = true;
    SetUp_ImGuiDESC("Texture Editer", ImVec2(250.f, 500.f), window_flags);

    Reset_TextureEditer();

	return S_OK;
}

void CWindow_TextureEditerView::Start()
{

}

void CWindow_TextureEditerView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    

}

HRESULT CWindow_TextureEditerView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;
    
    if (ImGui::Button("Bake All MipMaps Recursive"))
    {
        CMainApp::Bake_MipMaps();
    }

    if (ImGui::Button("Bake MipMap"))
    {
        GAMEINSTANCE->Generate_MipMap(szTextureFilePath.c_str());
    }

    
    if (ImGui::Button("Open Texture"))
    {
        Open_Texture();
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Texture"))
    {
        Save_Texture();
    }

    ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, szTextureFileName.c_str());

    ImGui::Separator();

    ImTextureID Texture_Handle = (void*)m_pSRV.Get();
    ImGui::Image(Texture_Handle, ImVec2(256, 256));

    ImGui::Separator();

    Render_RGBButton();

    ImGui::Separator();

#ifdef _DEBUG
    if (m_pSRV.Get())
    {
        GAMEINSTANCE->Render_EditTexture(m_pSRV, m_RGBs[1][0], m_RGBs[1][1], m_RGBs[1][2], m_RGBs[1][3]);
    }
#endif // _DEBUG

    

    ComPtr<ID3D11ShaderResourceView> pRenderTargetSRV = GAMEINSTANCE->Get_RenderTarget_SRV(TEXT("Target_ExtractTexture"));

    if (pRenderTargetSRV.Get())
    {
        ImTextureID RT_Handle = (void*)pRenderTargetSRV.Get();
        ImGui::Image(RT_Handle, ImVec2(256, 256));

        pRenderTargetSRV.Reset();
    }
    ImGui::Separator();

    __super::End();
	return S_OK;
}

_bool CWindow_TextureEditerView::Open_Texture()
{
    string szFileName;
    string szFilePath;

    if (!GET_SINGLE(CImGui_Manager)->Open_File(szFileName, szFilePath))
    {
        return false;
    }

    string szExtension;

    try
    {
        filesystem::path FilePath(szFilePath);
        szExtension = FilePath.extension().string();
    }
    catch (const std::exception&)
    {
        // Do nothing.
    }

    if (strcmp(szExtension.c_str(), ".png") != 0)
    {
        MSG_BOX("Is not png file.");
        return false;
    }

    Reset_TextureEditer();

    szTextureFileName = szFileName;
    szTextureFilePath = filesystem::path(szFilePath).wstring();

    HRESULT hr = CreateWICTextureFromFile(DEVICE, szTextureFilePath.c_str(), nullptr, m_pSRV.GetAddressOf());

    if (FAILED(hr))
    {
        return false;
    }


    return true;
}

_bool CWindow_TextureEditerView::Save_Texture()
{
    if (szTextureFilePath.empty())
        return false;

#ifdef _DEBUG
    GAMEINSTANCE->Extract_Texture(szTextureFilePath);
#endif // _DEBUG


    

    return true;
}

void CWindow_TextureEditerView::Render_RGBButton()
{
    ImVec4 vColor[4]{ {1.f, 0.f, 0.f, 1.f}, {0.f, 1.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f} };

    if (ImGui::Button("Reset##RGB"))
    {
        Reset_RGBs();
    }

    ImGui::Separator();

    ImGuiIO& io = ImGui::GetIO();

    for (_int n = 0; n < 8; n++)
    {
        _int i = n / 4;
        _int j = n % 4;
        _int iColorIndex = m_RGBs[i][j];

        ImGui::PushID(n);
        if ((j) != 0)
            ImGui::SameLine();

        ImGui::PushID("Color Button");
        ImGui::PushStyleColor(ImGuiCol_Button, vColor[iColorIndex]);
        ImGui::Button(szRGB[iColorIndex].c_str(), ImVec2(60, 60));
        if (ImGui::IsItemActive())
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(vColor[iColorIndex]), 4.0f);

        ImGui::PopStyleColor(1);
        ImGui::PopID();

        // Our buttons are both drag sources and drag targets here!
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("DND_RGB_CELL", &n, sizeof(_int));

            ImGui::TextColored(vColor[iColorIndex], szRGB[iColorIndex].c_str());

            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_RGB_CELL"))
            {
                IM_ASSERT(payload->DataSize == sizeof(int));
                int payload_n = *(const int*)payload->Data;

                _int payload_i = payload_n / 4;
                _int payload_j = payload_n % 4;

                if (0 != i)
                {
                    m_RGBs[i][j] = m_RGBs[payload_i][payload_j];
                }
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::PopID();
    }

}

void CWindow_TextureEditerView::Reset_TextureEditer()
{
    m_pSRV.Reset();
    Reset_RGBs();
}

void CWindow_TextureEditerView::Reset_RGBs()
{
    for (_int i = 0; i < 2; ++i)
    {
        for (_int j = 0; j < 4; ++j)
        {
            m_RGBs[i][j] = j;
        }
    }
}

void CWindow_TextureEditerView::OnDestroy()
{
    __super::OnDestroy();
}

void CWindow_TextureEditerView::Free()
{
    m_pSRV.Reset();
}
