//
// Created by mtzschoppe on 24/05/19.
//

#ifndef RAWTOCV_CONFIG_H
#define RAWTOCV_CONFIG_H
// Sensor -> n ADC's -> crop image in n rows and m columns

class ADC {
public:
    static ADC& getInstance();
    int nb_ADCs_x;
    int nb_ADCs_y;

};

#endif //RAWTOCV_CONFIG_H
