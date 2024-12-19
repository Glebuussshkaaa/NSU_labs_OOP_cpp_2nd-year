#include "Converters.h"
#include "WAV.h"
#include <stdexcept>
#include <cstdint>

void Mute::setArg(const std::vector<std::string> args) {
    this->start = stoi(args[0]);
    this->end = stoi(args[1]);
    if (start >= end || start < 0) {  // замена на ноль
        throw std::invalid_argument("Incorrect interval of time. (Mute)");
    }
}

void Mute::converting(std::vector<std::array<int16_t, 44100>> &stream) const {
    if (end >= static_cast<int>(stream.size())) {
        throw std::invalid_argument("The 'end' of the time interval exceeds the size of music. (Mute)");
    }
    if (start > 0) {
        for (int i = start; i <= end; ++i) {
            stream[i].fill(0);
        }
    } else {
        for (int j = 500; j < 44100; ++j) {
            stream[start][j] = 0;
        }
        for (int i = start + 1; i <= end; ++i) {
            stream[i].fill(0);
        }
    }
}

void Mix::setArg(const std::vector<std::string> args) {
    this->file = args[0];
    this->start = stoi(args[1]);
}

void Mix::converting(std::vector<std::array<int16_t, 44100>> &stream) const {
    if (start >= static_cast<int>(stream.size()) || start < 0) {  // замена на ноль
        throw std::invalid_argument("Incorrect interval of time. (Mix)");
    }

    std::ifstream inputFileStream(this->file, std::ios_base::binary);
    if (!inputFileStream.good()) { // заменить на другое
        throw std::ios_base::failure("Unable to open the file: " + this->file + " (Mix)");
    }
    WAV WAVHeader(inputFileStream);
    WAVHeader.readStream(inputFileStream);
    auto &WAVStream = WAVHeader.getStream();
    inputFileStream.close();

    std::size_t end = std::min(WAVStream.size(), stream.size());
    if (start > 0) {
        for (std::size_t i = start; i <= end; ++i) {
            for (int j = 0; j < 44100; ++j) {
                stream[i][j] = static_cast<int16_t>((stream[i][j] / 2) + (WAVStream[i][j] / 2));
            }
        }
    } else {
        for (int j = 500; j < 44100; ++j) {
            stream[start][j] = static_cast<int16_t>((stream[start][j] / 2) + (WAVStream[start][j] / 2));
        }
        for (std::size_t i = start + 1; i <= end; ++i) {
            for (int j = 0; j < 44100; ++j) {
                stream[i][j] = static_cast<int16_t>((stream[i][j] / 2) + (WAVStream[i][j] / 2));
            }
        }
    }
}

void Boost::setArg(const std::vector<std::string> args) {
    this->start = stoi(args[0]);
    this->end = stoi(args[1]);
    this->boostFactor = stoi(args[2]);

    if (boostFactor < 1 || boostFactor > 10) {
        throw std::invalid_argument("Boost coefficient should be between 1 and 10. (Boost)");
    }

    if (start >= end || start < 0) {
        throw std::invalid_argument("Incorrect interval of time. (Boost)");
    }
}

void Boost::converting(std::vector<std::array<int16_t, 44100>> &stream) const {
    if (end > static_cast<int16_t>(stream.size())) {
        throw std::invalid_argument("End of time interval exceeds the size of the stream. (Boost)");
    }

    if (start > 0) {
        for (std::size_t i = start; i <= end; ++i) {
            for (int j = 0; j < 44100; ++j) {
                auto tmp = static_cast<int16_t>(stream[i][j] * boostFactor);
                if (tmp > INT16_MAX) {
                    stream[i][j] = INT16_MAX;
                } else if (tmp < INT16_MIN) {
                    stream[i][j] = INT16_MIN;
                } else {
                    stream[i][j] = tmp;
                }
            }
        }
    } else {
        for (int j = 500; j < 44100; ++j) {
            auto tmp = static_cast<int16_t>(stream[start][j] * boostFactor);
            if (tmp > INT16_MAX) {
                stream[start][j] = INT16_MAX;
            } else if (tmp < INT16_MIN) {
                stream[start][j] = INT16_MIN;
            } else {
                stream[start][j] = tmp;
            }
        }
        for (std::size_t i = start + 1; i <= end; ++i) {
            for (int j = 0; j < 44100; ++j) {
                auto tmp = static_cast<int16_t>(stream[i][j] * boostFactor);
                if (tmp > INT16_MAX) {
                    stream[i][j] = INT16_MAX;
                } else if (tmp < INT16_MIN) {
                    stream[i][j] = INT16_MIN;
                } else {
                    stream[i][j] = tmp;
                }
            }
        }

    }

}