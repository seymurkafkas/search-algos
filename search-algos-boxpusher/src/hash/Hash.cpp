#include <Hash.h>

/**
 *      Combine the previous seed with the current key. Adapted from the Boost Hash libraries.
 * @param seed the previous value of the hash
 * @param coordinates The current position pair to be combined
 * @return The updated hash value
 */
std::size_t combineHashValue(std::size_t seed, const std::pair<int, int> &coordinates)
{
    std::hash<int> hasher;
    seed ^= hasher(coordinates.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hasher(coordinates.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

std::size_t hashGameState(const GameState &stateToHash)
{
    size_t hashValue = 0;
    hashValue = combineHashValue(hashValue, stateToHash.agentLocation);
    for (auto &coordinate : stateToHash.boxPositions)
    {
        hashValue = combineHashValue(hashValue, coordinate);
    }
    return hashValue;
}