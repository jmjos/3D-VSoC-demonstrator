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

ImageData* ImageFactory::createImage(ImageData* image)
{
    images.push_back(image);
    return image;
}

void ImageFactory::deleteImage(ImageData *image)
{
    auto it = std::find(images.begin(), images.end(), image);
    delete(*it);
    images.erase(it);
}