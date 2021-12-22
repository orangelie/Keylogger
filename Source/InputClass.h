#pragma once
#pragma comment(lib, "dxguid")
#pragma comment(lib, "dinput8")

#define DIRECTINPUT_VERSION (0x0800)

#include "keyState.h"

#include  <tuple>

#include <Windows.h>
#include <dinput.h>

namespace Mawi1e {
	class InputClass {
	public:
		InputClass();
		InputClass(const InputClass&);
		~InputClass();

		bool Initialize();
		void Shutdown();

		bool Frame();

		std::tuple<unsigned char*, int, int> GetDeviceState();

	private:
		void ErrorHandling(const char*);

	private:
		IDirectInput8* m_DirectInput;
		IDirectInputDevice8* m_MouseDevice;
		IDirectInputDevice8* m_KeyboardDevice;

		unsigned char m_KeyState[256];

		DIMOUSESTATE m_MouseState;
		int m_ScreenWidth, m_ScreenHeight;
		int m_MouseX, m_MouseY;

	};
}