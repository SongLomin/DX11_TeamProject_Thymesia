#include "stdafx.h"
#include "EffectGroup.h"
#include "CustomEffectMesh.h"
#include "Effect_Rect.h"
#include "GameManager.h"

GAMECLASS_C(CEffectGroup)
CLONE_C(CEffectGroup, CGameObject)

_bool CEffectGroup::Is_EffectMesh(const _uint& In_iIndex) const
{
    _int iIndex = (_int)In_iIndex + 1;

    iIndex -= m_pEffectMeshs.size();


    // 예: 이펙트 메쉬 3개, 인덱스 2일때 3-(2+1) = 0, 0번도 이펙트 메쉬다.
    return iIndex <= 0;
}

weak_ptr<CCustomEffectMesh> CEffectGroup::Get_EffectMesh(const _uint& In_iIndex) const
{
    return m_pEffectMeshs[In_iIndex];
}

weak_ptr<CEffect_Rect> CEffectGroup::Get_Particle(const _uint& In_iIndex) const
{
    return m_pEffectParticles[In_iIndex];
}

void CEffectGroup::Set_EffectGroupName(const _char* szName)
{
    m_szEffectGroupName = szName;
}

vector<string> CEffectGroup::Get_EffectMeshKeys()
{
    vector<string> EffectKeys;

    for (auto& elem : m_pEffectMeshs)
    {
        EffectKeys.emplace_back(elem.lock()->Get_EffectName());
    }

    return EffectKeys;
}

vector<string> CEffectGroup::Get_EffectParticleKeys()
{
    vector<string> EffectKeys;

    for (auto& elem : m_pEffectParticles)
    {
        EffectKeys.emplace_back(elem.lock()->Get_EffectName());
    }

    return EffectKeys;
}

void CEffectGroup::Set_TimeScaleLayerForEffects(const _uint& In_iTimeScaleLayer)
{
    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->Set_TimeScaleLayer(In_iTimeScaleLayer);
    }

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->Set_TimeScaleLayer(In_iTimeScaleLayer);
    }
}

HRESULT CEffectGroup::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CEffectGroup::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    return S_OK;
}

void CEffectGroup::Tick(_float fTimeDelta)
{
    if (!m_bUsing)
        return;

    __super::Tick(fTimeDelta);

    if (Check_Finish())
    {
        m_bUsing = false;
        return;
    }



}

void CEffectGroup::LateTick(_float fTimeDelta)
{
    if (!m_bUsing)
        return;

    __super::LateTick(fTimeDelta);

}

HRESULT CEffectGroup::Render()
{

    __super::Render();

    return S_OK;
}

void CEffectGroup::Add_EditEffectMesh(const EFFECTMESH_DESC& In_tEffectMeshDesc, const _char* In_szModelKey)
{
    weak_ptr<CCustomEffectMesh> pEffectMesh = GAMEINSTANCE->Add_GameObject<CCustomEffectMesh>(LEVEL_STATIC);
    pEffectMesh.lock()->Init_EffectMesh(In_tEffectMeshDesc, In_szModelKey);
    m_pEffectMeshs.emplace_back(pEffectMesh);
    pEffectMesh.lock()->Set_TimeScaleLayer((_uint)TIMESCALE_LAYER::EDITER);

}

void CEffectGroup::Add_EditParticle(const _char* In_szName, const _char* In_szTextureKey)
{
    weak_ptr<CEffect_Rect> pEffectPoint = GAMEINSTANCE->Add_GameObject<CEffect_Rect>(LEVEL_STATIC);
    pEffectPoint.lock()->Init_EffectParticle(In_szName, In_szTextureKey);
    pEffectPoint.lock()->Set_TimeScaleLayer((_uint)TIMESCALE_LAYER::EDITER);

    m_pEffectParticles.emplace_back(pEffectPoint);
}

void CEffectGroup::ReBake_EditParticle()
{
    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->ReBake_EditParticle();
    }
}

