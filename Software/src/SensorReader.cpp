//
// Created by joseph on 1/14/19.
//

#include "SensorReader.h"

SensorReader::SensorReader(const sc_module_name &nm) {
    SC_THREAD(process);
}

void SensorReader::process() {
    for (auto i=0;i<INT_MAX; i++){

        std::string file = "pikes-peak.nef";
        iProcessor.open_file(file.c_str());
        iProcessor.unpack();
        iProcessor.raw2image();
        auto image = new image_t();
        for(int i = 0; i < global.imageWidth; i++) {//TODO iProcessor.imgdata.sizes.iwidth
            for (int j = 0; j < global.imageHeigth; j++) {//TODO iProcessor.imgdata.sizes.iheight
                image->push_back(iProcessor.imgdata.image[i][j]);
            }
        }
        iProcessor.recycle();
        imageFactory.createImage(image);


        cout << "SensorReader sends data at " << sc_time_stamp() << endl;
        for (address_t dst = 0; dst <=8; dst++){
            address_t src = 999;

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