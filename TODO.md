# PID Inverted Pendulum Project TODO

Use this checklist in order. If you complete every item, you will understand the full project end-to-end.

---

## 0) Baseline Setup

- [.] Confirm toolchain is working:
  - [.] `cmake -S . -B build`
  - [.] `cmake --build build --target balance_pid`
  - [.] `.\build\balance_pid.exe`
- [.] Launch the app and verify these controls work:
  - [.] `SPACE` toggle controller
  - [.] `R` reset episode
  - [.] `F` disturbance
  - [.] `1/2/3/4` choose use-case
  - [.] `LEFT/RIGHT` cycle use-cases
  - [.] `Q/A`, `W/S`, `E/D` tune `Kp/Ki/Kd`
  - [.] `F11` toggle fullscreen

---

## 1) Understand the Big Picture

- [.] Draw the runtime flow on paper:
  - [.] `main -> input -> app/update -> control -> physics -> ui`
- [ ] Explain in one sentence what each module owns:
  - [ ] `sandbox_input` maps keyboard to intent
  - [ ] `sandbox_app` owns simulation state and stepping
  - [ ] `control_use_cases` chooses controller strategy
  - [ ] `pid_controller` computes PID output
  - [ ] `pendulum_model` computes physics
  - [ ] `sandbox_ui` renders scene and HUD
  - [ ] `common` contains math helpers/constants

---

## 2) Build System Deep Dive

- [ ] Read `CMakeLists.txt` line-by-line and explain:
  - [ ] Why `find_package(raylib REQUIRED)` is needed
  - [ ] Why project headers are added through `target_include_directories`
  - [ ] Why each `.c` file is listed in `SOURCES`
  - [ ] What `build_and_run` and `run` custom targets do
- [ ] Optional hardening:
  - [ ] Add compile warnings (`-Wall -Wextra`) and fix warnings if any

---

## 3) Entry Point (`src/main.c`)

- [ ] Trace one frame of execution in `main`:
  - [ ] Read input
  - [ ] Convert input into app API calls
  - [ ] Apply gain deltas
  - [ ] Step simulation
  - [ ] Draw UI
- [ ] Explain why `frame_dt_sec = GetFrameTime()` is used for time-scaled tuning.
- [ ] Change window title and verify rebuild/run cycle.

---

## 4) Input API (`include/pidlab/sandbox_input.h`, `src/sandbox_input.c`)

- [ ] Verify every `SandboxInput` field maps to exactly one key.
- [ ] Add one new action (example: `pause_physics`) and wire a key for it.
- [ ] Confirm no raylib calls leak outside the input/UI/main layers except where intended.

---

## 5) App Orchestration API (`include/pidlab/sandbox_app.h`, `src/sandbox_app.c`)

- [ ] Understand `SandboxConfig_Default()` values and why they are chosen.
- [ ] Explain why there are two accumulators:
  - [ ] `physics_accumulator_sec`
  - [ ] `control_accumulator_sec`
- [ ] Explain fixed-step update loops:
  - [ ] Control loop at `control_hz`
  - [ ] Physics loop at `physics_hz`
- [ ] Verify pass condition logic:
  - [ ] angle threshold
  - [ ] angular-velocity threshold
  - [ ] hold time threshold
- [ ] Change pass rule (example: `pass_angle_deg` from `5.0` to `3.0`) and observe behavior.

---

## 6) Physics Model (`include/pidlab/pendulum_model.h`, `src/pendulum_model.c`)

- [ ] Derive the equation used in `PendulumModel_Step`:
  - [ ] torque term
  - [ ] damping term
  - [ ] gravity destabilizing term
- [ ] Explain why angle wrapping (`PidLab_WrapPi`) is useful.
- [ ] Change damping from `0.30` to `0.05`, then `1.20`; observe difficulty change.
- [ ] Confirm units for every field (`kg`, `m`, `rad`, `N*m`, `s`).

---

## 7) PID Core (`include/pidlab/pid_controller.h`, `src/pid_controller.c`)

- [ ] Explain each state variable:
  - [ ] `integral`
  - [ ] `prev_error`
  - [ ] `has_prev`
- [ ] Explain anti-windup clamp (`integral_limit`) and output clamp (`output_limit`).
- [ ] Explain derivative calculation and why `safe_dt` exists.
- [ ] Implement a derivative low-pass filter (optional exercise).

---

## 8) Control Use Cases (`include/pidlab/control_use_cases.h`, `src/control_use_cases.c`)

- [ ] Read all four use cases and explain when each is useful:
  - [ ] `PID`
  - [ ] `PD`
  - [ ] `BANG_BANG`
  - [ ] `OFF`
- [ ] Implement your own controller in `ControlSystem_UserPid(...)`.
- [ ] Add a fifth use case:
  - [ ] Extend enum in header
  - [ ] Add implementation in `.c`
  - [ ] Add label in `ControlSystem_UseCaseName`
  - [ ] Add key binding in input/main

---

## 9) UI Layer (`include/pidlab/sandbox_ui.h`, `src/sandbox_ui.c`)

- [ ] Explain how screen-size scaling is done:
  - [ ] `GetScreenWidth()`, `GetScreenHeight()`
  - [ ] `min_dimension` based scaling
- [ ] Explain how rod/bob screen coordinates are computed from `angle_rad`.
- [ ] Change HUD density:
  - [ ] remove one metric
  - [ ] add one metric (example: control mode index or inertia)
- [ ] Add a small legend with color meaning.

---

## 10) Controlled Experiments (Write Results)

- [ ] Create a table in this file with columns:
  - [ ] Use-case
  - [ ] `Kp/Ki/Kd`
  - [ ] Disturbance strength
  - [ ] Passed? (Y/N)
  - [ ] Time to recover
- [ ] Run at least 10 experiments and fill the table.
- [ ] Pick your best stable PID and explain why it works.

---

## 11) API Design Mastery Tasks

- [ ] Add a `sandbox_logger` module:
  - [ ] header + source
  - [ ] append frame metrics to CSV
- [ ] Add a `sandbox_config` loader from a text file:
  - [ ] start with simple key-value parsing
- [ ] Add replay support:
  - [ ] record torque and state
  - [ ] replay without keyboard
- [ ] Add a headless mode (no raylib window) for controller benchmarking.

---

## 12) Final Self-Test

- [ ] I can explain every module boundary without opening code.
- [ ] I can re-implement `main.c` from memory.
- [ ] I can code PID in `ControlSystem_UserPid` from scratch.
- [ ] I can add a new controller mode end-to-end in less than 20 minutes.
- [ ] I can tune gains intentionally (not by random trial and error).

When all boxes are checked, you understand this project at a strong engineering level.
