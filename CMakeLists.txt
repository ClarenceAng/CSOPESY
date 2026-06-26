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

target_compile_definitions(app PRIVATE
    CONFIG_PATH="${CMAKE_SOURCE_DIR}/src/config.txt")
