//
// Created by joseph on 14/12/18.
//

#include "VSoC.h"

VSoC::VSoC(const sc_module_name &nm) {
    adc = new ADC("ADC", 0);
    interconnect = new Interconnect("Interconnect");
    cpu = new CPU_Bayer("CPU", 1);

    adc->image_port(*interconnect);
    interconnect->ports[0](*cpu);
}

VSoC::~VSoC() {
    delete adc;
    delete cpu;
}