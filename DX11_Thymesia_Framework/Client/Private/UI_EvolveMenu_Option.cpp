#include "stdafx.h"
#include "UI_EvolveMenu_Option.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "Texture.h"
#include "UI_Scroll.h"
#include "EasingComponent_Alpha.h"
#include "UIManager.h"
#include "UI_OptionItem.h"
#include "GameManager.h"
#include <Windows.h>
#include <iostream>
#include <Pdh.h>
#include "SubThread_Pool.h"
#include "UIManager.h"

GAMECLASS_C(CUI_EvolveMenu_Option)
CLONE_C(CUI_EvolveMenu_Option, CGameObject)


HRESULT CUI_EvolveMenu_Option::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_eRenderGroup = RENDERGROUP::RENDER_UI;

    m_fOptionItemOffsetY = 85.f;

    Init_UI();

    Init_OptionItem();

    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();

    m_HardwareInfos.resize(HARDWAREINFO_END);

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    DWORD dwNumberOfProcessors = sysinfo.dwNumberOfProcessors;

    m_CPU_usages.resize(dwNumberOfProcessors);
    m_CPU_RateInfos.resize(dwNumberOfProcessors);

    Set_Enable(false);

    return S_OK;
}

HRESULT CUI_EvolveMenu_Option::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_Option::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::Tick(fTimeDelta);

    if (m_fTextInfo_UpdateTimeAcc > 0.5f)
    {
        m_fTextInfo_UpdateTimeAcc = 0.f;
        Update_HardwareInfos();
    }
    else
    {
        m_fTextInfo_UpdateTimeAcc += fTimeDelta;
    }

    if (GET_SINGLE(CUIManager)->Is_OpenedMenu())
    {
        for (auto& elem : m_HardwareInfos)
        {
            GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, elem);
        }


        for (_int i = 0; i < m_CPU_RateInfos.size(); ++i)
        {
            GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_CPU_RateInfos[i]);
        }
    }

    for (DWORD i = 0; i < m_CPU_usages.size(); i++)
    {
        HQUERY query;
        PdhOpenQuery(NULL, NULL, &query);
        PDH_HCOUNTER counter;
        std::wstring counter_path = L"\\Processor(" + std::to_wstring(i) + L")\\% Processor Time";
        PdhAddEnglishCounter(query, counter_path.c_str(), NULL, &counter);
        PdhCollectQueryData(query);
        PDH_FMT_COUNTERVALUE value;
        PdhCollectQueryData(query);
        PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
        m_CPU_usages[i] += value.doubleValue;
        PdhCloseQuery(query);

    }
}

   


void CUI_EvolveMenu_Option::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_Option::Call_OnChangeOption(OPTION_TYPE eOption, _uint iAmount)
{
    GAMEINSTANCE->PlaySound2D("UI_ChangeIndex0", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT));

    Callback_OnChangeOption(eOption, iAmount);
    //or GAMEMANAGER Call

    GET_SINGLE(CGameManager)->Change_Option(eOption, iAmount);


}

void CUI_EvolveMenu_Option::Call_OnWheelMove(_float fScrollOffsetY)
{
    for (auto& elem : m_vecOptionItem)
    {
        elem.lock()->Set_ScrollOffsetY(-fScrollOffsetY);
    }
}

