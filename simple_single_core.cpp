#include <thread>
#include <vector>
#include <iostream>
#include <atomic>
#include <mutex>
#include <chrono>
#include <climits>
#include <pthread.h>
#include <mach/thread_policy.h>
#include <mach/mach.h>

std::atomic<bool> keep_running_simple_sc;
std::mutex cout_mutex_simple_sc;

void pin_thread_to_core(size_t core_id) {
    thread_affinity_policy_data_t policy = { static_cast<integer_t>(core_id) };
    thread_port_t mach_thread = pthread_mach_thread_np(pthread_self());
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY,
                      (thread_policy_t)&policy,
                      THREAD_AFFINITY_POLICY_COUNT);
}

void simple_stress_int(size_t core_id) {
    pin_thread_to_core(core_id);

    volatile unsigned long long x = 1;
    double overflow_count = 0;
    while (keep_running_simple_sc) {
        unsigned long long prev = x;
        x *= 3;
        x ^= 0xDEADBEEF;
        x += 7;
        if (x < prev){
            overflow_count += 1.0;
        }
    }
    double fraction = static_cast<double>(x) / static_cast<double>(ULLONG_MAX);
    overflow_count += fraction;

    {
        std::lock_guard<std::mutex> lock(cout_mutex_simple_sc);
        std::cout << "Core: " << core_id << ", Approx. overflows = " << overflow_count << '\n';
    }
}

void simple_stress_int_single_core() {
    unsigned int cores = std::thread::hardware_concurrency();
    
    for(unsigned int core = 0; core < cores; core++){
        keep_running_simple_sc = true;
        std::thread worker(simple_stress_int, core);

        std::this_thread::sleep_for(std::chrono::seconds(3));
        keep_running_simple_sc = false;
        worker.join();
    }

    std::cout << "Per-core stress test finished." << std::endl;
}
