#include "SoundProcessor.h"
#include "Converters.h"
#include <vector>

void SoundProcessor::launch(int argc, const char **argv) {
    commandParser.parser(argc, argv);
    configParser.parser(commandParser.getConfigPath());
    std::vector<std::string> inputFiles = commandParser.getInputFiles();

    std::ifstream inputFile;
    inputFile.open(inputFiles[0], std::ios_base::binary);
    if (!inputFile.is_open()) { // replace
        throw std::ios_base::failure("Unable to open input file: " + inputFiles[0]);
    }

    WAV inputFileWAV(inputFile);
    inputFileWAV.readStream(inputFile);
    inputFile.close();

    applyConverters(inputFileWAV);

    std::ofstream outputFile;
    outputFile.open(commandParser.getOutputFile(), std::ios_base::binary);
    if (!outputFile.is_open()) {
        throw std::ios_base::failure("Unable to open output file: " + commandParser.getOutputFile());
    }

    inputFileWAV.writeHeader(outputFile);
    inputFileWAV.writeStream(outputFile);
    outputFile.close();
}

void SoundProcessor::applyConverters(WAV &inputFileWAV) const {
    std::vector<std::string> inputFiles = commandParser.getInputFiles();
    std::vector<std::string> converters = configParser.getConverters();
    std::vector<std::string> arguments = configParser.getArguments();

    auto argumentsIt = arguments.begin();

    for (const std::string &converter : converters) {
        std::vector<std::string> operationParameters;

        if (converter == "mute") {
            CreatorMute creatorMuteObject;
            Converter *muteObject = creatorMuteObject.createConverter();

            for (int k = 0; k < 2 && argumentsIt != arguments.end(); ++k) {
                operationParameters.push_back(*argumentsIt++);
            }

            muteObject->setArg(operationParameters);
            muteObject->converting(inputFileWAV.getStream());
            delete muteObject;
        } else if (converter == "mix") {
            CreatorMix creatorMixObject;
            Converter *mixObject = creatorMixObject.createConverter();

            operationParameters.push_back(inputFiles[std::stoi(*argumentsIt++) - 1]);
            operationParameters.push_back(*argumentsIt++);

            mixObject->setArg(operationParameters);
            mixObject->converting(inputFileWAV.getStream());
            delete mixObject;
        } else if (converter == "boost") {
            CreatorBoost creatorBoostObject;
            Converter *boostObject = creatorBoostObject.createConverter();

            for (int k = 0; k < 3 && argumentsIt != arguments.end(); ++k) {
                operationParameters.push_back(*argumentsIt++);
            }

            boostObject->setArg(operationParameters);
            boostObject->converting(inputFileWAV.getStream());
            delete boostObject;
        }
    }
}