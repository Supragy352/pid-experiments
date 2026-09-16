#include "pidlab/sandbox_input.h"

#include <raylib.h>

SandboxInput SandboxInput_ReadKeyboard(void) {
    /* Key mapping consumed by main loop to trigger app actions each frame. */
    const SandboxInput input = {
        .toggle_controller = IsKeyPressed(KEY_SPACE),
        .reset_episode = IsKeyPressed(KEY_R),
        .disturb = IsKeyPressed(KEY_F),
        .toggle_fullscreen = IsKeyPressed(KEY_F11),

        .use_case_next = IsKeyPressed(KEY_RIGHT),
        .use_case_prev = IsKeyPressed(KEY_LEFT),
        .select_pid = IsKeyPressed(KEY_ONE),
        .select_pd = IsKeyPressed(KEY_TWO),
        .select_bang_bang = IsKeyPressed(KEY_THREE),
        .select_off = IsKeyPressed(KEY_FOUR),

        .increase_kp = IsKeyDown(KEY_Q),
        .decrease_kp = IsKeyDown(KEY_A),
        .increase_ki = IsKeyDown(KEY_W),
        .decrease_ki = IsKeyDown(KEY_S),
        .increase_kd = IsKeyDown(KEY_E),
        .decrease_kd = IsKeyDown(KEY_D)
    };
    return input;
}
