#include "FileHandler.h"
#include "GameUtil.h"
#include <iostream>

void FileHandler::validateNextInput(std::fstream &inputFileToValidate)
{
    if (!inputFileToValidate.good())
        throw "Error while reading input";
}

void FileHandler::validateFileHandler(std::fstream &fileHandlerToValidate)
{
    if (fileHandlerToValidate.fail())
        throw std::string("Invalid FilePath\n");
}

void FileHandler::fillGameFromFileHandler(Sudoku::Game &gameInstance, std::fstream &inputTextFileHandle)
{
    try
    {
        validateFileHandler(inputTextFileHandle);
        int **grid = GameUtil::allocateMemoryForGrid();
        std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> unassignedEntries;
        GameUtil::fillGridAndMapWithData(grid, unassignedEntries, inputTextFileHandle);
        Sudoku::Action nullAction;
        nullAction.number = -1;
        nullAction.position.first = -1;
        nullAction.position.second = -1;
        Sudoku::GameState initialState(unassignedEntries, grid, false, nullAction);
        gameInstance.setGrid(grid);
        gameInstance.setInitialState(initialState);
    }
    catch (std::string err)
    {
        std::cout << err;
    }
}

void FileHandler::createGameFromFile(std::string filePath, Sudoku::Game &gameInstance)
{
    std::fstream inputTextFileHandler;
    inputTextFileHandler.open(filePath);
    validateFileHandler(inputTextFileHandler);
    fillGameFromFileHandler(gameInstance, inputTextFileHandler);
}