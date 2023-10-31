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
#include <set>
#include <array>
#include <map>
#include <functional>
#include <signal.h>
#include <unistd.h>

using namespace std::chrono;

#define INCORRECT_PENALTY 3000
#define REPEAT_CYCLE 4

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

    long long average;
    int num_elems;
    
    // Historical data store
    // array = { difficulty, firstnum, rightnum, answer, operator(char) }
    std::multiset<std::array<int,5>, std::greater<std::array<int,5>>> data;
    // Maps firstnum, rightnum, operator to the difficulty
    std::map<std::array<int, 3>, int> q_diff;

    Alphadd() = default; // Default

    Alphadd(int hash, std::vector<char> o, int a1, int a2, int a3, int a4, int s1, int s2, int s3,
            int s4, int sd, int m1, int m2, int m3, int m4, int d1, int d2, int d3, int d4, int tlen) : hash{hash}, ops{o},
            adder{AddGenerator(a1, a2, a3, a4)}, subber{SubGenerator(s1, s2, s3, s4, sd)},
            muller{MulGenerator(m1, m2, m3, m4)}, diver{DivGenerator(d1, d2, d3, d4)}, time_length{tlen},
            rng{std::mt19937(std::random_device()())}, distr{std::uniform_int_distribution<>(0, o.size()-1)},
            run_prog{true}, average{0}, num_elems{0} { }

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
    } else if (s2 < s4) {
        std::cout << "Your smaller range cannot have a larger number than your large range for subtraction\n";
        exit(1);
    } else if (s4 + sdiff > s2) {
        std::cout << "The large value of your smaller range (RHS) for subtraction will never be reached because of sub difference.\n";
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
    long long avg;
    int nume;
    int dif, q1, q2, q3, op;
    std::string data_file = ".data/" + std::to_string(alpha.hash) + ".txt";
    auto in = std::ifstream(data_file.c_str());
    // Opens file and populates data and q_diff with the questions to use.
    if (in.is_open()) {
        in >> avg >> nume;
        alpha.average = avg;
        alpha.num_elems = nume;
        while (in >> dif >> q1 >> q2 >> q3 >> op) {
            alpha.data.insert({ dif, q1, q2, q3, op });
            alpha.q_diff.insert({ {q1, q2, op}, dif });
        }
        in.close();
    }
}

