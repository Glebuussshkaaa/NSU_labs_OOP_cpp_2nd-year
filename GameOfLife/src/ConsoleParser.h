#pragma once

#include <iostream>
#include "Life.h"

class ConsoleParser {
private:
    std::string gameMode;
    std::string inputFile;
    std::string outputFile;
    int countOfIterations = 0;
public:
    ConsoleParser() = default;

    [[nodiscard]] std::string getGameMode() const;

    [[nodiscard]] std::string getInputFile() const;

    [[nodiscard]] std::string getOutputFile() const;

    [[nodiscard]] int getCountOfIterations() const;

    void parseOptions(int argc, char **argv);
};


