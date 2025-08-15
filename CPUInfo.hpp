#pragma once
#include <string>

class CPUInfo {
    private:
        std::string cpuName;
        int cores;
        int threads;

        std::string getSysctlString(const std::string &name);
        int getSysctlInt(const std::string &name);

    public:
        CPUInfo();
        void printInfo();
};

