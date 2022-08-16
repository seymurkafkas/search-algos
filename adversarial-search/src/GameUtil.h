#ifndef GameUtilGuard
#define GameUtilGuard
#include <fstream>
#include <unordered_set>
#include "Hasher.h"

namespace GameUtil
{
    int **allocateMemoryForGrid();
    void fillGridAndMapWithData(int **grid, std::unordered_set<Coordinates, CoordinateHasher, CoordinateEquivalence> &unassignedEntries, std::fstream &fileHandle);
    void printGridInfo(int **grid);
};

#endif