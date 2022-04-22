// #include <iostream>
// #include <unordered_map>
// #include <vector>
// #include <string>
// #include <fstream>
// #include <stack>
// #include <unordered_set>
// #include <chrono>
// #include <algorithm>
// #include <random>
// #include <iomanip>
// #include <map>

// using std::cout;
// using std::endl;

// void ordered(int i);
// void heuristic(int i);
// void shuffle(int i);
// void heuristic_shuffle(int i);
// void load_words(int word_length, 
//                 std::unordered_map<std::string, std::vector<std::string>>& words, 
//                 std::vector<std::string>& valid_words);
// int circular_sequence(int word_length, 
//                       std::unordered_map<std::string, std::vector<std::string>>& words, 
//                       std::vector<std::string>& valid_words);
// int dfs(std::unordered_map<std::string, std::vector<std::string>>& words, 
//         std::vector<std::string>& valid_words);                     

// int main()
// {
//     for (int i = 4; i <= 15; i++)
//     {// loop across words of length 4 to 15 seperately

//         cout << "Finding Circular Sequences of Length: " << i << endl;

//         if (i == 4) //  this case is the same for all three algorithms
//             ordered(i);

//         else
//         {        
//             // perform normal DFS
//             ordered(i);

//             // perform heuristic based DFS
//             heuristic(i);

//             // // Perform shuffled DFS
//             // shuffle(i);

//             // // Perform shuffle and heuristic based DFS
//             // heuristic_shuffle(i);
//         }

//         cout << endl;
//     }

//     return 0;
// }

// void ordered(int i)
// {/*
//         This function performs a normal DFS based on the standard
//         ordering of all valid words. This function will find a 
//         sequence for all test cases but may not be the longest
//         possible for words of length x. 
//                                                                         */

//     // initialise structures and timer
//     auto start = std::chrono::high_resolution_clock::now();
//     std::unordered_map<std::string, std::vector<std::string>> words;
//     std::vector<std::string> valid_words;

//     // load words
//     load_words(i, words, valid_words);

//     // computer the sequence length
//     int length = circular_sequence(i, words, valid_words);

//     // compute time
//     auto stop = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast
//                     <std::chrono::milliseconds> (stop - start).count();

//     // report results
//     cout << "Order Based DFS Sequence Length: " << length << std::setprecision(8)
//          << "\nTime: " << duration / 1000. << " seconds" << endl;
// }

// void heuristic(int i)
// {/*
//     This function performs a normal DFS where the order of
//     the valid words is based upon a heuristic. The heuristic 
//     is the value of each key in the map structure. As an 
//     example, if the key 'ea' appears 5 times and 'ie' 
//     appears 3 times, the heuristic will be 'ea'. This
//     means that the function will order all 'ea' key words
//     in valid words first, followed by all 'ie' words.
//                                                                 */

//     // initialise structures and timer
//     auto start = std::chrono::high_resolution_clock::now();
//     std::unordered_map<std::string, std::vector<std::string>> words;
//     std::vector<std::string> valid_words;
//     int length = 0;

//     // load words into ordered and unordered structure
//     std::ifstream file("dictionary.txt"); 
//     std::string word;
//     std::map<std::string, std::vector<std::string>> w;

//     while (file >> word)
//         if (word.length() == i)
//         {
//             w[word.substr(word.size()-3, 2)].push_back(word); 
//             words[word.substr(word.size()-3, 2)].push_back(word);
//         }
    
//     // add to valid words based on highest count
//     for (auto it = w.rbegin(); it != w.rend(); it++)
//         for (auto elem : it->second)
//             valid_words.push_back(elem);

    
//     // computer the sequence length
//     length = circular_sequence(i, words, valid_words);  

//     // compute time
//     auto stop = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration_cast
//                     <std::chrono::milliseconds> (stop - start).count();

//     // report results
//     cout << "Heuristic Based DFS Sequence Length: " << length << std::setprecision(8) 
//          << "\nTime: " << duration / 1000. << " seconds" << endl;
// }

// void shuffle(int i)
// {/* 
//     This function performs a normal DFS based on the random
//     ordering of all valid words. After a sequence is found
//     it will shuffle the valid words and search for another
//     sequence. This will continue for at least one minute. 
//     After the timer and that sequence has finished it will
//     return the best sequence found in that time.
//                                                                 */

