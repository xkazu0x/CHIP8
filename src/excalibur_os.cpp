#include "excalibur_os_helper.cpp"

#if defined(OS_WINDOWS)
#include "excalibur_os_win32.h"
#include "excalibur_os_win32.cpp"
#else
#error no backend for excalibur_os.cpp on this operating system
#endif
