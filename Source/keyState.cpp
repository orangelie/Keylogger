#include "keyState.h"

namespace Mawi1e {
	std::string ConvertDIKToAscii(int dik, bool upper) {
		std::string ascii;
		ascii = "\0";

		switch (dik) {
		case DIK_A: ascii = "A"; break;
		case DIK_B: ascii = "B"; break;
		case DIK_C: ascii = "C"; break;
		case DIK_D: ascii = "D"; break;
		case DIK_E: ascii = "E"; break;
		case DIK_F: ascii = "F"; break;
		case DIK_G: ascii = "G"; break;
		case DIK_H: ascii = "H"; break;
		case DIK_I: ascii = "I"; break;
		case DIK_J: ascii = "J"; break;
		case DIK_K: ascii = "K"; break;
		case DIK_L: ascii = "L"; break;
		case DIK_M: ascii = "M"; break;
		case DIK_N: ascii = "N"; break;
		case DIK_O: ascii = "O"; break;
		case DIK_P: ascii = "P"; break;
		case DIK_Q: ascii = "Q"; break;
		case DIK_R: ascii = "R"; break;
		case DIK_S: ascii = "S"; break;
		case DIK_T: ascii = "T"; break;
		case DIK_U: ascii = "U"; break;
		case DIK_V: ascii = "V"; break;
		case DIK_W: ascii = "W"; break;
		case DIK_X: ascii = "X"; break;
		case DIK_Y: ascii = "Y"; break;
		case DIK_Z: ascii = "Z"; break;

		case DIK_RETURN: ascii = "[Enter]"; break;
		case DIK_ESCAPE: ascii = "[Esc]"; break;
		case DIK_LSHIFT: ascii = "[LShift]"; break;
		case DIK_RSHIFT: ascii = "[RShift]"; break;
		case DIK_CAPSLOCK: ascii = "[CapsLock]"; break;
		case DIK_TAB: ascii = "[Tab]"; break;
		case DIK_LALT: ascii = "[LAlt]"; break;
		case DIK_RALT: ascii = "[RAlt]"; break;
		case DIK_LCONTROL: ascii = "[LCtrl]"; break;
		case DIK_RCONTROL: ascii = "[RCtrl]"; break;

		case DIK_UP: ascii = "[Up]"; break;
		case DIK_DOWN: ascii = "[Down]"; break;
		case DIK_RIGHT: ascii = "[Right]"; break;
		case DIK_LEFT: ascii = "[Left]"; break;

		case DIK_0: ascii = "0"; break;
		case DIK_1: ascii = "1"; break;
		case DIK_2: ascii = "2"; break;
		case DIK_3: ascii = "3"; break;
		case DIK_4: ascii = "4"; break;
		case DIK_5: ascii = "5"; break;
		case DIK_6: ascii = "6"; break;
		case DIK_7: ascii = "7"; break;
		case DIK_8: ascii = "8"; break;
		case DIK_9: ascii = "9"; break;
		}

		if (ascii.size() == 1 && !upper) {
			if ('A' <= ascii[0] && ascii[0] <= 'Z') {
				ascii[0] += 32;
			}
		}

		if (ascii.size() == 1 && upper) {
			switch (ascii[0]) {
			case '0': ascii[0] = ')'; break;
			case '1': ascii[0] = '!'; break;
			case '2': ascii[0] = '@'; break;
			case '3': ascii[0] = '#'; break;
			case '4': ascii[0] = '$'; break;
			case '5': ascii[0] = '%'; break;
			case '6': ascii[0] = '^'; break;
			case '7': ascii[0] = '&'; break;
			case '8': ascii[0] = '*'; break;
			case '9': ascii[0] = '('; break;
			}
		}

		return ascii;
	}

	std::tuple<int, bool, bool> AsciiStringAssembly(const unsigned char* keyState, bool capsLock) {
		int dik;
		bool Upper, unClickedFlag;

		unClickedFlag = true;
		for (int i = 0; i < 256; ++i) {
			if (keyState[i]) {
				unClickedFlag = false;
				dik = i;

				if (capsLock) {
					if ((keyState[DIK_LSHIFT] & 0x80) || (keyState[DIK_RSHIFT] & 0x80)) {
						Upper = false;
					}
					else {
						Upper = true;
					}
				}
				else {
					if ((keyState[DIK_LSHIFT] & 0x80) || (keyState[DIK_RSHIFT] & 0x80)) {
						Upper = true;
					}
					else {
						Upper = false;
					}
				}

				break;
			}
		}

		return std::make_tuple(dik, Upper, unClickedFlag);
	}
}