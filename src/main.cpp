#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

#include "CPUInfo.hpp"
#include "simple_multi_core.hpp"
#include "simple_single_core.hpp"


std::vector<std::string> options({"Singlecore", "Multicore"});

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

        switch(choice) {
            case 0:
                std::cout << "Singlecore\n";
                simple_stress_int_single_core();
                break;
            case 1:
                std::cout << "Multicore\n";
                simple_stress_int_test();
                break;
            // case 2:
                // // run options[2]
                // std::cout << "Custom core\n";
                // break;
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
