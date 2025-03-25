#include "excalibur_window_helper.cpp"

#if defined(OS_WINDOWS)
#include "excalibur_window_win32.cpp"
#else
#error no backend for excalibur_window.cpp on this operating system
#endif
