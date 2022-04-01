#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using std::endl;
using std::cout;

struct Point
{// A point in 2D plane
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

int intersection(std::vector<Point> lines);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Run program with a file of points" << endl;
        exit(1);
    }

    //read in values from commmand line arguements
    std::vector<Point> lines;
    std::ifstream file(argv[1]);
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        int x1, y1, x2, y2;
        linestream >> x1 >> y1 >> x2 >> y2;
        lines.push_back(Point(x1, y1));
        lines.push_back(Point(x2, y2));
    }

    cout << intersection(lines) << endl;

    return 0;
}

int intersection(std::vector<Point> lines)
{
    
}