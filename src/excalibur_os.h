#ifndef EXCALIBUR_OS_H
#define EXCALIBUR_OS_H

/////////////////////////////////
// NOTE(xkazu0x): input constants

enum key {
    KEY_ESCAPE = 0x1B,
    
    KEY_LEFT   = 0x25,
    KEY_UP     = 0x26,
    KEY_RIGHT  = 0x27,
    KEY_DOWN   = 0x28,
    
    KEY_MAX = 256,
};

///////////////////////////////
// NOTE(xkazu0x): input structs

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

/////////////////////////////////
// NOTE(xkazu0x): window bitmap

struct bitmap_t {
    vec2i size;
    s32 bytes_per_pixel;
    s32 pitch;
    void *memory;
};

///////////////////////////////////////
// NOTE(xkazu0x): system window structs

struct os_window_t {
    b32 quit;
    b32 fullscreen;
    bitmap_t bitmap;
    input_t input;
    void *platform;
};

struct os_window_info_t {
    b32 fullscreen;
    vec2i size;
    vec2i bitmap_size;
    char *title;
};

///////////////////////////////
// NOTE(xkazu0x): system struct

struct os_t {
    s64 ticks_per_frame;
};

/////////////////////////////////////////
// NOTE(xkazu0x): system window functions

internal b32 os_window_create(os_window_t *window, os_window_info_t info);
internal void os_window_destroy(os_window_t *window);
internal void os_window_update(os_window_t *window);

internal void os_window_clear(os_window_t *window, vec3f color);
internal void os_window_present(os_window_t *window);

//////////////////////////////////
// NOTE(xkazu0x): system functions

internal void os_init();
internal s64 os_get_time();
internal void os_sleep_ms(u32 ms);

///////////////////////////////
// NOTE(xkazu0x): system helper

#include "excalibur_os_helper.h"

#endif // EXCALIBUR_OS_H
