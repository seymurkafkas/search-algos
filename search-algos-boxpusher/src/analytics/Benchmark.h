#ifndef ANALYTICS_BENCHMARK_H
#define ANALYTICS_BENCHMARK_H

class Benchmark
{
public:
    static void increaseNodeCountInMemory();
    static void decreaseNodeCountInMemory();
    static void increaseNumberOfNodeExpansions();
    static void initialiseNodeBookkeeper();
    static void printResults();

private:
    static int totalGeneratedNodeCount;
    static int currentNodeCountInMemory;
    static int maximumNodeCountInMemory;
    static int numberOfNodeExpansions;
};

#endif