//     // initialise structures and timer
//     auto start = std::chrono::high_resolution_clock::now();
//     auto elapsed = start, stop = start;
//     std::unordered_map<std::string, std::vector<std::string>> words;
//     std::vector<std::string> valid_words;
//     int best = 0, length = 0;

//     // load words
//     load_words(i, words, valid_words);

//     while (elapsed - start < std::chrono::seconds(60))
//     {// run restarts for 60 seconds recording the best sequence

//         // shuffle words
//         auto seed = std::default_random_engine {};
//         std::shuffle(valid_words.begin(), valid_words.end(), seed);

//         // computer the sequence length
//         length = circular_sequence(i, words, valid_words);

//         // record best sequence and its time
//         if (best < length)
//         {
//             best = length;
//             stop = std::chrono::high_resolution_clock::now();
//         }

//         // get current elapsed
//         elapsed = std::chrono::high_resolution_clock::now();
//     }

//     auto duration = std::chrono::duration_cast
//                     <std::chrono::milliseconds> (stop - start).count();
//     // report results
//     cout << "Random Shuffle Time Based DFS Sequence Length: " << best << std::setprecision(8)
//          << "\nTime: " << duration / 1000. << " seconds" << endl;
// }

// void heuristic_shuffle(int i)
// {/*
//     This function combines the heuristic and shuffled based
//     approaches above. It ranks words based on the heuristic 
//     above and shuffles each list of ranked words before
//     adding them to the valid words set. It does this rank
//     based shuffle after each successful sequence is found
//     for at least one minute. After the time is up it will
//     finish the seuence it is running before returning
//     the best sequence it found.
//                                                                 */
//     // initialise structures and timer
//     auto start = std::chrono::high_resolution_clock::now();
//     auto elapsed = start, stop = start;
//     std::unordered_map<std::string, std::vector<std::string>> words;
//     std::vector<std::string> valid_words;
//     int length = 0, best = 0;

//     // load words into ordered and unordered structure
//     std::ifstream file("dictionary.txt"); 
//     std::string word;
//     std::map<std::string, std::vector<std::string>> w;

//     while (file >> word)
//         if (word.length() == i)
//         {
//             w[word.substr(word.size()-3, 2)].push_back(word); 
//             words[word.substr(word.size()-3, 2)].push_back(word);
//         }
    
//     while (elapsed - start < std::chrono::seconds(60))
//     {// run restarts for 60 seconds recording the best sequence

//         for (auto it = w.rbegin(); it != w.rend(); it++)
//         {// loop through highest count of ordered structure

//             // save each highest count to temp
//             std::vector<std::string> temp;
//             for (auto elem : it->second)
//                 temp.push_back(elem);

//             // shuffle words in temp
//             auto seed = std::default_random_engine {};
//             std::shuffle(temp.begin(), temp.end(), seed);

//             // add shuffled words of this length to valid words
//             for (auto t : temp)
//                 valid_words.push_back(t);
//         }

//         // computer the sequence length
//         length = circular_sequence(i, words, valid_words);

//         // record best sequence and its time
//         if (best < length)
//         {
//             best = length;
//             stop = std::chrono::high_resolution_clock::now();
//         }

//         // get current elapsed
//         elapsed = std::chrono::high_resolution_clock::now();
//     }

//     // compute time
//     auto duration = std::chrono::duration_cast
//                     <std::chrono::milliseconds> (stop - start).count();

//     // report results
//     cout << "Heuristic Shuffle Time Based DFS Sequence Length: " << best << std::setprecision(8)
//          << "\nTime: " << duration / 1000. << " seconds" << endl;
// }

// void load_words(int word_length, 
//                 std::unordered_map<std::string, std::vector<std::string>>& words, 
//                 std::vector<std::string>& valid_words)
// {/*
//         This function reads in the dictionary for words of length x 
//         and saves them to an unordered structure where the key is the 
//         second and third last letters of the word and the value is the 
//         amount of words that have the same second and third last letters. 
//         It also saves each word to a valid words vector which is the 
//         list of all words that could appear in the sequence.
//                                                                             */
//     std::ifstream file("dictionary.txt"); 
//     std::string word;

