#ifndef FileHandlerGuard
#define FileHandlerGuard

#include <fstream>
#include "Sudoku.h"

namespace FileHandler
{

  void validateNextInput(std::fstream &inputFileToValidate);

  void validateFileHandler(std::fstream &fileHandlerToValidate);

  /**
 *
 * @param inputTextFileHandle Reference to the read file stream with game info.
 */
  void fillGameFromFileHandler(Sudoku::Game &gameInstance, std::fstream &inputTextFileHandle);

  /**
 *      Initialises the game world and the starting state from the given filepath.
 * @param filePath Path of the read file containing game info.
 */
  void createGameFromFile(std::string filePath, Sudoku::Game &gameInstance);

};

#endif