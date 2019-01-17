//
// Created by joseph on 1/16/19.
//

#include "ImageFactory.h"

using namespace std;

ImageFactory& ImageFactory::getInstance()
{
    static ImageFactory instance;
    return instance;
}

image_t* ImageFactory::createImage(image_t* image)
{
    images.push_back(image);
    return image;
}

void ImageFactory::deleteImage(image_t *image)
{
    auto it = std::find(images.begin(), images.end(), image);
    delete(*it);
    images.erase(it);
}