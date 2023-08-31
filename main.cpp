#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <thread>
#include <stdio.h>
#include <cctype>
#include <limits>

using namespace std::chrono;

/*
    CLASSES
*/

class AddGenerator {
    int add1, add2, add3, add4;
    std::mt19937 rng;
    std::uniform_int_distribution<> distr1, distr2;
public:
    AddGenerator() = default;
    AddGenerator(int a1, int a2, int a3, int a4) : add1{a1}, add2{a2}, add3{a3}, add4{a4}, 
                                                   rng{std::mt19937(std::random_device()())},
                                                   distr1{std::uniform_int_distribution<>(a1, a2)},
                                                   distr2{std::uniform_int_distribution<>(a3, a4)} {}
    void printRange() {
        std::cout << "  ranges: (" << add1 << ", " << add2 << ") + " << "(" << add3 << ", " << add4 << ")\n";
    }

    std::vector<int> generate() {
        int n1 = distr1(rng);
        int n2 = distr2(rng);
        return { n1, n2, n1 + n2 };
    }
};

class SubGenerator {
    int sub1, sub2, sub3, sub4, subdiff;
    std::mt19937 rng;
    std::uniform_int_distribution<> distr1, distr2;
public:
    SubGenerator() = default;
    SubGenerator(int s1, int s2, int s3, int s4, int sd) : sub1{s1}, sub2{s2}, sub3{s3}, sub4{s4}, subdiff{sd}, 
                                                           rng{std::mt19937(std::random_device()())} {
        distr2 = std::uniform_int_distribution<>(s3, s4);
    }
    void printRange() {
        std::cout << "  ranges: (" << sub1 << ", " << sub2 << ") - " << "(" << sub3 << ", " << sub4 << ")\n";
    }

    std::vector<int> generate() {
        int n2 = distr2(rng);
        distr1 = std::uniform_int_distribution<>(std::max(sub1, n2+subdiff), sub2);
        int n1 = distr1(rng);
        return { n1, n2, n1 - n2 };
    }
};

class MulGenerator {
    int mul1, mul2, mul3, mul4;
    std::mt19937 rng;
    std::uniform_int_distribution<> distr1, distr2;
public:
    MulGenerator() = default;
    MulGenerator(int m1, int m2, int m3, int m4) : mul1{m1}, mul2{m2}, mul3{m3}, mul4{m4}, 
                                                   rng{std::mt19937(std::random_device()())},
                                                   distr1{std::uniform_int_distribution<>(m1, m2)},
                                                   distr2{std::uniform_int_distribution<>(m3, m4)} {}
    void printRange() {
        std::cout << "  ranges: (" << mul1 << ", " << mul2 << ") X " << "(" << mul3 << ", " << mul4 << ")\n";
    }

    std::vector<int> generate() {
        int n1 = distr1(rng);
        int n2 = distr2(rng);
        return { n1, n2, n1 * n2 };
    }
};

class DivGenerator {
    int div1, div2, div3, div4;
    std::mt19937 rng;
    std::uniform_int_distribution<> distr1, distr2;
public:
    DivGenerator() = default;
    DivGenerator(int d1, int d2, int d3, int d4) : div1{d1}, div2{d2}, div3{d3}, div4{d4}, 
                                                   rng{std::mt19937(std::random_device()())},
                                                   distr1{std::uniform_int_distribution<>(d1, d2)},
                                                   distr2{std::uniform_int_distribution<>(d3, d4)} {}
    void printRange() {
        std::cout << "  ranges: (" << div1 << ", " << div2 << ") / " << "(" << div3 << ", " << div4 << ")\n";
    }

    std::vector<int> generate() {
        int n1 = distr1(rng);
        int n2 = distr2(rng);
        return { n1 * n2, n2, n1 };
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
    std::uniform_int_distribution<> distr;

    bool run_prog;

    Alphadd() = default; // Default

    Alphadd(int hash, std::vector<char> o, int a1, int a2, int a3, int a4, int s1, int s2, int s3,
            int s4, int sd, int m1, int m2, int m3, int m4, int d1, int d2, int d3, int d4, int tlen) : hash{hash}, ops{o},
            adder{AddGenerator(a1, a2, a3, a4)}, subber{SubGenerator(s1, s2, s3, s4, sd)},
            muller{MulGenerator(m1, m2, m3, m4)}, diver{DivGenerator(d1, d2, d3, d4)}, time_length{tlen},
            rng{std::mt19937(std::random_device()())}, distr{std::uniform_int_distribution<>(0, o.size()-1)},
            run_prog{true} { }

    char getRandomOperator() {
        return ops[distr(rng)];
    }
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
    if (!op1 && !op2 && !op3 && !op4) {
        std::cout << "Please input at least one possible operation (add, sub, mul, div)\n";
        exit(1);
    } else if (s1 > s2 || s3 > s4) {
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

void writeData() {
}

void shutdown() {
    std::cout << "Exiting alphadd...\n";
    writeData();
    exit(0);
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
}

// Runs the main loop to generate questions and receive input
void run() {
    alpha.run_prog = false;
    time_point<steady_clock> start_time = steady_clock::now();

    int ans = 0, inp = 0;
    bool keep_run = true;
    int score = 0;
    while (duration_cast<seconds>(steady_clock::now() - start_time).count() < alpha.time_length && keep_run) {
        char op = alpha.getRandomOperator();
        std::vector<int> question;
        if (op == '+') {
            question = alpha.adder.generate();
        } else if (op == '-') {
            question = alpha.subber.generate();
        } else if (op == '*') {
            question = alpha.muller.generate();
        } else if (op == '/') {
            question = alpha.diver.generate();
        }

        if (question.size() != 3) {
            std::cout << "ERROR: Incorrect number of elements in question!\n";
            exit(1);
        }
        ans = question[2];
        std::cout << "\n----  " << question[0] << ' ' << op << ' ' << question[1] << "  ----\n\n";
        std::cout << "      ";

        std::string tmp = "-1";
        inp = -1;
        // Wrong answer, add to database
        int incorrect_ct = 0;
        while (ans != inp) {
            std::cin >> tmp;
            if (tmp == "r") {
                alpha.run_prog = true;
                keep_run = false;
                std::cout << "\n    Restarting...\n";
                break;
            } else if (tmp == "q") {
                shutdown();
            } else if (tmp == "t") {
                std::cout << "\n    " << alpha.time_length - duration_cast<seconds>(steady_clock::now() - start_time).count() << " seconds left\n\n      ";
                inp = -1;
                continue;
            } else if (tmp == "s") {
                std::cout << "\n    " << "Score: " << score << "\n\n      ";
                inp = -1;
                continue;
            }
            inp = std::stoi(tmp);
            if (ans != inp) {
                printf("\a");
                std::cout << "\n     Incorrect    \n\n      ";
                incorrect_ct++;
            } else {
                score++;
            }
        }
    }

    std::cout << "\n====  Results  ====\n\n";
    std::cout << "    Score: " << score << "\n\n";
    std::cout << "    Time: " << alpha.time_length << " seconds\n\n";

    std::cout << "Please type q to quit. ENTER to restart." << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<int>::max(), '\n');
    char nl;
    std::cin.get(nl);
    if (nl != 'q') {
        alpha.run_prog = true;
    }


}

/*
    MAIN FUNCTION
*/

int main(int argv, char **argc) {
    intro();
    configure();
    confirm();
    while(alpha.run_prog) {
        run();
        writeData();
    }
    return 0;
}