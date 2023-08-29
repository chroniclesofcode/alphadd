# alphadd
Mental Arithmetic training program - designed to improve scores on Zetamac. All in the console. Entirely in C++.

# Goals
Targeted training for beginners to Zetamac who are looking to improve at a fast rate. I noticed that people usually struggle
with certain combinations of numbers e.g. 15 - 7 or 13 - 8. Of course these are fairly easy just to look at, but people may
hesistate for even a moment during long calculations. This program, over the course of many iterations, will discover
which patterns of numbers you struggle with the most, and make them appear more frequently - thus targeting your weak points
much more often, leading to faster improvement. As a disclaimer - there will not be any machine learning, just going to
use some sort of statistical randomness to feed you the questions you struggled with more often the ones that you didn't.
Sort of like spaced repetition in a way? The determining factors on how much you struggle will be whether you got the question
wrong or not, and the time it took for you to answer the question.

# How to run
Usually I like to use a build tool for my C++ projects, but I know that may turn people off since you have to download CMake or 
whatever. All you need is g++ in your command line and you can run this program with no hassles with a simple compilation. 
