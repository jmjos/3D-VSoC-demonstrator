//
// Created by joseph on 14/12/18.
//

#include "ADC.h"

ADC::ADC(const sc_module_name &nm, address_t addr) : addr(addr) {

    SC_THREAD(process);

}

void ADC::process() {
    while(1){
        if (addr == 0) {
            cout << "ADC " << addr << " sends data at " << sc_time_stamp() << endl;

            std::vector<control_t> control = {1, 2};
            std::vector<image_t> image = {2, 2};
            address_t dest = 1;

            imagePort->transmitImage(addr, 9, control, &image);
        }
        wait(1 / (double) framerate, SC_SEC);
    }
}

void ADC::transmitImage(address_t src, address_t dst, std::vector<control_t> control, std::vector<image_t> *data){
    assert(addr==dst);
    cout << "ADC " << addr << " recieved data at " << sc_time_stamp() << endl;
};