void CUI_EvolveMenu_Option::Init_UI()
{
    Set_UIPosition(26.f, 51.f, 686.f, 769.f, ALIGN_LEFTTOP);

    m_pBackground_Head = ADD_STATIC_CUSTOMUI;
    
    m_pBackground_Head.lock()->Set_UIPosition
    (
        24.f,
        0.f,
        693.f,
        200.f,
        ALIGN_LEFTTOP
    );

    m_pBackground_Head.lock()->Set_Texture("EvolveMenu_Option_BGHead");
    m_pBackground_Head.lock()->Set_Depth(0.9f);
    m_pBackground_Head.lock()->Set_AlphaColor(1.f);

    
    m_pBackground_Body = ADD_STATIC_CUSTOMUI;
    m_pBackground_Body.lock()->Set_UIPosition
    (
        24.f,
        200.f,
        693.f,
        510.f,
        ALIGN_LEFTTOP
    );
    m_pBackground_Body.lock()->Set_Texture("EvolveMenu_Option_BGBody");
    m_pBackground_Body.lock()->Set_Depth(0.9f);
    m_pBackground_Body.lock()->Set_AlphaColor(1.f);


    m_pBackground_Bottom = ADD_STATIC_CUSTOMUI;
    m_pBackground_Bottom.lock()->Set_UIPosition

    (
        24.f,
        710.f,
        693.f,
        190.f,
        ALIGN_LEFTTOP
    );

    m_pBackground_Bottom.lock()->Set_Texture("EvolveMenu_Option_BGBottom");
    m_pBackground_Bottom.lock()->Set_Depth(0.9f);
    m_pBackground_Bottom.lock()->Set_AlphaColor(1.f);

    m_pScroll = GAMEINSTANCE->Add_GameObject<CUI_Scroll>(LEVEL_STATIC);
    m_pScroll.lock()->SetUp_ScrollFromLeftTop(666.f, 201.f, 20.f, 500.f, 500.f, 1360.f);

    m_pScroll.lock()->Callback_OnWheelMove += bind(&CUI_EvolveMenu_Option::Call_OnWheelMove, this, placeholders::_1);

    Add_Child(m_pScroll);
    Add_Child(m_pBackground_Head);
    Add_Child(m_pBackground_Body);
    Add_Child(m_pBackground_Bottom);


    //이 위로 올라가는 아이템이 가려지도록 헤드는 렌더그룹을 다르게 준다.
    m_pBackground_Head.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);
    m_pBackground_Bottom.lock()->Set_RenderGroup(RENDERGROUP::RENDER_AFTER_UI);
}

void CUI_EvolveMenu_Option::Init_OptionItem()
{
    weak_ptr<CUI_OptionItem> pItem;

    for (_uint i = 0; i < (_uint)OPTION_TYPE::OPTION_TYPE_END; i++)
    {
        pItem = GAMEINSTANCE->Add_GameObject<CUI_OptionItem>(LEVEL_STATIC);
     
        pItem.lock()->Set_UIPosition
        (
            350.f,
            241.f + (i * m_fOptionItemOffsetY),
            600.f,
            70.f
        );
        pItem.lock()->Set_OriginCenterPosFromThisPos();
        
        pItem.lock()->Create_Element((OPTION_TYPE)i);

        pItem.lock()->Callback_OnChangeOption += bind(&CUI_EvolveMenu_Option::Call_OnChangeOption, this, placeholders::_1, placeholders::_2);

        m_vecOptionItem.push_back(pItem);
        Add_Child(pItem);
    }
}

