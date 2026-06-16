#pragma once

#include "globals.h"
#include "Process.h"
#include "ConsoleManager.h"

class Scheduler {
    public:
        static void initialize();
        static Scheduler* getInstance();
        static void destroy();
    
        void stop();
        void generateSingleProcess(std::string processName);
        void generateMultipleProcesses();
        void stopGenerator();
        
    private:
        Scheduler();
        ~Scheduler() = default;
        
        void generateProcess(uint64_t processId, std::string processName, uint8_t coreNumber);
        void runScheduler(uint8_t coreNumber);

        bool isReadyQueueEmpty(uint8_t coreNumber);
        Process& getProcess(uint8_t coreNumber);
        void dequeueProcess(uint8_t coreNumber);

        uint64_t processCounter = 1;
        uint8_t coreCounter = 0;

        static Scheduler* singleton;
        std::atomic<bool> running = false;
        std::atomic<bool> generatorRunning = false;
        std::vector<std::shared_ptr<Process>> processList; // for viewing screen -ls
        std::vector<std::queue<std::shared_ptr<Process>>> cpuReadyQueues;
        std::vector<std::thread> cpuThreads;

        std::mutex counter_mutex;
        std::shared_mutex process_mutex;
};