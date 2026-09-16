#ifndef PIDLAB_SANDBOX_INPUT_H
#define PIDLAB_SANDBOX_INPUT_H

#include <stdbool.h>

/* Snapshot of controls read from keyboard each frame. */
typedef struct {
    bool toggle_controller;
    bool reset_episode;
    bool disturb;
    bool toggle_fullscreen;

    bool use_case_next;
    bool use_case_prev;
    bool select_pid;
    bool select_pd;
    bool select_bang_bang;
    bool select_off;

    bool increase_kp;
    bool decrease_kp;
    bool increase_ki;
    bool decrease_ki;
    bool increase_kd;
    bool decrease_kd;
} SandboxInput;

/* Poll keyboard and build a SandboxInput snapshot. */
SandboxInput SandboxInput_ReadKeyboard(void);

#endif
