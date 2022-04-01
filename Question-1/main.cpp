#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using std::cout;
using std::endl;

struct Point
{// points in 2D plane
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

std::vector<Point> readfile(char* filename);
int sweep_line(std::vector<Point> lines);
// sort function for vector of Points
bool sort_x(const Point &a, const Point &b) { return a.x < b.x; }

int main(int argc, char **argv)
{
    if (argc < 2)
    {// ensure filename is passed
        cout << "Run program with a txt file of points" << endl;
        exit(1);
    }

    std::vector<Point> lines = readfile(argv[1]);  

    cout << sweep_line(lines);

    return 0;
}

std::vector<Point> readfile(char* filename)
{// read in lines from text file

    std::vector<Point> lines;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line))
    {// read points inputed as p1.x p1.y p2.x p2.y
        std::stringstream linestream(line); 
        int x1, y1, x2, y2;
        linestream >> x1 >> y1 >> x2 >> y2;
        lines.push_back(Point(x1, y1));
        lines.push_back(Point(x2, y2));
    }
    return lines;
}

int sweep_line(std::vector<Point> lines)
{/*
                Sweep Line Algorithm:
    Sort points from left to right (from x value)
    Add active line segments to a BST as per y coordinate
    Process all points (2n where n is number of lines)
                                                            */
    std::sort(lines.begin(), lines.end(), sort_x);                                
    
}


