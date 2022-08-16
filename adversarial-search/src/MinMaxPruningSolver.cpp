#include "MinMaxPruningSolver.h"

bool MinMaxPruningSolver::findWinner()
{
    return solveWithMinMaxPruning() == 1;
}

int MinMaxPruningSolver::solveWithMinMaxPruning()
{
    Sudoku::GameState initialState = this->gameToSolve.initialState;
    int alpha = -1;
    int beta = 1;
    if (initialState.isTerminalState())
    {
        return -1;
    }
    int maxChildUtility = -1; //min possible utility
    for (auto action : initialState.getAvailableActions())
    {
        Sudoku::GameState successorState = initialState.getNextState(action);
        maxChildUtility = std::max(maxChildUtility, minUtility(successorState, alpha, beta));
        if (maxChildUtility >= beta)
        {
            return maxChildUtility;
        }
        alpha = std::max(alpha, maxChildUtility);
    }
    return maxChildUtility;
}

int MinMaxPruningSolver::minUtility(Sudoku::GameState &state, int alpha, int beta)
{
    state.applyAction();
    if (state.isTerminalState())
    {
        state.undoAction();
        return 1;
    }
    int minChildUtility = 1; // min possible utility
    for (auto action : state.getAvailableActions())
    {
        Sudoku::GameState successorState = state.getNextState(action);
        minChildUtility = std::min(minChildUtility, maxUtility(successorState, alpha, beta));
        if (minChildUtility <= alpha)
        {
            state.undoAction();
            return minChildUtility;
        }
        beta = std::min(beta, minChildUtility);
    }
    state.undoAction();
    return minChildUtility;
}

int MinMaxPruningSolver::maxUtility(Sudoku::GameState &state, int alpha, int beta)
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
        maxChildUtility = std::max(maxChildUtility, minUtility(successorState, alpha, beta));
        if (maxChildUtility >= beta)
        {
            state.undoAction();
            return maxChildUtility;
        }
        alpha = std::max(alpha, maxChildUtility);
    }
    state.undoAction();
    return maxChildUtility;
}