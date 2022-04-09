#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <queue>
#include <chrono>

using std::cout;
using std::endl;

std::unordered_map<std::string, int> load_words(int word_length);
int shortest_path_ladder_gram(std::string source, std::string target,
                              std::unordered_map<std::string, int> words);
int letter_swap(std::string source, std::string target,
                std::unordered_map<std::string, int> words);
int bbfs(std::string source, std::string target,
         std::unordered_map<std::string, int> words);

int main(int argc, char **argv)
{
    if (argc < 3)
    { // ensure source and target words are passed
        cout << "ERROR! Expected Two Arguments 'Source' 'Target' " << endl;
        exit(1);
    }

    std::string source = argv[1], target = argv[2];
    if (source.size() != target.size())
    { // words must be same length
        cout << "ERROR! Passed words must be of same length" << endl;
        exit(1);
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, int> words = load_words(target.size());

    // ensure both words are in the dictionary
    if (words.find(source) == words.end())
    {
        cout << "ERROR! " << source << " is not in the dictionary" << endl;
        exit(1);
    }
    if (words.find(target) == words.end())
    {
        cout << "ERROR! " << target << " is not in the dictionary" << endl;
        exit(1);
    }

    // find the shortest distance ladder gram
    int distance = shortest_path_ladder_gram(source, target, words);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast
                    <std::chrono::milliseconds> (stop - start).count();

    cout << "Distance from " << source << " to " << target << " = " << 
            distance << " and took " << duration / 1000.0 << " seconds" << endl;

    return 0;
}

std::unordered_map<std::string, int> load_words(int word_length)
{ // loads all words of right length to unordered container
    std::unordered_map<std::string, int> words;

    // insert words of right length into the map
    std::ifstream file("dictionary.txt");
    std::string word;

    while (file >> word)
        if (word.length() == word_length)
            words[word] = 1;

    return words;
}

int shortest_path_ladder_gram(std::string source, std::string target,
                              std::unordered_map<std::string, int> words)
{ /*
     This function first checks the trivial case where a letter can simply
     be swapped from source to target or target to source to provide a
     solution. If this can not be done it performs a Bidirectional Breadth
     First Search to return the shortest distance between source and target word
                                                                                     */
    // Firstly check if letters can be swapped from source to target
    int count = letter_swap(source, target, words);
    if (count > 0)
        return count;

    // Secondly check if letters can be swapped from target to source
    count = letter_swap(target, source, words);
    if (count > 0)
        return count;

    // Otherwise Perform BBFS
    return bbfs(source, target, words);
}

int letter_swap(std::string source, std::string target,
                std::unordered_map<std::string, int> words)
{
    char swap;
    int count = 0;

    for (int i = 0; i < source.size(); i++)
    { // loop through all letters of source word
        
        // if a match move to next letter
        if (target[i] == source[i])
            continue;

        // assign next letter
        swap = source[i];
        source[i] = target[i];

        // check if now a match
        if (source == target)
            return count;

        // ensure new word is in dict and increase count
        if (words.find(source) != words.end())
            count++;

        // if it is not revert the swapped letter
        else
            source[i] = swap;
    }
    return -1;
}

int bbfs(std::string source, std::string target,
         std::unordered_map<std::string, int> words)
{
    // queue to perform bfs from source word
    std::queue<std::string> start;
    // queue to perform bfs from target word
    std::queue<std::string> end;
    // keep track of visited nodes from start
    std::unordered_map<std::string, int> start_visited;
    // keep track of visited nodes from end
    std::unordered_map<std::string, int> end_visited;

    // load source/target words into above containers
    start.push(source);
    end.push(target);

    // initialise their word ladder counts to 0
    start_visited[target] = 0;
    end_visited[target] = 0;

    int start_count = 0, end_count = 0, size = 0;
    while (!start.empty() && !end.empty())
    { // from the start
        start_count++;
        size = start.size();

        for (int i = 0; i < size; i++)
        { // loop through active words

            std::string current = start.front();
            start.pop();
            for (int j = 0; j < current.size(); j++)
            { // loop through characters in active word

                char initial = current[j];

                for (char c = 'a'; c <= 'z'; c++)
                { // try swap letters

                    current[j] = c;
                    // ensure new word is in dict

                    if (words.find(current) != words.end() &&
                        start_visited.find(current) == start_visited.end())
                    { // see if its a match

                        if (current == target)
                            return start_count;

                        // check if its found in the end BFS
                        if (end_visited.find(current) != end_visited.end())
                            return start_count + end_visited[current];

                        // update counts in the queue incase end finds a match
                        start_visited[current] = start_count;
                        start.push(current);
                    }
                }
                current[j] = initial;
            }
        }
        // from the end
        end_count++;
        size = end.size();

        for (int i = 0; i < size; i++)
        { // loop through active words

            std::string current = end.front();
            end.pop();

            for (int j = 0; j < current.size(); j++)
            { // loop through characters in active word

                char initial = current[j];

                for (char c = 'a'; c <= 'z'; c++)
                { // try swap letters

                    current[j] = c;

                    // ensure new word is in dict
                    if (words.find(current) != words.end() &&
                        end_visited.find(current) == end_visited.end())
                    { // see if its a match

                        if (current == target)
                            return end_count;

                        // check if its found in the end BFS
                        if (start_visited.find(current) != start_visited.end())
                            return end_count + start_visited[current];

                        // update counts in the queue incase end finds a match
                        end_visited[current] = end_count;
                        end.push(current);
                    }
                }
                current[j] = initial;
            }
        }
    }
    cout << "No Solution Found" << endl;
    return 0;
}