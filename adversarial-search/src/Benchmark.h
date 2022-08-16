#ifndef BenchmarkGuard
#define BenchmarkGuard

class Benchmark
{
public:
    static void increaseNodeCountInMemory();
    static void decreaseNodeCountInMemory();
    static void initialiseNodeBookkeeper();
    static void printResults();
    static void reset();

private:
    static int totalGeneratedNodeCount;
    static int currentNodeCountInMemory;
    static int maximumNodeCountInMemory;
};

#endif