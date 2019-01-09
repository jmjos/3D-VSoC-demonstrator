//
// Created by joseph on 14/12/18.
//

#include "ADC.h"

ADC::ADC(const sc_module_name &nm, address_t addr) : addr(addr) {

    SC_THREAD(process);

}

void ADC::process() {
    while(1){
        cout << "ADC sends data at " << sc_time_stamp() << endl;

        std::vector<control_t > control = {1,2};
        std::vector<image_t > image = {2,2};

        image_port->transmitImage(0, control, &image);

        wait(1/(double)framerate, SC_SEC);
    }
}
