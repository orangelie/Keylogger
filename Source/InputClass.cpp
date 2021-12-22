#include "InputClass.h"

namespace Mawi1e {
	InputClass::InputClass() {
		this->m_DirectInput = nullptr;
		this->m_KeyboardDevice = nullptr;
		this->m_MouseDevice = nullptr;
	}

	InputClass::InputClass(const InputClass&) {
	}

	InputClass::~InputClass() {
	}

	bool InputClass::Initialize() {
		HINSTANCE hInst;
		HWND hwnd;
		HRESULT result;

		this->m_ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		this->m_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		/** ------------------------------------------------------------------------
		[                            마우스 좌표 초기화                            ]
		------------------------------------------------------------------------ **/
		SetCursorPos(0, 0);
		this->m_MouseX = 0;
		this->m_MouseY = 0;

		hwnd = GetConsoleWindow();
		hInst = GetModuleHandle(0);
		if (hInst == nullptr) {
			this->ErrorHandling("#M300");
			return false;
		}

		result = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&this->m_DirectInput, 0);
		if (FAILED(result)) {
			this->ErrorHandling("#M301");
			return false;
		}
		
		/** ------------------------------------------------------------------------
		[                        키보드 디바이스 객체 취득                         ]
		------------------------------------------------------------------------ **/
		result = this->m_DirectInput->CreateDevice(GUID_SysKeyboard, &this->m_KeyboardDevice, 0);
		if (FAILED(result)) {
			this->ErrorHandling("#M302");
			return false;
		}

		result = this->m_KeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) {
			this->ErrorHandling("#M303");
			return false;
		}

		result = this->m_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result)) {
			this->ErrorHandling("#M304");
			return false;
		}

		result = this->m_KeyboardDevice->Acquire();
		/*
		if (FAILED(result)) {
			this->ErrorHandling("#M305");
			return false;
		}
		*/

		/** ------------------------------------------------------------------------
		[                        마우스 디바이스 객체 취득                         ]
		------------------------------------------------------------------------ **/
		result = this->m_DirectInput->CreateDevice(GUID_SysMouse, &this->m_MouseDevice, 0);
		if (FAILED(result)) {
			this->ErrorHandling("#M306");
			return false;
		}

		result = this->m_MouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result)) {
			this->ErrorHandling("#M307");
			return false;
		}

		result = this->m_MouseDevice->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result)) {
			this->ErrorHandling("#M308");
			return false;
		}

		result = this->m_MouseDevice->Acquire();
		if (FAILED(result)) {
			this->ErrorHandling("#M309");
			return false;
		}

		return true;
	}

	void InputClass::Shutdown() {
		if (this->m_KeyboardDevice) {
			this->m_KeyboardDevice->Unacquire();
			this->m_KeyboardDevice->Release();
			this->m_KeyboardDevice = nullptr;
		}

		if (this->m_MouseDevice) {
			this->m_MouseDevice->Unacquire();
			this->m_MouseDevice->Release();
			this->m_MouseDevice = nullptr;
		}

		if (this->m_DirectInput) {
			this->m_DirectInput->Release();
			this->m_DirectInput = nullptr;
		}

		return;
	}

	bool InputClass::Frame() {
		HRESULT result;

		result = this->m_KeyboardDevice->GetDeviceState(sizeof(this->m_KeyState), &this->m_KeyState);
		if (FAILED(result)) {
			if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
				this->m_KeyboardDevice->Acquire();
			}
			else {
				this->ErrorHandling("#M400");
				return false;
			}
		}

		result = this->m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
		if (FAILED(result)) {
			if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
				this->m_MouseDevice->Acquire();
			}
			else {
				this->ErrorHandling("#M401");
				return false;
			}
		}

		/** ------------------------------------------------------------------------
		[                         마우스의 오차범위 제거                           ]
		------------------------------------------------------------------------ **/
		this->m_MouseX += this->m_MouseState.lX;
		this->m_MouseY += this->m_MouseState.lY;

		if (this->m_MouseX < 0) { this->m_MouseX = 0; }
		if (this->m_MouseY < 0) { this->m_MouseY = 0; }
		if (this->m_MouseX > m_ScreenWidth) { this->m_MouseX = m_ScreenWidth; }
		if (this->m_MouseY > m_ScreenHeight) { this->m_MouseY = m_ScreenHeight; }

		return true;
	}

	std::tuple<unsigned char*, int, int> InputClass::GetDeviceState() {
		return std::make_tuple(this->m_KeyState, this->m_MouseX, this->m_MouseY);
	}

	void InputClass::ErrorHandling(const char* ErrorMessage) {
		MessageBoxA(0, ErrorMessage, "##Warning##", MB_ICONWARNING);

		return;
	}
}