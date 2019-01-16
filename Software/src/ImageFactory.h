//
// Created by joseph on 1/16/19.
//

#ifndef SIM_IMAGEFACTORY_H
#define SIM_IMAGEFACTORY_H

#include "GlobalConfiguration.h"
#include "TransmissionInterface.h"

class ImageFactory {
public:
    std::vector<image_t*> images;
    static ImageFactory& getInstance();
    image_t* createImage();
    void deleteImage(image_t *image);
};


#endif //SIM_IMAGEFACTORY_H
