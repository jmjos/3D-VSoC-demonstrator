//
// Created by mtzschoppe on 24/05/19.
//

#ifndef RAWTOCV_CONFIG_H
#define RAWTOCV_CONFIG_H

// Sensor -> n ADC's -> crop image in n rows and m columns
class ADC {
    private:
        ADC(); //Private constructor to prevent instancing

    public:
        static ADC* getInstance(); //Static access method
        static const int nb_ADCs_x = 4;
        static const int nb_ADCs_y = 4;
        static const int nb_CPUs_x = 2;
        static const int nb_CPUs_y = 2;

        //maximum object size for Vioala-Jones algorith
        //link: detection_based_tracker.hpp -> IDetector::maxObjSize(INT_MAX, INT_MAX)
        static const int MAX_OBJECT_SIZE_x = 50;
        static const int MAX_OBJECT_SIZE_y = 50;
};

#endif //RAWTOCV_CONFIG_H
