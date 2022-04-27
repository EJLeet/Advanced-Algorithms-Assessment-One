Advanced Algorithms Assessment 1

AUTHOR

    Ethan Leet
    s5223103


ABOUT

    This assignmnet involved writting c++ code to solve six advanced algorithm problems. As well as submitting c++ source code, a detailed report was to be included which derives the efficiency and proves the validity of each problem, test cases and experiements are also included. Question six required a more in depth analysis compared to the other five programs, and its details are listed within this report.

Compiling

    Each program was compiled using clang++ and the c++ standard library 14 without errors nor warnings. To compile a program, you first must be within the program directory and can then execute the following command, replacing x with the question you wish to compile:

    clang++ main.cpp -std=c++14 -o questionx -Ofast

    Alternitavely, if CMake is installed, you can run the 'make' command from either the individual question folder or the parent folder. Running the make command in the question folder will execute the above command whereas executing the make command from the parent folder will compile all six problems in this submission and place the binaries and the respective question folder.

Run Time

    After compilation, the below command can be executed from the question folder, replacing x with the question you wish to run:

    ./questionx

    Note, that some questions require additional arguments to be passed at run time. These arguments can be passed in the following way:

    ./questionx arg1 arg2 argx

    A detailed list of what programs require arguments is as follows:

Question 1 

    Requires a .txt file which contains a list of horizontal and/or vertical lines in the following format, example files are also included in this program folder.

    x1 y1 x2 y2

Question 2

    Requires a .txt file which contains a graph. The .txt file should be in the form of:

    vertices edges
    source dest weight
    ......................
    source dest weight

    Example:

    3 4           -----> vertices, edges
    0 1 1         -----> node 0 has a path to node 1 with weight 1
    1 2 3         -----> node 1 has a path to node 2 with weight 3
    2 3 4         -----> node 2 has a path to node 3 with weight 4
    3 0 -2        -----> node 3 has a path to node 0 with weight -2

    The graph given in the problem statement is included with this question in the above form.

Question 3

    Requires a .txt containing a graph. That graph should be in the following form:

    vertecis edges
    node edge
    node edge
    .........
    node edge

    Example:

    3 4             -----> vertices, edges
    0 1             -----> node 0 has a path to node 1
    1 2             -----> node 1 has a path to node 2
    2 3             -----> node 2 has a path to node 3
    3 0             -----> node 3 has a path to node 0

    A sparse and dense graph are used as test cases and are included in the submission for this question.

Question 5

    Requires a 'source' and 'target' word to be passed as agruments. These words must be present in the dictionary file (attached).