//     while (file >> word)
//         if (word.length() == word_length)
//         { 
//             words[word.substr(word.size()-3, 2)].push_back(word); 
//             valid_words.push_back(word);
//         }
// }

// int circular_sequence(int word_length, 
//                       std::unordered_map<std::string, std::vector<std::string>>& words, 
//                       std::vector<std::string>& valid_words)
// {/*
//         This function is the driver to return the longest circular sequence
//         found. For words of length 4 this is simply the highest key count in
//         the unordered structure. For words greater than length 4 a DFS is performed.
//                                                                                         */
//     if (word_length == 4)
//     {// return the value with highest key

//         int current_best = words.begin()->second.size();
//         for (auto it : words)
//             if (it.second.size() > current_best)
//                 current_best = it.second.size();

//         return current_best;
//     }

//     // perform DFS
//     else
//         return dfs(words, valid_words);
// }

// int dfs(std::unordered_map<std::string, std::vector<std::string>>& words, 
//         std::vector<std::string>& valid_words)
// {/*
//     This is a standard DFS function. It initialises each branch
//     seperated by the string 'NIL'. This speration is so we know
//     that we are at a new braching factor so we can pop a new
//     word and go down a different path. This function checks if
//     words create a circular sequence before adding them to the 
//     sequence. It will return the length of the first circulae
//     sequence it finds.
//                                                                     */
//     // initialise structures used by DFS
//     std::stack<std::string> graph;
//     std::unordered_set<std::string> visited;
//     std::vector<std::string> longest_sequence, current_sequence;
//     int sequence_length = 0;

//     for (auto it : valid_words)
//     {// add all elements to the graph with branching separation

//         graph.push("NIL"); // separate
//         graph.push(it);
//     }

//     // perform DFS
//     while (!graph.empty())
//     {
//         std::string current = graph.top();
//         graph.pop();

//         // no words were added at this level
//         if (current.compare("NIL") == 0)
//             current_sequence.pop_back();
        
//         else
//         {// add it to the current path

//             current_sequence.push_back(current);
//             visited.emplace(current);

//             if(current_sequence.size() > sequence_length &&
//                current_sequence[0][current.length() - 3] == current[1] &&
//                current_sequence[0][current.length() - 2] == current[2])
//             {// update sequence if it is current best and circular

//                 longest_sequence = current_sequence;
//                 sequence_length = longest_sequence.size();
//             }

//             // branch out the DFS
//             for (std::string next : words[current.substr(1, 2)])
//             // check the word against the second and third letter

//                 if (visited.find(next) == visited.end())
//                 {// check if its in visited

//                     graph.push("NIL"); 
//                     graph.push(next);
//                 }
//         }
//     }
//     return sequence_length;
// }                     






#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>
#include <map>

using std::cout;
using std::endl;

void ordered(int i);
void heuristic(int i);
void shuffle(int i);
void heuristic_shuffle(int i);
void load_words(int word_length, 
                std::unordered_map<std::string, std::vector<std::string>>& words, 
                std::vector<std::string>& valid_words);
int circular_sequence(int word_length, 
                      std::unordered_map<std::string, std::vector<std::string>>& words, 
                      std::vector<std::string>& valid_words);
int dfs(std::unordered_map<std::string, std::vector<std::string>>& words, 
        std::vector<std::string>& valid_words);                     

int main()
{
    for (int i = 4; i <= 15; i++)
    {// loop across words of length 4 to 15 seperately

        cout << "Finding Circular Sequences of Length: " << i << endl;

        if (i == 4) //  this case is the same for all three algorithms
            ordered(i);

        else
        {        
            // // perform normal DFS
            // ordered(i);

            // perform heuristic based DFS
            heuristic(i);

            // // Perform shuffled DFS
            // shuffle(i);

            // // Perform shuffle and heuristic based DFS
            // heuristic_shuffle(i);
        }

        cout << endl;
    }

    return 0;
}

