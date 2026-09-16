cmake_minimum_required(VERSION 3.2)
project(balance_pid)

# Export compile commands for editor tooling (clangd, etc).
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_PREFIX_PATH 
    "E:/raylib/w64devkit/external"
)

# Enforce modern C settings consistently across toolchains.
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Resolve raylib from the configured prefix path.
find_package(raylib REQUIRED)

# Project source list.
set(SOURCES
    ./src/main.c
    ./src/pid_controller.c
    ./src/pendulum_model.c
    ./src/control_use_cases.c
    ./src/sandbox_app.c
    ./src/sandbox_input.c
    ./src/sandbox_ui.c
)

add_executable(balance_pid ${SOURCES})

# Local include tree + explicit raylib headers for MinGW environment.
target_include_directories(balance_pid PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "E:/raylib/w64devkit/external/include"
    "E:/raylib/w64devkit/x86_64-w64-mingw32/include"
)

# raylib and required Windows platform libraries.
target_link_libraries(balance_pid raylib winmm gdi32 kernel32)

# Convenience target: build then execute.
add_custom_target(build_and_run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target balance_pid
    COMMAND balance_pid
    DEPENDS balance_pid
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Convenience target: run existing binary.
add_custom_target(run
    COMMAND balance_pid
    DEPENDS balance_pid
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
