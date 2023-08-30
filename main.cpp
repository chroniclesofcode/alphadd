#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <thread>

using namespace std::chrono;

/*
    CLASSES
*/

class AddGenerator {
    int add1, add2, add3, add4;
    std::mt19937 rng;
public:
    AddGenerator() = default;
    AddGenerator(int a1, int a2, int a3, int a4) : add1{a1}, add2{a2}, add3{a3}, add4{a4}, rng{std::mt19937(std::random_device()())} {}
    void printRange() {
        std::cout << "  ranges: (" << add1 << ", " << add2 << ") + " << "(" << add3 << ", " << add4 << ")\n";
    }
};

class SubGenerator {
    int sub1, sub2, sub3, sub4, subdiff;
    std::mt19937 rng;
public:
    SubGenerator() = default;
    SubGenerator(int s1, int s2, int s3, int s4, int sd) : sub1{s1}, sub2{s2}, sub3{s3}, sub4{s4}, subdiff{sd}, rng{std::mt19937(std::random_device()())} {}
    void printRange() {
        std::cout << "  ranges: (" << sub1 << ", " << sub2 << ") - " << "(" << sub3 << ", " << sub4 << ")\n";
    }
};

class MulGenerator {
    int mul1, mul2, mul3, mul4;
    std::mt19937 rng;
public:
    MulGenerator() = default;
    MulGenerator(int m1, int m2, int m3, int m4) : mul1{m1}, mul2{m2}, mul3{m3}, mul4{m4}, rng{std::mt19937(std::random_device()())} {}
    void printRange() {
        std::cout << "  ranges: (" << mul1 << ", " << mul2 << ") X " << "(" << mul3 << ", " << mul4 << ")\n";
    }
};

class DivGenerator {
    int div1, div2, div3, div4;
    std::mt19937 rng;
public:
    DivGenerator() = default;
    DivGenerator(int d1, int d2, int d3, int d4) : div1{d1}, div2{d2}, div3{d3}, div4{d4}, rng{std::mt19937(std::random_device()())} {}
    void printRange() {
        std::cout << "  ranges: (" << div1 << ", " << div2 << ") / " << "(" << div3 << ", " << div4 << ")\n";
    }
};

// Singleton class used throughout the program to manage questions and query histories
class Alphadd {
public: 
    int hash; // Used to determine filenames for data
    std::vector<char> ops;

    AddGenerator adder;
    SubGenerator subber;
    MulGenerator muller;
    DivGenerator diver;
    
    int time_length;

    std::mt19937 rng;

    Alphadd() = default; // Default

    Alphadd(int hash, std::vector<char> o, int a1, int a2, int a3, int a4, int s1, int s2, int s3,
            int s4, int sd, int m1, int m2, int m3, int m4, int d1, int d2, int d3, int d4, int tlen) : hash{hash}, ops{o},
            adder{AddGenerator(a1, a2, a3, a4)}, subber{SubGenerator(s1, s2, s3, s4, sd)},
            muller{MulGenerator(m1, m2, m3, m4)}, diver{DivGenerator(d1, d2, d3, d4)}, time_length{tlen},
            rng{std::mt19937(std::random_device()())} { }
};

/*
    FUNCTIONS
*/

// Global variable for singleton class
Alphadd alpha;

// Introduction message
void intro() {
    std::cout << "Welcome to Alphadd, an arithmetic training program.\n";
}

// Takes in the input of config.txt and applies it, does error checking as well
void processSettings() {
    std::ifstream fin;
    fin.open("config.txt");
    int op1, op2, op3, op4;
    fin >> op1 >> op2 >> op3 >> op4;
    int a1, a2, a3, a4;
    fin >> a1 >> a2 >> a3 >> a4;
    int s1, s2, s3, s4, sdiff;
    fin >> s1 >> s2 >> s3 >> s4 >> sdiff;
    int m1, m2, m3, m4;
    fin >> m1 >> m2 >> m3 >> m4;
    int d1, d2, d3, d4;
    fin >> d1 >> d2 >> d3 >> d4;
    int tlen;
    fin >> tlen;
    fin.close();

    // Input validation
    if (s1 > s2 || s3 > s4) {
        std::cout << "Wrong ranges for subtraction (must be small -> large)\n";
        exit(1);
    } else if (sdiff < 0) {
        std::cout << "Difference for subtraction cannot be negative\n";
        exit(1);
    } else if (s2 < s3 + sdiff) {
        std::cout << "Your second range + the difference is greater than the first range for subtraction\n";
        exit(1);
    } else if (a1 > a2 || a3 > a4) {
        std::cout << "Wrong ranges for addition (must be small -> large)\n";
        exit(1);
    } else if (m1 > m2 || m3 > m4) {
        std::cout << "Wrong ranges for multiplication (must be small -> large)\n";
        exit(1);
    } else if (d1 > d2 || d3 > d4) {
        std::cout << "Wrong ranges for division (must be small -> large)\n";
        exit(1);
    }

    std::vector<int> nums = { op1, op2, op3, op4, a1, a2, a3, a4, s1, s2, s3, s4, 
                              sdiff, m1, m2, m3, m4, d1, d2, d3, d4, tlen };
    const int p = 31;
    const int m = 1e9+9;
    long long hash_val = 0;
    long long p_pow = 1;
    for (int i : nums) {
        hash_val = (hash_val + (i + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    std::vector<char> ops;
    if (op1) ops.push_back('+');
    if (op2) ops.push_back('-');
    if (op3) ops.push_back('*');
    if (op4) ops.push_back('/');
    alpha = Alphadd(hash_val, ops, a1, a2, a3, a4, s1, s2, s3, s4, sdiff, m1, m2, m3, m4, d1, d2, d3, d4, tlen);
}

// Obtains hashed file from .data folder and adds it into history of user
void processHistory() {
    return;
}

// Outer loop to process settings / history
void configure() {
    std::cout << "Configuring settings..." << std::endl;
    processSettings();
    std::cout << "... End configure\n";
    std::cout << "Processing historical data..." << std::endl;
    processHistory();
    std::cout << "... End processing" << std::endl;
}

// Print settings and confirms if settings are correct
void confirm() {
    std::cout << "\n====== SETTINGS ======\n\n";
    std::cout << "Operations permitted:\n";
    for (char c : alpha.ops) {
        if (c == '+') {
            std::cout << "\n- Addition\n";
            alpha.adder.printRange();
        } else if (c == '-') {
            std::cout << "\n- Subtraction\n";
            alpha.subber.printRange();
        } else if ( c == '*') {
            std::cout << "\n- Multiplication\n";
            alpha.muller.printRange();
        } else if (c == '/') {
            std::cout << "\n- Division\n";
            alpha.diver.printRange();
        }
    }
    std::cout << std::endl;
    std::cout << "To confirm these settings and start Alphadd, please press ENTER" << std:: endl;
    char nl;
    std::cin.get(nl);
    std::cout << "\n\n\n";
}

// Runs the main loop to generate questions and receive input
void run() {
    time_point<steady_clock> start_time = steady_clock::now();

    while (duration_cast<seconds>(steady_clock::now() - start_time).count() < alpha.time_length) {
        
    }
}

/*
    MAIN FUNCTION
*/

int main(int argv, char **argc) {
    intro();
    configure();
    confirm();
    run();
    return 0;
}