#ifndef PIDLAB_PID_CONTROLLER_H
#define PIDLAB_PID_CONTROLLER_H

#include <stdbool.h>

/* Proportional/Integral/Derivative gains. */
typedef struct {
    double kp;
    double ki;
    double kd;
} PidGains;

/* Runtime safety limits for integrator and output saturation. */
typedef struct {
    double integral_limit;
    double output_limit;
} PidLimits;

/* Stateful PID controller used across control strategies. */
typedef struct {
    PidGains gains;
    PidLimits limits;
    double integral;
    double prev_error;
    bool has_prev;
} PidController;

/* Initialize PID gains/limits and clear state. */
void PidController_Init(PidController* controller, PidGains gains, PidLimits limits);
/* Reset only the dynamic state (integrator + previous error). */
void PidController_Reset(PidController* controller);
/* Run one PID update step and return saturated torque command. */
double PidController_Update(PidController* controller, double error, double dt_sec);

#endif
