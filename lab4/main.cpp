#include <iostream>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <sys/mman.h>
#include <thread>
#include <sstream>
#include <vector>

int main() {
    const int map_size = 4096;
    std::string filename;
    std::string results;
    std::vector<std::string> string_buffer;
    std::cout << "Enter filename  with test\n";
    std::cin >> filename;
    std::ifstream file_descriptor;
    file_descriptor.open(filename);
    if (!file_descriptor.is_open()) {
        perror("Can't open the file!");
        exit(1);
    }
    std::string str;
    int lb = 0;
    const char *input_sem_name = "input_sem";
    const char *output_sem_name = "output_sem";
    sem_unlink(input_sem_name);
    sem_unlink(output_sem_name);

    sem_t* input_semaphore = sem_open(input_sem_name, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 1);
    sem_t* output_semaphore = sem_open(output_sem_name, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);
    if(input_semaphore == SEM_FAILED || output_semaphore == SEM_FAILED){
        perror("Can't create semaphore");
        exit(1);
    }

    int shared_memory_file_descriptor1=shm_open("map_fd1.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    int shared_memory_file_descriptor2=shm_open("map_fd2.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    if (shared_memory_file_descriptor1 == -1 || shared_memory_file_descriptor2 == -1) {
        perror("Can't create shared memory!");
        exit(1);
    }

    char *memptr1 = (char *) mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    char *memptr2 = (char *) mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (memptr1 == MAP_FAILED || memptr2 == MAP_FAILED) {
        perror("can't open the file");
        exit(1);
    }

    int process_id = fork();
    if (process_id == -1) {
        perror("PID error!");
        exit(1);
    } else if (process_id == 0) {
        sem_wait(output_semaphore);
        sem_wait(input_semaphore);
        sem_post(output_semaphore);

        struct stat status;
        if (fstat(shared_memory_file_descriptor1, &status)) {
            perror("fstat error");
            exit(1);
        }

        int index = 0;
        int length_1 = 0;
        for(int i = 0; i <= status.st_size; ++i){
            if (memptr1[i] != '\n') {
                results += memptr1[i];
            } else {
                long double summ = 0;
                std::string result = "";
                double number;
                results += " ";

                for (unsigned j = 0; j < results.length(); ++j) {
                    if (results[j] != ' ') {
                        result += results[j];
                    } else {
                        number = stof(result);
                        summ += number;
                        result = "";
                    }
                }

                results = std::to_string(summ) + "\n";
                length_1 += results.length() * sizeof(char);
                if (ftruncate(shared_memory_file_descriptor2, length_1)) {
                    perror("ftruncate error");
                    exit(1);
                }

                for (unsigned j = 0; j < results.length(); ++j) {
                    memptr2[index] = results[j];
                    index += 1;
                }

                results = "";
            }
        }
        sem_post(input_semaphore);
    } else {
        sem_wait(input_semaphore);
        sem_post(output_semaphore);
        int index = 0;
        int length = 0;

        while (!file_descriptor.eof()) {
            getline(file_descriptor, results);
            if (results != "") {
                results += "\n";

                length += results.length() * sizeof(char);
                if (ftruncate(shared_memory_file_descriptor1, length)) {
                    perror("ftruncate error");
                    exit(1);
                }

                for (unsigned i = 0; i < results.length(); i++) {
                    memptr1[index] = results[i];
                    index += 1;
                }
            }
        }
        results = "";
        sem_post(input_semaphore);
        sem_wait(output_semaphore);
        sem_wait(input_semaphore);

        struct stat status;
        if (fstat(shared_memory_file_descriptor2, &status)) {
            perror("fstat error");
            exit(1);
        }

        for (int i = 0; i <= status.st_size; ++i) {
            if(memptr2[i] == '\n') {
                str.assign(memptr2+lb, i - lb +1);
                lb = i + 1;
                std::cout << str;
            }
        }
    }

    munmap(memptr1, map_size);
    munmap(memptr2, map_size);
    shm_unlink("map_fd1.txt");
    shm_unlink("map_fd2.txt");
    remove("map_fd1.txt");
    remove("map_fd2.txt");
    sem_destroy(input_semaphore);
    sem_destroy(output_semaphore);
    close(shared_memory_file_descriptor1);
    close(shared_memory_file_descriptor2);
}