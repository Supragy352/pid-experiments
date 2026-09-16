#ifndef PIDLAB_COMMON_H
#define PIDLAB_COMMON_H

/* Shared numeric helpers/constants used across control and simulation modules. */
#define PIDLAB_PI_D 3.14159265358979323846
#define PIDLAB_RAD_TO_DEG (180.0 / PIDLAB_PI_D)
#define PIDLAB_DEG_TO_RAD (PIDLAB_PI_D / 180.0)

/* Clamp a scalar into the closed interval [min_value, max_value]. */
static inline double PidLab_ClampDouble(const double value, const double min_value, const double max_value) {
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

/* Normalize angle into [-pi, pi] so error terms stay bounded. */
static inline double PidLab_WrapPi(double angle_rad) {
    while (angle_rad > PIDLAB_PI_D) {
        angle_rad -= 2.0 * PIDLAB_PI_D;
    }
    while (angle_rad < -PIDLAB_PI_D) {
        angle_rad += 2.0 * PIDLAB_PI_D;
    }
    return angle_rad;
}

#endif
