//
// Created by joseph on 1/16/19.
//

#ifndef SIM_IMAGEFACTORY_H
#define SIM_IMAGEFACTORY_H

#include "GlobalConfiguration.h"
#include "TransmissionInterface.h"
#include "ImageData.h"

class ImageFactory {
public:
    std::vector<ImageData*> images;
    static ImageFactory& getInstance();
    ImageData* createImage(ImageData*);
    void deleteImage(ImageData *image);
};


#endif //SIM_IMAGEFACTORY_H
