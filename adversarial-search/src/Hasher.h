#ifndef HasherGuard
#define HasherGuard

#include <unordered_map>
#include <unordered_set>

typedef std::pair<int, int> Coordinates;

/**
 *      Hash coordinates on the Sudoku grid.
 * @param coordinates The current position pair to be combined 
 * @return The updated hash value
 */
std::size_t hashCoordinate(Coordinates &position);

struct CoordinateHasher
{
    std::size_t operator()(Coordinates position) const;
};

struct CoordinateEquivalence
{
    std::size_t operator()(Coordinates const &positionFirst, Coordinates const &positionSecond) const;
};

#endif