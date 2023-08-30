#include <iostream>
#include <vector>
#include <fstream>
#include <string>

class AddGenerator {
    int add1, add2, add3, add4;
public:
    AddGenerator() = default;
    AddGenerator(int a1, int a2, int a3, int a4) : add1{a1}, add2{a2}, add3{a3}, add4{a4} {}
};

class SubGenerator {
    int sub1, sub2, sub3, sub4, subdiff;
public:
    SubGenerator() = default;
    SubGenerator(int s1, int s2, int s3, int s4, int sd) : sub1{s1}, sub2{s2}, sub3{s3}, sub4{s4}, subdiff{sd} {}
};

class MulGenerator {
    int mul1, mul2, mul3, mul4;
public:
    MulGenerator() = default;
    MulGenerator(int m1, int m2, int m3, int m4) : mul1{m1}, mul2{m2}, mul3{m3}, mul4{m4} {}
};

class DivGenerator {
    int div1, div2, div3, div4;
public:
    DivGenerator() = default;
    DivGenerator(int d1, int d2, int d3, int d4) : div1{d1}, div2{d2}, div3{d3}, div4{d4} {}
};

class Alphadd {
private:
    int hash; // Used to determine filenames for data
    std::vector<char> ops;

    AddGenerator adder;
    SubGenerator subber;
    MulGenerator muller;
    DivGenerator diver;

public: 
    Alphadd() = default; // Default

    Alphadd(int hash, std::vector<char> o, int a1, int a2, int a3, int a4, int s1, int s2, int s3,
            int s4, int sd, int m1, int m2, int m3, int m4, int d1, int d2, int d3, int d4) : hash{hash}, ops{o},
            adder{AddGenerator(a1, a2, a3, a4)}, subber{SubGenerator(s1, s2, s3, s4, sd)},
            muller{MulGenerator(m1, m2, m3, m4)}, diver{DivGenerator(d1, d2, d3, d4)} {
    }
};

Alphadd alpha;

void intro() {
    std::cout << "Welcome to Alphadd, an arithmetic training program.\n";
}

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
    std::vector<int> nums = { op1, op2, op3, op4, a1, a2, a3, a4, s1, s2, s3, s4, 
                              sdiff, m1, m2, m3, m4, d1, d2, d3, d4 };
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
    alpha = Alphadd(hash_val, ops, a1, a2, a3, a4, s1, s2, s3, s4, sdiff, m1, m2, m3, m4, d1, d2, d3, d4);
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