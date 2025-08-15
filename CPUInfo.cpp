#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>
#include <sys/sysctl.h>
#include "CPUInfo.hpp"

/* Provides info such as cpuname, core + thread count
 * TODO: clock speed
 */

// - show info
std::string CPUInfo::getSysctlString(const std::string &name){
    size_t size = 0;
    // std::string result =  ""; // DONT DO THIS. This creates an empty string 
                             // // and the data poitns to nonexistant writable 
                             // // memory, which the sysctl function cant read to
    
    if(sysctlbyname(name.c_str(), nullptr, &size, nullptr, 0) != 0){
        return std::string("Unknown");
    }
    std::string result(size, '\0');
    sysctlbyname(name.c_str(), result.data(), &size, nullptr, 0);
    return result;
}


int CPUInfo::getSysctlInt(const std::string &name){
    int result = 0;
    size_t size = sizeof(result);
    if(sysctlbyname(name.c_str(), &result, &size, nullptr, 0) != 0){
        return -1;
    }
    return result;
}
    

CPUInfo::CPUInfo() {
    cpuName = getSysctlString("machdep.cpu.brand_string");
    cores = getSysctlInt("machdep.cpu.core_count");
    threads = getSysctlInt("machdep.cpu.thread_count");
}

void CPUInfo::printInfo() {
    std::cout << "CPU: " << cpuName << "\n";
    std::cout << "Cores: " << cores << "\n";
    std::cout << "Threads: " << threads << "\n";
}

