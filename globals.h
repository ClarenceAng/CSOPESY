#pragma once

#include <cstdint>

enum SchedulerType {
    FCFS,
    RR
};

// config
inline uint8_t cfg_numCpu;
inline SchedulerType cfg_schedulerType;
inline uint32_t cfg_quantumCycles;
inline uint32_t cfg_batchProcessFreq;
inline uint32_t cfg_minIns;
inline uint32_t cfg_maxIns;
inline uint32_t cfg_delaysPerExec;

// globals
inline std::atomic<uint32_t> gl_cpuCycles;
inline uint32_t gl_processCount;