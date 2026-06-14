#include "Process.h"

Process::Process(uint32_t processId, std::string name, uint8_t coreNumber) {
    this->processId = processId;
    this->name = name;
    this->coreNumber = coreNumber;

    instructions = std::make_unique<InstructionQueue>();
    symbolTable = std::make_unique<SymbolTable>();
    logger = std::make_unique<Logger>();

    generateInstructions();
}

void Process::executeInstruction() {
    std::unique_lock lock(mtx);
    instructions->front()->execute();

    // checks whether instruction is FOR or SLEEP and is still running before removing the instruction
    if (!instructions->front()->isLooping()) {
        instructions->pop();
        lineNumber++;
    }
}

void Process::displayLog() {
    std::shared_lock lock(mtx);
    std::cout << "Logs:" << std::endl;
    for (const auto& log : *logger) {
        std::cout << "(" << std::get<0>(log) << ") Core:" << static_cast<int>(coreNumber) << " \"" << std::get<1>(log) << "\"" << std::endl;
    }
}

bool Process::isFinished() {
    std::shared_lock lock(mtx);
    return instructions->empty();
}

uint32_t Process::getProcessId() {
    return processId;
}

std::string Process::getProcessName() {
    return name;
}

uint32_t Process::getInstructionSize() {
    return instructions->size();
}

uint32_t Process::getLineNumber() {
    return lineNumber;
}

void Process::generateInstructions() {
    // TODO: Replace with randomization
    std::string msg = "Hello world from " + name + "!";
    
    // Example format
    instructions->push(cmdPrint(msg));
    instructions->push(cmdDeclare("var1", 5));
    instructions->push(cmdDeclare("var2", 10));
    instructions->push(cmdDeclare("var3", 2));
    instructions->push(cmdAdd("var4", "var1", "var2"));
    instructions->push(cmdSubtract("var5", "var2", "var3"));
    instructions->push(cmdPrint("var4"));
    instructions->push(cmdPrint("var5"));
    instructions->push(cmdSleep(10));

    // For loop instructions have to be delcared first then have instructions added
    auto forLoop1 = makeForLoop();
    auto forLoop2 = makeForLoop();

    forLoop1->push_back(cmdAdd("var6", "var6", "var1"));
    forLoop1->push_back(cmdPrint("var6"));
    // For loop contains nested for loop
    forLoop2->push_back(cmdFor(std::move(forLoop1), 2));
    
    instructions->push(cmdFor(std::move(forLoop2), 3));

    /*
    PRINT(msg)
    DECLARE(var1, 5)
    DECLARE(var2, 10)
    DECLARE(var3, 2)
    ADD(var4, var1, var2)
    SUBTRACT(var5, var2, var3)
    PRINT(var4)
    PRINT(var5)
    SLEEP(10)
    FOR([FOR([ADD(var6, var6, var1)], 2), PRINT(var6)]3)
    */
}

void Process::makeVariable(std::string var) {
    if (symbolTable->contains(var)) {
        return;
    }

    symbolTable->emplace(var, 0);
}

std::unique_ptr<Instruction> Process::cmdPrint(std::string msg) {
    return std::make_unique<IPrint>(msg, *symbolTable, *logger);
}

std::unique_ptr<Instruction> Process::cmdDeclare(std::string var, uint16_t val) {
    makeVariable(var);
    return std::make_unique<IDeclare>(symbolTable->at(var), val);
}

std::unique_ptr<Instruction> Process::cmdAdd(std::string var1, std::string var2, std::string var3) {
    makeVariable(var1);
    makeVariable(var2);
    makeVariable(var3);
    return std::make_unique<IAdd>(symbolTable->at(var1), symbolTable->at(var2), symbolTable->at(var3));
}

std::unique_ptr<Instruction> Process::cmdSubtract(std::string var1, std::string var2, std::string var3) {
    makeVariable(var1);
    makeVariable(var2);
    makeVariable(var3);
    return std::make_unique<ISubtract>(symbolTable->at(var1), symbolTable->at(var2), symbolTable->at(var3));
}

std::unique_ptr<Instruction> Process::cmdSleep(uint8_t ticks) {
    return std::make_unique<ISleep>(ticks);
}

std::unique_ptr<Instruction> Process::cmdFor(std::unique_ptr<ForLoop> instructions, uint16_t repeats) {
    return std::make_unique<IFor>(std::move(instructions), repeats);
}

std::unique_ptr<ForLoop> Process::makeForLoop() {
    return std::make_unique<ForLoop>();
}