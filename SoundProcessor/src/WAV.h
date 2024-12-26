#pragma once

#include <fstream>
#include <vector>
#include <array>
#include <cstdint>

typedef struct WAVHeader {
    char riffHeader[4];       // "RIFF"
    uint32_t fileSize;        // Общий размер файла минус 8 байт
    char waveHeader[4];       // "WAVE"

    char fmtChunkMarker[4];   // "fmt "
    uint32_t fmtChunkSize;    // Размер подданных в fmt-чанке (16 для PCM)
    uint16_t audioFormat;     // Аудио формат (1 для PCM)
    uint16_t numChannels;     // Количество каналов (моно = 1, стерео = 2 и т.д.)
    uint32_t sampleRate;      // Частота дискретизации (например, 44100 Гц)
    uint32_t byteRate;        // Количество байт в секунду (sampleRate * numChannels * bitsPerSample / 8)
    uint16_t blockAlign;      // Блоковая выравнивание (numChannels * bitsPerSample / 8)
    uint16_t bitsPerSample;   // Глубина звука в битах (например, 16 бит)

    char dataChunkHeader[4];  // "data"
    uint32_t dataSize;        // Размер данных (количество байт аудиоданных)
} WAVHeader;


class WAV {
private:
    WAVHeader header;
    std::vector<int16_t> stream;
public:
    explicit WAV(std::istream &in);

    void readStream(std::istream &in);

    void writeHeader(std::ostream &out) const;

    void writeStream(std::ostream &out) const;

    std::vector<int16_t> &getStream();
};
