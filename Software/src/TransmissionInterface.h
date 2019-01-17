//
// Created by joseph on 14/12/18.
//

#ifndef SIM_TRANSMISSIONINTERFACE_H
#define SIM_TRANSMISSIONINTERFACE_H

#include <systemc.h>
#include <vector>

using address_t = unsigned int;
using image_t = std::vector<unsigned int>;
using control_t = std::vector<int>;

class TransmissionInterface : virtual public sc_interface {
public:
    virtual void transmitImage(address_t src, address_t dst, control_t control, image_t *data) = 0;
};

#endif //SIM_TRANSMISSIONINTERFACE_H
