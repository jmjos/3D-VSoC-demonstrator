//
// Created by joseph on 1/16/19.
//

#include "ImageFactory.h"


ImageFactory& ImageFactory::getInstance()
{
    static ImageFactory instance;
    return instance;
}

image_t* ImageFactory::createImage() // TODO get file location
{
    // TODO read image from given file
    // generate a image_t and copy image data into this data structre
    // use new to have pionter on heap, not stack!-
    auto image = new image_t ({1,2});
    images.push_back(image);
    return image;
}

void ImageFactory::deleteImage(image_t *image)
{
    auto it = std::find(images.begin(), images.end(), image);
    delete(*it);
    images.erase(it);
}