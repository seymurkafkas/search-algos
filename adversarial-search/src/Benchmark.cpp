#include "Benchmark.h"
#include <iostream>

int Benchmark::totalGeneratedNodeCount;
int Benchmark::currentNodeCountInMemory;
int Benchmark::maximumNodeCountInMemory;

void Benchmark::increaseNodeCountInMemory()
{
    currentNodeCountInMemory++;
    totalGeneratedNodeCount++;
    if (currentNodeCountInMemory > maximumNodeCountInMemory)
    {
        maximumNodeCountInMemory = currentNodeCountInMemory;
    }
}

void Benchmark::decreaseNodeCountInMemory()
{
    currentNodeCountInMemory--;
}

void Benchmark::initialiseNodeBookkeeper()
{
    totalGeneratedNodeCount = 0;
    currentNodeCountInMemory = 0;
    maximumNodeCountInMemory = 0;
}

void Benchmark::printResults()
{
    std::cout << "Total count of generated nodes:" << totalGeneratedNodeCount << std::endl;
    std::cout << "The maximum number of nodes in memory:" << maximumNodeCountInMemory << std::endl;
}

void Benchmark::reset()
{
    totalGeneratedNodeCount = 0;
    currentNodeCountInMemory = 0;
    maximumNodeCountInMemory = 0;
}
