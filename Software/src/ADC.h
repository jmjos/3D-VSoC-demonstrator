//
// Created by joseph on 14/12/18.
//

#ifndef SIM_ADC_H
#define SIM_ADC_H

#include <systemc>
#include <vector>

#include "TransmissionInterface.h"

class ADC : public sc_module{
private:
    address_t addr;

public:
    sc_port<TransmissionInterface> transmissionPort;

    SC_HAS_PROCESS(ADC);
    ADC(const sc_module_name &nm, address_t addr);

};


#endif //SIM_ADC_H
