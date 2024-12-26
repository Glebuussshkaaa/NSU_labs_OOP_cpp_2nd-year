#include "SoundProcessor.h"
#include <iostream>

int main(int argc, const char **argv) {
    try {
        SoundProcessor workingSession;
        workingSession.launch(argc, argv);
    } catch (const std::ios_base::failure& e) {
        std::cerr << "File error: " << e.what() << std::endl;
        return 1;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Argument error: " << e.what() << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unknown error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}