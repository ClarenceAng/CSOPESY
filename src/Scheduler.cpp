#include "Scheduler.h"

Scheduler::Scheduler() {
    running = true;

    for (int i = 0; i < config.numCpu; i++) {
        cpuUsage.push_back(false);
    }

    cpuReadyQueues.resize(config.numCpu);

    if (config.schedulerType == FCFS) {
        for (int i = 0; i < config.numCpu; i++) {
            cpuThreads.emplace_back(&Scheduler::runFCFSScheduler, this, i);
        }
    } else if (config.schedulerType == RR) {
        for (int i = 0; i < config.numCpu; i++) {
            cpuThreads.emplace_back(&Scheduler::runRRScheduler, this, i);
        }
    }
}

Scheduler* Scheduler::singleton = nullptr;

void Scheduler::initialize() {
    singleton = new Scheduler();
}

Scheduler* Scheduler::getInstance() {
    return singleton;
}

void Scheduler::destroy() {
    delete singleton;
}

void Scheduler::stop() {
    stopGenerator();
    running = false;

    for (auto& t : cpuThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void Scheduler::generateSingleProcess(std::string processName) {
    if (!ConsoleManager::getInstance()->registerScreen(processName, true)) {
        return;
    }

    ConsoleManager::getInstance()->switchConsole(processName);
    
    auto [coreNumber, processNumber] = getProcessValues();

    std::thread(&Scheduler::generateProcess, this, processNumber, processName, coreNumber).detach();
}

void Scheduler::generateMultipleProcesses() {
    uint64_t ticks = 1;

    while (generatorRunning) {
        if (ticks != config.batchProcessFreq) {
            ticks++;
        } else {
            auto [coreNumber, processNumber] = getProcessValues();
            uint64_t processNameNo = processNumber;

            // handle duplicate process names
            std::string processName = "process" + std::to_string(processNameNo);
            while (!ConsoleManager::getInstance()->registerScreen(processName, false)) {
                processNameNo++;
                processName = "process" + std::to_string(processNameNo);
            }

            std::thread(&Scheduler::generateProcess, this, processNumber, processName, coreNumber).detach();

            ticks = 1;
        } 
    }
}

void Scheduler::startGenerator() {
    generatorRunning = true;
    generatorThread = std::thread(&Scheduler::generateMultipleProcesses, this);
}

void Scheduler::stopGenerator() {
    generatorRunning = false;
    if (generatorThread.joinable()) {
        generatorThread.join();
    }
}

bool Scheduler::isGeneratorRunning() {
    return generatorRunning;
}

void Scheduler::generateProcess(uint64_t processId, std::string processName, uint8_t coreNumber) { 
    std::shared_ptr<Process> process = std::make_shared<Process>(processId, processName, coreNumber, getTimestamp());

    ConsoleManager::getInstance()->setScreenProcess(process);

    std::lock_guard<std::mutex> lock(process_mutex);
    cpuReadyQueues[coreNumber].push(process);
}

void Scheduler::runFCFSScheduler(uint8_t coreNumber) {
    uint64_t ticks;

    while(running) {
        if (!isReadyQueueEmpty(coreNumber)) {
            // gets process pointer
            auto process = getProcess(coreNumber);
            setCpuUsage(coreNumber, true);
            ticks = 0;
            
            while (!process->isFinished() && running) {
                if (ticks == config.delaysPerExec) {
                    process->executeInstruction();
                    ticks = 0;
                } else {
                    ticks++;
                }
            }

            // dequeues after done
            dequeueProcess(coreNumber);
            setCpuUsage(coreNumber, false);
        }
        // wait for processes if there is none
    }
}

void Scheduler::runRRScheduler(uint8_t coreNumber) {
    uint64_t ticks;

    while(running) {
        if (!isReadyQueueEmpty(coreNumber)) {
            // gets process pointer
            auto process = getProcess(coreNumber);
            setCpuUsage(coreNumber, true);
            ticks = 0;

            // for loop for quantum cycles
            for (uint64_t i = 0; i < config.quantumCycles && running; i++) {
                // delay before executing
                while (ticks != config.delaysPerExec && running) {
                    ticks++;
                }
            
                process->executeInstruction();
                ticks = 0;

                if (process->isFinished()) {
                    break;
                }
            }
            
            if (!process->isFinished()) {
                rotateProcess(coreNumber);
            } else {
                dequeueProcess(coreNumber);
                setCpuUsage(coreNumber, false);
            }
        }
        // wait for processes if there is none
    }
}

std::tuple<uint8_t, uint64_t> Scheduler::getProcessValues() {
    std::lock_guard<std::mutex> lock(counter_mutex);
    uint8_t coreNumber = coreCounter++;
    uint64_t processNumber = processCounter++;

    if (coreCounter == config.numCpu) {
        coreCounter = 0;
    }

    return {coreNumber, processNumber};
}

bool Scheduler::isReadyQueueEmpty(uint8_t coreNumber) {
    std::lock_guard<std::mutex> lock(process_mutex);
    return cpuReadyQueues[coreNumber].empty();
}

std::shared_ptr<Process> Scheduler::getProcess(uint8_t coreNumber) {
    std::lock_guard<std::mutex> lock(process_mutex);
    return cpuReadyQueues[coreNumber].front();
}

void Scheduler::dequeueProcess(uint8_t coreNumber) {
    std::lock_guard<std::mutex> lock(process_mutex);
    finishedProcesses.push_back(cpuReadyQueues[coreNumber].front());
    cpuReadyQueues[coreNumber].pop();
}

void Scheduler::rotateProcess(uint8_t coreNumber) {
    std::lock_guard<std::mutex> lock(process_mutex);
    cpuReadyQueues[coreNumber].push(cpuReadyQueues[coreNumber].front());
    cpuReadyQueues[coreNumber].pop();
}

void Scheduler::setCpuUsage(uint8_t coreNumber, bool isUsing) {
    std::lock_guard<std::mutex> lock(usage_mutex);
    cpuUsage[coreNumber] = isUsing;
}

void Scheduler::getCpuUtilization(bool isFileOutput) {
    std::lock_guard<std::mutex> pLock(process_mutex);
    std::lock_guard<std::mutex> uLock(usage_mutex);
    
    int cpuUtil;
    uint8_t coresUsed = 0;
    uint8_t coresAvailable = 0;

    for (const auto& cpu : cpuUsage) {
        if (cpu == true) {
            coresUsed++;
        } else {
            coresAvailable++;
        }
    }

    cpuUtil = static_cast<int>(static_cast<double>(coresUsed) / config.numCpu * 100);

    if (isFileOutput == false) {
        std::cout << "CPU Utilization: " << cpuUtil << "%" << std::endl;
        std::cout << "Cores Used: " << static_cast<int>(coresUsed) << std::endl;
        std::cout << "Cores Available: " << static_cast<int>(coresAvailable )<< std::endl;
        std::cout << std::endl << "----------------------------------------------" << std::endl;
        std::cout << "Running Processes:" << std::endl;
        for (int i = 0; i < config.numCpu; i++) {
            if (cpuReadyQueues[i].empty()) continue;
            const auto& process = cpuReadyQueues[i].front();
            std::cout << std::format("{:<25s}", truncateString(process->getProcessName()))
                      << "   "
                      << "(" << process->getProcessTimestamp() << ")"
                      << "   "
                      << "Core: " << std::format("{:<3d}", i)
                      << "   "
                      << process->getLineNumber() << " / " << process->getInstructionSize()
                      << std::endl;
        }
        std::cout << std::endl << "Finished Processes:" << std::endl;
        for (const auto& process : finishedProcesses) {
            std::cout << std::format("{:<25s}", truncateString(process->getProcessName()))
                      << "   "
                      << "(" << process->getProcessTimestamp() << ")"
                      << "   " 
                      << "Finished"
                      << "   " 
                      << process->getLineNumber() << " / " << process->getInstructionSize() 
                      << std::endl;
        }
        std::cout << "----------------------------------------------" << std::endl << std::endl;
    } else {
        std::ofstream report;

        report.open("csopesy-log.txt");

        if (!report.is_open()) {
            std::cerr << "Error: File cannot be opened." << std::endl;
            return;
        }

        if (report) {
            report << "CPU Utilization: " << cpuUtil << "%" << std::endl;
            report << "Cores Used: " << static_cast<int>(coresUsed) << std::endl;
            report << "Cores Available: " << static_cast<int>(coresAvailable )<< std::endl;
            report << std::endl << "----------------------------------------------" << std::endl;
            report << "Running Processes:" << std::endl;
            for (int i = 0; i < config.numCpu; i++) {
                if (cpuReadyQueues[i].empty()) continue;
                const auto& process = cpuReadyQueues[i].front();
                report << std::format("{:<25s}", truncateString(process->getProcessName()))
                        << "   "
                        << "(" << process->getProcessTimestamp() << ")"
                        << "   "
                        << "Core: " << std::format("{:<3d}", i)
                        << "   "
                        << process->getLineNumber() << " / " << process->getInstructionSize() 
                        << std::endl;
            }
            report << std::endl << "Finished Processes:" << std::endl;
            for (const auto& process : finishedProcesses) {
                report << std::format("{:<25s}", truncateString(process->getProcessName()))
                        << "   "
                        << "(" << process->getProcessTimestamp() << ")"
                        << "   " 
                        << "Finished"
                        << "   " 
                        << process->getLineNumber() << " / " << process->getInstructionSize() 
                        << std::endl;
            }
            report << "----------------------------------------------" << std::endl << std::endl;
        }

        report.close();
    }
}