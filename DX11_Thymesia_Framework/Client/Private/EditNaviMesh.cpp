#include "stdafx.h"
#include "EditNaviMesh.h"

#include "Collider.h"
#include "Renderer.h"
#include "VIBuffer_Cell.h"
#include "Shader.h"

#include "imgui.h"

GAMECLASS_C(CEditNaviMesh)
CLONE_C(CEditNaviMesh, CGameObject)

HRESULT CEditNaviMesh::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEditNaviMesh::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pRendererCom = Add_Component<CRenderer>();

    ZeroMemory(m_iPickingIndex, sizeof(_uint) * 3);

    m_pCellShaderCom = Add_Component<CShader>();
    m_pCellShaderCom.lock()->Set_ShaderInfo(TEXT("Shader_Cell"), VTXCUBETEX_DECLARATION::Element, VTXCUBETEX_DECLARATION::iNumElements);

    return S_OK;
}

HRESULT CEditNaviMesh::Start()
{
    return S_OK;
}

void CEditNaviMesh::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    switch (m_eEditType)
    {
        case EDIT_MODE::VERTICE:
        {
            Update_Vertice(fTimeDelta);
        }
        break;

        case EDIT_MODE::HEIGHT:
        {
            Update_Height(fTimeDelta);
        }
        break;

        case EDIT_MODE::POSITION_XY:
        {
            Update_Position(fTimeDelta);
        }
        break;
    }
}

void CEditNaviMesh::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    m_pRendererCom.lock()->Add_RenderGroup(RENDERGROUP::RENDER_NONLIGHT, Weak_StaticCast<CGameObject>(m_this));
}

