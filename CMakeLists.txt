cmake_minimum_required(VERSION 3.15)
project(CSOPESY_Emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(
    app

    src/main.cpp
    src/ConsoleManager.cpp
    src/MainConsole.cpp
    src/Process.cpp
    src/Scheduler.cpp
    src/Screen.cpp

    src/instructions/IAdd.cpp
    src/instructions/IDeclare.cpp
    src/instructions/IFor.cpp
    src/instructions/IPrint.cpp
    src/instructions/ISleep.cpp
    src/instructions/ISubtract.cpp
)

# On Windows with MinGW (GCC), use -iquote instead of -I so that
# pthread.h's angle-bracket #include <process.h> does not resolve to
# our Process.h on the case-insensitive Windows filesystem.
# Linux is NOT affected: WIN32 is false on Linux, the else branch runs normally.
if(WIN32 AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(app PRIVATE
        -iquote${CMAKE_SOURCE_DIR}/src
        -iquote${CMAKE_SOURCE_DIR}/src/instructions
    )
else()
    target_include_directories(app PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/instructions
    )
endif()

target_link_libraries(app PRIVATE Threads::Threads)

# Copy config.txt into the build tree so the binary finds it at src/config.txt
# relative to the build directory (matches the path hardcoded in main.cpp).
add_custom_command(
    TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/config.txt
        ${CMAKE_BINARY_DIR}/src/config.txt
    COMMENT "Copying src/config.txt to build directory"
)
