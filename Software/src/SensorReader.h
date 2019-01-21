//
// Created by joseph on 1/14/19.
//

#ifndef SIM_SENSORREADER_H
#define SIM_SENSORREADER_H

#include <systemc.h>
#include <fstream>
#include <iostream>

#include "libraw/libraw.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "TransmissionInterface.h"
#include "GlobalConfiguration.h"
#include "ImageFactory.h"

class SensorReader : public sc_module {
private:
    int currentFrame = 0;
    GlobalConfiguration &global = GlobalConfiguration::getInstance();
    ImageFactory &imageFactory = ImageFactory::getInstance();
    LibRaw iProcessor;
public:
    sc_port<TransmissionInterface, 9> imagePort[9];

    SC_HAS_PROCESS(SensorReader);
    SensorReader(const sc_module_name &nm);

    void process();

};


#endif //SIM_SENSORREADER_H
