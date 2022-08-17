#include "Agent.h"


template <typename T>
Agent<T>::Agent(Sudoku::Game &game) : gameInstance(game), solver(game)
{
}

template <typename T>
int Agent<T>::getVictor()
{
    return solver.findWinner() ? 1 : 2;
}

template class Agent<MinMaxSolver>;
template class Agent<MinMaxPruningSolver>;