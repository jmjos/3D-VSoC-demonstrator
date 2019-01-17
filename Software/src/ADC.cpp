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

        int ADCwidth = global.imageWidth/3;
        int ADCheigth = global.imageHeigth/3;
        //int xStart = (addr%3) * ADCwidth;
        //int xEnd = xStart + ADCwidth;
        //int yStart = (addr/3) * ADCheigth;
        //int yEnd = yStart + ADCheigth;
        address_t addr_x = addr%3;
        address_t addr_y = addr/3;
        for (address_t dst_x = addr_x - 1 ; dst_x <= addr_x + 1 ; dst_x++){
            for (address_t dst_y = addr_y - 1 ; dst_y <= addr_y + 1 ; dst_y++){
                if (dst_x >= 0 && dst_y >= 0 && dst_x <= 3 && dst_y <= 3){
                    //calculate starting positions
                    int xStart = (dst_x) * ADCwidth;
                    int xEnd = xStart + ADCwidth;
                    int yStart = (dst_y) * ADCheigth;
                    int yEnd = yStart + ADCheigth;
                    address_t dst_addr = dst_x * 3 + dst_y;
                    control_t control = {xStart, yStart, xEnd, yEnd};
                    if (global.VERBOSE_SEND || global.VERBOSE_SEND_ADC)
                        cout << "ADC " << addr << " sends to " << dst_addr << " @ " << sc_time_stamp() << endl;
                    //imagePort->transmitImage(addr, addr+9, control, image);
                }
            }
        }
    }
}

void ADC::transmitImage(address_t src, address_t dst, control_t control, image_t *data){
    assert(addr==dst);
    if (global.VERBOSE_RECEIVE || global.VERBOSE_RECEIVE_ADC)
        cout << "ADC " << addr << " recieved data to " << dst << " at " << sc_time_stamp() << endl;
    image = data;
    sendEv.notify(SC_ZERO_TIME);
};
