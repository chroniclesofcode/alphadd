# alphadd
Mental Arithmetic training program - designed to improve scores on Zetamac. All in the console. Entirely in C++.

# Features

Mental math arithmetic training game in the console. Allows addition, subtraction, multiplication, division. Provides configurable functionality to determine the number pairs you struggle with the most, and to show them more frequently, leading to targeted and thus faster improvement. Read the configuration section of the README to edit the 'config.txt' file in this folder to adjust the ranges. 

Please try not to CTRL-C to exit the program - it will mess up the data saving functionality. Instead, when you are solving, there are a couple commands you can type instead of entering a number:

- 'q' which exits the program safely
- 't' to display how much time is left
- 's' to display your current score
- 'r' to quick restart the current game

After the game has ended, you can either press ENTER to restart another session, or 'q' to exit the program.

# Goals
Targeted training for beginners to Zetamac who are looking to improve at a fast rate. I noticed that people usually struggle
with certain combinations of numbers e.g. 15 - 7 or 13 - 8. Of course these are fairly easy to solve just by looking at them, but people may
hesistate for even just a moment during long calculations that involve these numbers. This program, over the course of many iterations, will discover
which patterns of numbers you struggle with the most, and make them appear more frequently - thus targeting your weak points
much more often, leading to faster improvement. As a disclaimer - there will not be any machine learning, just going to
use some sort of statistical randomness to feed you the questions you struggled with more often the ones that you didn't.
I like to call this micro spaced reptition. 

# Heuristics

The determining factors on how much you struggle will be whether you got the question wrong or not, and the time it took for you to answer the question.

# Configuration

Modify config.txt:
- the first line will contain 4 integers ranging from [0,1], representing which operations you would like to do in the order addition, subtraction, multiplication, division. For example, if I want to do subtraction and division only, the integers will be `0 1 0 1`.
- the second line will contain 4 integers, the range of the first number in addition, and the range of the second.
- the third line will contain 5 integers, the range of the first number in subtraction, and the range of the second. The 5th number represents the minimum difference between your two ranges. E.g. if you want to make sure you don't get any differences (answers) of 100, please set it as 100. Now every pair of numbers will be at least 100 apart. If you don't configure it properly, the program will break, so if you're worried set it to something small like 0-3. Please note that the program will throw an error if the range of the first number (largest) is smaller
than the range of the second, or if the different is not as large as the min subtraction difference.
- the fourth line will contain 4 integers, the range of the first number in multiplication, and the range of the second.
- The fifth line will contain 4 integers, first is the the range of the SOLUTION -> the number you will type in, second is the range of the DIVISOR. What will happen is we will do solution * divisor, then that will be your dividend (the number on the left). 
- The sixth line will contain 1 integer, the amount of time one playthrough will take in seconds

# Troubleshooting

I am aware this program will have certain bugs, and not every case is accounted for. The numbers you input
for your range cannot be too big (such that it exceeds 2^31, but I am sure noone is doing mental math at that scale...), and you theoretically should not take more than 2 weeks to solve a question on average. The point I am getting at is that - you can definitely break this program, but you won't if you are using it sensibly and not deliberately trying to break it. If your program just doesn't work for some reason, you can go into the .data folder and clear everything. For good measure, I have also included a defaultconfig.txt you can use if you want to reset config.txt as well. Resetting these will make sure the program starts in a default state. If you want to submit any improvements to the code, submit a pull request and I will review it.

# How to run
Usually I like to use a build tool for my C++ projects, but I know that may turn people off since you have to download CMake or 
whatever. All you need is g++ in your command line and you can run this program with no hassles with a simple compilation - as a
bonus - it will all be in one file so there's even less work (not necessarily best practices).

```
g++ -std=c++17 -O3 -o alphadd main.cpp
```

then run

```
./alphadd
```
