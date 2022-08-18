#include <iostream>
#include <Benchmark.h>

void Benchmark::increaseNodeCountInMemory()
{
    currentNodeCountInMemory++;
    totalGeneratedNodeCount++;
    if (currentNodeCountInMemory > maximumNodeCountInMemory)
        maximumNodeCountInMemory = currentNodeCountInMemory;
}

void Benchmark::decreaseNodeCountInMemory()
{
    currentNodeCountInMemory--;
}

void Benchmark::increaseNumberOfNodeExpansions()
{
    numberOfNodeExpansions++;
}

void Benchmark::initialiseNodeBookkeeper()
{
    totalGeneratedNodeCount = 0;
    currentNodeCountInMemory = 0;
    maximumNodeCountInMemory = 0;
    numberOfNodeExpansions = 0;
}

void Benchmark::printResults()
{

    std::cout << "Total count of generated nodes:" << totalGeneratedNodeCount << std::endl;
    std::cout << "The maximum number of nodes in memory:" << maximumNodeCountInMemory << std::endl;
    std::cout << "The number of node expansions:" << numberOfNodeExpansions << std::endl;
}

int Benchmark::totalGeneratedNodeCount;
int Benchmark::currentNodeCountInMemory;
int Benchmark::maximumNodeCountInMemory;
int Benchmark::numberOfNodeExpansions;