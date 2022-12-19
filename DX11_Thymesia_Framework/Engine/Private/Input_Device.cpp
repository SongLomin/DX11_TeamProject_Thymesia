#include "..\Public\Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device);

int g_Arr[(int)KEY::LAST] = {
	VK_LEFT,//LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	VK_OEM_3, '1','2','3','4','5','6','7','8','9','0',
	VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
	VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_NUMPAD0,
	VK_F1,VK_F2,VK_F3,VK_F4,VK_F5, VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_F11,VK_F12,
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	VK_MENU,//ALT,
	VK_LSHIFT,//LSHIFT,
	VK_SPACE,//SPACE,
	VK_CONTROL,//CTRL,
	VK_RETURN,//ENTER,
	VK_ESCAPE,//ESC,
	VK_TAB,
	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
	VK_BACK,//BACKSPACE
	VK_DELETE,
	VK_HOME,
	VK_INSERT
	
	//LAST
};

KEY_STATE CInput_Device::Get_KeyState(KEY _key)
{
	return m_vecKey[(int)_key].eState;
}

HRESULT CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	m_vecKey.reserve((UINT)KEY::LAST);

	for (int i = 0; i < (int)KEY::LAST; ++i)
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	
	m_Handle = hWnd;

	return S_OK;
}

HRESULT CInput_Device::SetUp_DeviceState()
{
	if (nullptr == m_pMouse)
		return E_FAIL;

	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	return S_OK;
}

HRESULT CInput_Device::Tick()
{
	if (m_Handle == GetFocus())
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			if (GetAsyncKeyState(g_Arr[i]) & 0x8000) { // 키가 눌려 있다
				if (m_vecKey[i].bPrevPush) // 전에도 눌렸으면
					m_vecKey[i].eState = KEY_STATE::HOLD; // 홀드인걸로
				else
					m_vecKey[i].eState = KEY_STATE::TAP; // 아니면 걍 탭

				m_vecKey[i].bPrevPush = true; // 전에 눌린거 활성화
			}
			else
			{ // 키가 안눌려있다
				if (m_vecKey[i].bPrevPush) // 전에는 눌렸다면
					m_vecKey[i].eState = KEY_STATE::AWAY;
				else
					m_vecKey[i].eState = KEY_STATE::NONE;

				m_vecKey[i].bPrevPush = false; // 전에 눌린거 비활성화
			}
		}
	}
	else
	{ // 포커싱이 null이라면
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;

			switch (m_vecKey[i].eState) {
			case KEY_STATE::TAP:
			case KEY_STATE::HOLD:
				m_vecKey[i].eState = KEY_STATE::AWAY;
				break;

			default:
				m_vecKey[i].eState = KEY_STATE::NONE;
				break;
			}
		}
	}

	return S_OK;
}

void CInput_Device::Free()
{
	m_pMouse->Release();
	m_pInputSDK->Release();
}

