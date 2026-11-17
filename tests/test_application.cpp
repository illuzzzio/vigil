#include "vigil/core/application.hpp"
#include <cassert>
#include <iostream>

void run_application_tests() {
    std::cout << "  - Running application initialization tests...\n";

    char* argv_default[] = { (char*)"vigil" };
    vigil::core::Application app(1, argv_default);
    assert(app.run() == 0);
}
