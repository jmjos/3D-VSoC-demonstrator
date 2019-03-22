//
// Created by joseph on 21.01.19.
//

#ifndef SIM_IMAGEDATA_H
#define SIM_IMAGEDATA_H

#include <vector>

using image_t = std::vector<unsigned int>;

class ImageData {
public:
    image_t* image;
    image_t* imageRaw;

    ImageData();
    virtual ~ImageData();
};


#endif //SIM_IMAGEDATA_H
