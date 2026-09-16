#ifndef PIDLAB_CONTROL_USE_CASES_H
#define PIDLAB_CONTROL_USE_CASES_H

#include "pidlab/pid_controller.h"

/* Available controller implementations exposed in the sandbox UI. */
typedef enum {
    CONTROL_USE_CASE_PID = 0,
    CONTROL_USE_CASE_PD = 1,
    CONTROL_USE_CASE_BANG_BANG = 2,
    CONTROL_USE_CASE_OFF = 3
} ControlUseCase;

/* Controller state container with active strategy + parameters. */
typedef struct {
    ControlUseCase active_use_case;
    PidController pid;
    double bang_bang_torque_nm;
} ControlSystem;

/* Initialize controller defaults and select PID strategy. */
void ControlSystem_Init(ControlSystem* system);
/* Reset strategy state without touching tuned gains. */
void ControlSystem_Reset(ControlSystem* system);
/* Switch active strategy and clear residual controller state. */
void ControlSystem_SetUseCase(ControlSystem* system, ControlUseCase use_case);
/* Compute torque command for one control tick. */
double ControlSystem_Update(ControlSystem* system, double angle_error_rad, double angle_velocity_rad_s, double dt_sec);
/* Return display name for a use case enum value. */
const char* ControlSystem_UseCaseName(ControlUseCase use_case);

/*
 * USER API:
 * Replace this function with your own PID logic when practicing.
 * Input: angle_error_rad = target_angle - current_angle
 * Output: motor torque command in N*m.
 */
double ControlSystem_UserPid(PidController* pid, double angle_error_rad, double dt_sec);

#endif
