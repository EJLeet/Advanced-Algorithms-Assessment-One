#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>

using std::cout;
using std::endl;

struct Point
{// points in 2D plane
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

struct Event
{/*
    type = 0: Vertical
    type = 1: Horizontal left
    type = 2: Horizontal right
                                */
    int type;
    Point p1;
    Point p2;
    Event(int type, Point p1, Point p2)
            : type(type), p1(p1), p2(p2) {}
};

std::vector<Event> readfile(const char* filename);
bool sort_x(const Event& e1, const Event& e2);
int sweep_line(std::vector<Event> lines);
int horizontal_intersect(std::set<int> active, Event current);
int vertical_intersect(std::set<int> active, Event current, int compare_x);

int main(int argc, char **argv)
{
    if (argc < 2)
    {// ensure filename is passed
        cout << "ERROR! Expected filename" << endl;
        exit(1);
    }
    // load file points into data structure
    std::vector<Event> line_segments = readfile(argv[1]);  
    // perform sweep line
    cout << "Intersections = " << sweep_line(line_segments) << endl;
    return 0;
}

std::vector<Event> readfile(const char* filename)
{// read in lines from text file

    std::vector<Event> line_segments;
    std::ifstream file(filename);
    std::string line;
    
    while (std::getline(file, line))
    {// read points inputed as p1.x p1.y p2.x p2.y
        std::stringstream linestream(line); 
        int x1, y1, x2, y2;
        linestream >> x1 >> y1 >> x2 >> y2;
        
        // determine line types
        if (y1 == y2)
        {// horizontal line
            if (x1 > x2)
            {// type 1 or 2
                std::swap(x2, x1);
                std::swap(y2, y1);
            }
            line_segments.push_back(Event(1, Point(x1, y1), Point(x2, y2))); // left
            line_segments.push_back(Event(2, Point(x2, y2), Point(x1, y1))); // right
        }
        else if (x1 == x2)
        {// vertical line
            if (y1 > y2)
            {// assure lowest point is read in as p1 
                std::swap(x2, x1);
                std::swap(y2, y1);
            }
            line_segments.push_back(Event(0, Point(x1, y1), Point(x2, y2)));
        }
        else
        {// invalid line entry
            cout << "ERROR! " << filename << " contains invalid points" << endl;
            exit(0);
        }
    }
    // sort by x coordinates before returning
    std::sort(line_segments.begin(), line_segments.end(), sort_x);
    return line_segments;
}

bool sort_x(const Event& lhs, const Event& rhs)
{// sort by x coord, if x equal sort by type, if type equal sort by y
    if (lhs.p1.x == rhs.p1.x)
        return lhs.type == rhs.type ? lhs.p1.y < rhs.p1.y : lhs.type;
    return lhs.p1.x < rhs.p1.x;
}

int sweep_line(std::vector<Event> line_segments)
{
    int count = 0, compare_x = line_segments[0].p1.x;
    std::set<int> active, endpoints; // store active lines by y
    for (auto it : line_segments)
    {// traverse lines
        Event current = it;

        if (current.type == 0)
        {// vertical line
            // check if line contains horizontal intersects
            count += horizontal_intersect(active, current);
            // check if line contains vertical intersects
            count += vertical_intersect(active, current, compare_x);
        }
        else if (current.type == 1)
        {// horizontal left event
            count += active.count(current.p1.y);
            active.insert(current.p1.y);
        }
        else // horizontal right event
            active.erase(active.lower_bound(current.p2.y));
    }
    return count; 
}

int horizontal_intersect(std::set<int> active, Event current)
{// returns how many horizontal intersections occur between two points
    if (active.empty())
        return 0;
    return (int)std::distance(
            active.lower_bound(std::min(current.p1.y, current.p2.y)), 
            active.upper_bound(std::max(current.p1.y, current.p2.y)));
}

int vertical_intersect(std::set<int> active, Event current, int compare_x)
{// counts how mant vertical intersections occur
    int count = 0;
    if (current.p1.x == compare_x)
    {// remove the vertical lines < p1.y
        active.erase(active.begin(), active.lower_bound(current.p1.y));
        count += active.size(); // remaining points intersect
    }
    else
    {// current line doesnt intersect
        active.clear();
        compare_x = current.p1.x;
    }
    active.insert(current.p2.y);
    return count;
}
