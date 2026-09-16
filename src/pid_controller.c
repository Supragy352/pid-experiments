#include "pidlab/pid_controller.h"

#include <math.h>
#include <stddef.h>

#include "pidlab/common.h"

void PidController_Init(PidController* controller, const PidGains gains, const PidLimits limits) {
    if (controller == NULL) {
        return;
    }

    /* Store parameters and sanitize limits to positive magnitudes. */
    controller->gains = gains;
    controller->limits = limits;
    controller->limits.integral_limit = fabs(controller->limits.integral_limit);
    controller->limits.output_limit = fabs(controller->limits.output_limit);

    PidController_Reset(controller);
}

void PidController_Reset(PidController* controller) {
    if (controller == NULL) {
        return;
    }

    /* Clear only dynamic state; keep tuned gains/limits unchanged. */
    controller->integral = 0.0;
    controller->prev_error = 0.0;
    controller->has_prev = false;
}

double PidController_Update(PidController* controller, const double error, const double dt_sec) {
    if (controller == NULL) {
        return 0.0;
    }

    /* Avoid divide-by-zero and huge derivative spikes for tiny dt. */
    const double safe_dt = (dt_sec > 1e-9) ? dt_sec : 1e-9;

    /* Integrator with anti-windup clamping. */
    controller->integral += error * safe_dt;
    controller->integral = PidLab_ClampDouble(
        controller->integral,
        -controller->limits.integral_limit,
        controller->limits.integral_limit
    );

    double d_error = 0.0;
    if (controller->has_prev) {
        d_error = (error - controller->prev_error) / safe_dt;
    }

    /* Classic PID control law. */
    const double output =
        controller->gains.kp * error +
        controller->gains.ki * controller->integral +
        controller->gains.kd * d_error;

    /* Persist state for the next derivative calculation. */
    controller->prev_error = error;
    controller->has_prev = true;

    /* Saturate command to actuator output bounds. */
    return PidLab_ClampDouble(output, -controller->limits.output_limit, controller->limits.output_limit);
}
