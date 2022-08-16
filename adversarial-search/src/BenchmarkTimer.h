#ifndef BenchmarkTimerGuard
#define BenchmarkTimerGuard

#include <string>
#include <chrono>
#include <iostream>

class BenchmarkTimer
{
public:
    BenchmarkTimer(std::string benchmarkDescriptor);
    ~BenchmarkTimer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
    std::string customBenchmarkDescription;
    long long calculateRuntimeInMicroSeconds();
    void displayRuntime(long long runtimeInMicroSeconds);
};

#endif