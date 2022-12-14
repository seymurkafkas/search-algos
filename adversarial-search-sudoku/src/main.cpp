#include <iostream>
#include <string>
#include <fstream>
#include "Timer.h"
#include "Benchmark.h"
#include "Sudoku.h"
#include "Agent.h"
#include "MinMaxPruningSolver.h"
#include "MinMaxSolver.h"

std::string fetchInputFilePath(int argc, char **argv)
{
    if (argc != 2)
        throw std::string("Invalid Number of Command Line Arguments\n");
    else
    {
        std::string inputFilePath = argv[1];
        return inputFilePath;
    }
}

int main(int argc, char **argv)
{
    try
    {
        std::string inputFilePath = fetchInputFilePath(argc, argv);
        Benchmark::initialiseNodeBookkeeper();
        Sudoku::Game gameInstance(inputFilePath);
        Agent<MinMaxPruningSolver> pruningAgent(gameInstance); // Alpha-Beta pruning algorithm
        {                                                      // Timer and node bookkeepers
            Timer timerForPruningBlock("Alpha-Beta Pruning Algorithm");
            std::cout << pruningAgent.getVictor() << std::endl;
        }
        Benchmark::printResults();
    }
    catch (std::string err)
    {
        std::cout << err;
    }
    return 0;
}