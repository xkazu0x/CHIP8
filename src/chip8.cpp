#include "excalibur_base.h"
#include "excalibur_math.h"
#include "excalibur_logger.h"
#include "excalibur_window.h"

#include "excalibur_base.cpp"
#include "excalibur_math.cpp"
#include "excalibur_logger.cpp"
#include "excalibur_window.cpp"

int main(void) {
    EXINFO("> operating system: %s", string_from_operating_system(operating_system_from_context()));
    EXINFO("> architecture: %s", string_from_architecture(architecture_from_context()));

    window_info_t info {};
    info.fullscreen = EX_FALSE;
    info.size = vec2i_create(960, 540);
    info.title = "CHIP8";
        
    window_t window = {};
    if (!window_create(&window, info)) return(1);
    
    input_t *input = &window.input;
    while (!window.quit) {
        window_update(&window);
        if (input->keyboard[KEY_ESCAPE].pressed) {
            window.quit = EX_TRUE;
            EXINFO("> goodbye world.");
        }
     }
    
    window_destroy(window);    
    return(0);
}
