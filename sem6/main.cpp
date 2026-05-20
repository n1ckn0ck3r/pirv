#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>

const char* SHM_NAME = "/my_shm_bbuffer";
const char* SEM_NAME = "/me_sem_b";
const int NUM_CHILDREN = 4;

struct SharedData {
    int data[NUM_CHILDREN];
};

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate failed");
        return 1;
    }

    SharedData* shared_data = (SharedData*)mmap(
        nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0
    );

    if (shared_data == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        shared_data->data[i] = 0;
    }

    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    std::cout << "Parent process started. Waiting for child processes to write data..." << std::endl;

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            return 1;
        }

        if (pid == 0) {
            std::cout << "Child process " << i << "(PID: "<< getpid() <<")" <<" started." << std::endl;
        }

        sleep(i + 1);

        shared_data->data[i] = (i + 1) * 10;
        std::cout << "Child process " << i << "(PID: "<< getpid() <<")" <<" wrote data: " << shared_data->data[i] << std::endl;

        std::cout << "Child process " << i << "(PID: "<< getpid() <<")" <<" reached the barrier." << std::endl;
        sem_post(sem); 

        munmap(shared_data, sizeof(SharedData));
        close(shm_fd);
        sem_close(sem);
    }

    std::cout << "Parent process waiting for child processes to reach the barrier..." << std::endl;
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        sem_wait(sem); 
    }

    std::cout << "All child processes have reached the barrier. Parent process is now reading data..." << std::endl;
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        std::cout << "Data from child process " << i << ": " << shared_data->data[i] << std::endl;
    }

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        wait(nullptr); 
    }

    munmap(shared_data, sizeof(SharedData));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    sem_close(sem);
    sem_unlink(SEM_NAME);

    std::cout << "Parent process finished." << std::endl;
    return 0;
}