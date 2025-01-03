#include "Converters.h"
#include "WAV.h"


#include <iostream>
void Mute::setArg(const std::vector<std::string> args) {
    this->start = stoi(args[0]) * 44100;
    this->end = stoi(args[1]) * 44100;
    if (start >= end || start < 0) {
        throw std::invalid_argument("Incorrect interval of time. (Mute)");
    }
}

void Mute::converting(std::vector<int16_t> &stream) const {
    if (end >= static_cast<int>(stream.size())) {
        throw std::invalid_argument("The 'end' of the time interval exceeds the size of music. (Mute)");
    }

    for (int i = start + 400; i <= end; ++i) {
        stream[i] = 0;
    }
}

void Mix::setArg(const std::vector<std::string> args) {
    this->file = args[0];
    this->start = stoi(args[1]) * 44100;
}

void Mix::converting(std::vector<int16_t> &stream) const {
    if (start >= static_cast<int>(stream.size()) || start < 0) {
        throw std::invalid_argument("Incorrect interval of time. (Mix)");
    }

    std::ifstream inputFileStream(this->file, std::ios_base::binary);
    if (!inputFileStream) {
        throw std::ios_base::failure("Unable to open the file: " + this->file + " (Mix)");
    }

    WAV WAVHeader(inputFileStream);
    WAVHeader.readStream(inputFileStream);
    auto &WAVStream = WAVHeader.getStream();
    inputFileStream.close();

    std::size_t end = std::min(WAVStream.size(), stream.size());
    for (std::size_t i = start + 400; i <= end; ++i) {
        stream[i] = static_cast<int16_t>((stream[i] / 2) + (WAVStream[i] / 2));
    }
}

void Boost::setArg(const std::vector<std::string> args) {
    this->start = stoi(args[0]) * 44100;
    this->end = stoi(args[1]) * 44100;
    this->boostFactor = stoi(args[2]);

    if (boostFactor < 1 || boostFactor > 10) {
        throw std::invalid_argument("Boost coefficient should be between 1 and 10. (Boost)");
    }

    if (start >= end || start < 0) {
        throw std::invalid_argument("Incorrect interval of time. (Boost)");
    }
}

void Boost::converting(std::vector<int16_t> &stream) const {
    if (end > static_cast<int>(stream.size())) {
        throw std::invalid_argument("End of time interval exceeds the size of the stream. (Boost)");
    }

    for (std::size_t i = start + 400; i <= end; ++i) {
        auto tmp = static_cast<int16_t>(stream[i] * boostFactor);
        if (tmp > INT16_MAX) {
            stream[i] = INT16_MAX;
        } else if (tmp < INT16_MIN) {
            stream[i] = INT16_MIN;
        } else {
            stream[i] = tmp;
        }
    }
}