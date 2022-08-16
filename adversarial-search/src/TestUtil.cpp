#include "TestUtil.h"
#include <iostream>

void Test::printAvailableActions(Sudoku::GameState &state)
{
    auto actionVector = state.getAvailableActions();
    for (auto &elem : actionVector)
    {
        std::cout << "At position (" << elem.position.first << ", " << elem.position.second << ") : " << elem.number << std::endl;
    }
}

void Test::printUnassignedEntries(Sudoku::GameState &state)
{
    std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> &unassignedEntries = state.unassignedEntries;
    for (auto position : unassignedEntries)
    {
        std::cout << "Unassigned position at (" << position.first << ", " << position.second << ")" << std::endl;
    }
}
