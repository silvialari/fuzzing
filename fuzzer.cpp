/******************************************************************************
 * fuzzer.cpp - Fuzzer program 
 * 
 * This program implements a mutative fuzzer in compliance with the project
 * spec. 
 *
 * It takes as input two argumennts <seed> and <number_of_iterations>.
 *  - The seed is used to initialize a random generator.
 *  - The number_of_iteration is used internally to iterate through a string 
 *     mutation logic the given number ot times
 *
 * Logic:
 *
 * 1. Upon startup the program reads into memory the contents a 'seed' file
 *    which should be a string of characters
 * 2. Seed the random generator withe seed value
 * 3. In a loop for the given number_of_iterations:
 *    a. Take the original string and for each byte given a 13% probaility
 *       relace it with a random byte
 *    b. Print the mutated string to stdout
 *    c. Replace the original seed string with the mutated string
 * 
 * Compilation:
 *    g++ -std=c++11 fuzzer.cpp -o fuzzer 
 *
 * Usage:
 *    ./fuzzer /dev/urandom/ 10000
 * 
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdint.h>
#include <cstdlib>
#include <random>

using namespace std;

const vector<string> SEED = {"seed", "_seed_"};
const int PERCENT_PROB = 13;

const int DEBUG = 0;

default_random_engine rn;

vector<char> read_seed() {
    /**
     * Read in a seed file and return a vector that contains
     * the seed file contents back up to the 
     * calling function.     
     */
    vector<char> vect;

    ifstream file_in;

    for (int i=0; i < SEED.size(); i++) {
        file_in.open(SEED[i].c_str(), ios::in | ios::binary);

        if (file_in.is_open()) {
            file_in.seekg(0, file_in.end);
            size_t flength = file_in.tellg();
            vect.resize(flength);
            file_in.seekg(0, file_in.beg);

            file_in.read(&vect[0], flength);
            file_in.close();
            break;
        }
    }

    if (vect.size() == 0)
        cout << "Unable to open the seed file\n"; 

    return vect;
}

char random_char() {
   /**
    * Generate a random byte from distrbution of 0 to 255
    **/
   uniform_int_distribution<int> dist(0,255);
   return static_cast<char>(dist(rn) & 0xFF);
}

void get_mutated_output(vector<char> &data, bool add_char) {
    /**
     * Given a random string in data if this add_char flag is 
     * true append 10 random bytes to data.
     *
     * For each byte in data calculate a 13% probability. If true
     * replace the the data byte with a random byte.
     * 
     * Print the mutated  strint to stdout
     * 
     * Replace data with the mutated string
     *
     * Inputs: data - vector containing string to be mutated
     *         add_char - bool if true append 10 random bytes
     */
    vector<char> work_data;
    char c;

    if (add_char==1) {
        vector<char> extra_chars;
        for (int j=0; j < 10; j++) {
            extra_chars.push_back(random_char());
        }
        data.insert(data.end()-1, extra_chars.begin(), extra_chars.end());
    }
         
    for (auto i = data.cbegin(); i != data.cend(); ++i) {
        bool mutate = (rand() % 100) < PERCENT_PROB;
        if (mutate) {
            c = random_char();
        } else {
            c = *i;
        }
        work_data.push_back(c);
        cout << c;
    }
    
    data = work_data;
    work_data.clear();
}


int main(int argc, char** argv) {
    /**
     * Entry point for fuzzer program.  
     * 
     * Logic:
     *   1. Parse command line and get seed value as ing prng and
     *      number of interations as int num_of_iterations
     *   2. Iterates num_of_iterations times:
     *      a. Every 500 iterations call get_mutated_output with bool add_char
     *         set.
     *      b. Otherwise mutate data string.
     */
    if (argc < 3) {
       cout << "usage: " << argv[0] << " <prng_seed> <num_of_iterations>\n";
       return 1;
    }

    int prng = atoi(argv[1]);
    int iterations = atoi(argv[2]);

    srand(prng);
    default_random_engine rn(prng);
    vector<char> data = read_seed();
 
    if (DEBUG) {
        for (auto i = data.cbegin(); i != data.cend(); ++i)
            cout << *i;
        cout << "\n";
    }

    for (int i=1; i < iterations+1; i++) 
        if ((i % 500) == 0 ) {
            get_mutated_output(data, 1);
        } else {
            get_mutated_output(data, 0);
        }


    return 0;
