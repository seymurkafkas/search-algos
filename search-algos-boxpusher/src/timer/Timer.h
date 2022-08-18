#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include <string>
#include <chrono>

class Timer
{
public:
    Timer(std::string benchmarkDescriptor = "");
    ~Timer();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTimePoint;
    std::string customBenchmarkDescription;

    long long calculateRuntimeInMicroSeconds();
    void displayRuntime(long long runtimeInMicroSeconds);
};

#endif