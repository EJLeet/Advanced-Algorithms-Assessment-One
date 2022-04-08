#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

#define HORIZONTAL 1
#define VERTICAL 2
#define ERROR 3

// A point in 2D plane
struct Point{
    int x, y;
};
 
// A line segment with left as Point
struct Line{
    Point one, two;
    int direction;
    Line(Point l, Point r){
        one = l;
        two = r;
        if(one.x == two.x)
            direction = HORIZONTAL;
        else if(one.y == two.y)
            direction = VERTICAL;
        else
            direction = ERROR;
    }
};

struct Event { 
    Line * line;
    bool enter;
    int x, y;
    Event(bool e, Line * line, int x, int y) : enter(e), line(line), x(x), y(y) {}
};

bool compareEvents(Event e1, Event e2){
    if(e1.x== e2.x){
        if(e1.y == e2.y){
            // if same point, make sure entry one is added first
            return e1.enter;
        }
        return e1.y < e2.y;
    }
    return e1.x < e2.x;
}

int ccw(Point a, Point b, Point c){
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

int checkIntersect(Line * a, Line * b) {
    if (ccw(a->one, a->two, b->one) * ccw(a->one, a->two, b->two) > 0) 
        return 0;
    if (ccw(b->one, b->two, a->one) * ccw(b->one, b->two, a->two) > 0) 
        return 0;
    
    return 1;
}

int intersections(Line arr[], int size){
    //set of active Lines
    set <Line *> active;
    // count set to 0
    int count = 0;

    // vector of events
    vector<Event> allEvents;
    
    // create events for each point
    for (int i = 0; i < size; i++){
        allEvents.push_back(Event(true, &arr[i],arr[i].one.x,arr[i].one.y)); 
        allEvents.push_back(Event(false, &arr[i],arr[i].two.x,arr[i].two.y)); 
    }
    // sort events by left/bottom most points first
    sort(allEvents.begin(), allEvents.end(), compareEvents);

    // iterate through all events (2*n)
    for(auto event : allEvents){
        if(!event.enter){
            active.erase(event.line);
            cout << "line exiting" << endl;
            continue;
        }
        cout << "line entering" << endl;
        for(auto line : active){
            count += checkIntersect(line, event.line);
        }
        // add line to active list
        active.emplace(event.line);
    }
    return count;
}

// Driver code
int main() {
    // Line arr[] = { {{1, 5}, {4, 5}}, {{2, 2}, {2, 5}},{{3, 2}, {10, 2}},{{6, 4}, {9, 4}},{{6, 1}, {8, 1}},{{7, 0}, {7, 6}}};
    Line arr[] = { {{1, 1}, {1, 5}}, {{1, 2}, {1, 4}},{{1, 3}, {1, 5}}};
    int n = sizeof(arr)/sizeof(arr[0]);

    // TODO load all line segments
    // TODO ensure the points are the correct way around

    int count = intersections(arr, n);
    cout << "Total Intersections: "<< count << endl;
    return 0;
}