#include <unistd.h>
#include <iostream>
#include <string>
#include <fcntl.h>
using namespace std;

int main(){

    string file_name;
    cin >> file_name;

    int file = open(file_name.c_str(), O_RDONLY);
    if (file == -1) {
        cerr << "error file\n";
        return 0;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1){
        cerr << "error pipe\n";
        return 0;
    }

    pid_t id = fork();
    if (id == -1){
        cerr << "error fork";
        return 0;
    } else if (id == 0){
        execlp("./child", to_string(file).c_str(), to_string(pipefd[0]).c_str(), to_string(pipefd[1]).c_str(), NULL);
    } else {
        char p;
        while (true){
            read(pipefd[0], &p, sizeof(p));
            if (p == '\0') {
                exit(0);
            } else {
                putchar(p);
            }
        }
    }

    close(pipefd[0]);
    close(pipefd[1]);
    close(file);

    return 0;   

}