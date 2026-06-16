cmake_minimum_required(VERSION 3.15)
project(CSOPESY_Emulator)
set(CMAKE_CXX_STANDARD 20)

add_executable(
    app

    src/main.cpp
    src/ConsoleManager.cpp
    src/MainConsole.cpp
    src/Process.cpp
    src/Scheduler.cpp
    src/Screen.cpp

    instructions/IAdd.cpp
    instructions/IDeclare.cpp
    instructions/IFor.cpp
    instructions/IPrint.cpp
    instructions/ISleep.cpp
    instructions/ISubtract.cpp
)

include_directories(app PRIVATE src src/instructions)
