#include "CommandParser.h"
#include <iostream>

void CommandParser::parser(const int argc, const char **argv) {
    if (argc < 2) {
        throw std::invalid_argument("Incorrect command format. At least one argument is required.");
    }

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            printHelp();
            return;
        }

        else if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 == argc) {
                throw std::invalid_argument("Missing argument for -c. Correct form: -c <configFile>");
            }
            this->configPath += argv[i + 1];
            i++;
        }

        else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 == argc) {
                throw std::invalid_argument("Missing argument for -o. Correct form: -o <outputFile>");
            }
            outputFile += argv[i + 1];
            i++;
        }

        else {
            std::string pathPrefix = "../filesForWork/inputFiles/";
            this->inputFiles.emplace_back(pathPrefix + argv[i]);
        }
    }

    if (inputFiles.empty()) {
        throw std::invalid_argument("No input files provided. Correct form: -c <config.txt> -o <output.wav> <input1.wav> [<input2.wav> ...]");
    }

    if (outputFile.empty()) {
        throw std::invalid_argument("Output file not specified. Correct form: -c <config.txt> -o <output.wav> <input1.wav> [<input2.wav> ...]");
    }
}

std::string CommandParser::getConfigPath() const {
    return this->configPath;
}
std::vector<std::string> CommandParser::getInputFiles() const {
    return this->inputFiles;
}

std::string CommandParser::getOutputFile() const {
    return this->outputFile;
}


void CommandParser::printHelp() const {
    std::cout << "Usage:" << std::endl;
    std::cout << "  ./SoundProcessor [-h] [-c config.txt -o output.wav input1.wav [input2.wav …]]" << std::endl;

    std::cout << std::endl;
    std::cout << "Supported Audio Format:" << std::endl;
    std::cout << "  - PCM audio format (without encoding)" << std::endl;
    std::cout << "  - Mono sound (1 channel)" << std::endl;
    std::cout << "  - Signed 16-bit (little-endian) sample representation" << std::endl;
    std::cout << "  - Frequency: 44100 Hz" << std::endl;

    std::cout << std::endl;
    std::cout << "Converters:" << std::endl;
    std::cout << "  1. mute [start] [end]" << std::endl;
    std::cout << "     - Mutes the audio between the specified start and end times (in seconds)." << std::endl;
    std::cout << "  2. mix $[number] [start]" << std::endl;
    std::cout << "     - Mixes the main audio stream with the additional input stream ($[number] specifies the additional input file)." << std::endl;
    std::cout << "     - The mixing starts at the specified start time (in seconds)." << std::endl;
    std::cout << "  3. boost [start] [end] [1-10]" << std::endl;
    std::cout << "     - Boosts the volume by a factor in the range of 1 to 10, within the specified time interval." << std::endl;

    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  -h             Show this help message." << std::endl;
    std::cout << "  -c config.txt  Path to the configuration file." << std::endl;
    std::cout << "  -o output.wav     Output WAV file where the result will be saved." << std::endl;
    std::cout << "  inputX.wav     One or more input WAV files to process." << std::endl;

    std::cout << std::endl;
    std::cout << "Example usage:" << std::endl;
    std::cout << "  ./SoundProcessor -c config.txt -o output.wav input1.wav input2.wav" << std::endl;

    std::cout << std::endl;
    std::cout << "Configuration File Syntax:" << std::endl;
    std::cout << "  - Each line specifies a converter operation." << std::endl;
    std::cout << "  - Comments start with # and are ignored." << std::endl;
    std::cout << "  - Converters are applied in the order they appear in the file." << std::endl;
    std::cout << "  - Example config file content:" << std::endl;
    std::cout << "      mute 0 30" << std::endl;
    std::cout << "      mix $2 10" << std::endl;
    std::cout << "      boost 120 180 2" << std::endl;
}