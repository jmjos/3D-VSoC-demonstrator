//
// Created by joseph on 14/12/18.
//

#ifndef SIM_VSOC_H
#define SIM_VSOC_H

#include <systemc>

#include "ADC.h"
#include "CPU_Bayer.h"
#include "Interconnect.h"

class VSoC : public sc_module {

    ADC *adc;
    CPU_Bayer *cpu;
    Interconnect *interconnect;

public:
    SC_HAS_PROCESS(VSoC);
    VSoC(const sc_module_name &nm);
    ~VSoC();

};


#endif //SIM_VSOC_H
