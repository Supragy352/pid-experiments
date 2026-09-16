#include "pidlab/control_use_cases.h"

#include <math.h>
#include <stddef.h>

#include "pidlab/common.h"

void ControlSystem_Init(ControlSystem* system) {
    if (system == NULL) {
        return;
    }

    /* Conservative defaults that stabilize typical randomized starts. */
    const PidGains default_gains = {
        .kp = 70.0,
        .ki = 12.0,
        .kd = 20.0
    };
    const PidLimits default_limits = {
        .integral_limit = 2.0,
        .output_limit = 45.0
    };

    /* Start in PID mode and expose bang-bang torque level for demos. */
    PidController_Init(&system->pid, default_gains, default_limits);
    system->active_use_case = CONTROL_USE_CASE_PID;
    system->bang_bang_torque_nm = 22.0;
}

void ControlSystem_Reset(ControlSystem* system) {
    if (system == NULL) {
        return;
    }
    PidController_Reset(&system->pid);
}

void ControlSystem_SetUseCase(ControlSystem* system, const ControlUseCase use_case) {
    if (system == NULL) {
        return;
    }

    /* Strategy changes always clear dynamic state to avoid carry-over artifacts. */
    system->active_use_case = use_case;
    ControlSystem_Reset(system);
}

/*
 * Practice API entrypoint:
 * Implement your PID strategy here.
 */
double ControlSystem_UserPid(PidController* pid, const double angle_error_rad, const double dt_sec) {
    /* Default user hook delegates to built-in PID implementation. */
    return PidController_Update(pid, angle_error_rad, dt_sec);
}

static double ControlSystem_RunPd(const ControlSystem* system, const double angle_error_rad, const double angle_velocity_rad_s) {
    /* PD omits integral action and damps directly on angular velocity. */
    const double torque =
        system->pid.gains.kp * angle_error_rad -
        system->pid.gains.kd * angle_velocity_rad_s;

    return PidLab_ClampDouble(torque, -system->pid.limits.output_limit, system->pid.limits.output_limit);
}

static double ControlSystem_RunBangBang(const ControlSystem* system, const double angle_error_rad, const double angle_velocity_rad_s) {
    /* Deadzone around upright prevents chatter when near equilibrium. */
    const bool near_upright =
        fabs(angle_error_rad) < (2.0 * PIDLAB_DEG_TO_RAD) &&
        fabs(angle_velocity_rad_s) < (8.0 * PIDLAB_DEG_TO_RAD);

    if (near_upright) {
        return 0.0;
    }

    const double sign = (angle_error_rad >= 0.0) ? 1.0 : -1.0;
    return sign * system->bang_bang_torque_nm;
}

double ControlSystem_Update(
    ControlSystem* system,
    const double angle_error_rad,
    const double angle_velocity_rad_s,
    const double dt_sec
) {
    if (system == NULL) {
        return 0.0;
    }

    /* Dispatch torque computation to selected strategy. */
    switch (system->active_use_case) {
        case CONTROL_USE_CASE_PID:
            return ControlSystem_UserPid(&system->pid, angle_error_rad, dt_sec);
        case CONTROL_USE_CASE_PD:
            return ControlSystem_RunPd(system, angle_error_rad, angle_velocity_rad_s);
        case CONTROL_USE_CASE_BANG_BANG:
            return ControlSystem_RunBangBang(system, angle_error_rad, angle_velocity_rad_s);
        case CONTROL_USE_CASE_OFF:
        default:
            return 0.0;
    }
}

const char* ControlSystem_UseCaseName(const ControlUseCase use_case) {
    /* String names are consumed directly by the HUD text. */
    switch (use_case) {
        case CONTROL_USE_CASE_PID:
            return "PID";
        case CONTROL_USE_CASE_PD:
            return "PD";
        case CONTROL_USE_CASE_BANG_BANG:
            return "BANG_BANG";
        case CONTROL_USE_CASE_OFF:
            return "OFF";
        default:
            return "UNKNOWN";
    }
}
