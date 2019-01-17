//
// Created by joseph on 1/14/19.
//

#ifndef SIM_GLOBALCONFIGURATION_H
#define SIM_GLOBALCONFIGURATION_H

#include "TransmissionInterface.h"

class GlobalConfiguration {
public:
    static GlobalConfiguration& getInstance()
    {
        static GlobalConfiguration instance;
        return instance;
    }

    address_t SensorReaderAddr = 999;
    const int framerate = 3;
    std::string file;
    int imageWidth = 9;
    int imageHeigth = 9;

private:
    GlobalConfiguration() {}

public:
    GlobalConfiguration(GlobalConfiguration const&) = delete;
    void operator=(GlobalConfiguration const&) = delete;

};


#endif //SIM_GLOBALCONFIGURATION_H
