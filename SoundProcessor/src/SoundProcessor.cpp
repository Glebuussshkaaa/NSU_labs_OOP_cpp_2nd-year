#include "SoundProcessor.h"
#include "Converters.h"
#include <vector>

void SoundProcessor::launch(int argc, const char **argv) {
    commandParser.parser(argc, argv);
    if (commandParser.getHelpMode()) {
        CommandParser::printHelp();
        return;
    }

    configParser.parser(commandParser.getConfigPath());
    std::vector<std::string> inputFiles = commandParser.getInputFilesPaths();

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
    outputFile.open(commandParser.getOutputFilePath(), std::ios_base::binary);
    if (!outputFile.is_open()) {
        throw std::ios_base::failure("Unable to open output file: " + commandParser.getOutputFilePath());
    }

    inputFileWAV.writeHeader(outputFile);
    inputFileWAV.writeStream(outputFile);
    outputFile.close();
}

void SoundProcessor::applyConverters(WAV &inputFileWAV) const {
    std::vector<std::string> inputFiles = commandParser.getInputFilesPaths();
    std::vector<std::string> converters = configParser.getConverters();
    std::vector<std::string> arguments = configParser.getArguments();

    auto argumentsIt = arguments.begin();

    for (const std::string &converter : converters) {
        std::vector<std::string> operationParameters;

        if (converter == "mute") {
            CreatorMute creatorMuteObject;
            std::unique_ptr<Converter> muteObject = creatorMuteObject.createConverter();

            for (int k = 0; k < 2 && argumentsIt != arguments.end(); ++k) {
                operationParameters.push_back(*argumentsIt++);
            }

            muteObject->setArg(operationParameters);
            muteObject->converting(inputFileWAV.getStream());
        } else if (converter == "mix") {
            CreatorMix creatorMixObject;
            std::unique_ptr<Converter> mixObject = creatorMixObject.createConverter();

            operationParameters.push_back(inputFiles[std::stoi(*argumentsIt++) - 1]);
            operationParameters.push_back(*argumentsIt++);

            mixObject->setArg(operationParameters);
            mixObject->converting(inputFileWAV.getStream());
        } else if (converter == "boost") {
            CreatorBoost creatorBoostObject;
            std::unique_ptr<Converter> boostObject = creatorBoostObject.createConverter();

            for (int k = 0; k < 3 && argumentsIt != arguments.end(); ++k) {
                operationParameters.push_back(*argumentsIt++);
            }

            boostObject->setArg(operationParameters);
            boostObject->converting(inputFileWAV.getStream());
        }
    }
}