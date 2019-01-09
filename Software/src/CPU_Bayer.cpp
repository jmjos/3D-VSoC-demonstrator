//
// Created by joseph on 14/12/18.
//

#include "CPU_Bayer.h"

CPU_Bayer::CPU_Bayer(const sc_module_name &nm, address_t addr) : sc_module(nm), addr(addr){

}

void CPU_Bayer::transmitImage(address_t src, address_t dst, std::vector<control_t> control, std::vector<image_t> *data){
    assert(addr == dst);
    cout << " Bayer CPU " << addr << " recieved data at " << sc_time_stamp() << endl;
    switch (control[1]){
        case 0:
            assert (!leftUpValid)
            left_up = data; //memcopy!!!
            leftUpValid = true;
            break;
    }
}