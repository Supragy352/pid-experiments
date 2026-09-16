#include "pidlab/pendulum_model.h"

#include <math.h>
#include <stddef.h>

#include "pidlab/common.h"

PendulumModel PendulumModel_Default(void) {
    /* Tuned defaults for a visibly responsive but controllable pendulum. */
    PendulumModel model = {
        .mass_kg = 1.0,
        .com_length_m = 0.8,
        .gravity_ms2 = 9.81,
        .damping = 0.30,
        .inertia = 0.0
    };

    model.inertia = model.mass_kg * model.com_length_m * model.com_length_m;
    return model;
}

void PendulumModel_Step(const PendulumModel* model, PendulumState* state, const double torque_nm, const double dt_sec) {
    if (model == NULL || state == NULL) {
        return;
    }
    if (dt_sec <= 0.0) {
        return;
    }

    /* Protect against invalid inertia values during integration. */
    const double safe_inertia = (model->inertia > 1e-9) ? model->inertia : 1e-9;
    /* Rotational dynamics: torque - damping + gravity restoring component. */
    const double alpha =
        (torque_nm
        - model->damping * state->angular_velocity_rad_s
        + model->mass_kg * model->gravity_ms2 * model->com_length_m * sin(state->angle_rad))
        / safe_inertia;

    /* Semi-implicit Euler integration for angular velocity and angle. */
    state->angular_velocity_rad_s += alpha * dt_sec;
    state->angle_rad += state->angular_velocity_rad_s * dt_sec;
    /* Keep angle bounded for stable error computations and display. */
    state->angle_rad = PidLab_WrapPi(state->angle_rad);
}
