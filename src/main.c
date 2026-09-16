#include <raylib.h>

#include "pidlab/control_use_cases.h"
#include "pidlab/sandbox_app.h"
#include "pidlab/sandbox_input.h"
#include "pidlab/sandbox_ui.h"

int main(void) {
    /* Window setup and vsync keep rendering smooth while resizing/fullscreen. */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1920, 1200, "Fixed Inverted Pendulum");
    ToggleFullscreen();

    /* Sandbox config drives fixed update frequencies and pass criteria. */
    const SandboxConfig config = SandboxConfig_Default();
    SetTargetFPS(config.render_fps);

    /* App owns model, controller, runtime timers and progress status. */
    SandboxApp app = {0};
    SandboxApp_Init(&app);

    /* Main loop: input -> simulation step -> render. */
    while (!WindowShouldClose()) {
        const double frame_dt_sec = GetFrameTime();
        const SandboxInput input = SandboxInput_ReadKeyboard();

        /* Global window controls. */
        if (input.toggle_fullscreen) {
            ToggleFullscreen();
        }

        /* Episode/controller controls. */
        if (input.toggle_controller) {
            SandboxApp_ToggleController(&app);
        }
        if (input.reset_episode) {
            SandboxApp_ResetEpisode(&app);
        }
        if (input.disturb) {
            SandboxApp_ApplyDisturbance(&app);
        }

        /* Strategy selection controls. */
        if (input.use_case_next) {
            SandboxApp_CycleUseCase(&app, 1);
        }
        if (input.use_case_prev) {
            SandboxApp_CycleUseCase(&app, -1);
        }

        if (input.select_pid) {
            SandboxApp_SetUseCase(&app, CONTROL_USE_CASE_PID);
        }
        if (input.select_pd) {
            SandboxApp_SetUseCase(&app, CONTROL_USE_CASE_PD);
        }
        if (input.select_bang_bang) {
            SandboxApp_SetUseCase(&app, CONTROL_USE_CASE_BANG_BANG);
        }
        if (input.select_off) {
            SandboxApp_SetUseCase(&app, CONTROL_USE_CASE_OFF);
        }

        /* Continuous gain tuning: held keys apply change proportional to frame dt. */
        const double delta_kp = (input.increase_kp ? 1.0 : 0.0) - (input.decrease_kp ? 1.0 : 0.0);
        const double delta_ki = (input.increase_ki ? 1.0 : 0.0) - (input.decrease_ki ? 1.0 : 0.0);
        const double delta_kd = (input.increase_kd ? 1.0 : 0.0) - (input.decrease_kd ? 1.0 : 0.0);
        SandboxApp_AdjustPidGains(&app, delta_kp * frame_dt_sec, delta_ki * frame_dt_sec, delta_kd * frame_dt_sec);

        /* Update physics/control before drawing latest state. */
        SandboxApp_StepFrame(&app, &config, frame_dt_sec);

        BeginDrawing();
        SandboxUi_Draw(&app, &config);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
