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
    
    auto [coreNumber, processNumber] = getProcessValues();

    std::thread(&Scheduler::generateProcess, this, processNumber, processName, coreNumber).detach();
}

void Scheduler::generateMultipleProcesses() {
    generatorRunning = true;

    while (generatorRunning) {
        auto [coreNumber, processNumber] = getProcessValues();
        uint64_t processNameNo = processNumber;

        // handle duplicate process names
        std::string processName = "process" + std::to_string(processNameNo);
        while (!ConsoleManager::getInstance()->registerScreen(processName, false)) {
            processNameNo++;
            processName = "process" + std::to_string(processNameNo);
        }

        std::thread(&Scheduler::generateProcess, this, processNumber, processName, coreNumber).detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Scheduler::stopGenerator() {
    generatorRunning = false;
    // std::cout << generatorRunning << std::endl;
}

void Scheduler::generateProcess(uint64_t processId, std::string processName, uint8_t coreNumber) { 
    std::shared_ptr<Process> process = std::make_shared<Process>(processId, processName, coreNumber);

    ConsoleManager::getInstance()->consoleTable[processName]->setProcess(process);

    std::unique_lock lock(process_mutex);
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
            
            process->setStartingTimestamp();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Scheduler::runRRScheduler(uint8_t coreNumber) {

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
    std::shared_lock lock(process_mutex);
    return cpuReadyQueues[coreNumber].empty();
}

std::shared_ptr<Process> Scheduler::getProcess(uint8_t coreNumber) {
    std::shared_lock lock(process_mutex);
    return cpuReadyQueues[coreNumber].front();
}

void Scheduler::dequeueProcess(uint8_t coreNumber) {
    std::unique_lock lock(process_mutex);
    finishedProcesses.push_back(cpuReadyQueues[coreNumber].front());
    cpuReadyQueues[coreNumber].pop();
}

void Scheduler::setCpuUsage(uint8_t coreNumber, bool isUsing) {
    std::unique_lock lock(usage_mutex);
    cpuUsage[coreNumber] = isUsing;
}

void Scheduler::getCpuUtilization(bool isFileOutput) {
    std::shared_lock pLock(process_mutex);
    std::shared_lock uLock(usage_mutex);
    
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
            if (isReadyQueueEmpty(i)) continue;
            std::shared_ptr<Process> process = getProcess(i);
            std::cout << process->getProcessName() 
                      << "   " 
                      << "(" << process->getStartingTimestamp() << ")"
                      << "   " 
                      << "Core: " << i 
                      << "   " 
                      << process->getLineNumber() - 1 << " / " << process->getInstructionSize() 
                      << std::endl;
        }
        std::cout << std::endl << "Finished Processes:" << std::endl;
        for (const auto& process : finishedProcesses) {
            std::cout << process->getProcessName() 
                      << "   " 
                      << "(" << process->getStartingTimestamp() << ")"
                      << "   " 
                      << "Finished"
                      << "   " 
                      << process->getLineNumber() - 1 << " / " << process->getInstructionSize() 
                      << std::endl;
        }
        std::cout << "----------------------------------------------" << std::endl << std::endl;
    }
}