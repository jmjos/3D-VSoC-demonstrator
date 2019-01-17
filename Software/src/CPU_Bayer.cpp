//
// Created by joseph on 14/12/18.
//

#include "CPU_Bayer.h"
#include "GlobalConfiguration.h"

CPU_Bayer::CPU_Bayer(const sc_module_name &nm, address_t addr) : sc_module(nm), addr(addr){
    imageData.resize(xSize*ySize);

    SC_THREAD(sendProcess)
}

void CPU_Bayer::transmitImage(address_t src, address_t dst, control_t control, image_t *data) {
    assert(addr == dst);
    if (global.VERBOSE_RECEIVE || global.VERBOSE_RECEIVE_BAYER)
    cout << " Bayer CPU " << addr << " recieved data at " << sc_time_stamp() << endl;
    //assert(!validData[control[0]]);
    /*int xStart = 0;
    int xLength = 1;
    int yStart = 0;
    int yLength = 1;
    if (control[1] == 0){

    }
    int i = 0;
    for (int x = xStart; x <= xLength ; x++){
        for (int y = yStart; y <= yLength ; y++){
            //imageData[y*xSize+x] = data[i++];
        }
    }

    validData[control[0]] = true;
    sendEv.notify();*/
}

void CPU_Bayer::sendProcess(){
    while(1) {
        wait(sendEv);
        if (global.VERBOSE_SEND || global.VERBOSE_SEND_BAYER)
            cout << "the event was triggered correctly at" << sc_time_stamp() << endl;

        //check if all data are availabe. send data.
    }
}