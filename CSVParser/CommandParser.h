#pragma once

#include <string>

class CommandParser {
private:
    std::string CSVFilePath = "../CSVTest/";
    int NumSkipLines;
    bool helpMode = false;

    static void printHelp();

public:
    CommandParser() = default;

    void parseOptions(int argc, const char **argv);

    [[nodiscard]] std::string getCSVFilePath() const;

    [[nodiscard]] int getNumSkipLines() const;

    [[nodiscard]] bool getHelpMode() const;
};
