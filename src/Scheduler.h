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
        void getCpuUtilization(bool isFileOutput);
        
    private:
        Scheduler();
        ~Scheduler() = default;
        
        void generateProcess(uint64_t processId, std::string processName, uint8_t coreNumber);
        void runFCFSScheduler(uint8_t coreNumber);
        void runRRScheduler(uint8_t coreNumber);

        std::tuple<uint8_t, uint64_t> getProcessValues();
        bool isReadyQueueEmpty(uint8_t coreNumber);
        std::shared_ptr<Process> getProcess(uint8_t coreNumber);
        void dequeueProcess(uint8_t coreNumber);
        void rotateProcess(uint8_t coreNumber);
        void setCpuUsage(uint8_t coreNumber, bool isUsing);

        uint64_t processCounter = 1;
        uint8_t coreCounter = 0;

        static Scheduler* singleton;
        std::atomic<bool> running = false;
        std::atomic<bool> generatorRunning = false;
        std::vector<bool> cpuUsage;
        std::vector<std::queue<std::shared_ptr<Process>>> cpuReadyQueues;
        std::vector<std::thread> cpuThreads;
        std::vector<std::shared_ptr<Process>> finishedProcesses;

        std::mutex counter_mutex;
        std::shared_mutex process_mutex;
        std::shared_mutex usage_mutex;
};