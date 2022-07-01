#include "NetworkClass.h"

namespace Mawi1e {
	NetworkClass::NetworkClass() {

	}

	NetworkClass::NetworkClass(const NetworkClass&) {
	}

	NetworkClass::~NetworkClass() {
	}

	bool NetworkClass::Listen(LPWSAOVERLAPPED_COMPLETION_ROUTINE Routine) {
		WSADATA wsaData;
		SOCKADDR_IN sockAddr;
		int result;

		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			this->ErrorHandling("#M200");
			return false;
		}

		this->m_ServSock = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
		if (this->m_ServSock == INVALID_SOCKET) {
			this->ErrorHandling("#M201");
			return false;
		}

		memset(&sockAddr, 0x00, sizeof(sockAddr));
		sockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(8080);

		result = bind(this->m_ServSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
		if (result == SOCKET_ERROR) {
			this->ErrorHandling("#M202");
			return false;
		}

		result = listen(this->m_ServSock, SOMAXCONN);
		if (result == SOCKET_ERROR) {
			this->ErrorHandling("#M203");
			return false;
		}

		/** ------------------------------------------------------------------------
		[                       동기적으로 클라이언트를 수용                       ]
		------------------------------------------------------------------------ **/
		for (;;) {
			SOCKET client;
			DWORD Flags;
			SOCKADDR_IN tempAddr;
			int tempAddrSize;

			tempAddrSize = sizeof(tempAddr);
			client = accept(this->m_ServSock, (SOCKADDR*)&tempAddr, &tempAddrSize);
			if (client == INVALID_SOCKET) {
				return true;
			}

			LPPER_IO tempData = new PER_IO;
			memset(&tempData->overlapped, 0x00, sizeof(WSAOVERLAPPED));
			memset(&tempData->Message, 0x00, 0x400);
			tempData->s = client;
			tempData->wsaBuf.len = 0x400;
			tempData->wsaBuf.buf = tempData->Message;
			Flags = 0x00;

			result = WSARecv(tempData->s, &tempData->wsaBuf, 1, 0, &Flags, &tempData->overlapped, Routine);
			if (result) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					return false;
				}
			}
		}

		return true;
	}

	bool NetworkClass::Connect(const char* ipAddress) {
		WSADATA wsaData;
		SOCKADDR_IN sockAddr;
		int result;

		result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			this->ErrorHandling("#M200");
			return false;
		}

		this->m_ServSock = WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
		if (this->m_ServSock == INVALID_SOCKET) {
			this->ErrorHandling("#M201");
			return false;
		}

		memset(&sockAddr, 0x00, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(8080);
		inet_pton(AF_INET, ipAddress, &sockAddr.sin_addr);

		result = connect(this->m_ServSock, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
		if (result == INVALID_SOCKET) {
			this->ErrorHandling("#M202");
			return false;
		}

		return true;
	}

	void NetworkClass::ForcefullyPause() {
		closesocket(this->m_ServSock);
		this->Shutdown();

		return;
	}

	void NetworkClass::Shutdown() {
		WSACleanup();

		return;
	}

	bool NetworkClass::SendKeyState(KeyState ptr, std::size_t size) {
		if (send(this->m_ServSock, (const char*)&ptr, size, 0) <= 0) {
			return false;
		}

		return true;
	}

	void NetworkClass::ErrorHandling(const char* ErrorMessage) {
		MessageBoxA(0, ErrorMessage, "##Warning##", MB_ICONWARNING);

		return;
	}
}