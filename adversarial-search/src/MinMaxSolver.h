#ifndef MinMaxSolverGuard
#define MinMaxSolverGuard

#include "Sudoku.h"
#include "SudokuSolver.h"

class MinMaxSolver : public SudokuSolver<MinMaxSolver>
{
public:
    bool findWinner();
    int solveWithMinMax();
    int minUtility(Sudoku::GameState &state);
    int maxUtility(Sudoku::GameState &state);
};

#endif