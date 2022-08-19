#ifndef MinMaxPruningSolverGuard
#define MinMaxPruningSolverGuard

#include "SudokuSolver.h"
#include "Sudoku.h"

class MinMaxPruningSolver : public SudokuSolver<MinMaxPruningSolver>
{
public:
    bool findWinner();

private:
    int solveWithMinMaxPruning();
    int minUtility(Sudoku::GameState &state, int alpha, int beta);
    int maxUtility(Sudoku::GameState &state, int alpha, int beta);
};

#endif