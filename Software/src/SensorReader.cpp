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
        if (iProcessor.open_file(file.c_str()) != LIBRAW_SUCCESS)
        {
            fprintf(stderr, "Cannot open %s: %s\n", file.c_str(), libraw_strerror(iProcessor.open_file(file.c_str())));
        }
        iProcessor.unpack();
        iProcessor.raw2image();
        auto imageData = new ImageData();
        for(int i = 0; i < global.imageWidth; i++) {//TODO iProcessor.imgdata.sizes.iwidth
            for (int j = 0; j < global.imageHeigth; j++) {//TODO iProcessor.imgdata.sizes.iheight
                imageData->image->push_back(iProcessor.imgdata.image[i][j]);
            }
        }
        for (int i = 0 ; i < global.imageWidth * global.imageHeigth ; i++){
            imageData->imageRaw->push_back(iProcessor.imgdata.rawdata.raw_image[i]);
        }
        iProcessor.recycle();
        imageFactory.createImage(imageData);

        if (global.VERBOSE_SEND || global.VERBOSE_SEND_SENSOR)
            cout << "SensorReader sends data at " << sc_time_stamp() << endl;
        for (address_t dst = 0; dst <=8; dst++){
            address_t src = global.sensorReaderAddr;
            int ADCwidth = global.imageWidth/3;
            int ADCheigth = global.imageHeigth/3;
            int xStart = (dst%3) * ADCwidth;
            int xEnd = xStart + ADCwidth;
            int yStart = (dst/3) * ADCheigth;
            int yEnd = yStart + ADCheigth;
            control_t control = {xStart, yStart, xEnd, yEnd};
            imagePort[dst]->transmitImage(src, dst, control, imageData);
        }
        wait(1/(float) global.framerate, SC_SEC);
    }
}