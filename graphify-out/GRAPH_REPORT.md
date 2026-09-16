# Graph Report - .  (2026-05-24)

## Corpus Check
- Corpus is ~4,024 words - fits in a single context window. You may not need a graph.

## Summary
- 88 nodes · 119 edges · 17 communities (11 shown, 6 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 28 edges (avg confidence: 0.79)
- Token cost: 48,352 input · 0 output

## Community Hubs (Navigation)
- [[_COMMUNITY_Control Loop & Entry Point|Control Loop & Entry Point]]
- [[_COMMUNITY_Controller Management Layer|Controller Management Layer]]
- [[_COMMUNITY_Utility Functions & Implementations|Utility Functions & Implementations]]
- [[_COMMUNITY_Controller Algorithms|Controller Algorithms]]
- [[_COMMUNITY_Domain Data Structures|Domain Data Structures]]
- [[_COMMUNITY_UI Rendering|UI Rendering]]
- [[_COMMUNITY_PID Controller Internals|PID Controller Internals]]
- [[_COMMUNITY_Use Case Enum|Use Case Enum]]
- [[_COMMUNITY_Sandbox Config|Sandbox Config]]
- [[_COMMUNITY_Input Interface|Input Interface]]
- [[_COMMUNITY_CMake Build Target|CMake Build Target]]

## God Nodes (most connected - your core abstractions)
1. `main()` - 12 edges
2. `main` - 11 edges
3. `SandboxApp_Init()` - 6 edges
4. `ControlSystem_Reset()` - 5 edges
5. `ControlSystem_Update()` - 5 edges
6. `SandboxApp_ResetEpisode()` - 5 edges
7. `SandboxApp_StepFrame()` - 5 edges
8. `ControlSystem_Update` - 5 edges
9. `SandboxApp_StepFrame` - 5 edges
10. `PidLab_ClampDouble()` - 4 edges

## Surprising Connections (you probably didn't know these)
- `PID Inverted Pendulum Project TODO` --references--> `main`  [EXTRACTED]
  TODO.md → src/main.c
- `PID Inverted Pendulum Project TODO` --references--> `PidController`  [EXTRACTED]
  TODO.md → include/pidlab/pid_controller.h
- `PID Inverted Pendulum Project TODO` --references--> `ControlSystem`  [EXTRACTED]
  TODO.md → include/pidlab/control_use_cases.h
- `PID Inverted Pendulum Project TODO` --references--> `SandboxApp`  [EXTRACTED]
  TODO.md → include/pidlab/sandbox_app.h
- `ControlSystem_RunPd()` --calls--> `PidLab_ClampDouble()`  [INFERRED]
  src/control_use_cases.c → include/pidlab/common.h

## Hyperedges (group relationships)
- **Main Loop Frame Pipeline: Input -> Step -> Draw** — main_c_main, sandbox_input_c_SandboxInput_ReadKeyboard, sandbox_app_c_SandboxApp_StepFrame, sandbox_ui_h_SandboxUi_Draw [EXTRACTED 1.00]
- **Controller Strategy Dispatch: ControlSystem selects PID/PD/BangBang/Off** — control_use_cases_c_ControlSystem_Update, control_use_cases_c_ControlSystem_UserPid, control_use_cases_c_ControlSystem_RunPd, control_use_cases_c_ControlSystem_RunBangBang [EXTRACTED 1.00]
- **Decoupled Fixed-Rate Physics and Control Loops** — sandbox_app_c_SandboxApp_StepFrame, pendulum_model_c_PendulumModel_Step, control_use_cases_c_ControlSystem_Update [EXTRACTED 1.00]

## Communities (17 total, 6 thin omitted)

### Community 0 - "Control Loop & Entry Point"
Cohesion: 0.18
Nodes (18): ControlSystem_Init(), ControlSystem_Reset(), ControlSystem_SetUseCase(), main(), SandboxApp_AdjustPidGains(), SandboxApp_ApplyDisturbance(), SandboxApp_CycleUseCase(), SandboxApp_Init() (+10 more)

### Community 1 - "Controller Management Layer"
Cohesion: 0.15
Nodes (18): ControlSystem_Init, ControlSystem_Reset, ControlSystem_SetUseCase, ControlSystem_UseCaseName, main, PendulumModel_Default, PidController_Init, PidController_Reset (+10 more)

### Community 2 - "Utility Functions & Implementations"
Cohesion: 0.23
Nodes (10): PidLab_ClampDouble(), PidLab_WrapPi(), ControlSystem_RunBangBang(), ControlSystem_RunPd(), ControlSystem_Update(), ControlSystem_UserPid(), PendulumModel_Default(), PendulumModel_Step() (+2 more)

### Community 3 - "Controller Algorithms"
Cohesion: 0.21
Nodes (12): PidLab_ClampDouble, PidLab_WrapPi, ControlSystem_RunBangBang, ControlSystem_RunPd, ControlSystem_Update, ControlSystem_UserPid, PendulumModel_Step, PidController_Update (+4 more)

### Community 4 - "Domain Data Structures"
Cohesion: 0.32
Nodes (8): ControlSystem, PendulumModel, PendulumState, PidController, PidGains, PidLimits, SandboxApp, PID Inverted Pendulum Project TODO

## Knowledge Gaps
- **15 isolated node(s):** `PidLab_WrapPi`, `PidGains`, `PidLimits`, `PendulumModel`, `PendulumState` (+10 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `main` connect `Controller Management Layer` to `Controller Algorithms`, `Domain Data Structures`?**
  _High betweenness centrality (0.140) - this node is a cross-community bridge._
- **Why does `SandboxApp_StepFrame` connect `Controller Algorithms` to `Controller Management Layer`?**
  _High betweenness centrality (0.084) - this node is a cross-community bridge._
- **Why does `main()` connect `Control Loop & Entry Point` to `Utility Functions & Implementations`, `UI Rendering`?**
  _High betweenness centrality (0.065) - this node is a cross-community bridge._
- **Are the 11 inferred relationships involving `main()` (e.g. with `SandboxConfig_Default()` and `SandboxApp_Init()`) actually correct?**
  _`main()` has 11 INFERRED edges - model-reasoned connections that need verification._
- **Are the 3 inferred relationships involving `SandboxApp_Init()` (e.g. with `main()` and `PendulumModel_Default()`) actually correct?**
  _`SandboxApp_Init()` has 3 INFERRED edges - model-reasoned connections that need verification._
- **Are the 3 inferred relationships involving `ControlSystem_Reset()` (e.g. with `PidController_Reset()` and `SandboxApp_ResetEpisode()`) actually correct?**
  _`ControlSystem_Reset()` has 3 INFERRED edges - model-reasoned connections that need verification._
- **What connects `PidLab_WrapPi`, `PidGains`, `PidLimits` to the rest of the system?**
  _18 weakly-connected nodes found - possible documentation gaps or missing edges._