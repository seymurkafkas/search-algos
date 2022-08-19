#include <util.h>

int util::getBoxNumber(std::string gridContent)
{
    int i;
    if (sscanf(gridContent.c_str(), "S%d", &i) != 1)
    {
        throw "Invalid Grid Content";
    }
    return i - 1;
}

bool util::isBoxPresent(std::string gridContent)
{
    if (gridContent.length() < 1 || gridContent[0] != 'S')
        return false;
    return true;
}

int util::getManhattanDistance(std::pair<int, int> firstCoordinate, std::pair<int, int> secondCoordinate)
{
    int deltaY = firstCoordinate.first - secondCoordinate.first;
    int deltaX = firstCoordinate.second - secondCoordinate.second;
    return abs(deltaX) + abs(deltaY);
}

std::pair<int, int> std::operator+(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    return std::make_pair(a.first + b.first, a.second + b.second);
}