void CUI_EvolveMenu_Option::Update_HardwareInfos()
{
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);
    if (error != cudaSuccess) {
        return;
    }

    _float xPosition = 770.f;

    m_HardwareInfos[GPU_NAME].bAlways = false;
    m_HardwareInfos[GPU_NAME].bCenterAlign = false;
    m_HardwareInfos[GPU_NAME].eRenderGroup = RENDERGROUP::RENDER_UI;
    m_HardwareInfos[GPU_NAME].vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_HardwareInfos[GPU_NAME].vPosition = _float2(xPosition, 10.f);
    m_HardwareInfos[GPU_NAME].vScale = _float2(0.7f, 0.7f);

    /*m_HardwareInfos[GPU_LOAD].bAlways = false;
    m_HardwareInfos[GPU_LOAD].bCenterAlign = false;
    m_HardwareInfos[GPU_LOAD].eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;
    m_HardwareInfos[GPU_LOAD].vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_HardwareInfos[GPU_LOAD].vPosition = _float2(1200.f, 110.f);
    m_HardwareInfos[GPU_LOAD].vScale = _float2(1.f, 1.f);*/

    m_HardwareInfos[GPU_MEMORYUSAGE].bAlways = false;
    m_HardwareInfos[GPU_MEMORYUSAGE].bCenterAlign = false;
    m_HardwareInfos[GPU_MEMORYUSAGE].eRenderGroup = RENDERGROUP::RENDER_UI;
    m_HardwareInfos[GPU_MEMORYUSAGE].vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_HardwareInfos[GPU_MEMORYUSAGE].vPosition = _float2(xPosition, 30.f);
    m_HardwareInfos[GPU_MEMORYUSAGE].vScale = _float2(0.7f, 0.7f);


    


    cudaDeviceProp deviceProperties;
    cudaGetDeviceProperties(&deviceProperties, 0);
   // m_HardwareInfos[GPU_NAME].szText = L"GPU: " + filesystem::path(deviceProperties.name).wstring();

    cudaSetDevice(0);
    cudaDeviceSynchronize();
    error = cudaGetLastError();
    if (error != cudaSuccess) {
        return;
    }

    size_t GMemoryUsing, GMemoryTotal;
    cudaMemGetInfo(&GMemoryUsing, &GMemoryTotal);
    GMemoryUsing = GMemoryTotal - GMemoryUsing;
    _double GMemoryFreeToGB, GMemoryTotalToGB;
    GMemoryFreeToGB = GMemoryUsing / (1024.0 * 1024.0 * 1024.0);
    GMemoryTotalToGB = GMemoryTotal / (1024.0 * 1024.0 * 1024.0);

    wstring szMemoryText;
    szMemoryText = L"VRAM: " + to_wstring(_int(GMemoryFreeToGB)) + L"." + to_wstring(_int((GMemoryFreeToGB - _int(GMemoryFreeToGB)) * 10)) + L"GB / " +
        to_wstring(_int(GMemoryTotalToGB)) + L"." + to_wstring(_int((GMemoryTotalToGB - _int(GMemoryTotalToGB)) * 10)) + L"GB";

    m_HardwareInfos[GPU_MEMORYUSAGE].szText = szMemoryText;
    
    //std::cout << "Total global memory: " << deviceProperties.totalGlobalMem / (1024 * 1024) << "MB" << std::endl;
    //std::cout << "Shared memory per block: " << deviceProperties.sharedMemPerBlock / 1024 << "KB" << std::endl;
    
    //boost::process::system_info

    

    /*_int iCapturedCallCount = m_iCallCount;

    if (iCapturedCallCount > 0)
    {
        for (DWORD i = 0; i < m_CPU_usages.size(); i++)
        {
            m_CPU_RateInfos[i].bAlways = false;
            m_CPU_RateInfos[i].bCenterAlign = false;
            m_CPU_RateInfos[i].eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;
            m_CPU_RateInfos[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
            m_CPU_RateInfos[i].vPosition = _float2(xPosition, 60.f + i * 20.f);
            m_CPU_RateInfos[i].vScale = _float2(0.7f, 0.7f);
            m_CPU_RateInfos[i].szText = L"CPU" + to_wstring(i) + L": " + to_wstring((_int)((m_CPU_usages[i] * 2.f) / iCapturedCallCount)) + L" % ";
            m_CPU_usages[i] = 0.0;
        }
        m_iCallCount = 0;
    }*/

    //for (DWORD i = 0; i < m_CPU_usages.size(); i++)
    //{
    //    m_CPU_RateInfos[i].bAlways = false;
    //    m_CPU_RateInfos[i].bCenterAlign = false;
    //    m_CPU_RateInfos[i].eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;
    //    m_CPU_RateInfos[i].vColor = _float4(1.f, 1.f, 1.f, 1.f);
    //    m_CPU_RateInfos[i].vPosition = _float2(xPosition, 60.f + i * 20.f);
    //    m_CPU_RateInfos[i].vScale = _float2(0.7f, 0.7f);
    //    m_CPU_RateInfos[i].szText = L"CPU" + to_wstring(i) + L": " + to_wstring((_int)((m_CPU_usages[i] * 2.f))) + L" % ";
    //    m_CPU_usages[i] = 0.0;
    //}

    /*for (DWORD i = 0; i < dwNumberOfProcessors; i++)
    {
        std::wcout << "Average utilization of Processor " << i << ": "
            << m_CPU_usages[i] / 5 << "%" << std::endl;
        m_CPU_usages[i] = 0;
    }*/

    /*static int count = 0;
    if (++count >= 10)
    {
        for (DWORD i = 0; i < dwNumberOfProcessors; i++)
        {
            std::wcout << "Average utilization of Processor " << i << ": "
                << m_CPU_usages[i] / 10 << "%" << std::endl;
            m_CPU_usages[i] = 0;
        }
        count = 0;
    }*/

    // Get the system's processors usage
    //for (DWORD i = 0; i < dwNumberOfProcessors; i++)
    //{
    //    HQUERY query;
    //    PdhOpenQuery(NULL, NULL, &query);
    //    PDH_HCOUNTER counter;
    //    std::wstring counter_path = L"\\Processor(" + std::to_wstring(i) + L")\\% Processor Time";
    //    PdhAddEnglishCounter(query, counter_path.c_str(), NULL, &counter);
    //    PdhCollectQueryData(query);
    //    PDH_FMT_COUNTERVALUE value;
    //    PdhCollectQueryData(query);
    //    PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
    //    std::wcout << "Current utilization of Processor " << i << ": " << value.doubleValue << "%" << std::endl;
    //    PdhCloseQuery(query);

    //    PdhOpenQuery(NULL, NULL, &query);
    //    counter_path = L"\\Processor(" + std::to_wstring(i) + L")\\% Processor Time";
    //    PdhAddEnglishCounter(query, counter_path.c_str(), NULL, &counter);
    //    PdhCollectQueryData(query);
    //    PDH_FMT_COUNTERVALUE value1;
    //    PdhCollectQueryData(query);
    //    PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value1);

    //    // Sleep for the measurement interval
    //    Sleep(1000);

    //    PdhCollectQueryData(query);
    //    PDH_FMT_COUNTERVALUE value2;
    //    PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value2);
    //    double average_usage = (value2.doubleValue - value1.doubleValue) / 1000;
    //    std::wcout << "Average utilization of Processor " << i << ": " << average_usage << "%" << std::endl;
    //    PdhCloseQuery(query);
    //}

    //m_HardwareInfos[GPU_LOAD].szText = to_wstring(_int(usage)) + L"%";

}

