# DEPENDENCIES:

    The fuzzer is implemented in C++.  It requires the following libraries:

        #include <iostream>
        #include <vector>
        #include <fstream>
        #include <string>
        #include <stdint.h>
        #include <cstdlib>
        #include <random>


    Compile using the following flags in g++

        g++ -std=c++11 fuzzer.cpp -o fuzzer

