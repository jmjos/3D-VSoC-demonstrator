//
// Created by joseph on 1/16/19.
//

#ifndef SIM_IMAGEFACTORY_H
#define SIM_IMAGEFACTORY_H

#include <fstream>
#include <iostream>

#include "libraw/libraw.h"

#include "GlobalConfiguration.h"
#include "TransmissionInterface.h"



class ImageFactory {
public:
    std::vector<image_t*> images;
    static ImageFactory& getInstance();
    image_t* createImage(std::string);
    void deleteImage(image_t *image);

private:
    LibRaw iProcessor;
};


#endif //SIM_IMAGEFACTORY_H
