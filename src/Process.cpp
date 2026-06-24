#include "Process.h"
#include <random>
#include <vector>

Process::Process(uint64_t processId, std::string name, uint8_t coreNumber, std::string timestamp) {
    this->processId = processId;
    this->name = name;
    this->coreNumber = coreNumber;
    this->timestamp = timestamp;

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
        lineNumber += instructions->front()->getExecutionCount();
        instructions->pop();
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

uint64_t Process::getProcessId() {
    return processId;
}

std::string Process::getProcessName() {
    return name;
}

uint64_t Process::getInstructionSize() {
    return instructionSize;
}

uint64_t Process::getLineNumber() {
    return lineNumber;
}

std::string Process::getProcessTimestamp() {
    return timestamp;
}

void Process::generateInstructions() {
    std::mt19937 rng(std::random_device{}());
    uint64_t n = std::uniform_int_distribution<uint64_t>(config.minIns, config.maxIns)(rng);
    instructionSize = n;

    ForLoop temp;
    generateInstructionList(temp, n, 0);
    for (auto& instr : temp)
        instructions->push(std::move(instr));
}

void Process::generateInstructionList(ForLoop& container, uint64_t& budget, int depth) {
    thread_local std::mt19937 rng(std::random_device{}());
    while (budget > 0) {
        bool makeFor = depth < 3 && budget >= 2 &&
                       std::uniform_int_distribution<int>(0, 99)(rng) < 30;
        if (makeFor) {
            uint64_t maxRep = std::min<uint64_t>(4, budget);
            uint64_t repeats = std::uniform_int_distribution<uint64_t>(2, maxRep)(rng);
            uint64_t bodyBudget = budget / repeats;
            uint64_t savedBodyBudget = bodyBudget;
            auto body = makeForLoop();
            generateInstructionList(*body, bodyBudget, depth + 1);
            budget -= savedBodyBudget * repeats;
            container.push_back(cmdFor(std::move(body), static_cast<uint16_t>(repeats)));
        } else {
            container.push_back(makeRandomSimpleInstruction());
            budget--;
        }
    }
}

std::unique_ptr<Instruction> Process::makeRandomSimpleInstruction() {
    static const std::vector<std::string> vars = {"v0", "v1", "v2", "v3", "v4"};
    thread_local std::mt19937 rng(std::random_device{}());
    auto randVar = [&]() -> const std::string& {
        return vars[std::uniform_int_distribution<int>(0, 4)(rng)];
    };
    auto randVal = [&]() -> uint16_t {
        return static_cast<uint16_t>(std::uniform_int_distribution<int>(0, 255)(rng));
    };

    auto useLit = [&]{ return std::uniform_int_distribution<int>(0, 1)(rng) == 0; };
    switch (std::uniform_int_distribution<int>(0, 4)(rng)) {
        case 0: return useLit()
                    ? cmdPrint("Hello world from " + name + "!")
                    : cmdPrint(std::string(randVar()));
        case 1: return cmdDeclare(randVar(), randVal());
        case 2: return useLit() ? cmdAdd(randVar(), randVal(), randVal())
                                : cmdAdd(randVar(), randVar(), randVar());
        case 3: return useLit() ? cmdSubtract(randVar(), randVal(), randVal())
                                : cmdSubtract(randVar(), randVar(), randVar());
        case 4: return cmdSleep(static_cast<uint8_t>(std::uniform_int_distribution<int>(1, 10)(rng)));
        default: return cmdPrint("Hello world from " + name + "!");
    }
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

std::unique_ptr<Instruction> Process::cmdAdd(std::string var1, uint16_t lit2, uint16_t lit3) {
    makeVariable(var1);
    return std::make_unique<IAdd>(symbolTable->at(var1), lit2, lit3);
}

std::unique_ptr<Instruction> Process::cmdSubtract(std::string var1, std::string var2, std::string var3) {
    makeVariable(var1);
    makeVariable(var2);
    makeVariable(var3);
    return std::make_unique<ISubtract>(symbolTable->at(var1), symbolTable->at(var2), symbolTable->at(var3));
}

std::unique_ptr<Instruction> Process::cmdSubtract(std::string var1, uint16_t lit2, uint16_t lit3) {
    makeVariable(var1);
    return std::make_unique<ISubtract>(symbolTable->at(var1), lit2, lit3);
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