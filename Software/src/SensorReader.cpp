//
// Created by joseph on 1/14/19.
//

#include "SensorReader.h"

SensorReader::SensorReader(const sc_module_name &nm) {
    SC_THREAD(process);
}

void SensorReader::process() {
    for (;;){

        cout << "SensorReader sends data at " << sc_time_stamp() << endl;
        //TODO read image data

        //TODO allocate memory for image?

        //TODO send image data to all individual ADCs
        for (address_t dst = 0; dst <=8; dst++){
            address_t src = 999;

            std::vector<image_t> image = {2, 2};
            // control: first field: position. second field: length of data
            std::vector<control_t> control = {1, 2};

            imagePort[dst]->transmitImage(src, dst, control, &image);
            //imagePort->transmitImage(src, dst,)
        }

        wait(1/(float) global.framerate, SC_SEC);

    }
}