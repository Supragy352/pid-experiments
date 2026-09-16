#include "pidlab/sandbox_ui.h"

#include <math.h>

#include <raylib.h>

#include "pidlab/common.h"

void SandboxUi_Draw(const SandboxApp* app, const SandboxConfig* config) {
    /* Responsive sizing scales pendulum and panel to current window size. */
    const int screen_w = GetScreenWidth();
    const int screen_h = GetScreenHeight();

    const float min_dimension = (float)((screen_w < screen_h) ? screen_w : screen_h);
    const float rod_length_px = min_dimension * 0.30f;
    const float bob_radius_px = min_dimension * 0.018f;

    const Vector2 pivot = {
        .x = (float)screen_w * 0.50f,
        .y = (float)screen_h * 0.72f
    };

    /* Convert pendulum angle into bob endpoint in screen coordinates. */
    const Vector2 bob = {
        .x = pivot.x + rod_length_px * (float)sin(app->state.angle_rad),
        .y = pivot.y - rod_length_px * (float)cos(app->state.angle_rad)
    };

    const int panel_w = (int)(screen_w * 0.36f);
    const int panel_h = 230;
    const Rectangle panel = {14.0f, 14.0f, (float)panel_w, (float)panel_h};

    /* Scene background + simple pendulum geometry. */
    ClearBackground((Color){246, 248, 251, 255});

    DrawLineEx(
        (Vector2){pivot.x, pivot.y - rod_length_px},
        pivot,
        2.0f,
        (Color){40, 175, 75, 100}
    );
    DrawLineEx(pivot, bob, 6.0f, (Color){45, 45, 45, 255});
    DrawCircleV(bob, bob_radius_px, (Color){220, 90, 75, 255});
    DrawCircleV(pivot, bob_radius_px * 0.52f, DARKBLUE);

    /* HUD panel with state, controller gains and pass progress. */
    DrawRectangleRounded(panel, 0.15f, 8, (Color){255, 255, 255, 224});
    DrawRectangleRoundedLines(panel, 0.15f, 8, 1.0f, (Color){165, 175, 190, 160});

    const int x = (int)panel.x + 14;
    int y = (int)panel.y + 12;

    DrawText("PID Inverted Pendulum Lab API", x, y, 18, (Color){25, 30, 40, 255});
    y += 28;
    DrawText(TextFormat("Use case: %s", ControlSystem_UseCaseName(app->control.active_use_case)), x, y, 15, DARKBLUE);
    y += 24;
    DrawText(TextFormat("Angle: %.2f deg", app->state.angle_rad * PIDLAB_RAD_TO_DEG), x, y, 15, BLACK);
    y += 21;
    DrawText(TextFormat("Velocity: %.2f deg/s", app->state.angular_velocity_rad_s * PIDLAB_RAD_TO_DEG), x, y, 15, BLACK);
    y += 21;
    DrawText(TextFormat("Torque: %.2f N*m", app->control_torque_nm), x, y, 15, BLACK);
    y += 24;
    DrawText(
        TextFormat(
            "Kp %.2f  Ki %.2f  Kd %.2f",
            app->control.pid.gains.kp,
            app->control.pid.gains.ki,
            app->control.pid.gains.kd
        ),
        x,
        y,
        15,
        BLACK
    );
    y += 24;
    DrawText(
        TextFormat("Stable: %.2f / %.0f s @ +/-%.1f deg", app->stable_timer_sec, config->pass_hold_sec, config->pass_angle_deg),
        x,
        y,
        14,
        DARKGRAY
    );

    if (app->controller_enabled) {
        DrawText("Controller ON", x, (int)panel.y + panel_h - 30, 15, DARKGREEN);
    } else {
        DrawText("Controller OFF", x, (int)panel.y + panel_h - 30, 15, MAROON);
    }

    /* Controls legend + pass indicator + FPS meter. */
    const int footer_y = screen_h - 36;
    DrawText(
        "SPACE toggle  R reset  F disturb  1-4 use cases  <-/-> cycle  Q/A W/S E/D tune gains  F11 fullscreen",
        16,
        footer_y,
        14,
        DARKGRAY
    );

    if (app->passed) {
        DrawText("TEST PASSED", screen_w - 220, 18, 26, DARKGREEN);
    }

    DrawFPS(screen_w - 108, 10);
}
