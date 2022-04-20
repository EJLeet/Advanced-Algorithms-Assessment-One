#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using std::cout;
using std::endl;

// A point in 2D space
struct Point
{ int x, y; };

// A line in 2D space
struct Line
{
    Point p1, p2;
    Line(Point p1, Point p2) : p1(p1), p2(p2) {}
};

// Line Events
struct Event
{
    int x, y;     // points of a line
    Line line;    // the line of above points
    bool is_left; // true if start of line
    Event(int x, int y, Line line, bool is_left) : x(x), y(y), line(line), is_left(is_left) {}
};

std::vector<Event> readfile(const char *filename);
int sweep_line(std::vector<Event> line_events);
bool event_sort(const Event &lhs, const Event &rhs);
int intersections(const Line &l1, const Line &l2);
int ccw(const Point &p, const Point &q, const Point &r) // counter clockwise algorithm
{ return (q.x - p.x) * (r.y - p.y) - (r.x - p.x) * (q.y - p.y); }

int main(int argc, char **argv)
{
    if (argc < 2)
    { // ensure filename is passed
        cout << "ERROR! Expected filename" << endl;
        exit(1);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // read in data points
    std::vector<Event> line_events = move(readfile(argv[1]));

    // Perform sweep line algorithm
    int count = sweep_line(line_events);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();

    // Report results
    cout << "File: " << argv[1] << endl<< "Contains " << count
         << " intersections and took " << duration
         << " milliseconds" << endl;

    return 0;
}

std::vector<Event> readfile(const char *filename)
{ /*
     This function loads all data from the text file
     to a vector of Lines which is sorted based on
     x and y value. It then loads these lines into
     a vector of events and initialises their values.
     It sorts this vector of events before returning.
                                                         */

    // vector of lines
    std::vector<Line> line_segments;
    // all line events (innactive/active)
    std::vector<Event> line_events;

    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    { // read points inputed as p1.x p1.y p2.x p2.y

        std::stringstream linestream(line);
        int x1, y1, x2, y2;
        linestream >> x1 >> y1 >> x2 >> y2;

        if (x1 == x2)
        { // vertical line

            if (y2 < y1)
            { // second point is less than first point

                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            // save points to vector of lines
            line_segments.push_back(Line(Point({x1, y1}), Point({x2, y2})));
        }

        else if (y1 == y2)
        { // horizontal line

            if (x2 < x1)
            { // second point is less than first point

                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            // save points to vector of lines
            line_segments.push_back(Line(Point({x1, y1}), Point({x2, y2})));
        }

        else
        { // there is a line that is not horizontal nor vertical
            cout << "Invalide line in " << filename << " exiting" << endl;
            exit(1);
        }
    }

    for (int i = 0; i < line_segments.size(); i++)
    { // push vector of lines to a vector of events

        line_events.push_back(Event(line_segments[i].p1.x, line_segments[i].p1.y,
                                    line_segments[i], true));
        line_events.push_back(Event(line_segments[i].p2.x, line_segments[i].p2.y,
                                    line_segments[i], false));
    }

    // sort all events based on x, y and type
    std::sort(line_events.begin(), line_events.end(), event_sort);

    return line_events;
}

bool event_sort(const Event &lhs, const Event &rhs)
{ /*
     This function first sorts by x coordinate. If the x coordinates
     are equal it will sort by the y coordinate. If the y coordinate
     is equal it will sort by the boolean is_left attribute.
                                                                         */
    if (lhs.x == rhs.x)
        return (lhs.y == rhs.y) ? lhs.is_left : lhs.y < rhs.y;
    return lhs.x < rhs.x;
}

int sweep_line(std::vector<Event> line_events)
{ /*
     This function performs the sweep line algorithm. It
     loops through all lines checking if they're active.
     If they are active it counts the amount of intersections.
     Otherwise it removes the line from the active_lines vector.
                                                                     */

    // all lines that haven't returned a false is_left (active lines)
    std::vector<Line> active_lines;
    int count = 0;

    for (auto event : line_events)
    { // iterate through active lines

        if (event.is_left)
        { // its active so count intersects

            for (auto line : active_lines)
                count += intersections(line, event.line);

            // add line to active_lines list
            active_lines.push_back(event.line);
        }

        else
            // not an active line so delete it

            for (auto it = active_lines.begin(); it < active_lines.end(); it++)
                // search for the iterator

                if (it->p1.x == event.line.p1.x && it->p1.y == event.line.p1.y &&
                    it->p2.x == event.line.p2.x && it->p2.y == event.line.p2.y)
                { // delete the line

                    active_lines.erase(it);
                    break;
                }
    }
    return count;
}

int intersections(const Line &l1, const Line &l2)
{ /*
     This function perfroms the counter clockwise algorithm 
     on two lines. If the ccw of these 2 lines is > 0 there 
     can't be an intersection.Add 1 to count if the ccw is not > 0
                                                                     */
    return ((ccw(l1.p1, l1.p2, l2.p1) *
             ccw(l1.p1, l1.p2, l2.p2) > 0) ||
            (ccw(l2.p1, l2.p2, l1.p1) *
             ccw(l2.p1, l2.p2, l1.p2) > 0) ? 0 : 1);
}