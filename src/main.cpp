#include "vigil/core/application.hpp"
#include "vigil/core/logger.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        vigil::core::Application app(argc, argv);
        return app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown Fatal Exception occurred.\n";
        return 1;
    }
}
