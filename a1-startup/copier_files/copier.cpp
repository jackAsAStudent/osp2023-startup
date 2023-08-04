#include "copier.h"

Copier::Copier(std::ifstream* input, std::ofstream* output) {
    this->input = input;
    this->output = output;
}

void Copier::copy() {
    const size_t bufferSize = BUFFER_SIZE;  
    char buffer[bufferSize];

    while (!input->eof()) {
        input->read(buffer, bufferSize);
        std::streamsize count = input->gcount();
        output->write(buffer, count);
    }
}


int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cout << "Usage: copier <input> <output>" << std::endl;
    }

    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);

    //open up both of the input and output files for reading and writing
    if (input.is_open() && output.is_open()) {
        Copier copier(&input, &output);
        clock_t start = clock();
        copier.copy();
        clock_t end = clock();
        double elapsed_time = end - start;
        std::cout << "Copied in " << elapsed_time << "ms" << std::endl;
    }

    return EXIT_SUCCESS;
}