#ifndef PIDLAB_SANDBOX_APP_H
#define PIDLAB_SANDBOX_APP_H

#include <stdbool.h>

#include "pidlab/control_use_cases.h"
#include "pidlab/pendulum_model.h"

/* Fixed simulation and pass/fail settings for the lab runtime. */
typedef struct {
    int physics_hz;
    int control_hz;
    int render_fps;
    double pass_angle_deg;
    double pass_hold_sec;
    double max_frame_dt_sec;
} SandboxConfig;

/* Full application state used by update and render code. */
typedef struct {
    PendulumModel model;
    PendulumState state;
    ControlSystem control;

    double target_angle_rad;
    double control_torque_nm;

    double physics_accumulator_sec;
    double control_accumulator_sec;
    double stable_timer_sec;

    bool controller_enabled;
    bool passed;
} SandboxApp;

/* Return default runtime configuration. */
SandboxConfig SandboxConfig_Default(void);
/* Initialize model/controller and start first randomized episode. */
void SandboxApp_Init(SandboxApp* app);
/* Re-randomize pendulum state and clear pass criteria timers. */
void SandboxApp_ResetEpisode(SandboxApp* app);
/* Explicitly enable/disable active controller output. */
void SandboxApp_SetControllerEnabled(SandboxApp* app, bool enabled);
/* Convenience toggle for controller enable state. */
void SandboxApp_ToggleController(SandboxApp* app);
/* Select an exact use case and reset pass timer. */
void SandboxApp_SetUseCase(SandboxApp* app, ControlUseCase use_case);
/* Cycle use case index forward/backward with wrap-around. */
void SandboxApp_CycleUseCase(SandboxApp* app, int direction);
/* Apply a random velocity kick to challenge recovery behavior. */
void SandboxApp_ApplyDisturbance(SandboxApp* app);
/* Runtime gain tuning helper driven by keyboard input. */
void SandboxApp_AdjustPidGains(SandboxApp* app, double delta_kp, double delta_ki, double delta_kd);
/* Advance control + physics accumulators for one render frame. */
void SandboxApp_StepFrame(SandboxApp* app, const SandboxConfig* config, double frame_dt_sec);

#endif