HRESULT CEditNaviMesh::Render(ID3D11DeviceContext* pDeviceContext)
{
    if (!m_bEdit && !GAMEINSTANCE->Is_Debug())
    {
        return S_OK;
    }

#ifdef _DEBUG
    for (auto& elem : m_pPickPointColliderComs)
    {
        _vector WorldPos = XMLoadFloat3(&(elem.lock()->Get_ColliderDesc().vTranslation));
        WorldPos.m128_f32[3] = 1.f;

        if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(WorldPos))
        {
            elem.lock()->Render_IgnoreDebugCheck();
        }
    }

    m_pCellShaderCom.lock()->Set_RawValue("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
    m_pCellShaderCom.lock()->Set_RawValue("g_ViewMatrix" , (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
    m_pCellShaderCom.lock()->Set_RawValue("g_ProjMatrix" , (void*)GAMEINSTANCE->Get_Transform_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

    _float4 vColor(0.f, 1.f, 0.f, 0.1f);
    m_pCellShaderCom.lock()->Set_RawValue("g_vColor", &vColor, sizeof(_float4));

    m_pCellShaderCom.lock()->Begin(0, pDeviceContext);

    for (auto& elem : m_pCells)
    {
        elem.lock()->Render(pDeviceContext);
    }

    __super::Render(pDeviceContext);
#endif // _DEBUG

    return S_OK;
}

void CEditNaviMesh::Init_Collider(const _float& In_iWidth, const _float& In_iHeight, const _float& In_fPitch, ifstream* is)
{
    Remove_Components<CCollider>();
    Remove_Components<CVIBuffer_Cell>();

    m_pPickPointColliderComs.clear();
    m_pCells.clear();
    m_bSelectedPickPoints.clear();
    Reset_Picking();

    m_iWidthCount  = _uint(In_iWidth  / In_fPitch);
    m_iHeightCount = _uint(In_iHeight / In_fPitch);

    m_pPickPointColliderComs.reserve(m_iWidthCount * m_iHeightCount);

    _float3 vInputStreamPosition;

    for (_uint i(0); i < m_iHeightCount; ++i)
    {
        for (_uint j(0); j < m_iWidthCount; ++j)
        {
            weak_ptr<CCollider> pCollider = Add_Component<CCollider>();
            COLLIDERDESC ColliderDesc;
            ColliderDesc.iLayer = (_uint)COLLISION_LAYER::EDIT_NAVIMESH;
            //위치는 사이즈의 반만큼 가운데로 옮긴다.

            if (is)
            {
                read_typed_data(*is, vInputStreamPosition);
                ColliderDesc.vTranslation = vInputStreamPosition;
            }
            else
                ColliderDesc.vTranslation = _float3((_float)j * In_fPitch - (In_iWidth * 0.5f), 0.f, (_float)i * In_fPitch - (In_iHeight * 0.5f));

            ColliderDesc.vScale    = _float3(In_fPitch * 0.3f, 0.f, 0.f);
            ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 0.f);

            pCollider.lock()->Init_Collider(COLLISION_TYPE::SPHERE, ColliderDesc);

            m_pPickPointColliderComs.push_back(pCollider);
            m_bSelectedPickPoints.push_back(false);

        }
    }

    m_eEditType = EDIT_MODE::VERTICE;
}

void CEditNaviMesh::Load_DebugNaviMesh(const _char* In_szModelName)
{
    m_szNaviMeshName = In_szModelName;

    m_pModelData = GAMEINSTANCE->Get_ModelFromKey(m_szNaviMeshName.c_str());

    if (!m_pModelData)
        return;

    _uint VerticeCount(m_pModelData->iNumMeshs);

    for (_uint i(0); i < m_pModelData->iNumMeshs; i++)
    {
        weak_ptr<CVIBuffer_Cell> pCell = Add_Component<CVIBuffer_Cell>();
        pCell.lock()->Init_VIBufferCell(&m_pModelData->Mesh_Datas[i]->pPosVertices[0].vPosition);

        m_pCells.push_back(pCell);
    }
}

void CEditNaviMesh::Reset_Picking()
{
#ifdef _DEBUG
    for (auto& elem : m_pPickPointColliderComs)
    {
        elem.lock()->Set_DebugColor({ 1.f, 1.f, 1.f, 1.f });
    }
#endif // _DEBUG

    m_iPickingIndex[0]     = -1;
    m_iPickingIndex[1]     = -1;
    m_iPickingIndex[2]     = -1;
    m_iCurrentPickingIndex =  0;

    if (!m_bSelectedPickPoints.empty())
        fill(m_bSelectedPickPoints.begin(), m_bSelectedPickPoints.end(), false);

    m_iLastSelectedIndex = -1;
}

void CEditNaviMesh::Update_Vertice(_float fTimeDelta)
{
    //세개 픽킹 됨
    if (3 == m_iCurrentPickingIndex)
    {
        _bool isSameCell(true);
        _float3 vVerticePositions[3];

        list<weak_ptr<CVIBuffer_Cell>> pSameCells = m_pCells;

        for (_uint i(0); i < m_iCurrentPickingIndex; ++i)
        {
            _vector CurrentPosition = m_pPickPointColliderComs[m_iPickingIndex[i]].lock()->Get_CurrentPosition();
            XMStoreFloat3(&vVerticePositions[i], CurrentPosition);
        }

        SMath::Set_ClockwiseTriangle(vVerticePositions);

        for (_int i(0); i < 3; ++i)
        {
            for (auto iter = pSameCells.begin(); iter != pSameCells.end();)
            {
                if (!SMath::Equal_Float3(vVerticePositions[i], (*iter).lock()->Get_CellPosition(i)))
                    iter = pSameCells.erase(iter);
                else
                    ++iter;
            }
        }

        if (pSameCells.empty())
        {
            weak_ptr<CVIBuffer_Cell> pCell = Add_Component<CVIBuffer_Cell>();

            pCell.lock()->Init_VIBufferCell(vVerticePositions);
            m_pCells.push_back(pCell);
        }

        Reset_Picking();
    }

    if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
    {
        _int iSelected_Index = Select_PickPoint();

        if (-1 == iSelected_Index)
            return;

        if (iSelected_Index != m_iPickingIndex[0] &&
            iSelected_Index != m_iPickingIndex[1] &&
            iSelected_Index != m_iPickingIndex[2])
        {
            m_iPickingIndex[m_iCurrentPickingIndex] = iSelected_Index;

            _vector PickingColliderColor = { 0.f, 0.f, 0.f, 1.f };
            PickingColliderColor.m128_f32[m_iCurrentPickingIndex] = 1.f;

#ifdef _DEBUG
            m_pPickPointColliderComs[iSelected_Index].lock()->Set_DebugColor(PickingColliderColor);
#endif // _DEBUG

            ++m_iCurrentPickingIndex;
        }
    }

    if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
    {
        if (KEY_INPUT(KEY::Z, KEY_STATE::TAP))
        {
            if (m_pCells.empty())
                return;

            m_pCells.pop_back();
        }
    }

}

void CEditNaviMesh::Update_Height(_float fTimeDelta)
{
    if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
    {
        if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
            Select_MultiHeightPoints(fTimeDelta);
        else if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
            Select_RangeHeightPoints(fTimeDelta);
        else
            Select_SingleHeightPoint(fTimeDelta);
    }

    if (KEY_INPUT(KEY::UP, KEY_STATE::HOLD))
        Add_Height(1.f * fTimeDelta);

    if (KEY_INPUT(KEY::DOWN, KEY_STATE::HOLD))
        Add_Height(-1.f * fTimeDelta);
}

void CEditNaviMesh::Update_Position(_float fTimeDelta)
{
    if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
    {
        if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD))
            Select_MultiHeightPoints(fTimeDelta);
        else if (KEY_INPUT(KEY::LSHIFT, KEY_STATE::HOLD))
            Select_RangeHeightPoints(fTimeDelta);
        else
            Select_SingleHeightPoint(fTimeDelta);
    }

    const _float fMoveSpeed = 1.f;

    if (KEY_INPUT(KEY::UP, KEY_STATE::HOLD))
        Add_Pos(_float3(0.f, 0.f, fMoveSpeed * fTimeDelta));

    if (KEY_INPUT(KEY::DOWN, KEY_STATE::HOLD))
        Add_Pos(_float3(0.f, 0.f, -fMoveSpeed * fTimeDelta));

    if (KEY_INPUT(KEY::RIGHT, KEY_STATE::HOLD))
        Add_Pos(_float3(fMoveSpeed * fTimeDelta, 0.f, 0.f));

    if (KEY_INPUT(KEY::LEFT, KEY_STATE::HOLD))
        Add_Pos(_float3(-fMoveSpeed * fTimeDelta, 0.f, 0.f));
}

