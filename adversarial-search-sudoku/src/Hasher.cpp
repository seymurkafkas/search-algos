#include "Hasher.h"

std::size_t hashCoordinate(Coordinates &position)
{
    size_t hashValue = 0;
    std::hash<int> hasher;
    hashValue ^= hasher(position.first) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= hasher(position.second) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    return hashValue;
}

std::size_t CoordinateHasher::operator()(Coordinates position) const
{
    return hashCoordinate(position);
}

std::size_t CoordinateEquivalence::operator()(Coordinates const &positionFirst, Coordinates const &positionSecond) const
{
    return true;
}
