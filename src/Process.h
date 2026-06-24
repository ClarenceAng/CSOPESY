#pragma once

#include "globals.h"
#include "ProcessInstructions.h"

using InstructionQueue = std::queue<std::unique_ptr<Instruction>>;

class Process {
    public:
        Process(uint64_t processId, std::string name, uint8_t coreNumber, std::string timestamp);
        ~Process() = default;

        void executeInstruction();
        void displayLog();
        bool isFinished();

        uint64_t getProcessId();
        std::string getProcessName();
        uint64_t getInstructionSize();
        uint64_t getLineNumber();

        std::string getProcessTimestamp();

    private:
        void generateInstructions();
        void generateInstructionList(ForLoop& container, uint64_t& budget, int depth);
        std::unique_ptr<Instruction> makeRandomSimpleInstruction();

        void generateCustomInstructions();

        void makeVariable(std::string var);
        std::unique_ptr<Instruction> cmdPrint(std::string msg);
        std::unique_ptr<Instruction> cmdDeclare(std::string var, uint16_t val);
        std::unique_ptr<Instruction> cmdAdd(std::string var, Operand op1, Operand op2);
        std::unique_ptr<Instruction> cmdSubtract(std::string var, Operand op1, Operand op2);
        std::unique_ptr<Instruction> cmdSleep(uint8_t ticks);
        std::unique_ptr<Instruction> cmdFor(std::unique_ptr<ForLoop> instructions, uint16_t repeats);
        std::unique_ptr<ForLoop> makeForLoop();

        Operand operandBuilder(std::string var);
        Operand operandBuilder(uint16_t lit);

        uint64_t processId;
        std::string name;
        uint8_t coreNumber;
        uint64_t instructionSize;
        std::string timestamp;

        std::atomic<uint64_t> lineNumber = 0;
        uint8_t sleepCounter;
        bool isLooping = 0;

        std::unique_ptr<InstructionQueue> instructions;
        std::unique_ptr<SymbolTable> symbolTable;
        std::unique_ptr<Logger> logger;

        std::mutex mtx;
};