void CEditNaviMesh::Select_MultiHeightPoints(_float fTimeDelta)
{
    _int iSelected_Index = Select_PickPoint();

    if (-1 == iSelected_Index)
        return;

    m_iLastSelectedIndex = iSelected_Index;
    m_bSelectedPickPoints[iSelected_Index] = true;

#ifdef _DEBUG
    //선택하면 노란색으로
    m_pPickPointColliderComs[iSelected_Index].lock()->Set_DebugColor({ 1.f, 1.f, 0.f, 1.f });
#endif // _DEBUG

}

void CEditNaviMesh::Select_RangeHeightPoints(_float fTimeDelta)
{
    _int iSelected_Index = Select_PickPoint();

    if (-1 == iSelected_Index)
        return;

    if (-1 == m_iLastSelectedIndex)
    {
        m_iLastSelectedIndex = iSelected_Index;
        
        m_bSelectedPickPoints[iSelected_Index] = true;
        
#ifdef _DEBUG
        //선택하면 노란색으로
        m_pPickPointColliderComs[iSelected_Index].lock()->Set_DebugColor({ 1.f, 1.f, 0.f, 1.f });
#endif // _DEBUG

        return;
    }

    _uint iLastHeight = m_iLastSelectedIndex / m_iWidthCount;
    _uint iLastWidth  = m_iLastSelectedIndex % m_iWidthCount;

    _uint iNewHeight  = iSelected_Index / m_iWidthCount;
    _uint iNewWidth   = iSelected_Index % m_iWidthCount;

    _uint iMaxHeight  = max(iLastHeight, iNewHeight);
    _uint iMaxWidth   = max(iLastWidth, iNewWidth);

    _uint iMinHeight  = min(iLastHeight, iNewHeight);
    _uint iMinWidth   = min(iLastWidth, iNewWidth);

    for (_uint i(iMinHeight); i <= iMaxHeight; ++i)
    {
        for (_uint j(iMinWidth); j <= iMaxWidth; ++j)
        {
            m_bSelectedPickPoints[i * m_iWidthCount + j] = true;

#ifdef _DEBUG
            //선택하면 노란색으로
            m_pPickPointColliderComs[i * m_iWidthCount + j].lock()->Set_DebugColor({ 1.f, 1.f, 0.f, 1.f });
#endif // _DEBUG
           
        }
    }

    m_iLastSelectedIndex = iSelected_Index;
}

