cmake_minimum_required (VERSION 3.8)

set(InputClass "InputClass.cpp" "InputClass.h")
set(NetworkClass "NetworkClass.cpp" "NetworkClass.h")
set(KeyState "keyState.cpp" "keyState.h")
set(Keylogger "Keylogger.cpp" "Keylogger.h")


add_executable (KeyloggerClient "CMain.cpp" ${Keylogger} ${InputClass} ${NetworkClass} ${KeyState})
add_executable (KeyloggerServer "SMain.cpp" ${Keylogger} ${InputClass} ${NetworkClass} ${KeyState})
target_compile_features(KeyloggerClient PRIVATE cxx_std_17)
target_compile_features(KeyloggerServer PRIVATE cxx_std_17)