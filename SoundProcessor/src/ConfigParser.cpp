#include "ConfigParser.h"
#include <fstream>

std::vector<std::string> ConfigParser::getConverters() const {
    return converters;
}

std::vector<std::string> ConfigParser::getArguments() const {
    return arguments;
}

void ConfigParser::parser(const std::string &configPath) {
    std::ifstream configFile(configPath);

    if (!configFile.is_open()) {
        throw std::ios_base::failure("Failed to open the config file: " + configPath);
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(configFile, line)) {
        lineNumber++;

        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (line.substr(0, 5) == "mute ") {
            readArgMute(line, lineNumber);
        } else if (line.substr(0, 4) == "mix " && line[4] == '$') {
            readArgMix(line, lineNumber);
        } else if (line.substr(0, 6) == "boost ") {
            readArgBoost(line, lineNumber);
        } else {
            throw std::invalid_argument(
                    "Invalid configuration command at line " + std::to_string(lineNumber) + ": " + line);
        }
    }
}

void ConfigParser::readArgMute(const std::string &line, int lineNumber) {
    converters.emplace_back("mute");
    std::string start;
    std::string end;

    bool readStart = true;

    for (int i = 5; i < (int) line.size(); ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (readStart) {
                start += line[i];
            } else {
                end += line[i];
            }
        } else if (line[i] == ' ') {
            readStart = false;
        } else {
            throw std::invalid_argument(
                    "Invalid character detected in mute command at line " + std::to_string(lineNumber) + ": " +
                    line[i]);
        }
    }

    if (start.empty() || end.empty()) {
        throw std::invalid_argument(
                "Both start and end values must be specified for mute command at line " + std::to_string(lineNumber));
    }


    arguments.push_back(start);
    arguments.push_back(end);
}


void ConfigParser::readArgMix(const std::string &line, int lineNumber) {
    converters.emplace_back("mix");

    std::string number;
    std::string start;

    bool readNumber = true;

    for (int i = 5; i < (int) line.size(); ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (readNumber) {
                number += line[i];
            } else {
                start += line[i];
            }
        } else if (line[i] == ' ') {
            readNumber = false;
        } else if (line[i] == '$') {
            continue;
        } else {
            throw std::invalid_argument(
                    "Invalid character detected in mix command at line " +
                    std::to_string(lineNumber) + ": " + line[i]);
        }
    }

    if (number.empty() || start.empty()) {
        throw std::invalid_argument(
                "Both number and start values must be specified for mix command at line " +
                std::to_string(lineNumber));
    }

    arguments.push_back(number);
    arguments.push_back(start);
}

void ConfigParser::readArgBoost(const std::string &line, int lineNumber) {
    converters.emplace_back("boost");
    std::string start;
    std::string end;
    std::string boostFactor;


    bool readStart = true;
    bool readEnd = false;

    for (int i = 6; i < static_cast<int>(line.size()); ++i) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (readStart) {
                start += line[i];
            } else if (readEnd) {
                end += line[i];
            } else {
                boostFactor += line[i];
            }
        } else if (line[i] == ' ') {
            if (readStart) {
                readStart = false;
                readEnd = true;
            } else if (readEnd) {
                readEnd = false;
            }
        } else {
            throw std::invalid_argument(
                    "Invalid character detected in boost command at line " +
                    std::to_string(lineNumber) + ": " + line[i]);
        }
    }

    if (start.empty() || end.empty() || boostFactor.empty()) {
        throw std::invalid_argument(
                "All parameters (start, end, and factor) must be specified for boost command at line " +
                std::to_string(lineNumber));
    }

    arguments.push_back(start);
    arguments.push_back(end);
    arguments.push_back(boostFactor);
}