void CEditNaviMesh::Select_SingleHeightPoint(_float fTimeDelta)
{
    Reset_Picking();

    _int iSelected_Index = Select_PickPoint();

    if (-1 == iSelected_Index)
        return;

    m_iLastSelectedIndex = iSelected_Index;

    m_bSelectedPickPoints[iSelected_Index] = true;

#ifdef _DEBUG
    //선택하면 노란색으로
    m_pPickPointColliderComs[iSelected_Index].lock()->Set_DebugColor({ 1.f, 1.f, 0.f, 1.f });
#endif // _DEBUG

    
}

_int CEditNaviMesh::Select_PickPoint() const
{
    RAY MouseRayInWorldSpace = SMath::Get_MouseRayInWorldSpace(g_iWinCX, g_iWinCY);

    for (_size_t i(0); i < m_pPickPointColliderComs.size(); ++i)
    {
        _float3 vPickPointColliderPos = m_pPickPointColliderComs[i].lock()->Get_ColliderDesc().vTranslation;

        _vector CurrentColliderWorldPos = XMLoadFloat3(&vPickPointColliderPos);
        CurrentColliderWorldPos.m128_f32[3] = 1.f;

        //컬링된 콜라이더는 픽킹 검사를 하지 않음.
        if (!GAMEINSTANCE->isIn_Frustum_InWorldSpace(CurrentColliderWorldPos))
            continue;

        if (m_pPickPointColliderComs[i].lock()->Intersect_Ray(MouseRayInWorldSpace))
            return (_int)i;
    }

    return -1;
}

void CEditNaviMesh::Add_Height(const _float& In_fHeight)
{
    if (m_pPickPointColliderComs.empty())
        return;

    if (m_iCurrentPickingIndex >= m_pPickPointColliderComs.size())
        return;

    _float3 vHeight(0.f, In_fHeight, 0.f);

    for (_size_t i(0); i < m_pPickPointColliderComs.size(); ++i)
    {
        if (m_bSelectedPickPoints[i])
            m_pPickPointColliderComs[i].lock()->Update_AddPosition(vHeight);
    }
}

void CEditNaviMesh::Add_Pos(const _float3& In_vAddPosXY)
{
    if (m_pPickPointColliderComs.empty())
        return;

    if (m_iCurrentPickingIndex >= m_pPickPointColliderComs.size())
        return;

    for (_size_t i(0); i < m_pPickPointColliderComs.size(); ++i)
    {
        if (m_bSelectedPickPoints[i])
            m_pPickPointColliderComs[i].lock()->Update_AddPosition(In_vAddPosXY);
    }
}

