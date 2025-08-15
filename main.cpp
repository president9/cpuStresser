#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "CPUInfo.hpp"
#include "simple_multi_core.hpp"
#include "simple_single_core.hpp"
#include "fpu_multi_core.hpp"


std::vector<std::string> options({"Singlecore - Simple", "Multicore - Simple", "Multicore - FPU"});

void startLoop(){
    while(1){
        auto cpuinf = CPUInfo();
        cpuinf.printInfo();

        std::cout << "Select Stress Test:" << '\n';
        for(int i = 0; i < options.size(); i++){
            std::cout << i << ")" << options[i] << '\n';
        }

        int choice;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        // change this to using a hashmap (?)
        switch(choice) {
            case 0:
                std::cout << "Singlecore - Simple\n";
                simple_stress_int_single_core();
                break;
            case 1:
                std::cout << "Multicore - Simple\n";
                simple_stress_int_test();
                break;
            case 2:
                std::cout << "Multicore - FPU\n";
                stress_fpu_cores();
                break;
            default:
                return;
        }
        std::cout << '\n';
    }
}

int main(int argc, char* argv[]){
    startLoop();
    return 0;
}
