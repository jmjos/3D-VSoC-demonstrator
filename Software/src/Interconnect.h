//
// Created by joseph on 14/12/18.
//

#ifndef SIM_INTERCONNECT_H
#define SIM_INTERCONNECT_H


#include "TransmissionInterface.h"

class Interconnect : public sc_module, public TransmissionInterface {
public:
    static const int componentCount = 1;
    sc_port<TransmissionInterface> ports[componentCount];

    SC_HAS_PROCESS(Interconnect);
    Interconnect(const sc_module_name &nm);


    virtual void transmitImage(address_t dst, std::vector<control_t> control, std::vector<image_t> *data);


};


#endif //SIM_INTERCONNECT_H
