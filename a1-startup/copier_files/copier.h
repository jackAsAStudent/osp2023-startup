#ifndef COPIER_H
#define COPIER_H
#include <fstream>
#include <iostream>
#include <time.h>

#define BUFFER_SIZE 2048

class Copier {
private:
    std::ifstream* input;
    std::ofstream* output;

public:
    Copier(std::ifstream* input, std::ofstream* output);
    void copy();
};
#endif // COPIER_H