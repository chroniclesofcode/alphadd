#include <iostream>

class Alphadd {
    int hash; // Used to determine filenames for data
    int add1, add2;
    int sub1, sub2, subdiff;
    int mul1, mul2;
    int div1, div2;
};

Alphadd alpha;

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
    std::cout << "... End processing" << std::endl;
}


int main(int argv, char **argc) {
    intro();
    configure();
    return 0;
}