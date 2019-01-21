//
// Created by joseph on 14/12/18.
//

#include "Interconnect.h"

Interconnect::Interconnect(const sc_module_name &nm) : sc_module(nm){

}

void Interconnect::transmitImage(address_t src, address_t dst, control_t control, ImageData *data){
    assert(dst<=componentCount-1);
    //TODO write trace file
    ports[dst]->transmitImage(src, dst, control, data);
}