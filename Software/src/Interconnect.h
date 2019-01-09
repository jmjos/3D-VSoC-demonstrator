//
// Created by joseph on 14/12/18.
//

#ifndef SIM_INTERCONNECT_H
#define SIM_INTERCONNECT_H


#include "TransmissionInterface.h"

class Interconnect : public sc_module, public TransmissionInterface {
public:
    sc_port <TransmissionInterface> bayerPort;

    virtual void transmitImage(address_t dst, std::vector<control_t> control, std::vector<image_t> *data);


};


#endif //SIM_INTERCONNECT_H
