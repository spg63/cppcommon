#include "../../CLHelper.hpp"
#include <iostream>
#include <vector>

int main(){
    // Will use the default device, which the default c'tor assumes is device #2, the dGPU
    CLHelper clh_d;

    // Specify using device #1, which is iGPU on 2016 MBP
    CLHelper clh_i(3, 1);

    // Specify using device $0, which is CPU on 2016 MBP
    CLHelper clh_cpu(3, 0);

    auto success = clh_d.setup_opencl();
    success = clh_i.setup_opencl();
    success = clh_cpu.setup_opencl();

    return 0;
}
