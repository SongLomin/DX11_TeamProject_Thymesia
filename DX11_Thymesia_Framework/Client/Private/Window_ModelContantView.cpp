#include "stdafx.h"
#include "Window_ModelContantView.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "ModelData.h"
#include "Client_Components.h"
#include "Client_GameObjects.h"

IMPLEMENT_SINGLETON(CWindow_ModelContantView)

HRESULT CWindow_ModelContantView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("Model Contant", ImVec2(250.f, 500.f), window_flags);

    m_pModelData = make_shared<MODEL_DATA>();

    m_pPreviewAnimModel = GAMEINSTANCE->Add_GameObject<CPreviewAnimationModel>(LEVEL_EDIT);
    m_pPreviewModel = GAMEINSTANCE->Add_GameObject<CPreview_Prop>(LEVEL_EDIT);

    m_pPreviewAnimModel.lock()->Set_Enable(false);
    m_pPreviewModel.lock()->Set_Enable(false);

    return S_OK;
}

void CWindow_ModelContantView::Start()
{

}

void CWindow_ModelContantView::Open_ModelFile(const MODEL_TYPE In_ModelType)
{

    string szFileName;
    string szFilePath;
    if (!GET_SINGLE(CImGui_Manager)->Open_File(szFileName, szFilePath))
    {
        return;
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

    if (strcmp(szExtension.c_str(), ".bin") != 0 &&
        strcmp(szExtension.c_str(), ".fbx") != 0 &&
        strcmp(szExtension.c_str(), ".FBX") != 0)
    {
        MSG_BOX("Is not model file.");
        return;
    }

    m_pModelData->Make_ModelData(szFilePath.c_str(), In_ModelType,XMMatrixIdentity());

    if (MODEL_TYPE::ANIM == m_pModelData->eModelType)
    {
        m_pPreviewAnimModel.lock()->Set_Enable(true);
        m_pPreviewModel.lock()->Set_Enable(false);

        m_pPreviewAnimModel.lock()->Get_Component<CModel>().lock()->Init_Model(m_pModelData);
        m_pPreviewAnimModel.lock()->Play_Animation(0.f);
    }
    else if(MODEL_TYPE::NONANIM == m_pModelData->eModelType)
    {
        m_pPreviewAnimModel.lock()->Set_Enable(false);
        m_pPreviewModel.lock()->Set_Enable(true);

        m_pPreviewModel.lock()->Get_Component<CModel>().lock()->Init_Model(m_pModelData);
    }
}

void CWindow_ModelContantView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    RECT ClientRect = { 0, 0, 99999, 99999 };
    ClipCursor(&ClientRect);
}

HRESULT CWindow_ModelContantView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    
    if (ImGui::CollapsingHeader("Mesh Containers"))
    {
        string szMeshName;
        shared_ptr<MESH_DATA> pCurrentMeshData;

        for (_int i = 0; i < m_pModelData->iNumMeshs; ++i)
        {
            pCurrentMeshData = m_pModelData->Mesh_Datas[i];

            szMeshName = to_string(i) + ". ";
            szMeshName += pCurrentMeshData->szName;
        }
    }

    
    __super::End();

    return S_OK;
}

void CWindow_ModelContantView::OnDestroy()
{
    __super::OnDestroy();

    m_pPreviewAnimModel.lock()->Set_Dead();
    m_pPreviewModel.lock()->Set_Dead();

}

void CWindow_ModelContantView::Free()
{
}