void CEffectGroup::Add_EffectMesh()
{
    weak_ptr<CCustomEffectMesh> pEffectMesh = GAMEINSTANCE->Add_GameObject<CCustomEffectMesh>(LEVEL_STATIC);
    m_pEffectMeshs.emplace_back(pEffectMesh);
}

void CEffectGroup::Add_Particle()
{
    weak_ptr<CEffect_Rect> pEffectParticle = GAMEINSTANCE->Add_GameObject<CEffect_Rect>(LEVEL_STATIC);
    m_pEffectParticles.emplace_back(pEffectParticle);
}

void CEffectGroup::Remove_EffectMesh(weak_ptr<CCustomEffectMesh> pEffectMesh)
{
    for (auto iter = m_pEffectMeshs.begin(); iter != m_pEffectMeshs.end();)
    {
        if ((*iter).lock() == pEffectMesh.lock())
        {
            (*iter).lock()->Set_Dead();
            m_pEffectMeshs.erase(iter);
            break;
        }

        else
        {
            ++iter;
        }
    }
}

void CEffectGroup::Remove_EffectParticle(weak_ptr<CEffect_Rect> pEffectParticle)
{
    for (auto iter = m_pEffectParticles.begin(); iter != m_pEffectParticles.end();)
    {
        if ((*iter).lock() == pEffectParticle.lock())
        {
            (*iter).lock()->Set_Dead();
            m_pEffectParticles.erase(iter);
            break;
        }

        else
        {
            ++iter;
        }
    }

}

void CEffectGroup::Remove_AllEffects()
{

    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->Set_Dead();
    }

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->Set_Dead();
    }
}

void CEffectGroup::Sync_Animation()
{
    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->Sync_Animation();
    }

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->Sync_Animation();
    }

}

void CEffectGroup::Play(_float fTimeDelta)
{
    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->Play(fTimeDelta);
    }
}

void CEffectGroup::Reset_Effects()
{
    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->Reset_Effect();
    }

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->Reset_Effect(weak_ptr<CTransform>());
    }

}

void CEffectGroup::Reset_Effects(weak_ptr<CTransform> pParentTransformCom)
{
    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->Reset_Effect(pParentTransformCom);
    }

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->Reset_Effect(pParentTransformCom);
    }
}

_bool CEffectGroup::Use_EffectGroup(weak_ptr<CTransform> pParentTransformCom, const _uint& In_iTimeScaleLayer)
{
    if (m_bUsing)
        return false;

    m_bUsing = true;
    Reset_Effects(pParentTransformCom);
    
    // uint 제일 끝 값
    if (In_iTimeScaleLayer != (_uint)-1)
    {
        m_iTimeScaleLayer = In_iTimeScaleLayer;
        Set_TimeScaleLayerForEffects(m_iTimeScaleLayer);
    }


    return true;
}

void CEffectGroup::UnUse_EffectGroup()
{
    /*if (!m_bUsing)
    {
        DEBUG_ASSERT;
    }*/

    for (auto& elem : m_pEffectMeshs)
    {
        if (elem.lock())
            elem.lock()->Set_Enable(false);
    }

    for (auto& elem : m_pEffectParticles)
    {
        if(elem.lock())
            elem.lock()->UnUse_EffectParticle();
    }
}

weak_ptr<CEffectGroup> CEffectGroup::Clone_EffectGroup()
{
    weak_ptr<CEffectGroup> m_pClonedEffectGroup = GAMEINSTANCE->Add_GameObject<CEffectGroup>(m_iCreatedLevel);
    m_pClonedEffectGroup.lock()->Load_EffectJson(m_szPath, m_iTimeScaleLayer, m_iCreatedLevel);
    
    return m_pClonedEffectGroup;
}

