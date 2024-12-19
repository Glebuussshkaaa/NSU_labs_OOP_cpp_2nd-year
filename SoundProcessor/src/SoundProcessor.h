#pragma once

#include "CommandParser.h"
#include "ConfigParser.h"
#include "WAV.h"


class SoundProcessor {
private:
    CommandParser commandParser;
    ConfigParser configParser;

    void applyConverters(WAV &fileWAV) const;

public:
    SoundProcessor() = default;

    void launch(int argc, const char **argv);
};
