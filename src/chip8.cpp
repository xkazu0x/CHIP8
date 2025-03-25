#include "excalibur_base.h"
#include "excalibur_math.h"
#include "excalibur_logger.h"
#include "excalibur_os.h"

#include "excalibur_base.cpp"
#include "excalibur_math.cpp"
#include "excalibur_logger.cpp"
#include "excalibur_os.cpp"

// TODO(xkazu0x):
// gamepad input
// compute time

int main(void) {
    EXINFO("> operating system: %s", string_from_operating_system(operating_system_from_context()));
    EXINFO("> architecture: %s", string_from_architecture(architecture_from_context()));

    os_t os = {};
    os_init(&os);
    
    os_window_info_t info {};
    info.title = "CHIP8";
    info.size = vec2i_create(960, 540);
    info.bitmap_size = info.size;
    info.fullscreen = EX_FALSE;
        
    os_window_t window = {};
    if (!os_window_create(&window, info)) return(1);
    
    input_t *input = &window.input;
    vec2i offset = vec2i_create(0, 0);

    while (!window.quit) {
        os_window_update(&window);
        
        if (input->keyboard[KEY_ESCAPE].pressed) {
            window.quit = EX_TRUE;
            EXINFO("> goodbye world.");
        }

        if (input->keyboard[KEY_LEFT].down) {
            offset.x--;
        }
        if (input->keyboard[KEY_RIGHT].down) {
            offset.x++;
        }
        if (input->keyboard[KEY_UP].down) {
            offset.y--;
        }
        if (input->keyboard[KEY_DOWN].down) {
            offset.y++;
        }

        os_window_clear(&window, vec3f_create(0.2f, 0.3f, 0.3f));
        os_window_present(&window);
        os_sleep_ms(1);
     }
    os_window_destroy(&window);
    return(0);
}
