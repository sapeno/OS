#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>


#define SHM_SIZE 4096

const char SHM_NAME[] = "shm";
const char SEM_EMPTY[] = "sem_empty";
const char SEM_FULL[] = "sem_full";

typedef struct {
    uint32_t length;
    bool terminate;
} SharedData;

int main(int argc, char *argv[]) {
    int shm = shm_open(SHM_NAME, O_RDWR, 0);
    if (shm == -1) {
        const char msg[] = "error: failed to open SHM\n";
		write(STDERR_FILENO, msg, sizeof(msg) - 1);
		_exit(EXIT_FAILURE);
    }

    char *shm_buf = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
    if (shm_buf == MAP_FAILED) {
		const char msg[] = "error: failed to map SHM\n";
		write(STDERR_FILENO, msg, sizeof(msg));
		_exit(EXIT_FAILURE);
	}

    sem_t *sem_empty = sem_open(SEM_EMPTY, 0);
    sem_t *sem_full = sem_open(SEM_FULL, 0);
    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED) {
		const char msg[] = "error: failed to open semaphore\n";
		write(STDERR_FILENO, msg, sizeof(msg) - 1);
		_exit(EXIT_FAILURE);
	}

    int result_file = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (result_file == -1) {
        const char msg[] = "error: failed to open result file\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        _exit(EXIT_FAILURE);
    }

    SharedData *data = (SharedData *)shm_buf;
    float *shm_numbers = (float *)(shm_buf + sizeof(SharedData));

    bool running = true;
    while (running){
        sem_wait(sem_full);

        if (data->terminate) {
            running = false;
        } else {
            float sum = 0.0f;
            for (uint32_t i = 0; i < data->length; i++) {
                sum += shm_numbers[i];
            }

            char out_buf[256];
            int len = snprintf(out_buf, sizeof(out_buf), "Sum: %f\n", sum);
            write(result_file, out_buf, len);
        }
        
        sem_post(sem_empty);
    }
    sem_close(sem_empty);
    sem_close(sem_full);
    munmap(shm_buf, SHM_SIZE);
    close(shm);
    return 0;
}