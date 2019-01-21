//
// Created by joseph on 21.01.19.
//

#include "ImageData.h"

ImageData::ImageData() {
    image = new image_t();
    imageRaw = new image_t();
}

ImageData::~ImageData() {
    delete(image);
    delete(imageRaw);
}
