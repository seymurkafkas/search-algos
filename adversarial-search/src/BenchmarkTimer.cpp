#include "BenchmarkTimer.h"

BenchmarkTimer::BenchmarkTimer(std::string benchmarkDescriptor = "")
{
    startTimePoint = std::chrono::high_resolution_clock::now();
    customBenchmarkDescription = benchmarkDescriptor;
}

BenchmarkTimer::~BenchmarkTimer()
{
    endTimePoint = std::chrono::high_resolution_clock::now();
    long long runtimeInBlockInMicroSeconds = calculateRuntimeInMicroSeconds();
    displayRuntime(runtimeInBlockInMicroSeconds);
}

long long BenchmarkTimer::calculateRuntimeInMicroSeconds()
{
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
    auto runtimeInMicroSeconds = end - start;
    return runtimeInMicroSeconds;
}

void BenchmarkTimer::displayRuntime(long long runtimeInMicroSeconds)
{
    std::cout << "The block " << (customBenchmarkDescription == "" ? "" : "for ") << customBenchmarkDescription << " ran for " << runtimeInMicroSeconds << " microseconds." << std::endl;
}