void ordered(int i)
{/*
        This function performs a normal DFS based on the standard
        ordering of all valid words. This function will find a 
        sequence for all test cases but may not be the longest
        possible for words of length x. 
                                                                        */

    // initialise structures and timer
    auto start = std::chrono::high_resolution_clock::now();
    std::unordered_map<std::string, std::vector<std::string>> words;
    std::vector<std::string> valid_words;

    // load words
    load_words(i, words, valid_words);

    // computer the sequence length
    int length = circular_sequence(i, words, valid_words);

    // compute time
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();

    // report results
    cout << "Order Based DFS Sequence Length: " << length << std::setprecision(8)
         << "\nTime: " << duration / 1000. << " seconds" << endl;
}

void heuristic(int i)
{/*
    This function performs a normal DFS where the order of
    the valid words is based upon a heuristic. The heuristic 
    is the value of each key in the map structure. As an 
    example, if the key 'ea' appears 5 times and 'ie' 
    appears 3 times, the heuristic will be 'ea'. This
    means that the function will order all 'ea' key words
    in valid words first, followed by all 'ie' words.
                                                                */

    // initialise structures and timer
    auto start = std::chrono::high_resolution_clock::now();
    std::unordered_map<std::string, std::vector<std::string>> words;
    std::vector<std::string> valid_words;
    int length = 0;

    // load words into ordered and unordered structure
    std::ifstream file("dictionary.txt"); 
    std::string word;
    std::map<std::string, std::vector<std::string>> w;

    while (file >> word)
        if (word.length() == i)
        {
            w[word.substr(1, 2)].push_back(word); 
            words[word.substr(1, 2)].push_back(word);
        }
    
    // add to valid words based on highest count
    for (auto it = w.rbegin(); it != w.rend(); it++)
        for (auto elem : it->second)
            valid_words.push_back(elem);

    
    // computer the sequence length
    length = circular_sequence(i, words, valid_words);  

    // compute time
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();

    // report results
    cout << "Heuristic Based DFS Sequence Length: " << length << std::setprecision(8) 
         << "\nTime: " << duration / 1000. << " seconds" << endl;
}

void shuffle(int i)
{/* 
    This function performs a normal DFS based on the random
    ordering of all valid words. After a sequence is found
    it will shuffle the valid words and search for another
    sequence. This will continue for at least one minute. 
    After the timer and that sequence has finished it will
    return the best sequence found in that time.
                                                                */

    // initialise structures and timer
    auto start = std::chrono::high_resolution_clock::now();
    auto elapsed = start, stop = start;
    std::unordered_map<std::string, std::vector<std::string>> words;
    std::vector<std::string> valid_words;
    int best = 0, length = 0;

    // load words
    load_words(i, words, valid_words);

    while (elapsed - start < std::chrono::seconds(60))
    {// run restarts for 60 seconds recording the best sequence

        // shuffle words
        auto seed = std::default_random_engine {};
        std::shuffle(valid_words.begin(), valid_words.end(), seed);

        // computer the sequence length
        length = circular_sequence(i, words, valid_words);

        // record best sequence and its time
        if (best < length)
        {
            best = length;
            stop = std::chrono::high_resolution_clock::now();
        }

        // get current elapsed
        elapsed = std::chrono::high_resolution_clock::now();
    }

    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();
    // report results
    cout << "Random Shuffle Time Based DFS Sequence Length: " << best << std::setprecision(8)
         << "\nTime: " << duration / 1000. << " seconds" << endl;
}

