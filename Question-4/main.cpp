#include <iostream>
#include <array>

using std::cout;
using std::endl;

template<int N>
class FastList
{
    // store the index of each value
    std::array<int, N> index, value;
    // keep track of the length of the list compared to N
    int space = 0;

public:
    // initialise the index array to -1
    FastList() { index.fill(-1); }
    // add items to the list
    void add(int val);
    // remove items from the list
    void remove(int val);
    // clear the list
    void clear();
    // return if the value is in the list
    bool contains(int val) { return index[val] != -1 ; }
    // overload indexing operator
    int operator[](int it) { return value[it]; }
    // return the size of the list - used for test cases
    int size() { return space; }
};

template<int N>
void FastList<N>::add(int val)
{
    // invalid value
    if (val > N || val < 0)
        cout << "ERROR! " << val << " must be > 0 and < "<< N << endl;
    
    // item is already in list
    else if(contains(val))
        cout << "ERROR! " << val << " is already in list" << endl;

    else
    {// otherwise add it to the list
        index[val] = space;
        value[space++] = val;
    }
}

template<int N>
void FastList<N>::remove(int val)
{
    if(!contains(val))
    {// check if value is present
        std::cout << val << " is not present in list" << endl;
        exit(1);
    }

    // replace val with last item in value array
    int it = index[val];
    value[it] = value[space - 1];

    // update the index of the removed value
    index[value[it]] = it;
    value[space] = -1;

    // update index array
    index[val] = -1;
    space--;
}

template<int N>
void FastList<N>::clear()
{
    // set all values in index to -1 
    for (int i = 0; i < space; i++)
        index[value[i]] = -1;
    // reset space
    space = 0;
}

int main()
{
    const int n = 50;

    FastList<n> list = FastList<n>();

    // Add
    cout << "Testing Add: " << endl;
    list.add(4);
    list.add(6);
    list.add(2);
    list.add(31);
    list.add(22);
    list.add(17);

    // Add duplicate
    cout << "Testing Add Duplicate: " << endl;
    list.add(4);

    // Add out of bounds
    cout << "Testing Add Out of Bounds: " << endl;
    list.add(55);

    // Iterate
    cout << "Testing Iterate After Add: " << endl;
    for(int i = 0; i < list.size(); i++)
        cout << "Item " << i + 1 << " = " << list[i] << endl;;
    cout << endl;

    // Delete
    cout << "Testing Delete: " << endl;
    list.remove(22);
    list.remove(6);

    // Iterate
    cout << "Testing Iterate After Deleting 22 and 6: " << endl;
    for(int i = 0; i < list.size(); i++)
        cout << "Item " << i + 1 << " = " << list[i] << endl;
    cout << endl;    

    // In the list
    cout << "Testing if item is in list: " << endl;
    cout << "Looking for 31... ";
    if(list.contains(31))
        cout << "31 is in the list" << endl;
    cout << "Looking for 6... ";
    list.contains(6) ? cout << "6 is in the list" << endl : 
                       cout << "6 is not in the list" << endl;

    cout << endl;

    // Clear
    cout << "Testing Clear: " << endl;
    list.clear();
    cout << "List contains the following items... " << endl;
    for(int i = 0; i < list.size(); i++)
        cout  << list[i] << " ";

    return 0;
}