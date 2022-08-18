#ifndef IO_FILEHANDLER_H
#define IO_FILEHANDLER_H

#include <string>
#include <BoxPusherGame.h>
#include <SearchNode.h>
#include <fstream>

class FileHandler
{
public:
    /**
     *      Initialises the game world and the starting state from the given filepath.
     * @param filePath Path of the read file containing game info.
     * @param game Reference to the game that will be mutated
     */
    static void createGameFromFile(std::string filePath, BoxPusherGame &game);

    /**
     *      Writes the action sequence that leads to the node in the file.
     * @param currentNode Search node to be traversed in the reverse order
     * @param fileName Name of the output file with printed steps.
     */
    static void writeStepsToFile(SearchNode *currentNode, std::string fileName);

    static void deleteGameMap(std::string **gameMap, int rowCount);

    static void printMap(std::string **gameMap, int rowCount, int columnCount);

    static bool isNextInputValid(std::fstream &inputFileToValidate);

    static void validateFileHandler(std::fstream &fileHandlerToValidate);

    static std::string **createMapWithDimensions(int rowCount, int columnCount);

    /**
     *
     * @param game Reference to the game instance that will be mutated.
     * @param inputTextFileHandler Reference to the read file stream with game info.
     */
    static void fillGameFromFileHandler(BoxPusherGame &game, std::fstream &inputTextFileHandler);
};

#endif