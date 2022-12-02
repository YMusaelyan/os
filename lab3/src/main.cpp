#include <iostream>
#include <pthread.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

struct Data 
{
    vector<vector<int> > &vec;
    vector<int> &res;
    int step;
    int size_arr;
    int k;
    int number_threads;
    int count_threads;
};

void* thread_func(void *argc)
{
    Data* arguments = (Data*) argc;
    vector<vector<int> > &vec = arguments->vec;
    vector<int> &res = arguments->res;
    int step = arguments->step;
    int size_arr = arguments->size_arr;
    int k = arguments->k;
    int number_threads = arguments->number_threads;
    int count_threads = arguments->count_threads;
    
    if (count_threads != number_threads - 1) {
        for (int i = count_threads * step; i < count_threads * step + step; ++i) {
            for (int j = 0; j < k; ++j) {
                res[i] += vec[j][i];
            }
        }   
    } else {
        for (int i = count_threads * step; i < size_arr; ++i) { 
            for (int j = 0; j < k; ++j) {
                res[i] += vec[j][i];
            }
        }   
    }

    return 0;
}

int main(int argc, const char** argv) {

    

    if (argc != 2) {
        cerr << "not number of threads\n";
        return 1;
    }

    int number_threads = atoi(argv[1]);
    int size_arr, k;
    cout << "Введите количество массивов\n";
    cin >> k;
    cout << "Введите размер массивов \n";
    cin >> size_arr;

    vector<vector<int> > vec(k, vector<int>(size_arr, 0));
    vector<int> res(size_arr, 0);

    cout << "Введите массивы\n";
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < size_arr; ++j) {
            cin >> vec[i][j];
        }
    }

    system_clock::time_point start = system_clock::now();
    
    if (number_threads > size_arr) {
        number_threads = size_arr;
    }
    int step = size_arr / number_threads;

    vector<pthread_t> threads(number_threads);
    vector<Data> data(number_threads,{vec, res, step, size_arr, k, number_threads, 0});


    for (int i = 0; i < number_threads; i++){
        data[i].count_threads = i;
        if(pthread_create(&threads[i], NULL, thread_func, &data[i]) != 0){
            cerr << "cannot create thread " << i << '\n';
            return 1;
        }
    }

    for (int i = 0; i < number_threads; i++){
        if(pthread_join(threads[i], NULL) != 0){
            cerr << "cannot join thread " << i << '\n';
            return 1;
        }
    }
    
    system_clock::time_point end = system_clock::now();

    for(int i = 0; i < size_arr; ++i) {
        cout << data[number_threads - 1].res[i] << " "; 
    }
    cout << '\n';
    duration<double> sec = end - start;
    cout << sec.count() << " сек." << endl;
    return 0;
}