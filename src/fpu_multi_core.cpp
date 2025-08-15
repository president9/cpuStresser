#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>


std::atomic<bool> keep_running_fpu_multi;
std::mutex cout_mutex_fpu_multi;
std::mutex overflow_mutex_fpu_multi;
double total_overflows_fpu_multi = 0.0;

//stress test
void stress_fpu_multi(){
    volatile double x = 1.0;
    double thread_overflows = 0.0; // overflows on this thread 
    while(keep_running_fpu_multi){
        x = x * 1e20 + 1e10; // chnaged form x*1.000001 + 1.0 cocs it wasnt reaching inf fast enough
        if(x == std::numeric_limits<double>::infinity()){
            x = 1.0;
            thread_overflows += 1.0;
        }
    }

    double fraction = x / std::numeric_limits<double>::max();
    thread_overflows += fraction;
    
    {
        std::lock_guard<std::mutex> lock(overflow_mutex_fpu_multi);
        total_overflows_fpu_multi += thread_overflows;
    }
}

// stress the cores now
void stress_fpu_multi_cores(){
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> workers;
    
    keep_running_fpu_multi = true; // declare here cos the other threads need to work;

    // stress all other threads
    for(int i = 0; i < num_threads - 1; i++){
        workers.emplace_back(stress_fpu_multi);
    }

    // stress this core as thread is being declared in this scope
    std::thread main_worker(stress_fpu_multi); // this part still be running even if next line is sleep ye?
    std::this_thread::sleep_for(std::chrono::seconds(3));
    keep_running_fpu_multi = false;

    for(auto& worker : workers){
        worker.join();
    }

    main_worker.join(); // this is redundant right?
    std::cout << "total overflows: " << total_overflows_fpu_multi <<'\n';
    total_overflows_fpu_multi = 0;
}

