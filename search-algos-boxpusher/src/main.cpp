
#include <utility>
#include <string>
#include <Actions.h>
#include <BoxPusherGame.h>
#include <Benchmark.h>
#include <SearchAlgorithm.h>
#include <Timer.h>
#include <iostream>

std::pair<std::string, std::string> fetchCommandLineArguments(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        throw std::string("Invalid Number of Command Line Arguments\n");
    }
    else
    {
        std::string outputFileName = argc == 3 ? argv[2] : "output.txt";
        std::string inputFilePath = argv[1];
        return std::make_pair(inputFilePath, outputFileName);
    }
}

int main(int argc, char **argv)
{
    try
    {
        auto iOFilePair = fetchCommandLineArguments(argc, argv);
        std::string inputFilePath = iOFilePair.first;
        std::string outputFileName = iOFilePair.second;
        BoxPusherGame gameInstance(inputFilePath, outputFileName);
        Actions::defineActionSet();
        gameInstance.createGameFromFile();
        Benchmark::initialiseNodeBookkeeper();
        {
            Timer timerForBlock("Main Algorithm");
            gameInstance.searchForSolutionWithAlgorithm(aStar); // breadthFirst, iterativeDeepening, aStar
        }
        Benchmark::printResults();
    }
    catch (std::string err)
    {
        std::cout << err;
    }
    return 0;
}