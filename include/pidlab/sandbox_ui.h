#ifndef PIDLAB_SANDBOX_UI_H
#define PIDLAB_SANDBOX_UI_H

#include "pidlab/sandbox_app.h"

/* Render pendulum scene and status panel for current app state. */
void SandboxUi_Draw(const SandboxApp* app, const SandboxConfig* config);

#endif
