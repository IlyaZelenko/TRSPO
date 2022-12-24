#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m;

class A {
public:
    float read() {
        return some_float;
    }
    void write(float new_float) {
        some_float = new_float;
    }
private:
    float some_float = 0.0;
};


class B {
public:
    double read() {
        return some_double;
    }
    void write(double new_double) {
        some_double = new_double;
    }
private:
    double some_double = 0.0;
};

A a;
B b;

float genFloat() {
    float new_float;
    new_float = rand() % 100 + (rand() % 1000) / 1000.f;
    return new_float;
}

void add_num(A* a) {
    float num = genFloat();
    m.lock();
    float new_num = num + a->read();
    a->write(new_num);
    m.unlock();
}

void add_num(B* b) {
    double num = (double)genFloat();
    m.lock();
    double new_num = num + b->read();
    b->write(new_num);
    m.unlock();
}

void func(int iter) {
    for (int i = 0; i < iter; i++)
    {
        add_num(&a);
        add_num(&b);
    }
}



int main() {
    srand(time(0));

    auto start = std::chrono::system_clock::now();

    int k1 = rand() % 10001 + 10000;
    int k2 = rand() % 10001 + 10000;

    int num_of_threads = rand() % 11 + 10;
    std::thread* threads_arr = new std::thread[num_of_threads];

    for (int i = 0; i < num_of_threads / 2; i++) {
        threads_arr[i] = std::thread(func, k1);
        threads_arr[i].join();
    }
    for (int i = num_of_threads/2; i < num_of_threads; i++) {
        threads_arr[i] = std::thread(func, k2);
        threads_arr[i].join();
    }
    auto finish = std::chrono::system_clock::now();

    delete[] threads_arr;

    double prog_duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    std::cout <<num_of_threads<<"\t" << a.read() << "\t" << b.read() << "\n" << "\t\t\t~//~\n\n\tThe program was running " << prog_duration << " milliseconds\n\n\t\t\t~//~\n\n";
    return 0;
}