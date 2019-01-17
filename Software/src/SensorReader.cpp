//
// Created by joseph on 1/14/19.
//

#include "SensorReader.h"

SensorReader::SensorReader(const sc_module_name &nm) {
    SC_THREAD(process);
}

void SensorReader::process() {
    for (auto i=0;i<INT_MAX; i++){

        cout << "SensorReader sends data at " << sc_time_stamp() << endl;
        for (address_t dst = 0; dst <=8; dst++){
            address_t src = 999;

            auto image = imageFactory.createImage("pikes-peak.nef"); //TODO implement correct image reader
            // control data structure containing the start and end adresses within the image
            // TODO get correct image-positions.
            int xStart = 0;
            int yStart = 1;
            int xEnd = 1;
            int yEnd = 2;
            control_t control = {xStart, yStart, xEnd, yEnd};

            imagePort[dst]->transmitImage(src, dst, control, image);
        }
        wait(1/(float) global.framerate, SC_SEC);
    }
}