#ifndef EXCALIBUR_WINDOW_WIN32_H
#define EXCALIBUR_WINDOW_WIN32_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct win32_t {
    HMODULE window_instance;
    ATOM window_atom;
    HWND window_handle;
    s32 _padding;
    WINDOWPLACEMENT window_placement;
};

internal void win32_window_toggle_fullscreen(HWND window, WINDOWPLACEMENT *placement);
internal LRESULT CALLBACK win32_window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

#endif // EXCALIBUR_WINDOW_WIN32_H
