#include "MinMaxSolver.h"
#include <iostream>

bool MinMaxSolver::findWinner()
{
    return solveWithMinMax() == 1;
}

int MinMaxSolver::solveWithMinMax()
{
    Sudoku::GameState initialState = this->gameToSolve.initialState;
    if (initialState.isTerminalState())
    {
        return -1;
    }
    int maxChildUtility = -1; // min possible utility (INF)
    for (auto action : initialState.getAvailableActions())
    {
        Sudoku::GameState successorState = initialState.getNextState(action);
        maxChildUtility = std::max(maxChildUtility, minUtility(successorState));
    }
    return maxChildUtility;
}

int MinMaxSolver::minUtility(Sudoku::GameState &state)
{
    state.applyAction();
    if (state.isTerminalState())
    {
        state.undoAction();
        return 1;
    }
    int minChildUtility = 1; // max possible utility (INF)
    for (auto action : state.getAvailableActions())
    {
        Sudoku::GameState successorState = state.getNextState(action);
        minChildUtility = std::min(minChildUtility, maxUtility(successorState));
    }
    state.undoAction();
    return minChildUtility;
}

int MinMaxSolver::maxUtility(Sudoku::GameState &state)
{
    state.applyAction();
    if (state.isTerminalState())
    {
        state.undoAction();
        return -1;
    }
    int maxChildUtility = -1; // min possible utility
    for (auto action : state.getAvailableActions())
    {
        Sudoku::GameState successorState = state.getNextState(action);
        maxChildUtility = std::max(maxChildUtility, minUtility(successorState));
    }
    state.undoAction();
    return maxChildUtility;
}
