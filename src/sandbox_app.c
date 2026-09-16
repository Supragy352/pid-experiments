#include "pidlab/sandbox_app.h"

#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "pidlab/common.h"

static void SandboxSeedRandom(void) {
    static bool seeded = false;
    if (!seeded) {
        /* One-time RNG seed for randomized initial states/disturbances. */
        srand((unsigned int)time(NULL));
        seeded = true;
    }
}

static double SandboxRandomRange(const double min_value, const double max_value) {
    const double alpha = (double)rand() / (double)RAND_MAX;
    return min_value + (max_value - min_value) * alpha;
}

static void SandboxRandomizeState(PendulumState* state) {
    if (state == NULL) {
        return;
    }

    /* Spawn away from equilibrium so each episode requires active recovery. */
    const double sign = ((rand() % 2) == 0) ? -1.0 : 1.0;
    const double angle_deg = sign * SandboxRandomRange(10.0, 35.0);
    const double velocity_deg_s = SandboxRandomRange(-40.0, 40.0);

    state->angle_rad = angle_deg * PIDLAB_DEG_TO_RAD;
    state->angular_velocity_rad_s = velocity_deg_s * PIDLAB_DEG_TO_RAD;
}

static int SandboxUseCaseCount(void) {
    /* Keep in sync with ControlUseCase enum values. */
    return 4;
}

SandboxConfig SandboxConfig_Default(void) {
    /* Fixed-rate simulation/control gives deterministic behavior per frame dt. */
    const SandboxConfig config = {
        .physics_hz = 1000,
        .control_hz = 200,
        .render_fps = 144,
        .pass_angle_deg = 5.0,
        .pass_hold_sec = 8.0,
        .max_frame_dt_sec = 0.05
    };
    return config;
}

void SandboxApp_Init(SandboxApp* app) {
    if (app == NULL) {
        return;
    }

    /* Seed RNG before any randomized episode setup. */
    SandboxSeedRandom();

    /* Initialize model/controller and reset runtime accumulators. */
    app->model = PendulumModel_Default();
    ControlSystem_Init(&app->control);
    app->target_angle_rad = 0.0;
    app->controller_enabled = true;
    app->physics_accumulator_sec = 0.0;
    app->control_accumulator_sec = 0.0;

    SandboxApp_ResetEpisode(app);
}

void SandboxApp_ResetEpisode(SandboxApp* app) {
    if (app == NULL) {
        return;
    }

    /* Reset to a fresh randomized start and clear controller history. */
    SandboxRandomizeState(&app->state);
    ControlSystem_Reset(&app->control);

    app->control_torque_nm = 0.0;
    app->stable_timer_sec = 0.0;
    app->passed = false;
    app->physics_accumulator_sec = 0.0;
    app->control_accumulator_sec = 0.0;
}

void SandboxApp_SetControllerEnabled(SandboxApp* app, const bool enabled) {
    if (app == NULL) {
        return;
    }

    /* Turning controller on/off also clears pass tracking and stale torque. */
    app->controller_enabled = enabled;
    app->control_torque_nm = 0.0;
    app->stable_timer_sec = 0.0;
    app->passed = false;
    ControlSystem_Reset(&app->control);
}

void SandboxApp_ToggleController(SandboxApp* app) {
    if (app == NULL) {
        return;
    }
    SandboxApp_SetControllerEnabled(app, !app->controller_enabled);
}

void SandboxApp_SetUseCase(SandboxApp* app, const ControlUseCase use_case) {
    if (app == NULL) {
        return;
    }

    /* Use-case changes reset completion progress for fair evaluation. */
    ControlSystem_SetUseCase(&app->control, use_case);
    app->stable_timer_sec = 0.0;
    app->passed = false;
}

void SandboxApp_CycleUseCase(SandboxApp* app, const int direction) {
    if (app == NULL) {
        return;
    }

    /* Wrap negative/positive steps into valid enum range. */
    const int count = SandboxUseCaseCount();
    const int raw_next = (int)app->control.active_use_case + direction;
    const int wrapped = ((raw_next % count) + count) % count;

    SandboxApp_SetUseCase(app, (ControlUseCase)wrapped);
}

void SandboxApp_ApplyDisturbance(SandboxApp* app) {
    if (app == NULL) {
        return;
    }

    /* Inject an impulse-like velocity kick in degrees/s range. */
    app->state.angular_velocity_rad_s += SandboxRandomRange(-220.0, 220.0) * PIDLAB_DEG_TO_RAD;
}

void SandboxApp_AdjustPidGains(SandboxApp* app, const double delta_kp, const double delta_ki, const double delta_kd) {
    if (app == NULL) {
        return;
    }

    /* Runtime gain edits are clamped at zero to avoid inverted signs by accident. */
    PidGains* gains = &app->control.pid.gains;
    gains->kp = fmax(0.0, gains->kp + delta_kp);
    gains->ki = fmax(0.0, gains->ki + delta_ki);
    gains->kd = fmax(0.0, gains->kd + delta_kd);
}

void SandboxApp_StepFrame(SandboxApp* app, const SandboxConfig* config, const double frame_dt_sec) {
    if (app == NULL || config == NULL) {
        return;
    }
    if (config->physics_hz <= 0 || config->control_hz <= 0) {
        return;
    }

    const double frame_dt = PidLab_ClampDouble(frame_dt_sec, 0.0, config->max_frame_dt_sec);
    const double physics_dt = 1.0 / (double)config->physics_hz;
    const double control_dt = 1.0 / (double)config->control_hz;

    /* Accumulate real frame time for fixed-step control + physics updates. */
    app->physics_accumulator_sec += frame_dt;
    app->control_accumulator_sec += frame_dt;

    /* Run controller at its own fixed frequency, decoupled from rendering. */
    while (app->control_accumulator_sec >= control_dt) {
        if (app->controller_enabled) {
            const double angle_error_rad = app->target_angle_rad - app->state.angle_rad;
            app->control_torque_nm = ControlSystem_Update(
                &app->control,
                angle_error_rad,
                app->state.angular_velocity_rad_s,
                control_dt
            );
        } else {
            app->control_torque_nm = 0.0;
        }
        app->control_accumulator_sec -= control_dt;
    }

    /* Integrate physics at higher rate for smoother, stable dynamics. */
    while (app->physics_accumulator_sec >= physics_dt) {
        PendulumModel_Step(&app->model, &app->state, app->control_torque_nm, physics_dt);
        app->physics_accumulator_sec -= physics_dt;
    }

    /* Pass condition: stay near upright with low angular velocity long enough. */
    const double angle_error_rad = app->target_angle_rad - app->state.angle_rad;
    const bool is_balanced =
        app->controller_enabled &&
        fabs(angle_error_rad) < (config->pass_angle_deg * PIDLAB_DEG_TO_RAD) &&
        fabs(app->state.angular_velocity_rad_s) < 1.0;

    if (is_balanced) {
        app->stable_timer_sec += frame_dt;
        if (app->stable_timer_sec >= config->pass_hold_sec) {
            app->passed = true;
        }
    } else {
        app->stable_timer_sec = 0.0;
    }
}
