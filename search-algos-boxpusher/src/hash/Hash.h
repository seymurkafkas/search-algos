#ifndef HASH_HASH_H
#define HASH_HASH_H

#include <iostream>
#include <GameState.h>
#include <SearchNode.h>

struct GameStateHasher
{
    std::size_t operator()(const GameState *hashedState) const;
};

std::size_t combineHashValue(std::size_t seed, const std::pair<int, int> &coordinates);
std::size_t hashGameState(const GameState &stateToHash);

#endif