void CEditNaviMesh::CellList()
{
    if (ImGui::BeginListBox("Cell List"))
    {
        static int Cellitem_current_idx = 0;

        for (_int i(0); i < m_pCells.size();)
        {
            const bool is_selected = (Cellitem_current_idx == i);

            if (m_pCells.empty())
                return;

            if (m_iPickingIndex[i] >= m_pPickPointColliderComs.size())
                continue;

            string szItem = string(" ( ") + to_string(i) + string(" ) ");

            if (ImGui::Selectable(szItem.c_str(), is_selected))
                Cellitem_current_idx = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndListBox();
    }
}

void CEditNaviMesh::SelectPointList()
{
    static int Pointitem_current_idx = 0;

    if (ImGui::BeginListBox("Select Point List"))
    {
        for (_uint i(0); i < 3; ++i)
        {
            const bool is_selected = (Pointitem_current_idx == i);

            if (m_pPickPointColliderComs.empty())
                return;

            if (0 > m_iPickingIndex[i])
                continue;

            if (m_iPickingIndex[i] >= m_pPickPointColliderComs.size())
                continue;

            _float3 vPos = m_pPickPointColliderComs[m_iPickingIndex[i]].lock()->Get_ColliderDesc().vTranslation;

            string szItem
                = string(" ( ") + to_string(i) + string(" ) ")
                + string(" Pos : [ ")
                + to_string(vPos.x) + string(" , ")
                + to_string(vPos.y) + string(" , ")
                + to_string(vPos.z) + string(" ]");

            if (ImGui::Selectable(szItem.c_str(), is_selected))
                Pointitem_current_idx = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndListBox();
    }
}

void CEditNaviMesh::Bake_NaviMesh()
{
    MODEL_DATA tModelData;

    tModelData.eModelType      = MODEL_TYPE::NAVI;
    tModelData.iNumAnimations  = 0;
    tModelData.iNumMaterials   = 0;
    tModelData.iNumMeshs       = (_uint)m_pCells.size();
    tModelData.szModelFileName = m_szNaviMeshName;
    tModelData.szModelFilePath = "../bin/NaviMeshData/" + m_szNaviMeshName + ".bin";
    XMStoreFloat4x4(&tModelData.TransformMatrix, XMMatrixIdentity());

    tModelData.RootNode = make_shared<NODE_DATA>();
    tModelData.RootNode->iNumChildren = 0;
    XMStoreFloat4x4(&tModelData.RootNode->TransformationMatrix, XMMatrixIdentity());

    _float3 vVertice[3];

    for (auto& elem : m_pCells)
    {
        shared_ptr<MESH_DATA> pMeshData = make_shared<MESH_DATA>();
        pMeshData->eModelType           = MODEL_TYPE::NAVI;
        pMeshData->iMaterialIndex       = 0;
        pMeshData->iNumBones            = 0;
        pMeshData->iNumFaces            = 0;
        pMeshData->iNumVertices         = 3;

        vVertice[0] = elem.lock()->Get_CellPosition(0);
        vVertice[1] = elem.lock()->Get_CellPosition(1);
        vVertice[2] = elem.lock()->Get_CellPosition(2);

        pMeshData->pPosVertices = shared_ptr<VTXPOS[]>(DBG_NEW VTXPOS[3]);

        for (_uint i(0); i < 3; ++i)
        {
            _float3 vCellPos = elem.lock()->Get_CellPosition(i);
            memcpy(&pMeshData->pPosVertices[i].vPosition, &vCellPos, sizeof(_float3));
        }

        tModelData.Mesh_Datas.push_back(pMeshData);
    }

    tModelData.Bake_Binary();

    GAMEINSTANCE->Load_Model(m_szNaviMeshName.c_str(), tModelData.szModelFilePath.c_str(), 
        MODEL_TYPE::NAVI, XMMatrixIdentity(), MEMORY_TYPE::MEMORY_STATIC);

    //Edit Write
    string szEditPath;
    szEditPath = "../bin/NaviMeshData/" + m_szNaviMeshName;
    szEditPath += "Edit.bin";

    ofstream os(szEditPath, ios::binary);

    write_typed_data(os, m_vSize);
    write_typed_data(os, m_fPitch);

    //write_typed_data(os, m_pPickPointColliderComs.size());

    for (_size_t i(0); i < m_pPickPointColliderComs.size(); ++i)
        write_typed_data(os, m_pPickPointColliderComs[i].lock()->Get_CurrentPosition());

    os.close();
}

void CEditNaviMesh::Load_NaviMesh()
{
    m_pModelData = GAMEINSTANCE->Get_ModelFromKey(m_szNaviMeshName.c_str());

    if (!m_pModelData)
        return;
    
    //Edit Load
    string szEditPath;
    szEditPath = "../bin/NaviMeshData/" + m_szNaviMeshName;
    szEditPath += "Edit.bin";

    ifstream is(szEditPath, ios::binary);

    read_typed_data(is, m_vSize);
    read_typed_data(is, m_fPitch);

    //_size_t PickPoinColliderSize;
    //read_typed_data(is, PickPoinColliderSize);

    Init_Collider(m_vSize.x, m_vSize.y, m_fPitch, &is);

    is.close();

    _uint VerticeCount = m_pModelData->iNumMeshs;

    for (_uint i(0); i < m_pModelData->iNumMeshs; i++)
    {
        weak_ptr<CVIBuffer_Cell> pCell = Add_Component<CVIBuffer_Cell>();
        pCell.lock()->Init_VIBufferCell(&m_pModelData->Mesh_Datas[i]->pPosVertices[0].vPosition);

        m_pCells.push_back(pCell);
    }
}

void CEditNaviMesh::OnEventMessage(_uint iArg)
{
    if ((_uint)EVENT_TYPE::ON_EDITINIT == iArg)
        m_bEdit = true;

    if ((_uint)EVENT_TYPE::ON_EDITDRAW == iArg)
    {
		if (ImGui::CollapsingHeader("Edit NaviMesh"), ImGuiTreeNodeFlags_DefaultOpen)
		{
			ImGui::Text("Size");
			ImGui::DragFloat2("##Size", &m_vSize.x, 1.f);
			ImGui::Separator();

			ImGui::Text("Pitch");
			ImGui::DragFloat("##Pitch", &m_fPitch, 0.5f);
			ImGui::Separator();

            if (ImGui::Button("Apply"))
                Init_Collider(m_vSize.x, m_vSize.y, m_fPitch);

            if (ImGui::Button("Reset Selection"))
                Reset_Picking();

            const char* items[] = 
            {
                "Vertice Editer",
                "Height Editer",
                "Position XY Editer",
            };

            if (KEY_INPUT(KEY::NUM1, KEY_STATE::HOLD))  
                m_eEditType = EDIT_MODE::VERTICE;
            else if (KEY_INPUT(KEY::NUM2, KEY_STATE::HOLD))  
                m_eEditType = EDIT_MODE::HEIGHT;
            else if (KEY_INPUT(KEY::NUM3, KEY_STATE::HOLD))  
                m_eEditType = EDIT_MODE::POSITION_XY;

            if (ImGui::BeginListBox("Editer Type"))
            {
                for (int n(0); n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = ((_uint)m_eEditType == n);

                    if (ImGui::Selectable(items[n], is_selected))
                        m_eEditType = (EDIT_MODE)n;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            ImGui::Separator();

            _char szName[64];
            strcpy_s(szName, m_szNaviMeshName.c_str());

            if (ImGui::InputText("NaviMeshName", szName, 64))
                m_szNaviMeshName = szName;

            if (ImGui::Button("Save"))
                Bake_NaviMesh();

            if (ImGui::Button("Load"))
                Load_NaviMesh();
		}

        if (EDIT_MODE::VERTICE == m_eEditType)
        {
            if (ImGui::CollapsingHeader("Cell Info"), ImGuiTreeNodeFlags_DefaultOpen)
            {
                if (ImGui::TreeNode("Select Point List"))
                {
                    SelectPointList();
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Craete Cell Info"))
                {
                    CellList();
                    ImGui::TreePop();
                }            
            }
        }
    }
}

void CEditNaviMesh::Free()
{
}
