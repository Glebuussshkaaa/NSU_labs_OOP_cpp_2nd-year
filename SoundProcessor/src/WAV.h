#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstdint>

typedef struct WAVHeader {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];

    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;        // Частота дискретизации (например, 44100)
    uint32_t byteRate;          // Байт в секунду (sample_rate * num_channels * bits_per_sample / 8
    uint16_t blockAlign;        // Байт на сэмпл (num_channels * bits_per_sample / 8)
    uint16_t bitsPerSample;     // Бит на сэмпл (например, 16)

    char subchunk2ID[4];
    uint32_t subchunk2Size;
} WAVHeader;


class WAV {
private:
    WAVHeader header;
    std::vector<std::array<int16_t, 44100>> stream;
public:
    explicit WAV(std::istream &in);

    void readStream(std::istream &in);

    void writeHeader(std::ostream &out) const;

    void writeStream(std::ostream &out) const;

    std::vector<std::array<int16_t, 44100>> &getStream();
};

