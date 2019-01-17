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

image_t* ImageFactory::createImage(std::string file)
{
    iProcessor.open_file(file.c_str());
    iProcessor.unpack();
    iProcessor.raw2image();
    auto image = new image_t();
    for(int i = 0; i < 9; i++) {//TODO iProcessor.imgdata.sizes.iwidth
        for (int j = 0; j < 9; j++) {//TODO iProcessor.imgdata.sizes.iheight
            image->push_back(iProcessor.imgdata.image[i][j]);
            //cout << "Value " << i << "," << j << ": " << iProcessor.imgdata.image[i][j] << endl;
        }
    }
    iProcessor.recycle();
    images.push_back(image);
    return image;
}

void ImageFactory::deleteImage(image_t *image)
{
    auto it = std::find(images.begin(), images.end(), image);
    delete(*it);
    images.erase(it);
}