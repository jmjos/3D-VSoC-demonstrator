//
// Created by joseph on 14/12/18.
//

#ifndef SIM_ADC_H
#define SIM_ADC_H

#include <systemc>
#include <vector>

#include "TransmissionInterface.h"
#include "GlobalConfiguration.h"

class ADC : public sc_module, public TransmissionInterface{
private:
    GlobalConfiguration &global = GlobalConfiguration::getInstance();

    address_t addr;
    ImageData* image;
    sc_event sendEv;

public:

    sc_port<TransmissionInterface> imagePort;
    SC_HAS_PROCESS(ADC);
    ADC(const sc_module_name &nm, address_t addr);

    void process();
    virtual void transmitImage(address_t src, address_t dst, control_t control, ImageData *data);
};


#endif //SIM_ADC_H
