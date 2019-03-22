#include <fstream>
#include <iostream>

#include "libraw/libraw.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

int main(int arg_num, char *arg_vec[]) {

    LibRaw iProcessor;

    std::string file = "pikes-peak.nef";
    if (iProcessor.open_file(file.c_str()) != LIBRAW_SUCCESS) {
        fprintf(stderr, "Cannot open %s: %s\n", file.c_str(), libraw_strerror(iProcessor.open_file(file.c_str())));
    }
    iProcessor.unpack();
    iProcessor.dcraw_process();

    int ret = 0;
    libraw_processed_image_t *image = iProcessor.dcraw_make_mem_image(&ret);

    auto img = cv::Mat(image->height, image->width, CV_16UC3);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int linindex = (i * image->width + j)*3;
            cv::Vec3s tripel = cv::Vec3s((short)image->data[linindex+2]*256, (short)image->data[linindex +1]*256, (short)image->data[linindex ]*256);
            img.at<cv::Vec3s>(i, j) = tripel;
        }
    }


    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display window", img);
    cv::waitKey(0);

    iProcessor.recycle();

    return 0;
}

//Weitere Ideen
/*
 *  //Variante 2
    auto
        img1 = cv::Mat(H, B, CV_16UC3);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < B; j++) {
            int linindex = (j * H + i);
            cv::Vec4s quadrupel = cv::Vec4s(iProcessor.imgdata.image[linindex][0], (iProcessor.imgdata.image[linindex][1]), iProcessor.imgdata.image[linindex][2], iProcessor.imgdata.image[linindex][3]);
            cv::Vec3s tripel;
            if (remainder(i, 2))//ungerade Zeile
                if (remainder(j, 2))//ungerade Spalte
                    tripel=cv::Vec3s(quadrupel[2], quadrupel[1], quadrupel[0]);
                else//gerade Spalte
                    tripel=cv::Vec3s(quadrupel[2], (quadrupel[1]+quadrupel[3])/2, quadrupel[0]);
            else//gerade Zeile
                tripel=cv::Vec3s(quadrupel[2], quadrupel[1], quadrupel[0]);
            img1.at<cv::Vec3s>(H-i-1,j) = tripel;
        }
    }
 */

/*
 * 2. grün ignorierend und mit Gamma-Verzerrung

     auto
         img1 = cv::Mat(H, B, CV_16UC3);
     for (int i = 0; i < H; i++) {
         for (int j = 0; j < B; j++) {
             int linindex = (j * H + i);
             cv::Vec4i quadrupel =
cv::Vec4i(iProcessor.imgdata.image[linindex][0],
(iProcessor.imgdata.image[linindex][1]),
(ushort)iProcessor.imgdata.image[linindex][2],
iProcessor.imgdata.image[linindex][3]);
             cv::Vec3i tripel;
             //if (remainder(i, 2))//ungerade Zeile
             //    if (remainder(j, 2))//ungerade Spalte
             //        tripel=cv::Vec3i(quadrupel[2], quadrupel[1],
quadrupel[0]);
             //    else//gerade Spalte
             //        tripel=cv::Vec3i(quadrupel[2],
(quadrupel[1]+quadrupel[3])/2, quadrupel[0]);
             //else//gerade Zeile
                 tripel=cv::Vec3i(quadrupel[2], quadrupel[1], quadrupel[0]);
             for (int c = 0; c < 3; c++){//Gamma-Expansion
                 double test = (double)(tripel[c]);
                 tripel[c] = sqrt((double)(tripel[c]))*sqrt(0xFFFF);
             }
             //img1.at<cv::Vec3s>(H-i-1,j) = tripel;
             img1.at<cv::Vec3s>(H - i - 1, j) = tripel;
             //img1[H - i - 1][j] = tripel;
         }
     }
 */