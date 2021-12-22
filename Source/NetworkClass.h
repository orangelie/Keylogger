#pragma once
#pragma comment(lib, "ws2_32")

#include "keyState.h"

#include <iostream>
#include <thread>
#include <tuple>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

namespace Mawi1e {
	/** ------------------------------------------------------------------------
	[                                서버 패킷                                 ]
	------------------------------------------------------------------------ **/
	typedef struct {
		WSAOVERLAPPED overlapped;
		WSABUF wsaBuf;
		SOCKET s;
		char Message[0x400];
		unsigned char prev_key;
		bool key_upper;
		bool capsLock;
	} PER_IO, *LPPER_IO;

	class NetworkClass {
	public:
		NetworkClass();
		NetworkClass(const NetworkClass&);
		~NetworkClass();

		bool Listen(LPWSAOVERLAPPED_COMPLETION_ROUTINE);
		bool Connect(const char*);

		void ForcefullyPause();
		void Shutdown();

		bool SendKeyState(KeyState, std::size_t);

	private:
		void ErrorHandling(const char*);

	private:
		SOCKET m_ServSock;

	};
}