void CEffectGroup::Load_FromBinary(const string& In_szPath)
{      
#pragma region 바이너리_불러오기
    ifstream is(In_szPath, ios::binary);

#ifdef _DEBUG
    assert(is.is_open());
#endif // _DEBUG

    size_t istringSize;
    read_typed_data(is, istringSize);
    m_szEffectGroupName.resize(istringSize);
    is.read(&m_szEffectGroupName[0], istringSize);

    size_t iEffectMeshSize;

    read_typed_data(is, iEffectMeshSize);

    for (_size_t i = 0; i < iEffectMeshSize; ++i)
    {
        read_typed_data(is, istringSize);
        string szModelKey;
        szModelKey.resize(istringSize);
        is.read(&szModelKey[0], istringSize);

        EFFECTMESH_DESC EffectMeshDesc;
        read_typed_data(is, EffectMeshDesc);

        Add_EditEffectMesh(EffectMeshDesc, szModelKey.c_str());
    }

    is.close();
#pragma endregion


}

void CEffectGroup::Write_FromBinary()
{
    string szPath = "../Bin/EffectData/";
    szPath += m_szEffectGroupName;
    szPath += ".bin";

    ofstream os(szPath, ios::binary);

    if (!os.is_open())
    {
        DEBUG_ASSERT;
        return;
    }

    write_typed_data(os, m_szEffectGroupName.size());
    os.write(&m_szEffectGroupName[0], m_szEffectGroupName.size());
    
    write_typed_data(os, m_pEffectMeshs.size());

    for (_size_t i = 0; i < m_pEffectMeshs.size(); ++i)
    {
        m_pEffectMeshs[i].lock()->Write_FromBinary(os);
    }

    os.close();

}

void CEffectGroup::Load_EffectJson(const string& In_szPath, const _uint& In_iTimeScaleLayer, const _uint& In_iCreatedLevel)
{
    json Load_Json;
    CJson_Utility::Load_Json(In_szPath.c_str(), Load_Json);

    m_szEffectGroupName = Load_Json["EffectGroupName"];
    _size_t iEffectMeshCount = Load_Json["EffectMeshCount"];
    _size_t iEffectParticleCount = Load_Json["EffectParticleCount"];

    for (_int i = 0; i < (_int)iEffectMeshCount; ++i)
    {
        Add_EffectMesh();
        m_pEffectMeshs[i].lock()->Load_EffectJson(Load_Json["EffectMesh"][to_string(i)], In_iTimeScaleLayer);
    }

    for (_int i = 0; i < (_int)iEffectParticleCount; i++)
    {
        Add_Particle();
        m_pEffectParticles[i].lock()->Load_EffectJson(Load_Json["EffectParticle"][to_string(i)], In_iTimeScaleLayer);
    }



    UnUse_EffectGroup();

    m_szPath = In_szPath;
    m_iTimeScaleLayer = In_iTimeScaleLayer;
    m_iCreatedLevel = In_iCreatedLevel;

    GET_SINGLE(CGameManager)->Register_EffectGroup(m_szEffectGroupName,
        Weak_Cast<CEffectGroup>(m_this));

}

void CEffectGroup::Write_EffectJson(json& Out_Json)
{
    Out_Json["EffectGroupName"] = m_szEffectGroupName;
    Out_Json["EffectMeshCount"] = m_pEffectMeshs.size();
    Out_Json["EffectParticleCount"] = m_pEffectParticles.size();

    _int iIndex = 0;

    for (auto& elem : m_pEffectMeshs)
    {
        Out_Json[to_string(iIndex)].emplace();

        elem.lock()->Write_EffectJson(Out_Json["EffectMesh"][to_string(iIndex)]);

        ++iIndex;
    }

    iIndex = 0;

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->Write_EffectJson(Out_Json["EffectParticle"][to_string(iIndex)]);

        ++iIndex;
    }


}

_bool CEffectGroup::Check_Finish()
{
    for (auto& elem : m_pEffectMeshs)
    {
        if (elem.lock()->Get_Enable())
            return false;
    }

    for (auto& elem : m_pEffectParticles)
    {
        if (elem.lock()->Get_Enable())
            return false;
    }

    return true;
}

void CEffectGroup::OnChangeAnimationKey(const _uint& In_Key)
{
    for (auto& elem : m_pEffectMeshs)
    {
        elem.lock()->OnChangeAnimationKey(In_Key);
    }

    for (auto& elem : m_pEffectParticles)
    {
        elem.lock()->OnChangeAnimationKey(In_Key);
    }

}

void CEffectGroup::Free()
{

}
