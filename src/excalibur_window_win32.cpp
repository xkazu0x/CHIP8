#include "excalibur_window_win32.h"

internal void
win32_window_toggle_fullscreen(HWND window, WINDOWPLACEMENT *placement) {
    DWORD window_style = GetWindowLong(window, GWL_STYLE);
    if (window_style & WS_OVERLAPPEDWINDOW) {
        MONITORINFO monitor_info = {};
        monitor_info.cbSize = sizeof(monitor_info);
        if (GetWindowPlacement(window, placement) &&
            GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &monitor_info)) {
            SetWindowLong(window, GWL_STYLE, (window_style & ~(WS_OVERLAPPEDWINDOW)) | WS_POPUP);
            SetWindowPos(window, HWND_TOP,
                         monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                         monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                         monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    } else {
        SetWindowLongPtr(window, GWL_STYLE, (window_style & ~(WS_POPUP)) | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(window, placement);
        SetWindowPos(window, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

internal LRESULT CALLBACK
win32_window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    LRESULT result = 0;
    switch (message) {
        case WM_CLOSE:
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        case WM_SIZE: {
            //vec2i window_size = win32_get_window_size(window);
            //win32_resize_bitmap(&g_bitmap, window_size);
        } break;
        default: {
            result = DefWindowProcA(window, message, wparam, lparam);
        }
    }
    return(result);
}

internal b32
window_create(window_t *window, window_info_t info) {
    window->platform = VirtualAlloc(0, sizeof(win32_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    win32_t *platform = (win32_t *)window->platform;
    platform->window_instance = GetModuleHandleA(0);
    
    DEVMODE monitor_info;
    monitor_info.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &monitor_info);
    
    vec2i monitor_size = vec2i_create(monitor_info.dmPelsWidth, monitor_info.dmPelsHeight);
    s32 monitor_refresh_rate = monitor_info.dmDisplayFrequency;
    EXINFO("> monitor size: %dx%d", monitor_size.x, monitor_size.y);
    EXINFO("> monitor refresh rate: %dHz", monitor_refresh_rate);

    window->title = info.title;
    window->size = info.size;
    window->position = (monitor_size - window->size) / 2;
    EXINFO("> window size: %dx%d", window->size.x, window->size.y);
    EXINFO("> window position: x:%d y:%d", window->position.x, window->position.y);

    s32 window_width = window->size.x;
    s32 window_height = window->size.y;

    RECT window_rectangle = {};
    window_rectangle.left = 0;
    window_rectangle.right = window_width;
    window_rectangle.top = 0;
    window_rectangle.bottom = window_height;
    if (AdjustWindowRect(&window_rectangle, WS_OVERLAPPEDWINDOW, 0)) {
        window_width = window_rectangle.right - window_rectangle.left;
        window_height = window_rectangle.bottom - window_rectangle.top;
    }
    
    WNDCLASSA window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = win32_window_proc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = platform->window_instance;
    window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    window_class.hbrBackground = 0;
    window_class.lpszMenuName = 0;
    window_class.lpszClassName = "EXCALIBUR_WINDOW";
    platform->window_atom = RegisterClassA(&window_class);
    if (!platform->window_atom) {
        EXFATAL("< failed to register win32 window");
        return(EX_FALSE);
    }
    EXDEBUG("> win32 window registered");

    u32 window_style = WS_OVERLAPPEDWINDOW;
    u32 window_style_ex = 0;

    platform->window_handle = CreateWindowExA(window_style_ex, MAKEINTATOM(platform->window_atom),
                                              window->title, window_style,
                                              window->position.x, window->position.y,
                                              window_width, window_height,
                                              0, 0, platform->window_instance, 0);
    if (!platform->window_handle) {
        EXFATAL("< failed to create win32 window");
        return(EX_FALSE);
    }
    EXDEBUG("> win32 window created");
    
    if (info.fullscreen) {
        win32_window_toggle_fullscreen(platform->window_handle, &platform->window_placement);
        window->fullscreen = info.fullscreen;
    }
    ShowWindow(platform->window_handle, SW_SHOW);

    RAWINPUTDEVICE raw_input_device = {};
    raw_input_device.usUsagePage = 0x01;
    raw_input_device.usUsage = 0x02;
    raw_input_device.hwndTarget = platform->window_handle;
    if (!RegisterRawInputDevices(&raw_input_device, 1, sizeof(raw_input_device))) {
        EXFATAL("< failed to register mouse as raw input device");
        return(EX_FALSE);
    }
    EXDEBUG("> mouse registered as raw input device");    
    return(EX_TRUE);
}

internal void
window_destroy(window_t window) {
    win32_t *platform = (win32_t *)window.platform;
    UnregisterClassA(MAKEINTATOM(platform->window_atom), platform->window_instance);
    DestroyWindow(platform->window_handle);
}

internal void
window_update(window_t *window) {
    win32_t *platform = (win32_t *)window->platform;
    input_t *input = &window->input;

    input->mouse.left.pressed = EX_FALSE;
    input->mouse.left.released = EX_FALSE;
    input->mouse.right.pressed = EX_FALSE;
    input->mouse.right.released = EX_FALSE;
    input->mouse.middle.pressed = EX_FALSE;
    input->mouse.middle.released = EX_FALSE;
    input->mouse.x1.pressed = EX_FALSE;
    input->mouse.x1.released = EX_FALSE;
    input->mouse.x2.pressed = EX_FALSE;
    input->mouse.x2.released = EX_FALSE;
    input->mouse.delta_wheel = 0;
    input->mouse.delta_position.x = 0;
    input->mouse.delta_position.y = 0;
    
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
        switch (message.message) {
            case WM_QUIT: {
                window->quit = EX_TRUE;
            } break;
            case WM_INPUT: {
                UINT size;
                GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, 0, &size, sizeof(RAWINPUTHEADER));
                char *buffer = (char *)_alloca(size);
                if (GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) == size) {
                    RAWINPUT *raw_input = (RAWINPUT *)buffer;
                    if (raw_input->header.dwType == RIM_TYPEMOUSE && raw_input->data.mouse.usFlags == MOUSE_MOVE_RELATIVE){
                        input->mouse.delta_position.x += raw_input->data.mouse.lLastX;
                        input->mouse.delta_position.y += raw_input->data.mouse.lLastY;

                        USHORT button_flags = raw_input->data.mouse.usButtonFlags;
                        b32 left_button_down = input->mouse.left.down;
                        if (button_flags & RI_MOUSE_LEFT_BUTTON_DOWN) left_button_down = EX_TRUE;
                        if (button_flags & RI_MOUSE_LEFT_BUTTON_UP) left_button_down = EX_FALSE;
                        _window_process_digital_button(&input->mouse.left, left_button_down);

                        b32 right_button_down = input->mouse.right.down;
                        if (button_flags & RI_MOUSE_RIGHT_BUTTON_DOWN) right_button_down = EX_TRUE;
                        if (button_flags & RI_MOUSE_RIGHT_BUTTON_UP) right_button_down = EX_FALSE;
                        _window_process_digital_button(&input->mouse.right, right_button_down);
                            
                        b32 middle_button_down = input->mouse.middle.down;
                        if (button_flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) middle_button_down = EX_TRUE;
                        if (button_flags & RI_MOUSE_MIDDLE_BUTTON_UP) middle_button_down = EX_FALSE;
                        _window_process_digital_button(&input->mouse.middle, middle_button_down);

                        b32 x1_button_down = input->mouse.x1.down;
                        if (button_flags & RI_MOUSE_BUTTON_4_DOWN) x1_button_down = EX_TRUE;
                        if (button_flags & RI_MOUSE_BUTTON_4_UP) x1_button_down = EX_FALSE;
                        _window_process_digital_button(&input->mouse.x1, x1_button_down);
                            
                        b32 x2_button_down = input->mouse.x2.down;
                        if (button_flags & RI_MOUSE_BUTTON_5_DOWN) x2_button_down = EX_TRUE;
                        if (button_flags & RI_MOUSE_BUTTON_5_UP) x2_button_down = EX_FALSE;
                        _window_process_digital_button(&input->mouse.x2, x2_button_down);
                            
                        if (raw_input->data.mouse.usButtonFlags & RI_MOUSE_WHEEL) {
                            input->mouse.delta_wheel += ((SHORT)raw_input->data.mouse.usButtonData) / WHEEL_DELTA;
                        }
                    }
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
            } break;
            default: {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
        }
    }

    BYTE keyboard_state[KEY_MAX];
    if (!GetKeyboardState(keyboard_state)) return;
    for (s32 key = 0; key < KEY_MAX; key++) {
        _window_process_digital_button(input->keyboard + key, keyboard_state[key] >> 7);
    }

    if (input->keyboard[VK_F11].pressed) {
        win32_window_toggle_fullscreen(platform->window_handle, &platform->window_placement);
        window->fullscreen = !window->fullscreen;
    }
    
    input->mouse.wheel += input->mouse.delta_wheel;
    
    POINT mouse_position;
    GetCursorPos(&mouse_position);
    ScreenToClient(platform->window_handle, &mouse_position);

    if (mouse_position.x < 0) mouse_position.x = 0;
    if (mouse_position.y < 0) mouse_position.y = 0;
    if (mouse_position.x > window->size.x) mouse_position.x = window->size.x;
    if (mouse_position.y > window->size.y) mouse_position.y = window->size.y;

    input->mouse.position.x = mouse_position.x;
    input->mouse.position.y = mouse_position.y;

    RECT client_rectangle;
    GetClientRect(platform->window_handle, &client_rectangle);
    
    s32 new_window_width = client_rectangle.right - client_rectangle.left;
    s32 new_window_height = client_rectangle.bottom - client_rectangle.top;
    if ((window->size.x != new_window_width) ||
        (window->size.y != new_window_height)) {
        window->size.x = new_window_width;
        window->size.y = new_window_height;
    }

    RECT window_rectangle;
    GetWindowRect(platform->window_handle, &window_rectangle);
    
    if ((window->position.x != window_rectangle.left) ||
        (window->position.y != window_rectangle.top)) {
        window->position.x = window_rectangle.left;
        window->position.y = window_rectangle.top;
    }
}
