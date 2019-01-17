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

        // TODO for loop iterating all regions and sending them to the correct destination. no processing required, these are still the bayer coded images.

        int ADCwidth = global.imageWidth/3;
        int ADCheigth = global.imageHeigth/3;
        int xStart = (addr%3) * ADCwidth;
        int xEnd = xStart + ADCwidth;
        int yStart = (addr/3) * ADCheigth;
        int yEnd = yStart + ADCheigth;
        control_t control = {xStart, yStart, xEnd, yEnd};

        //imagePort->transmitImage(addr, addr+9, control, &image);
    }
}

void ADC::transmitImage(address_t src, address_t dst, control_t control, image_t *data){
    assert(addr==dst);
    cout << "ADC " << addr << " recieved data to " << dst << " at " << sc_time_stamp() << endl;
    cout << "ADC can read image data: " << data->at(0) << endl;
    sendEv.notify(SC_ZERO_TIME);
};
