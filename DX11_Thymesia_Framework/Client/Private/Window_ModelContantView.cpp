#include "stdafx.h"
#include "Window_ModelContantView.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "ModelData.h"
#include "Client_Components.h"
#include "Client_GameObjects.h"
#include <imgui_impl_win32.h>

IMPLEMENT_SINGLETON(CWindow_ModelContantView)

HRESULT CWindow_ModelContantView::Initialize()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
    // window_flags |= ImGuiWindowFlags_NoResize;
    // window_flags |= ImGuiWindowFlags_NoMove;

    m_bEnable = true;
    SetUp_ImGuiDESC("Model Contant", ImVec2(250.f, 500.f), window_flags);

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

    if (strcmp(szExtension.c_str(), ".bin") != 0)
    {
        MSG_BOX("Is not model file.");
        return;
    }

    m_pModelData.reset();

    m_pModelData = make_shared<MODEL_DATA>();

    m_pModelData->Make_ModelData(szFilePath.c_str(), In_ModelType,XMMatrixIdentity());

    if (MODEL_TYPE::ANIM == m_pModelData->eModelType)
    {
        m_bAnimModel = true;
        Load_PreviewAnimModel(m_pModelData);
    }
    else if(MODEL_TYPE::NONANIM == m_pModelData->eModelType)
    {
        m_bAnimModel = false;
        Load_PreviewModel(m_pModelData);
    }
}

void CWindow_ModelContantView::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    RECT ClientRect = { 0, 0, 99999, 99999 };
    ClipCursor(&ClientRect);

    if (m_bAnimModel && m_bPlay)
    {
        weak_ptr<CModel> pAnimModelCom = m_pPreviewAnimModel.lock()->Get_Component<CModel>();

        // 모델이 있으면
        if (pAnimModelCom.lock()->Get_ModelData().lock())
        {
            pAnimModelCom.lock()->Play_Animation(fTimeDelta);
        }
    }

    if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::S, KEY_STATE::TAP))
    {
        Save_Model();
    }

    
}

