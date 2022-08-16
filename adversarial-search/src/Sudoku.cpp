#include "Sudoku.h"
#include "FileHandler.h"
#include "GameUtil.h"
#include <iostream>

Sudoku::Game::Game()
{
}

Sudoku::Game::Game(std::string filePath)
{
    FileHandler::createGameFromFile(filePath, *this);
}

void Sudoku::Game::setGrid(int **grid)
{
    this->grid = grid;
}

void Sudoku::Game::printMap()
{
    GameUtil::printGridInfo(grid);
}

void Sudoku::Game::setInitialState(GameState initialState)
{
    this->initialState = initialState;
}

bool Sudoku::GameState::isActionValid(Action consideredAction)
{
    if (checkRowConstraint(consideredAction) && checkColumnConstraint(consideredAction) && checkGroupConstraint(consideredAction))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Sudoku::GameState::checkRowConstraint(Action consideredAction)
{
    int row = consideredAction.position.first;
    for (int column = 0; column < 9; column++)
    {
        if (grid[row][column] == consideredAction.number)
        {
            return false;
        }
    }
    return true;
}

bool Sudoku::GameState::checkColumnConstraint(Action consideredAction)
{
    int column = consideredAction.position.second;
    for (int row = 0; row < 9; row++)
    {
        if (grid[row][column] == consideredAction.number)
        {
            return false;
        }
    }
    return true;
}

bool Sudoku::GameState::checkGroupConstraint(Action consideredAction)
{
    int topLeftRow = consideredAction.position.first - (consideredAction.position.first % 3);
    int topLeftColumn = consideredAction.position.second - (consideredAction.position.second % 3);
    for (int rowDiff = 0; rowDiff < 3; rowDiff++)
    {
        for (int columnDiff = 0; columnDiff < 3; columnDiff++)
        {
            if (grid[topLeftRow + rowDiff][topLeftColumn + columnDiff] == consideredAction.number)
            {
                return false;
            }
        }
    }

    return true;
}

void Sudoku::GameState::fillAvailableActions()
{
    for (auto position : unassignedEntries)
    {
        Action potentialAction;
        potentialAction.position = position;
        for (int i = 1; i <= 9; i++)
        {
            potentialAction.number = i;
            if (isActionValid(potentialAction))
            {
                availableActions.push_back(potentialAction);
            }
        }
    }
    return;
}

std::vector<Sudoku::Action> &Sudoku::GameState::getAvailableActions()
{
    if (!areActionsInitialized)
    {
        fillAvailableActions();
        areActionsInitialized = true;
    }
    return availableActions;
}

int Sudoku::GameState::getRow(Coordinates position)
{
    int topLeftRow = position.first - (position.first % 3);
    return topLeftRow;
}

int Sudoku::GameState::getColumn(Coordinates position)
{
    int topLeftColumn = position.second - (position.second % 3);
    return topLeftColumn;
}

int Sudoku::GameState::getGroup(Coordinates position)
{
    int groupNumber = (int)(position.first % 3) * 3 + position.second % 3;
    return groupNumber;
}

void Sudoku::GameState::applyAction()
{
    int row = previousAction.position.first;
    int column = previousAction.position.second;
    int assignedNumber = previousAction.number;
    grid[row][column] = assignedNumber;
}

void Sudoku::GameState::undoAction()
{
    int row = previousAction.position.first;
    int column = previousAction.position.second;
    grid[row][column] = 0;
}

Sudoku::GameState::GameState(GameState &previous, Action prevAction)
{
    Benchmark::increaseNodeCountInMemory();
    unassignedEntries = previous.unassignedEntries;
    unassignedEntries.erase(prevAction.position);
    previousAction = prevAction;
    areActionsInitialized = false;
    grid = previous.grid;
}

Sudoku::GameState::GameState()
{
    Benchmark::increaseNodeCountInMemory();
}

Sudoku::GameState::~GameState()
{
    Benchmark::decreaseNodeCountInMemory();
}

Sudoku::GameState::GameState(std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> unassignedEntries, int **grid, bool areActionsInitialized, Action previousAction) : unassignedEntries(unassignedEntries), grid(grid), areActionsInitialized(areActionsInitialized), previousAction(previousAction)
{
    Benchmark::increaseNodeCountInMemory();
}

bool Sudoku::GameState::isTerminalState()
{
    if (unassignedEntries.empty())
    {
        return true;
    }
    else if (getAvailableActions().empty())
    {
        return true;
    }
    return false;
}

Sudoku::GameState Sudoku::GameState::getNextState(Action currentAction)
{
    return GameState(*this, currentAction);
}
