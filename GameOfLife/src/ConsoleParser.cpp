#include "ConsoleParser.h"

std::string ConsoleParser::getGameMode() const {
    return gameMode;
}

std::string ConsoleParser::getInputFile() const {
    return inputFile;
}

std::string ConsoleParser::getOutputFile() const {
    return outputFile;
}

int ConsoleParser::getCountOfIterations() const {
    return countOfIterations;
}

void ConsoleParser::parseOptions(int argc, char **argv) {
    if (argc == 1) {
        this->gameMode = "default";
    } else if (argc == 2) {
        this->gameMode = "input file";
        this->inputFile = argv[1];
    } else if (argc == 6) {
        this->gameMode = "offline";
        this->inputFile = argv[1];
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--iterations") == 0 || strcmp(argv[i], "-i") == 0) {
                this->countOfIterations = std::stoi(argv[i + 1]);
                ++i;
            } else if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
                this->outputFile = argv[i + 1];
                ++i;
            } else {
                throw std::invalid_argument("Invalid arguments. Correct format: ./GameOfLife <input_file> -i <iterations> -o <output_file>");
            }
        }
    } else {
        throw std::invalid_argument("Invalid arguments. Correct format: ./GameOfLife <input_file> -i <iterations> -o <output_file>");
    }
}
