
#include <iostream>
#include <queue>
#include <set>
#include <fstream>
#include <sstream>
#include <chrono>

// Simple Point
struct Point
{
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};


/*
 * This is an Event that we process whenever the sweep line hits one.
 * type=0: Horizontal line event where p1 is the leftmost point.
 * type=1: Vertical line event.
 * type=2: Horizontal line event where p1 is the rightmost point (end of line event).
 */
struct Event
{
    uint type;
    Point p1;
    Point p2;

    Event(const int type, const Point& p1, const Point& p2)
            : type(type), p1(p1), p2(p2) {}
};


// Comparator functor for sorting Events based on the p1 x-coord.
struct EventCompare
{
    bool operator()(const Event& lhs, const Event& rhs)
    {
        // Sort by x-coord.
        // If x coords are equal, sort by type in order:
        // Horizontal starting event < Vertical line event < Horizontal Ending event.
        // If type is equal, sort by y-coord.

        if (lhs.p1.x == rhs.p1.x) {
            // Ensure leftmost > vertical > rightmost.
            if (lhs.type != rhs.type)
                return lhs.type > rhs.type;

            // Otherwise if same type, sort by y.
            return lhs.p1.y > rhs.p1.y;
        }

        return lhs.p1.x >= rhs.p1.x;
    }
};
// Priority queue containing events stored according to the EventCompare functor.
typedef std::priority_queue<Event, std::vector<Event>, EventCompare> EventPQ;

// Counts the number of values between a given range in a multiset
int countRange(std::multiset<int>& s, int a, int b)
{
    if (s.empty())
        return 0;
    int low = std::min(a, b);
    int high = std::max(a,b);

    // 2*log(n) + k complexity, where k is the distance (count).
    // = O(log(n)) (worst case O(n), very rare case when distance (count) = n)
    int count = (int)std::distance(s.lower_bound(low), s.upper_bound(high));
    return count;
}

// This function
int countVerticalOverlap(std::multiset<int>& upper_endpoints, const Event& event, int& prev_x)
{
    uint count = 0;
    if (event.p1.x == prev_x) {
        // Remove all vertical lines that are less than event.p1.y (the lower endpoint).
        upper_endpoints.erase(upper_endpoints.begin(), upper_endpoints.lower_bound(event.p1.y));
        // Remaining points are intersecting with current vertical line.
        count += upper_endpoints.size();
    } else {
        // Current vertical line is not overlapping with previous vertical line.
        upper_endpoints.clear();
        prev_x = event.p1.x;
    }
    upper_endpoints.insert(event.p2.y);
    return count;
}


// Takes n horizontal and vertical lines and returns the number of intersections.
int sweepLineIntersect(EventPQ& event_pq)
{
    int count = 0;
    // Multiset containing the y-coordinates of horizontal lines, of which the
    // leftmost point has been hit and the rightmost has yet to be hit.
    std::multiset<int> active_lines;

    // We need a way to check if multiple consecutive vertical lines overlap.
    // This can be achieved by using a multiset for consecutive vertical lines' p2.y endpoints.
    // For the case of consecutive vertical lines, it is guaranteed that each successive
    // p1.y is greater than the last. Therefore if we erase all previous.p2.y < event.p1.y,
    // then we can count the remaining points and add our event.p2.y for the next consecutive vertical event.
    std::multiset<int> upper_endpoints;
    int prev_x = event_pq.top().p1.x - 1;


    while (!event_pq.empty())
    {
        // Handle next event
        Event event = event_pq.top();
        event_pq.pop();
        switch (event.type)
        {
            case 0 :
                // We hit leftmost point of a new horizontal line.
                // Check to see how many active lines have the same y-coord,
                // which means our current horizontal line lies on other active lines
                // of the same y-coord.
                count += active_lines.count(event.p1.y); // O(logn + k) where k is the count
                // Add the line's y-coordinate to the active lines set.
                active_lines.insert(event.p1.y);
                break;
            case 1 :
                // We have reached a vertical line.
                // Check to see if it lies on any of the previous
                // vertical lines of the same x-coordinate.
                count += countVerticalOverlap(upper_endpoints, event, prev_x);

                // Check to see if the active_lines set contains
                // any y-coords in the range of e.p1.y and e.p2.y.
                // (if the vertical line intersects with any active horizontal lines).
                count += countRange(active_lines, event.p1.y, event.p2.y);
                break;
            case 2 :
                // We have reached rightmost point  of a horizontal line.
                // This means the line has been processed.
                // Remove y-coordinate from the active lines set.
                active_lines.erase(active_lines.lower_bound(event.p2.y));
                break;
        }

    }

    return count;
}

// Loads the lines segments from text file into the Priority queue.
void loadLineSegments(EventPQ& event_pq, const std::string& filepath)
{
    std::ifstream file(filepath);
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream linestream(line);
        int x1, y1, x2, y2;
        linestream >> x1 >> y1 >> x2 >> y2;

        // Determine type
        if (x1 == x2)
        {
            // Lower y point is p1.
            if (y2 < y1) {
                std::swap(x2, x1);
                std::swap(y2, y1);
            }
            // Vertical Line Event
            event_pq.emplace(1, Point(x1,y1), Point(x2,y2));
        }
        else if (y1 == y2)
        {
            // Horizontal Line Event - we have two events for
            // left-most point (type=0) and right-most point (type=2)
            if (x2 < x1) {
                std::swap(x2, x1);
                std::swap(y2, y1);
            }
            Point lp(x1, y1), rp(x2, y2); // leftmost and rightmost points
            event_pq.emplace(0, lp, rp); // leftmost point is p1
            event_pq.emplace(2, rp, lp); // rightmost point is p1
        }
        else
        {
            std::cerr << "ERROR: "+filepath+" contains a line that"
                      << " is neither horizontal nor vertical.\n";
            exit(0);
        }
    }
}


int main(int argc, char**argv)
{
    if (argc < 2) {
        std::cerr << "Expected argument: <filepath>\n";
        std::terminate();
    }

    auto start = std::chrono::system_clock::now();

    EventPQ events_pq;

    loadLineSegments(events_pq, argv[1]);

    int intersects = sweepLineIntersect(events_pq);

    auto end = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Number of Intersects: " << intersects
              << "\nProgram duration: " << (duration / 1000.0) << " seconds"
              << std::endl;

    return 0;
}