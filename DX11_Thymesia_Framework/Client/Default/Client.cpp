// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include "GameInstance.h"
#define _CRTDBG_MAP_ALLOC

#define MAX_LOADSTRING 100

HWND	g_hWnd;
HINSTANCE g_hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

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

    OutputDebugStringW(L">>>>>>>>>>>>>>>>>>>> Direct3D Object ref count 메모리 누수 체크 <<<<<<<<<<<<<<<<<<<<\r\n");
    debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
    OutputDebugStringW(L">>>>>>>>>>>>>>>>>>>> 반환되지 않은 IUnknown 객체가 있을경우 위에 나타납니다. <<<<<<<<<<<<<<<<<<<<\r\n");

    debug->Release();
}
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,    _In_ int       nCmdShow) {
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
#endif // _DEBUG

    srand((_uint)time(nullptr));
    unique_ptr<CMainApp> pMainApp;

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;


    pMainApp = CMainApp::Create();
    if (nullptr == pMainApp.get())
        return FALSE;

    if (FAILED(GAMEINSTANCE->Add_Timer((_uint)TIMER::TIMER_DEFAULT)))
        return FALSE;
    if (FAILED(GAMEINSTANCE->Add_Timer((_uint)TIMER::TIMER_FRAME)))
        return FALSE;

    _float fTimerAcc(0.f);
    _float fDeltaTime(0.f);

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

        if (
#ifdef _144HZ_
            fTimerAcc > HZ_144
#else
            1
#endif // _144HZ_
            ) 
        {
            if (KEY_INPUT(KEY::ESC, KEY_STATE::TAP))
            {
                break;
            }

            if (fTimerAcc > 1.f)
            {
                fTimerAcc = 0.f;
                continue;
            }

            pMainApp->Tick(fTimerAcc);
            
            fTimerAcc = 0.f;

            if (FAILED(pMainApp->Render()))
                break;
        }
    }

    pMainApp.reset();

#ifdef  _DEBUG
    //system("pause");
    fclose(ConsoleStream);
    D3DMemoryLeakCheck();
    //_CrtDumpMemoryLeaks()6;
#endif //  _DEBUG

    return (int)msg.wParam;
}

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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance;
	RECT rcWindow{ 0, 0, (_long)g_iWinCX, (_long)g_iWinCY };
	LONG WndWidth   { rcWindow.right    - rcWindow.left };
	LONG WndHeight  { rcWindow.bottom   - rcWindow.top  };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindowW(
		szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		_int(g_fScreenCenterX - WndWidth / 2.f),
		_int(g_fScreenCenterY - WndHeight / 2.f) - 50,
		_int(rcWindow.right - rcWindow.left),
		_int(rcWindow.bottom - rcWindow.top),
		nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
		return FALSE;

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	return TRUE;
}

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
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        /*case IDM_EXIT:
            DestroyWindow(hWnd);
            break;*/
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
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