HRESULT CWindow_ModelContantView::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (FAILED(__super::Begin()))
        return E_FAIL;

    if (m_pModelData)
    {
        ImGui::TextColored(ImVec4{ 1.f, 0.f, 0.f, 1.f }, m_pModelData->szModelFileName.c_str());
    }
    

    if (ImGui::Button("ReLoad Model") || KEY_INPUT(KEY::F5, KEY_STATE::TAP))
    {
        if (m_bAnimModel)
        {
            Load_PreviewAnimModel(m_pModelData);
        }
        else
        {
            Load_PreviewModel(m_pModelData);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Animation Play"))
    {
        m_bPlay = !m_bPlay;
    }

    if (!m_pModelData)
    {
        __super::End();
		return S_OK;
    }
        


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

    if (ImGui::CollapsingHeader("Materials"))
    {
        string szMeshName;
        shared_ptr<MATERIAL_DATA> pCurrentMaterialData;
        _char szTextureName[AI_TEXTURE_TYPE_MAX][MAX_PATH];
        ZeroMemory(szTextureName, sizeof(_char) * AI_TEXTURE_TYPE_MAX * MAX_PATH);

        for (_int i = 0; i < m_pModelData->iNumMaterials; ++i)
        {
            pCurrentMaterialData = m_pModelData->Material_Datas[i];

            string szText = "Material" + to_string(i) + ". ";

            ImGui::TextColored(ImVec4{ 1.f, 0.f, 1.f, 1.f }, szText.c_str());

            for (_int j = 0; j < (_int)AI_TEXTURE_TYPE_MAX; ++j)
            {
                Edit_Texture((aiTextureType)j, pCurrentMaterialData, i);
            }
            ImGui::Separator();
        }
    }

    if (ImGui::CollapsingHeader("Animation"))
    {
        Clip_AnimationName();

        shared_ptr<ANIMATION_DATA> pCurrentAnimationlData;

        for (_int i = 0; i < m_pModelData->iNumAnimations; ++i)
        {
            pCurrentAnimationlData = m_pModelData->Animation_Datas[i];

            Edit_Animation(pCurrentAnimationlData, i);

            ImGui::Separator();
        }

        

    }

    
    __super::End();

    return S_OK;
}

void CWindow_ModelContantView::Save_Model()
{
    if (!m_pModelData)
        return;

    m_pModelData->Bake_Binary();
}

void CWindow_ModelContantView::Load_PreviewAnimModel(shared_ptr<MODEL_DATA> pModelData)
{
    if (!m_pModelData)
        return;

    /*if (m_pPreviewAnimModel.lock())
    {
        m_pPreviewAnimModel.lock()->Set_Dead();
    }

    m_pPreviewAnimModel = GAMEINSTANCE->Add_GameObject<CPreviewAnimationModel>(LEVEL_EDIT);*/

    m_pPreviewAnimModel.lock()->Set_Enable(true);
    m_pPreviewModel.lock()->Set_Enable(false);

    m_pPreviewAnimModel.lock()->Get_Component<CModel>().lock()->Init_Model(m_pModelData);
    m_pPreviewAnimModel.lock()->Play_Animation(0.f);
}

void CWindow_ModelContantView::Load_PreviewModel(shared_ptr<MODEL_DATA> pModelData)
{
    if (!m_pModelData)
        return;

    /*if (m_pPreviewModel.lock())
    {
        m_pPreviewModel.lock()->Set_Dead();
    }

    m_pPreviewModel = GAMEINSTANCE->Add_GameObject<CPreview_Prop>(LEVEL_EDIT);*/
    m_pPreviewAnimModel.lock()->Set_Enable(false);
    m_pPreviewModel.lock()->Set_Enable(true);
    

    m_pPreviewModel.lock()->Get_Component<CModel>().lock()->Init_Model(m_pModelData);
}

void CWindow_ModelContantView::Edit_Texture(const aiTextureType In_TextureType, shared_ptr<MATERIAL_DATA> pMaterialData, const _int In_iIndex)
{
    string IndexToString = string(to_string(In_iIndex) + "_" + to_string((_int)In_TextureType));

    string TextureTypeToString;

    switch (In_TextureType)
    {
    case aiTextureType_NONE:
        TextureTypeToString = "NONE";
        break;

    case aiTextureType_DIFFUSE:
        TextureTypeToString = "DIFFUSE";
        break;

    case aiTextureType_SPECULAR :
        TextureTypeToString = "SPECULAR";
        break;

    case aiTextureType_AMBIENT:
        TextureTypeToString = "AMBIENT";
        break;

    case aiTextureType_EMISSIVE:
        TextureTypeToString = "EMISSIVE";
        break;

    case aiTextureType_HEIGHT:
        TextureTypeToString = "HEIGHT";
        break;

    case aiTextureType_NORMALS:
        TextureTypeToString = "NORMALS";
        break;

    case aiTextureType_SHININESS:
        TextureTypeToString = "SHININESS";
        break;

    case aiTextureType_OPACITY:
        TextureTypeToString = "OPACITY";
        break;

    case aiTextureType_DISPLACEMENT:
        TextureTypeToString = "DISPLACEMENT";
        break;

    case aiTextureType_LIGHTMAP:
        TextureTypeToString = "LIGHTMAP";
        break;

    case aiTextureType_REFLECTION:
        TextureTypeToString = "REFLECTION";
        break;

    case aiTextureType_BASE_COLOR:
        TextureTypeToString = "BASE_COLOR";
        break;

    case aiTextureType_NORMAL_CAMERA:
        TextureTypeToString = "NORMAL_CAMERA";
        break;

    case aiTextureType_EMISSION_COLOR:
        TextureTypeToString = "EMISSION_COLOR";
        break;

    case aiTextureType_METALNESS:
        TextureTypeToString = "METALNESS";
        break;

    case aiTextureType_DIFFUSE_ROUGHNESS:
        TextureTypeToString = "DIFFUSE_ROUGHNESS";
        break;

    case aiTextureType_AMBIENT_OCCLUSION:
        TextureTypeToString = "AMBIENT_OCCLUSION";
        break;

    case aiTextureType_UNKNOWN:
        TextureTypeToString = "UNKNOWN";
        break;


    default:
        break;
    }

    _char szTextureName[MAX_PATH];
    ZeroMemory(szTextureName, sizeof(_char) * MAX_PATH);

    strcpy_s(szTextureName, pMaterialData->szTextureName[In_TextureType].c_str());
    if (ImGui::InputText(string("##" + IndexToString).c_str(), szTextureName, MAX_PATH))
    {
        pMaterialData->szTextureName[In_TextureType] = szTextureName;
    }

    ImGui::SameLine();

    if (ImGui::Button((IndexToString + ". " + TextureTypeToString).c_str()))
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

        pMaterialData->szTextureName[In_TextureType] = szFileName;
    }

}

void CWindow_ModelContantView::Clip_AnimationName()
{
    

    if (ImGui::InputInt("##ClipLength", &m_iClipLength))
    {
        m_iClipLength = max(0, m_iClipLength);
    }

    ImGui::SameLine();

    if (ImGui::Button("Cut All Anim Name"))
    {
        shared_ptr<ANIMATION_DATA> pCurrentAnimationlData;

        for (_int i = 0; i < m_pModelData->iNumAnimations; ++i)
        {
            pCurrentAnimationlData = m_pModelData->Animation_Datas[i];

            if (pCurrentAnimationlData->szName.size() > m_iClipLength)
            {
                pCurrentAnimationlData->szName = pCurrentAnimationlData->szName.substr(m_iClipLength);
            }
        }
    }

}

void CWindow_ModelContantView::Edit_Animation(shared_ptr<ANIMATION_DATA> pAnimationData, const _int In_iIndex)
{
    _char szAnimationName[MAX_PATH];
    ZeroMemory(szAnimationName, sizeof(_char) * MAX_PATH);

    strcpy_s(szAnimationName, pAnimationData->szName.c_str());

    if (ImGui::InputText((to_string(In_iIndex) + string(". Animation Name")).c_str(), szAnimationName, MAX_PATH))
    {
        pAnimationData->szName = szAnimationName;
    }

    ImGui::InputFloat((to_string(In_iIndex) + string(". Animation Tick")).c_str(), &pAnimationData->fTickPerSecond, 0.f, 0.f, "%2.f");

    if (ImGui::Button((to_string(In_iIndex) + string(". Play")).c_str()))
    {
        m_pPreviewAnimModel.lock()->Get_Component<CModel>().lock()->Set_CurrentAnimation(In_iIndex, 0, 0.f);
    }

    ImGui::Separator();
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

