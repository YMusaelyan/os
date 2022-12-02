#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sstream>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char const *argv[])
{
    string file_name;
    cin >> file_name;

    int file = open(file_name.c_str(), O_RDONLY);

    if (file == -1) {
        cerr << "error file\n";
        return 0;
    }

    sem_unlink("_sem");
    sem_t *sem = sem_open("_sem",  O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH, 0);

    int state = 0;
    while (++state < 2) {
        sem_post(sem);
    }

    while (--state > 1) {
        sem_wait(sem);
    }

    sem_getvalue(sem, &state);

    pid_t id = fork();
    if (id == -1) {
        cerr << "error fork";
        return 0;
    } else if (id == 0) {
        sem_close(sem);
        execlp("./child", to_string(file).c_str(), NULL);
    } else {
        while (true) {
            sem_getvalue(sem, &state);
            if (state == 0) {
                int fd = shm_open("_back", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                char *mapped = (char *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
                char *allocated = (char *) malloc(sizeof(mapped));
                memcpy(allocated, mapped, sizeof(mapped));
                if (*allocated == '\0') {
                    free(allocated);
                    close(fd);
                    munmap(mapped, sizeof(int));
                    close(file);
                    sem_close(sem);
                    return 0;
                } else {
                    printf("%s\n", allocated);
                }

                free(allocated);
                close(fd);
                munmap(mapped, sizeof(int));
                sem_post(sem);
                sem_post(sem);
            }
        }
    }

    return 0;
}
