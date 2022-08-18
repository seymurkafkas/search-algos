#ifndef ACTION_ACTIONS_H
#define ACTION_ACTIONS_H

#include <vector>

class Actions
{
public:
    static std::vector<char> &getActionSet();
    static void defineActionSet();

private:
    static void addAction(char actionToAdd);
    static std::vector<char> actions;
};

#endif