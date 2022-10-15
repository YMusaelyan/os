#include <unistd.h>
#include <sstream>
#include <iostream>

using namespace std;

int func(int number) {
    int composite = 0;

    if (number < 2) {
        composite = -1;
    } else {
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) {
                composite = 1;
                break;
            }
        }
        if (composite != 1) {
            composite = -1;
        }
    }
    return composite;
}

int main(int argc, char *argv[]){

    int pipefd[2];
    pipefd[0] = atoi(argv[1]);
    pipefd[1] = atoi(argv[2]);

    int file = atoi(argv[0]);
    
    if (dup2(file, 0) == -1) {
        cerr << "error dub\n";
        return 0;
    }
    if (dup2(pipefd[1], 1) == -1) {
        cerr << "error dub\n";
        return 0;
    }

    int n;
    while (cin >> n) {
        if (func(n) == 1) {
            cout << n << "\n";
        } else {
            cout << '\0';
            exit(0);
        }
    }
    close(pipefd[0]);
    close(pipefd[1]);

}
