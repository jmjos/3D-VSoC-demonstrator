//
// Created by joseph on 14/12/18.
//

#include "ADC.h"

ADC::ADC(const sc_module_name &nm, address_t addr) : addr(addr) {

    SC_THREAD(process);

}

void ADC::process() {
    while (1){
        wait(sendEv);
        cout << "ADC can now send." << endl;

        //send middle data to counterpart
        std::vector<image_t> image = {2, 2};
        // control: first field: position. second field: length of data
        std::vector<control_t> control = {1, 2};

        imagePort->transmitImage(addr, addr+9, control, &image);
        //send edge data


    }
}

void ADC::transmitImage(address_t src, address_t dst, std::vector<control_t> control, std::vector<image_t> *data){
    assert(addr==dst);
    cout << "ADC " << addr << " recieved data to " << dst << " at " << sc_time_stamp() << endl;
    sendEv.notify(SC_ZERO_TIME);

};
