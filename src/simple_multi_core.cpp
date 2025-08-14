#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <iostream>
#include <mutex>

std::atomic<bool> keep_running;
std::mutex cout_mutex;
std::mutex overflow_mutex;
double total_overflows = 0;


void simple_stress_int() {
    volatile unsigned long long x = 1;
    double thread_overflows = 0; // dont need to mutex this as it is local
    while (keep_running) {
        unsigned long long prev = x;
        x *= 3;
        x ^= 0xDEADBEEF;
        x += 7;
        if (x < prev){
            double fraction = static_cast<double>(x) / static_cast<double>(ULLONG_MAX);
            thread_overflows += 1.0 + fraction;
        }
    }

    {
        std::lock_guard<std::mutex> lock(overflow_mutex);
        total_overflows += thread_overflows;
    }
}

void simple_stress_int_test(){
    unsigned int threads = std::thread::hardware_concurrency();
    std::vector<std::thread> workers;

    keep_running = true;

    for(unsigned int i = 0; i < threads - 1; i++){
        workers.emplace_back(simple_stress_int); // work immediately starts
    }

    std::thread main_worker(simple_stress_int); 
    std::this_thread::sleep_for(std::chrono::seconds(3));

    keep_running = false;

    for (auto &t : workers) {
        t.join();
    }
    main_worker.join();

    std::cout << "Total overflows: " << total_overflows << '\n';
    std::cout << "Multicore stress test finished." << std::endl;
    total_overflows = 0;
}
