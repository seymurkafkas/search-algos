#ifndef SudokuSolverGuard
#define SudokuSolverGuard

#include "Sudoku.h"

template <typename T>
class SudokuSolver
{
public:
    SudokuSolver(Sudoku::Game &game);
    bool findWinner();

protected:
    Sudoku::Game &gameToSolve;
};

#endif