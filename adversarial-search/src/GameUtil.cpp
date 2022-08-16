#include "GameUtil.h"
#include "FileHandler.h"
#include <iostream>

int **GameUtil::allocateMemoryForGrid()
{
    int **grid = new int *[9];
    for (int i = 0; i < 9; ++i)
    {
        grid[i] = new int[9];
    }
    return grid;
}

void GameUtil::fillGridAndMapWithData(int **grid, std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> &unassignedEntries, std::fstream &fileHandle)
{
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            FileHandler::validateNextInput(fileHandle);

            fileHandle >> grid[row][column];
            if (grid[row][column] == 0)
            {
                Coordinates positionOfUnassignedEntry;
                positionOfUnassignedEntry.first = row;
                positionOfUnassignedEntry.second = column;
                unassignedEntries.insert(positionOfUnassignedEntry);
            }
        }
    }
}

void GameUtil::printGridInfo(int **grid)
{
    for (int row = 0; row < 9; ++row)
    {
        for (int column = 0; column < 9; column++)
        {
            std::cout << grid[row][column] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
}
