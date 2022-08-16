#ifndef AgentGuard
#define AgentGuard

#include "Sudoku.h"
#include "SudokuSolver.h"

template <typename T>
class Agent
{
public:
    Agent(Sudoku::Game &game);
    int getVictor();

private:
    Sudoku::Game &gameInstance;
    SudokuSolver<T> solver;
};

#endif