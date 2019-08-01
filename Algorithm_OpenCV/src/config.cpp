//
// Created by mtzschoppe on 01/08/19.
//

#include "config.h"

Sensor* Sensor::getInstance() //only one instance of ADC
{
    static Sensor instance;
    return &instance;
}

Sensor::Sensor() {}