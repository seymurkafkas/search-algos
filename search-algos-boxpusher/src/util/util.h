#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include <string>
#include <utility>

namespace util
{
    int getBoxNumber(std::string gridContent);
    bool isBoxPresent(std::string gridContent);
    int getManhattanDistance(std::pair<int, int> firstCoordinate, std::pair<int, int> secondCoordinate);
}

namespace std
{
    std::pair<int, int> operator+(const std::pair<int, int> &a, const std::pair<int, int> &b);
}

#endif