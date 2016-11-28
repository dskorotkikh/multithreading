#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <cmath>
#include <stack>
#include <sem
#include <functional>
#include <vector>

int n_reader = 0;
std::mutex read;
std::mutex write;
std::mutex order;
std::vector<int> shared_data;


void safe_read(std::function<void()> reader) {
    write.lock();
    order.lock();
    if (n_reader == 0)  read.lock();
    ++n_reader;
    order.unlock();
    write.unlock();

    reader();

    order.lock();
    --n_reader;
    if (n_reader == 0)  read.unlock();
    order.unlock();
}

void safe_write(std::function<void()> writer) {
    write.lock();   // запрет добавлять читателей
    read.lock();    // ждём пока читатели выйдут
    write.unlock(); // разрешаем стакаться в очередь на read (для читающих и пишёщих)
    writer();
    read.unlock();
}

void writer() {
    shared_data.push_back(shared_data.size());
}


void reader() {
    if (shared_data.size()) std::cout << shared_data[shared_data.size()-1] << std::endl;
    else                    std::cout << "****" << std::endl;
    std::cout.flush();
}

int main() {

    int rolls = 1000;
    std::stack<std::thread> threads;

    while (rolls--) {
        if (rand()%4 == 0)      threads.push(std::thread([](){  safe_write(writer); }));
        else                    threads.push(std::thread([](){  safe_read(reader);  }));
    }
    while(!threads.empty()) {
        threads.top().join();
        threads.pop();
    }
    return 0;
}