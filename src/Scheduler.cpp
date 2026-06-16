#include "Scheduler.h"

Scheduler::Scheduler() {
    // temporary, replace with actual implementation later
    cpuThreads.emplace_back(&Scheduler::runScheduler, this, 0);
    cpuReadyQueues.resize(1);
    running = true;
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
    
    std::lock_guard<std::mutex> lock(counter_mutex);

    std::thread(&Scheduler::generateProcess, this, processCounter, processName, coreCounter).detach();
    processCounter++;
    coreCounter++;

    if (coreCounter == config.numCpu) {
        coreCounter = 0;
    }
}

void Scheduler::generateProcess(uint64_t processId, std::string processName, uint8_t coreNumber) { 
    std::shared_ptr<Process> process = std::make_shared<Process>(processId, processName, coreNumber);

    ConsoleManager::getInstance()->consoleTable[processName]->setProcess(process);

    // adds process pointer to vector (for traversal) and cpu queue (for running)
    std::unique_lock lock(process_mutex);
    processList.push_back(process);
    cpuReadyQueues[coreNumber].push(process);
}

void Scheduler::runScheduler(uint8_t coreNumber) {
    while(running) {
        if (!isReadyQueueEmpty(coreNumber)) {
            // gets process pointer
            Process& process = getProcess(coreNumber);

            // loops until finished (fcfs)
            while (!process.isFinished() && running) {
                process.executeInstruction();
            }

            // dequeues after done
            dequeueProcess(coreNumber);
        }

        // wait for processes if there is none
    }
}

bool Scheduler::isReadyQueueEmpty(uint8_t coreNumber) {
    std::shared_lock lock(process_mutex);
    return cpuReadyQueues[coreNumber].empty();
}

Process& Scheduler::getProcess(uint8_t coreNumber) {
    std::shared_lock lock(process_mutex);
    return *cpuReadyQueues[coreNumber].front();
}

void Scheduler::dequeueProcess(uint8_t coreNumber) {
    std::unique_lock lock(process_mutex);
    cpuReadyQueues[coreNumber].pop();
}