#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <vector>

const size_t map_size = 4096;
const char* output = "output.back";
const char* semaphore = "output.semaphore";

int main(int argc, char **argv) {
    std::vector<double> results;
    assert(argc == 2);
    const char* filename = argv[1];
    FILE *file = fopen(filename, "r");
    double number;
    char symbol;
    double summ=0.;
    int shared_memory;
    shared_memory = shm_open(output, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    void* shared_pointer = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    while ((fscanf(file, "%lf%c", &number, &symbol) != EOF)) {
        summ+=number;
        if (symbol == '\n') {
            results.push_back(summ);
            summ=0.;
            continue;
        }
    }
    if(shm_unlink(output) != 0) {
        exit(EXIT_FAILURE);
    }
    if(munmap(shared_pointer, map_size) != 0) {
        exit(EXIT_FAILURE);
    }
    return 0;
}