void CUI_EvolveMenu_Option::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);
    
    //m_pEasingAlpha.lock()->Set_Lerp_once(0.f, 1.f, 0.5f, EASING_TYPE::QUAD_IN, false);

    GET_SINGLE(CGameManager)->Activate_Zoom(-1.5f, 0.5f, EASING_TYPE::CUBIC_OUT, DIRECTION::RIGHT);
    GET_SINGLE(CGameManager)->Set_MoveTargetCamera(false);


    GET_SINGLE(CUIManager)->EnableCursor();

    m_fTextInfo_UpdateTimeAcc = 0.f;
    Update_HardwareInfos();

    m_bLoopStop = false;
    //m_pCheckCoreUsageThreadPool = CSubThread_Pool::Create(1);

    /*m_pCheckCoreUsageThreadPool->Enqueue_Job([this]() {
        while (!m_bLoopStop)
        {
            for (DWORD i = 0; i < m_CPU_usages.size(); i++)
            {
                HQUERY query;
                PdhOpenQuery(NULL, NULL, &query);
                PDH_HCOUNTER counter;
                std::wstring counter_path = L"\\Processor(" + std::to_wstring(i) + L")\\% Processor Time";
                PdhAddEnglishCounter(query, counter_path.c_str(), NULL, &counter);
                PdhCollectQueryData(query);
                PDH_FMT_COUNTERVALUE value;
                PdhCollectQueryData(query);
                PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
                m_CPU_usages[i] += value.doubleValue;
                PdhCloseQuery(query);
                
            }
            ++m_iCallCount;
        }
        });*/
}

void CUI_EvolveMenu_Option::OnDisable()
{
    __super::OnDisable();

    GET_SINGLE(CUIManager)->DisableCursor();
    GET_SINGLE(CGameManager)->Deactivate_Zoom(1.5f, EASING_TYPE::QUINT_OUT);
    GET_SINGLE(CGameManager)->Set_MoveTargetCamera(true);

    m_bLoopStop = true;
    //m_pCheckCoreUsageThreadPool.reset();
}

void CUI_EvolveMenu_Option::Free()
{
    if (m_pCheckCoreUsageThreadPool)
    {
        m_bLoopStop = true;
        //m_pCheckCoreUsageThreadPool.reset();
    }
}
