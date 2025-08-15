#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
#include "helpers.hpp"

std::atomic<bool> keep_running_fpu_single;
std::mutex cout_mutex_fpu_single;

//stress test
void stress_fpu_single_cores(size_t core_id){
    pin_thread_to_core(core_id); // what is the point of this if the thread has alr been assigned?

    volatile double x = 1.0;
    double thread_overflows = 0.0; // overflows on this thread 
    while(keep_running_fpu_single){
        x = x * 1e20 + 1e10; // chnaged form x*1.000001 + 1.0 cocs it wasnt reaching inf fast enough
        if(x == std::numeric_limits<double>::infinity()){
            x = 1.0;
            thread_overflows += 1.0;
        }
    }
    double fraction = x / std::numeric_limits<double>::max();
    thread_overflows += fraction;
    
    {
        std::lock_guard<std::mutex> lock(cout_mutex_fpu_single);
        std::cout << "Core: " << core_id << ", Approx. overflows = " << thread_overflows << '\n';
    }
}


void stress_fpu_single_cores_test(){
    unsigned int num_threads = std::thread::hardware_concurrency();
    
    // instead of making a vector and startig multicore request on this thread
    // we make a single core request ont his thread instead

    for(unsigned int thread = 0; thread < num_threads; thread++){
        keep_running_fpu_single = true;
        // create a single core request on this thread
        // and then in that function you "switch" threads by cpu pinning
        std::thread worker(stress_fpu_single_cores, thread);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        keep_running_fpu_single = false;
        worker.join();
    }
    std::cout << "Per-core stress test finished." << std::endl;
}
