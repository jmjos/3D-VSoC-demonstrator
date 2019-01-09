//
// Created by joseph on 14/12/18.
//

#ifndef SIM_CPU_1_H
#define SIM_CPU_1_H

#include "TransmissionInterface.h"

class CPU_Bayer : public sc_module, public TransmissionInterface {
private:
    address_t addr = 0;
    std::vector<image_t> center;
    std::vector<image_t> left;
    std::vector<image_t> right;
    std::vector<image_t> top;
    std::vector<image_t> down;
    std::vector<image_t> left_up;
    std::vector<image_t> right_up;
    std::vector<image_t> left_down;
    std::vector<image_t> right_down;

    void proc(); // this is the actual process, which executes the image processing

public:

    SC_HAS_PROCESS(CPU_Bayer);
    CPU_Bayer(const sc_module_name &nm, address_t addr);

    virtual void transmitImage(address_t dst, std::vector<control_t> control, std::vector<image_t> *data);
};


#endif //SIM_CPU_1_H
