// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "imgui.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HWND	g_hWnd;
HINSTANCE g_hInst; // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#ifdef _DEBUG
void D3DMemoryLeakCheck()
{
    HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
    decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));

    IDXGIDebug* debug;

    GetDebugInterface(IID_PPV_ARGS(&debug));

    OutputDebugStringW(L"▽▽▽▽▽▽▽▽▽▽ Direct3D Object ref count 메모리 누수 체크 ▽▽▽▽▽▽▽▽▽▽▽\r\n");
    debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    OutputDebugStringW(L"△△△△△△△△△△ 반환되지 않은 IUnknown 객체가 있을경우 위에 나타납니다. △△△△△△△△△△△△\r\n");

    debug->Release();
}
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
    FILE* ConsoleStream;

    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen_s(&ConsoleStream, "CON", "w", stdout);

    printf("HELLO!!! I AM THE CONSOLE!\n");
#endif

    srand((_uint)time(nullptr));
    // TODO: 여기에 코드를 입력합니다.
    unique_ptr<CMainApp> pMainApp;

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    pMainApp = CMainApp::Create();
    if (nullptr == pMainApp.get())
        return FALSE;


    // 기본 메시지 루프입니다.

    if (FAILED(GAMEINSTANCE->Add_Timer((_uint)TIMER::TIMER_DEFAULT)))
        return FALSE;
    if (FAILED(GAMEINSTANCE->Add_Timer((_uint)TIMER::TIMER_FRAME)))
        return FALSE;

    _float		fTimerAcc = 0.f;
    _float      fDeltaTime = 0.f;

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        fTimerAcc += GAMEINSTANCE->Compute_Timer((_uint)TIMER::TIMER_DEFAULT);

        if (fTimerAcc > 0.00694f) //144Hz
        {
            pMainApp->Tick(fTimerAcc);

            fTimerAcc = 0.f;

            if (FAILED(pMainApp->Render()))
                break;
        }

        //프레임 제한 해제
        //fDeltaTime = GAMEINSTANCE->Compute_Timer((_uint)TIMER::TIMER_DEFAULT);
        ////fTimerAcc += fDeltaTime;
        //pMainApp->Tick(fDeltaTime);
        //if (FAILED(pMainApp->Render()))
        //    break;

        //if (fTimerAcc > 0.00694f) //144Hz
        //{
        //    fTimerAcc = 0.f;

        //    pMainApp->Tick(GAMEINSTANCE->Compute_Timer((_uint)TIMER::TIMER_FRAME));

        //    if (FAILED(pMainApp->Render()))
        //        break;
        //}
    }

    pMainApp.reset();

#ifdef  _DEBUG
    fclose(ConsoleStream);
    D3DMemoryLeakCheck();
#endif //  _DEBUG

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance;
	RECT rcWindow{ 0, 0, static_cast<_long>(g_iWinCX), static_cast<_long>(g_iWinCY) };
	LONG WndWidth{ rcWindow.right - rcWindow.left };
	LONG WndHeight{ rcWindow.bottom - rcWindow.top };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindowW(
		szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		int(g_fScreenCenterX - WndWidth / 2.f),
		int(g_fScreenCenterY - WndHeight / 2.f),
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top,
		nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
		return FALSE;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다.
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            ::DestroyWindow(hWnd);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


