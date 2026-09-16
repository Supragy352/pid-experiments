#ifndef PIDLAB_PENDULUM_MODEL_H
#define PIDLAB_PENDULUM_MODEL_H

/* Physical parameters for a simple rotary inverted pendulum model. */
typedef struct {
    double mass_kg;
    double com_length_m;
    double gravity_ms2;
    double damping;
    double inertia;
} PendulumModel;

/* Dynamic state evolved by the physics integrator. */
typedef struct {
    double angle_rad;
    double angular_velocity_rad_s;
} PendulumState;

/* Build a reasonable default model for the sandbox. */
PendulumModel PendulumModel_Default(void);
/* Advance model state by one time step given applied motor torque. */
void PendulumModel_Step(const PendulumModel* model, PendulumState* state, double torque_nm, double dt_sec);

#endif
