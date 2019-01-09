//
// Created by joseph on 14/12/18.
//

#include "Interconnect.h"

Interconnect::Interconnect(const sc_module_name &nm) : sc_module(nm){

}

void Interconnect::transmitImage(address_t dst, std::vector<control_t> control, std::vector<image_t> *data){
    assert(dst<=componentCount);
    ports[dst]->transmitImage(dst, control, data);
}