#include "Sudoku.h"
#include "MinMaxPruningSolver.h"
#include "MinMaxSolver.h"

template <typename T>
SudokuSolver<T>::SudokuSolver(Sudoku::Game &game) : gameToSolve(game)
{
}

template <typename T>
bool SudokuSolver<T>::findWinner()
{
    T &derivedSolver = static_cast<T &>(*this);
    return derivedSolver.findWinner();
}

template class SudokuSolver<MinMaxSolver>;
template class SudokuSolver<MinMaxPruningSolver>;