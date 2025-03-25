#ifndef EXCALIBUR_OS_WIN32_H
#define EXCALIBUR_OS_WIN32_H

////////////////////////////////
// NOTE(xkazu0x): win32 includes and defines

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

//////////////////////////////
// NOTE(xkazu0x): win32 struct

struct win32_t {
    HMODULE window_instance;
    ATOM window_atom;
    HWND window_handle;
    BITMAPINFO bitmap_info;
    WINDOWPLACEMENT window_placement;
};

/////////////////////////////////
// NOTE(xkazu0x): win32 functions

internal void win32_resize_bitmap(os_window_t *window, vec2i size);
internal vec2i win32_get_window_size(HWND window);
internal void win32_window_toggle_fullscreen(HWND window, WINDOWPLACEMENT *placement);
internal LRESULT CALLBACK win32_window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

#endif // EXCALIBUR_OS_WIN32_H
