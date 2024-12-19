#include "WAV.h"


WAV::WAV(std::istream &inputFile) : header(), stream() {
    if (!inputFile) {
        throw std::runtime_error("Input file stream is not readable.");
    }

    inputFile.read(reinterpret_cast<char *>(&header), sizeof(WAVHeader));
    if (inputFile.gcount() != sizeof(WAVHeader)) {
        throw std::runtime_error("Failed to read WAV header.");
    }

    if (header.audioFormat != 1) {
        throw std::invalid_argument("Invalid audio format. Expected PCM format (audioFormat=1).");
    }
    if (header.numChannels != 1) {
        throw std::invalid_argument("Invalid number of channels. Expected mono (numChannels=1).");
    }
    if (header.bitsPerSample != 16) {
        throw std::invalid_argument("Invalid bits per sample. Expected 16 bits per sample.");
    }
    if (header.sampleRate != 44100) {
        throw std::invalid_argument("Invalid sample rate. Expected 44100 Hz.");
    }



}

void WAV::readStream(std::istream &inputFile) {
    std::array<int16_t, 44100> second = {0};

    while (inputFile.read(reinterpret_cast<char *>(&second[0]), sizeof(second[0]) * second.size())) {
        stream.push_back(second);
    }

    if (inputFile.gcount() > 0) {
        std::array<int16_t, 44100> lastSecond = {0};
        std::memcpy(&lastSecond[0], second.data(), inputFile.gcount());
        stream.push_back(lastSecond);
    }
}


void WAV::writeHeader(std::ostream &outputFile) const {

    outputFile.write(reinterpret_cast<const char *>(&header), sizeof(WAVHeader));
}

void WAV::writeStream(std::ostream &outputFile) const {

    for (auto i: this->stream) {
        outputFile.write(reinterpret_cast<const char *> (&i[0]), sizeof(i[0]) * i.size());
    }
}

std::vector<std::array<int16_t, 44100>> &WAV::getStream() {
    return this->stream;
}