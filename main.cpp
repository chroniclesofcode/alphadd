#include <iostream>

void intro() {
    std::cout << "Welcome to Alphadd, an arithmetic training program.\n";
}

void processSettings() {
    return;
}

void processHistory() {
    return;
}

void configure() {
    std::cout << "Configuring settings..." << std::endl;
    processSettings();
    std::cout << "... End configure\n";
    std::cout << "Processing historical data..." << std::endl;
    processHistory();
    std::cout << "... End process" << std::endl;
}


int main(int argv, char **argc) {
    intro();
    configure();
    return 0;
}