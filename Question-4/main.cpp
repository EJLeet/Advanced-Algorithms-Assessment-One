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
    int list_length = 0;

public:
    // initialise the index array to -1
    FastList() { index.fill(-1); }

    // add items to the list
    void add(int item);

    // remove items from the list
    void remove(int item);

    // clear the list
    void clear();

    // return if the item is in the list
    bool contains(int item) { return index[item] != -1 ; }

    // overload indexing operator
    int operator[](int it) { return value[it]; }

    // return the length of the list - used for test cases
    int length() { return list_length; }
};

template<int N>
void FastList<N>::add(int item)
{
    // invalid item
    if (item > N || item < 0)
        cout << "ERROR! " << item << " must be > 0 and < "<< N << endl;
    
    // item is already in list
    else if(contains(item))
        cout << "ERROR! " << item << " is already in list" << endl;

    else
    {// otherwise add it to the list
        index[item] = list_length;
        value[list_length++] = item;
    }
}

template<int N>
void FastList<N>::remove(int item)
{
    if(!contains(item))
    {// check if item is present
        std::cout << item << " is not present in list" << endl;
        exit(1);
    }

    // replace item with last item in value array
    int it = index[item];
    value[it] = value[list_length - 1];

    // update the index of the removed value
    index[value[it]] = it;
    value[list_length] = -1;

    // update index array
    index[item] = -1;
    list_length--;
}

template<int N>
void FastList<N>::clear()
{
    // set all values in index to -1 
    for (int k = 0; k < list_length; k++)
        index[value[k]] = -1;

    // reset list_length
    list_length = 0;
}

int main()
{

    /* 
        This is the upper bound given in the problem statement.
        It is hard coded and declared outside the class so it can
        be declared constant and passed to the templated class
        to initialise the list containers.
                                                                    */                                                             
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

    // Iterate
    cout << "List after adding {4, 6, 2, 31, 22, 17}" << endl;
    for(int i = 0; i < list.length(); i++)
        cout << "Item " << i + 1 << " = " << list[i] << endl;
    cout << endl;    

    // Add duplicate
    cout << "Testing Add Duplicate: " << endl;
    list.add(4);
    cout << endl;    

    // Add out of bounds
    cout << "Testing Add Out of Bounds: " << endl;
    list.add(55);
    cout << endl;    

    // Delete
    cout << "Testing Delete: " << endl;
    list.remove(22);
    list.remove(6);

    // Iterate
    cout << "List after deleting {22, 6}" << endl;
    for(int i = 0; i < list.length(); i++)
        cout << "Item " << i + 1 << " = " << list[i] << endl;
    cout << endl;  

    // In the list
    cout << "Testing if item is in list: " << endl;
    cout << "Looking for 31... ";
    list.contains(31) ? cout << "31 is in the list" << endl : 
                        cout << "31 is not in the list" << endl;

    cout << "Looking for 6... ";
    list.contains(6) ? cout << "6 is in the list" << endl : 
                       cout << "6 is not in the list" << endl;
    cout << endl;

    // Clear
    cout << "Testing Clear: " << endl;
    list.clear();
    cout << "List contains the following items... " << endl;
    for(int i = 0; i < list.length(); i++)
        cout  << list[i] << " ";
 
    return 0;
}