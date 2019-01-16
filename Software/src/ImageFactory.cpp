//
// Created by joseph on 1/16/19.
//

#include "ImageFactory.h"


ImageFactory& ImageFactory::getInstance()
{
    static ImageFactory instance;
    return instance;
}

image_t* ImageFactory::createPacket()
{
    image_t image = {1, 2};
    images.push_back(&image);
    return &image;
}

void ImageFactory::deletePacket(image_t* image)
{
    auto it = std::find(images.begin(), images.end(), image);
    delete(*it);
    images.erase(it);
}