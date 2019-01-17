//
// Created by joseph on 14/12/18.
//

#include "ADC.h"

ADC::ADC(const sc_module_name &nm, address_t addr) : addr(addr) {

    SC_THREAD(process);

}

void ADC::process() {
    for (int i = 0; i < INT_MAX; ++i) {
        wait(sendEv);
        cout << "ADC can now send." << endl;

        //send middle data to counterpart
        image_t image = {2, 2};

        // control data structure containing the start and end adresses within the image
        int xStart = 0;
        int yStart = 1;
        int xEnd = 1;
        int yEnd = 2;
        control_t control = {xStart, yStart, xEnd, yEnd};

        imagePort->transmitImage(addr, addr+9, control, &image);
        //send edge data


    }
}

void ADC::transmitImage(address_t src, address_t dst, control_t control, image_t *data){
    assert(addr==dst);
    cout << "ADC " << addr << " recieved data to " << dst << " at " << sc_time_stamp() << endl;
    cout << "ADC can read image data: " << data->at(0) << endl;
    sendEv.notify(SC_ZERO_TIME);
};