void heuristic_shuffle(int i)
{/*
    This function combines the heuristic and shuffled based
    approaches above. It ranks words based on the heuristic 
    above and shuffles each list of ranked words before
    adding them to the valid words set. It does this rank
    based shuffle after each successful sequence is found
    for at least one minute. After the time is up it will
    finish the seuence it is running before returning
    the best sequence it found.
                                                                */
    // initialise structures and timer
    auto start = std::chrono::high_resolution_clock::now();
    auto elapsed = start, stop = start;
    std::unordered_map<std::string, std::vector<std::string>> words;
    std::vector<std::string> valid_words;
    int length = 0, best = 0;

    // load words into ordered and unordered structure
    std::ifstream file("dictionary.txt"); 
    std::string word;
    std::map<std::string, std::vector<std::string>> w;

    while (file >> word)
        if (word.length() == i)
        {
            w[word.substr(1, 2)].push_back(word); 
            words[word.substr(1, 2)].push_back(word);
        }
    
    while (elapsed - start < std::chrono::seconds(60))
    {// run restarts for 60 seconds recording the best sequence

        for (auto it = w.rbegin(); it != w.rend(); it++)
        {// loop through highest count of ordered structure

            // save each highest count to temp
            std::vector<std::string> temp;
            for (auto elem : it->second)
                temp.push_back(elem);

            // shuffle words in temp
            auto seed = std::default_random_engine {};
            std::shuffle(temp.begin(), temp.end(), seed);

            // add shuffled words of this length to valid words
            for (auto t : temp)
                valid_words.push_back(t);
        }

        // computer the sequence length
        length = circular_sequence(i, words, valid_words);

        // record best sequence and its time
        if (best < length)
        {
            best = length;
            stop = std::chrono::high_resolution_clock::now();
        }

        // get current elapsed
        elapsed = std::chrono::high_resolution_clock::now();
    }

    // compute time
    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();

    // report results
    cout << "Heuristic Shuffle Time Based DFS Sequence Length: " << best << std::setprecision(8)
         << "\nTime: " << duration / 1000. << " seconds" << endl;
}

void load_words(int word_length, 
                std::unordered_map<std::string, std::vector<std::string>>& words, 
                std::vector<std::string>& valid_words)
{/*
        This function reads in the dictionary for words of length x 
        and saves them to an unordered structure where the key is the 
        second and third last letters of the word and the value is the 
        amount of words that have the same second and third last letters. 
        It also saves each word to a valid words vector which is the 
        list of all words that could appear in the sequence.
                                                                            */
    std::ifstream file("dictionary.txt"); 
    std::string word;

    while (file >> word)
        if (word.length() == word_length)
        { 
            words[word.substr(1, 2)].push_back(word); 
            valid_words.push_back(word);
        }
}

int circular_sequence(int word_length, 
                      std::unordered_map<std::string, std::vector<std::string>>& words, 
                      std::vector<std::string>& valid_words)
{/*
        This function is the driver to return the longest circular sequence
        found. For words of length 4 this is simply the highest key count in
        the unordered structure. For words greater than length 4 a DFS is performed.
                                                                                        */
    if (word_length == 4)
    {// return the value with highest key

        int current_best = words.begin()->second.size();
        for (auto it : words)
            if (it.second.size() > current_best)
                current_best = it.second.size();

        return current_best;
    }

    // perform DFS
    else
        return dfs(words, valid_words);
}

int dfs(std::unordered_map<std::string, std::vector<std::string>>& words, 
        std::vector<std::string>& valid_words)
{/*
    This is a standard DFS function. It initialises each branch
    seperated by the string 'NIL'. This speration is so we know
    that we are at a new braching factor so we can pop a new
    word and go down a different path. This function checks if
    words create a circular sequence before adding them to the 
    sequence. It will return the length of the first circulae
    sequence it finds.
                                                                    */
    // initialise structures used by DFS
    std::stack<std::string> graph;
    std::unordered_set<std::string> visited;
    std::vector<std::string> longest_sequence, current_sequence;
    int sequence_length = 0;

    for (auto it : valid_words)
    {// add all elements to the graph with branching separation

        graph.push("NIL"); // separate
        graph.push(it);
    }

    // perform DFS
    while (!graph.empty())
    {
        std::string current = graph.top();
        graph.pop();

        // no words were added at this level
        if (current.compare("NIL") == 0)
            current_sequence.pop_back();
        
        else
        {// add it to the current path

            current_sequence.push_back(current);
            visited.emplace(current);

            if(current_sequence.size() > sequence_length &&
               current_sequence[0][1] == current[current.length() - 3] &&
               current_sequence[0][2] == current[current.length() - 2])
            {// update sequence if it is current best and circular

                longest_sequence = current_sequence;
                sequence_length = longest_sequence.size();
            }

            // branch out the DFS
            for (std::string next : words[current.substr(current.size()-3, 2)])
            // check the word against the second and third letter

                if (visited.find(next) == visited.end())
                {// check if its in visited

                    graph.push("NIL"); 
                    graph.push(next);
                }
        }
    }
    return sequence_length;
}                     




/*
zymes
tryma
*/

/*
zymes
ameer
*/