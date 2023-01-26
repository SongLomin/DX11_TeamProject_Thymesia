#include "stdafx.h"
#include "Fog.h"

#include "GameInstance.h"
#include "GameManager.h"
#include "imgui.h"
#include "SMath.h"
#include "Easing_Utillity.h"

GAMECLASS_C(CFog);
CLONE_C(CFog, CGameObject);

HRESULT CFog::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFog::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    return S_OK;
}

HRESULT CFog::Start()
{
    __super::Start();

    GET_SINGLE(CGameManager)->Registration_Fog(Weak_Cast<CFog>(m_this));

    if (!m_FogList.empty())
    {
        GAMEINSTANCE->Set_FogDesc(m_FogList[0].vColor, m_FogList[0].fRange);
    }

    CallBack_Act += bind(&CFog::Act_Fog, this, placeholders::_1);

    return S_OK;
}

void CFog::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    /*if (KEY_INPUT(KEY::M, KEY_STATE::HOLD) && KEY_INPUT(KEY::LEFT, KEY_STATE::TAP))
    {
        GAMEINSTANCE->Set_FogDesc(m_FogList[0].vColor, m_FogList[0].fRange);
    }

    else if (KEY_INPUT(KEY::M, KEY_STATE::HOLD) && KEY_INPUT(KEY::RIGHT, KEY_STATE::TAP))
    {
        GAMEINSTANCE->Set_FogDesc(_float4(0.f, 0.f, 0.f, 0.f), 1000.f);
    }*/

    if (m_bAction)
        CallBack_Act(fTimeDelta);
}

void CFog::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CFog::Render(ID3D11DeviceContext* pDeviceContext)
{
    return S_OK;
}

void CFog::Act_Fog(_float fTimeDelta)
{
    if (m_iPreIndex == m_iCurIndex)
    {
        m_bAction = false;
        GAMEINSTANCE->Set_FogDesc(m_FogList[m_iCurIndex].vColor, m_FogList[m_iCurIndex].fRange);

        return;
    }

    m_fAccTime += fTimeDelta;

    if (m_FogList[m_iCurIndex].fTime <= m_fAccTime)
    {
        m_bAction  = false;
        m_fAccTime = 0.f;

        GAMEINSTANCE->Set_FogDesc(m_FogList[m_iCurIndex].vColor, m_FogList[m_iCurIndex].fRange);

        return;
    }
    
    _vector vColorDistance = CEasing_Utillity::Linear(XMLoadFloat4(&m_FogList[m_iPreIndex].vColor), XMLoadFloat4(&m_FogList[m_iCurIndex].vColor), m_fAccTime, m_FogList[m_iCurIndex].fTime);
    _float4 vColor;
    XMStoreFloat4(&vColor, vColorDistance);

    _float fRange = CEasing_Utillity::Linear(m_FogList[m_iPreIndex].fRange, m_FogList[m_iCurIndex].fRange, m_fAccTime, m_FogList[m_iCurIndex].fTime);

    cout << "Fog Range : " << fRange << endl;

    GAMEINSTANCE->Set_FogDesc(vColor, fRange);
}

void CFog::Activate_Fog(_uint _iFogIndex)
{
    if (m_FogList.size() <= _iFogIndex)
        return;

    m_bAction   = true;
    m_fAccTime  = 0.f;
    m_iPreIndex = m_iCurIndex;
    m_iCurIndex = _iFogIndex;
}

void CFog::OnEventMessage(_uint iArg)
{
    switch ((EVENT_TYPE)iArg)
    {
        case EVENT_TYPE::ON_EDITDRAW:
        {
            static FOG_DESC Desc;
            static _int iSelectIndex = 0;

            if (ImGui::BeginListBox("##Fog List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (_uint n = 0; n < (_uint)m_FogList.size(); n++)
                {
                    const bool is_selected = ((_uint)iSelectIndex == n);

                    string szTag 
                        = "(" + to_string(n) + ") "
                        + "(" + to_string(m_FogList[n].vColor.x)
                        + ", " + to_string(m_FogList[n].vColor.y)
                        + ", " + to_string(m_FogList[n].vColor.z)
                        + ") / " + to_string(m_FogList[n].fRange);

                    if (ImGui::Selectable(szTag.c_str(), is_selected))
                    {
                        iSelectIndex = n;
                        Desc = m_FogList[n];
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }

            ImGui::InputFloat4("FogColor", &Desc.vColor.x);
            ImGui::InputFloat("FogRange", &Desc.fRange);
            ImGui::InputFloat("FogTime", &Desc.fTime);

            if (ImGui::Button("Add", ImVec2(100.f, 25.f)))
            {
                m_FogList.push_back(Desc);
            }

            ImGui::SameLine();

            if (ImGui::Button("Del", ImVec2(100.f, 25.f)))
            {
                if ((_uint)m_FogList.size() <= iSelectIndex)
                    return;

                vector<FOG_DESC>::iterator iter_find = m_FogList.begin() + iSelectIndex;
                
                m_FogList.erase(iter_find);
                iSelectIndex = 0;
            }

            if (ImGui::Button("Change", ImVec2(100.f, 25.f)))
            {
                if ((_uint)m_FogList.size() <= iSelectIndex)
                    return;

                vector<FOG_DESC>::iterator iter_find = m_FogList.begin() + iSelectIndex;

                *iter_find = Desc;
            }

            ImGui::SameLine();

            if (ImGui::Button("Action", ImVec2(100.f, 25.f)))
            {
                if ((_uint)m_FogList.size() <= iSelectIndex)
                    return;

                m_bAction   = true;
                m_iPreIndex = m_iCurIndex;
                m_iCurIndex = iSelectIndex;
                m_fAccTime  = 0.f;
            }
        }
        break;
    }
}

void CFog::Write_Json(json& Out_Json)
{
    Out_Json["FogSize"] = (_uint)m_FogList.size();

    for (_uint i = 0; i < m_FogList.size(); ++i)
    {
        Out_Json["FogDesc"][i]["Time"]  = m_FogList[i].fTime;
        Out_Json["FogDesc"][i]["Range"] = m_FogList[i].fRange;
        CJson_Utility::Write_Float4(Out_Json["FogDesc"][i]["Color"], m_FogList[i].vColor);
    }
}

void CFog::Load_FromJson(const json& In_Json)
{
    _uint iSize = In_Json["FogSize"];

    FOG_DESC Desc;

    for (_uint i = 0; i < iSize; ++i)
    {
        Desc.fTime  = In_Json["FogDesc"][i]["Time"];
        Desc.fRange = In_Json["FogDesc"][i]["Range"];
        CJson_Utility::Load_Float4(In_Json["FogDesc"][i]["Color"], Desc.vColor);

        m_FogList.push_back(Desc);
    }
}

void CFog::OnDestroy()
{
    GAMEINSTANCE->Set_FogDesc(_float4(0.f, 0.f, 0.f, 0.f), 1000.f);
    m_FogList.clear();
}

void CFog::Free()
{
}