void writeData() {
    std::string data_file = ".data/" + std::to_string(alpha.hash) + ".txt";
    auto of = std::ofstream(data_file.c_str());
    of << alpha.average << ' ' << alpha.num_elems << '\n';
    for (auto it = alpha.data.begin(); it != alpha.data.end(); it++) {
        auto ar = *it;
        of << ar[0] << ' ' << ar[1] << ' ' << ar[2] << ' ' << ar[3] << ' ' << ar[4] << '\n';
    }
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

int kbhit(int secs)
{
    // timeout structure passed into select
    struct timeval tv;
    // fd_set passed into select
    fd_set fds;
    // Set up the timeout.  here we can wait for 1 second
    tv.tv_sec = secs;
    tv.tv_usec = 0;

    // Zero out the fd_set - make sure it's pristine
    FD_ZERO(&fds);
    // Set the FD that we want to read
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    // select takes the last file descriptor value + 1 in the fdset to check,
    // the fdset for reads, writes, and errors.  We are only passing in reads.
    // the last parameter is the timeout.  select will return if an FD is ready or 
    // the timeout has occurred
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    // return 0 if STDIN is not ready to be read.
    return FD_ISSET(STDIN_FILENO, &fds);
}

// Runs the main loop to generate questions and receive input
void run() {
    alpha.run_prog = false;
    time_point<steady_clock> start_time = steady_clock::now();

    int ans = 0, inp = 0;
    bool keep_run = true;
    bool show_results = true;
    bool input_taken = false;
    int score = 0;
    int ct = 1;
    // Keep previous operator and question to not repeat questions
    char prevop = '?';
    std::vector<int> prevq;
    while (duration_cast<seconds>(steady_clock::now() - start_time).count() < alpha.time_length && keep_run) {
        char op;
        std::vector<int> question;
        int difficulty = 0;
        bool repeated = false;
        if (ct > REPEAT_CYCLE && ct % REPEAT_CYCLE == 0 && alpha.data.size()) {
            // Grab the most 'difficult' number from data set
            std::set<std::array<int,5>>::iterator it = alpha.data.begin();
            difficulty = (*it)[0];
            question = { (*it)[1], (*it)[2], (*it)[3] };
            op = (char)(*it)[4];
            repeated = true;
        }
        
        if (!repeated || (op == prevop && question == prevq)) {
            // Random generate a set of numbers
            op = alpha.getRandomOperator();
            difficulty = 0;

            if (op == '+') {
            question = alpha.adder.generate();
            } else if (op == '-') {
                question = alpha.subber.generate();
            } else if (op == '*') {
                question = alpha.muller.generate();
            } else if (op == '/') {
                question = alpha.diver.generate();
            }

            auto it = alpha.q_diff.find({ question[0], question[1], (int)op });
            if (it != alpha.q_diff.end()) {
                difficulty = it->second;
            }
        }

        if (question.size() != 3) {
            std::cout << "ERROR: Incorrect number of elements in question!\n";
            exit(1);
        }
        ans = question[2];
        std::cout << "\n----  " << question[0] << ' ' << op << ' ' << question[1] << "  ----\n\n";
        std::cout << "      ";

        // Start clock to measure the time taken to solve the question
        time_point<steady_clock> q_begin = steady_clock::now();

        std::string tmp = "-1";
        inp = -1;
        // Wrong answer, add to database
        int incorrect_ct = 0;
        while (ans != inp) {
            int time_left = alpha.time_length - duration_cast<seconds>(steady_clock::now() - start_time).count();
            int has_input = kbhit(time_left);
            if (has_input) {
                std::cin >> tmp;
                input_taken = true;
            } else {
                //alpha.run_prog = true;
                keep_run = false;
                break;
            }

            if (tmp == "r") {
                alpha.run_prog = true;
                keep_run = false;
                show_results = false;
                std::cout << "\n====  Restarting  ====\n";
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
        if (!keep_run) break;
        int q_end = duration_cast<milliseconds>(steady_clock::now() - q_begin).count();
        long long avg = alpha.num_elems > 0 ? alpha.average : INT_MAX;

        int time_over = avg < q_end ? q_end : 0;
        // If difficulty > 0, it means we took this element from the data array 
        // since we haven't set the difficulty yet unless we found it first
        if (difficulty > 0) {
            auto it = alpha.data.find({ difficulty, question[0], question[1], question[2], (int)op });
            if (it == alpha.data.end()) {
                std::cout << "\nCouldn't find data even though difficulty was set...\n";
                exit(1);
            }
            // Lower penalty if fast, otherwise increase it to penalty.
            difficulty = std::max((difficulty * 3) / 5, incorrect_ct * INCORRECT_PENALTY + time_over);
            alpha.data.erase(it);
            // Only insert if greater than average
            if (difficulty > avg) {
                alpha.data.insert({ difficulty, question[0], question[1], question[2], (int)op });
            } 
            auto it2 = alpha.q_diff.find({ question[0], question[1], (int)op });
            if (it2 == alpha.q_diff.end()) {
                std::cout << "\nCouldn't find element in qdiff for some reason...\n";
                exit(1);
            }
            if (difficulty > avg) {
                it2->second = difficulty;
            } else {
                alpha.q_diff.erase(it2);
            }
        } else if (time_over + incorrect_ct * INCORRECT_PENALTY > avg) {
            // New question, insert it with penalty.
            difficulty = time_over + incorrect_ct * INCORRECT_PENALTY;
            alpha.data.insert({ difficulty, question[0], question[1], question[2], (int)op });
            alpha.q_diff.insert({ {question[0], question[1], (int)op}, difficulty });
        }

        // Calibrate scores inside data array
        alpha.average = (alpha.average * alpha.num_elems + q_end) / (alpha.num_elems + 1);
        alpha.num_elems++;

        // Set previous values
        prevop = op;
        prevq = std::move(question);

        ct++;
    }
    if (!keep_run && !show_results) return;

    std::cin.clear();
    if (input_taken) {
        std::cin.ignore(std::numeric_limits<int>::max(), '\n');
    }

    std::cout << "\n====  Results  ====\n\n";
    std::cout << "    Score: " << score << "\n\n";
    std::cout << "    Time: " << alpha.time_length << " seconds\n\n";
    std::cout << "Please type q to quit. ENTER to restart." << std::endl;

    char nl;
    std::cin.get(nl);
    if (nl != 'q') {
        alpha.run_prog = true;
    } else {
        alpha.run_prog = false;
    }
}

// Writes data when user presses CTRL-C
void sigint_handler(int sigint) {
    shutdown();
    exit(0);
}

/*
    MAIN FUNCTION
*/

int main(int argv, char **argc) {
    intro();
    configure();
    confirm();
    signal(SIGINT, sigint_handler);
    while(alpha.run_prog) {
        run();
        writeData();
    }
    shutdown();
    return 0;
}