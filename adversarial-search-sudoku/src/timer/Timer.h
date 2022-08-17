#ifndef TimerGuard
#define TimerGuard

#include <string>
#include <chrono>
#include <iostream>

class Timer
{
public:
    Timer(std::string benchmarkDescriptor);
    ~Timer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
    std::string customBenchmarkDescription;
    long long calculateRuntimeInMicroSeconds();
    void displayRuntime(long long runtimeInMicroSeconds);
};

#endif