#pragma once

#include <vector>
#include <string>
#include <array>
#include <cstdint>

class Converter {
public:
    virtual ~Converter() = default;

    virtual void setArg(std::vector<std::string> args) = 0;

    virtual void converting(std::vector<int16_t> &stream) const = 0;
};

class Mute : public Converter {
private:
    int start;
    int end;
public:
    Mute() : start(0), end(0) {}

    void setArg(std::vector<std::string> args) override;

    void converting(std::vector<int16_t> &stream) const override;
};

class Mix : public Converter {
private:
    std::string file;
    int start;
public:
    Mix() : start(0) {}

    void setArg(std::vector<std::string> args) override;

    void converting(std::vector<int16_t> &stream) const override;
};

class Boost : public Converter {
private:
    int start;
    int end;
    int boostFactor;
public:
    Boost() : start(0), end(0), boostFactor(0) {}

    void setArg(std::vector<std::string> args) override;

    void converting(std::vector<int16_t> &stream) const override;
};

class Creator {
public:
    [[nodiscard]] virtual std::unique_ptr<Converter> createConverter() const = 0;
};

class CreatorMute : public Creator {
public:
    [[nodiscard]] std::unique_ptr<Converter> createConverter() const override {
        return std::make_unique<Mute>();
    }
};

class CreatorMix : public Creator {
public:
    [[nodiscard]] std::unique_ptr<Converter> createConverter() const override {
        return std::make_unique<Mix>();
    }
};

class CreatorBoost : public Creator {
public:
    [[nodiscard]] std::unique_ptr<Converter> createConverter() const override {
        return std::make_unique<Boost>();
    }
};
