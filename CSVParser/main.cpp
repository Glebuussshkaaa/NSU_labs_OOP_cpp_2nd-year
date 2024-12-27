#include <iostream>
#include <fstream>
#include <stdexcept>

#include "CSVParser.h"
#include "CommandParser.h"

int main(int argc, const char *argv[]) {
    try {
        CommandParser commandParser;

        commandParser.parseOptions(argc, argv);
        if (commandParser.getHelpMode()) {
            return 0;
        }

        std::string CSVFilePath = commandParser.getCSVFilePath();
        int numSkipLines = commandParser.getNumSkipLines();

        std::ifstream CSVFile(CSVFilePath);
        if (!CSVFile.is_open()) {
            std::cerr << "Error opening file: " << CSVFilePath << std::endl;
            return 1;
        }

        CSVParser<int, std::string, float> parser(CSVFile, numSkipLines);
        for (const auto &row: parser) {
            std::cout << row << std::endl;
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}