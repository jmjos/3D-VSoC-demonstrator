//
// Created by joseph on 14/12/18.
//

#include "VSoC.h"

VSoC::VSoC(const sc_module_name &nm) {

    //Sensor reader reads image data
    reader = std::make_unique<SensorReader>("SensorReader");

    //instances of adcs with addrs 0 to 8
    adc[0] = new ADC("ADC0", 0); //u l
    adc[1] = new ADC("ADC1", 1); //u m
    adc[2] = new ADC("ADC2", 2); //u r
    adc[3] = new ADC("ADC3", 3); //m l
    adc[4] = new ADC("ADC4", 4); //m m
    adc[5] = new ADC("ADC5", 5); //m r
    adc[6] = new ADC("ADC6", 6); //d l
    adc[7] = new ADC("ADC7", 7); //d m
    adc[8] = new ADC("ADC8", 8); //d r

    //instances of bayer filter cpu units with addrs 9 to 17
    cpuBayer[0] = new CPU_Bayer("CPUBayer0", 9);
    cpuBayer[1] = new CPU_Bayer("CPUBayer1", 10);
    cpuBayer[2] = new CPU_Bayer("CPUBayer2", 11);
    cpuBayer[3] = new CPU_Bayer("CPUBayer3", 12);
    cpuBayer[4] = new CPU_Bayer("CPUBayer4", 13);
    cpuBayer[5] = new CPU_Bayer("CPUBayer5", 14);
    cpuBayer[6] = new CPU_Bayer("CPUBayer6", 15);
    cpuBayer[7] = new CPU_Bayer("CPUBayer7", 16);
    cpuBayer[8] = new CPU_Bayer("CPUBayer8", 17);

    //interconnect network. spreads messages to correct dst and writes trace file
    interconnect = new Interconnect("Interconnect");

    //connect SensorReader to all ADCs
    int i =0;
    for (auto it : adc){
        reader->imagePort[i++](*it);
    }

    // connect outgoing ports of ADCs to Interconnect
    for (auto it : adc) {
        it->imagePort(*interconnect);
    }

    // connect outgoing ports of Bayer CPUs to Interconnect
    for (auto it : cpuBayer) {
        it->imagePort(*interconnect);
    }

    //connect outgoint ports of Interconnect to correct receiver. Must be correct for addresses!!!
    for (address_t addr = 0; addr <=8; addr++){
        interconnect->ports[addr](*adc[addr]);
    }
    for (address_t addr = 9; addr <=17; addr++){
        interconnect->ports[addr](*cpuBayer[addr-9]);
    }
}

VSoC::~VSoC() {
    for (auto it : adc) delete it;
    for (auto it : cpuBayer) delete it;
}