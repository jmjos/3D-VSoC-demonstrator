//
// Created by joseph on 14/12/18.
//

#include "CPU_Bayer.h"

CPU_Bayer::CPU_Bayer(const sc_module_name &nm, address_t addr) : sc_module(nm), addr(addr){

}

void CPU_Bayer::transmitImage(address_t dst, std::vector<control_t> control, std::vector<image_t> *data){
}