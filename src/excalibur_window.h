#ifndef EXCALIBUR_WINDOW_H
#define EXCALIBUR_WINDOW_H

enum key {
    KEY_ESCAPE = 0x1B,
    KEY_MAX = 256,
};

struct digital_button_t {
    b32 down;
    b32 pressed;
    b32 released;
};

struct mouse_t {
    digital_button_t left;
    digital_button_t right;
    digital_button_t middle;
    digital_button_t x1;
    digital_button_t x2;
    s32 wheel;
    s32 delta_wheel;
    vec2i position;
    vec2i delta_position;
};

struct input_t {
    digital_button_t keyboard[KEY_MAX];
    mouse_t mouse;
};

struct window_t {
    b32 quit;
    b32 fullscreen;

    vec2i size;
    vec2i position;
    char *title;

    input_t input;
    
    void *platform;
};

struct window_info_t {
    b32 fullscreen;
    vec2i size;
    char *title;
};

internal b32 window_create(window_t *window, window_info_t info);
internal void window_destroy(window_t window);
internal void window_update(window_t *window);

#include "excalibur_window_helper.h"

#endif // EXCALIBUR_WINDOW_H
