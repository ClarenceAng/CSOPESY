#pragma once

#include "globals.h"
#include "ProcessInstructions.h"

using InstructionQueue = std::queue<std::unique_ptr<Instruction>>;

class Process {
    public:
        Process(uint32_t processId, std::string name, uint8_t coreNumber);
        ~Process() = default;

        void executeInstruction();
        void displayLog();
        bool isFinished();

        uint32_t getProcessId();
        std::string getProcessName();
        uint32_t getInstructionSize();
        uint32_t getLineNumber();

    private:
        void generateInstructions();
        
        void makeVariable(std::string var);
        std::unique_ptr<Instruction> cmdPrint(std::string msg);
        std::unique_ptr<Instruction> cmdDeclare(std::string var, uint16_t val);
        std::unique_ptr<Instruction> cmdAdd(std::string var1, std::string var2, std::string var3);
        std::unique_ptr<Instruction> cmdSubtract(std::string var1, std::string var2, std::string var3);
        std::unique_ptr<Instruction> cmdSleep(uint8_t ticks);
        std::unique_ptr<Instruction> cmdFor(std::unique_ptr<ForLoop> instructions, uint16_t repeats);
        std::unique_ptr<ForLoop> makeForLoop();

        uint32_t processId;
        std::string name;
        uint8_t coreNumber;

        std::atomic<uint32_t> lineNumber = 1;
        uint8_t sleepCounter;
        bool isLooping = 0;

        std::unique_ptr<InstructionQueue> instructions;
        std::unique_ptr<SymbolTable> symbolTable;
        std::unique_ptr<Logger> logger;

        std::shared_mutex mtx;
};