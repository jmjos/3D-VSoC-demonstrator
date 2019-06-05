#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <string>
#include <factory.h>
#include <structures.h>
#include <config.h>
#include <detection.h>
#include <math.h>

#include "libraw/libraw.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp> // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

const int detection_loop = 100;//how often
const int MAX_COUNT = 100;
const int max_img = 100;
const int min_points = 35;

//path to images
//const string path_key[2] = {"/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/RAWImages/example_",".dng"};
//const string path_key[2] = {"/home/mtzschoppe/Desktop/face images/3/filename000",".png"}; //zweistellige Zahlen
const string path_key[2] = {"/home/mtzschoppe/Desktop/face images/4/filename00",".png"}; //dreistellige Zahlen

int main(int arg_num, char *arg_vec[]) {
    //variable for image read
    unsigned long number_images; //8 oder 28 oder 506
    int first_image = 399; //0 oder 48 oder 449 oder 10
    int detect_img = 1;
    string file;
    string string_k;

    LibRaw iProcessor;
    namedWindow("LK Demo", cv::WINDOW_NORMAL);

    //intialization for tracking
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);
    Mat gray, prevGray, img, grey_b;
    int* Faces_height;
    int* Faces_width;
    int* Faces_x;
    int* Faces_y;
    int facepoints;
    bool tracking_end = false;

    //get maximum of images to read
    vector<string> vec_string;
    int r=0;
    int first_image_new = first_image;
    for(;;){
        stringstream ss;
        ss << first_image_new;
        string_k = ss.str();
        //get file path
        file = path_key[0] + string_k + path_key[1];
        vec_string.push_back(file);

        if(imread(vec_string[r],1).empty()){
            break;
        }

        first_image_new++;
        r++;
    }
    number_images=vec_string.size() - 3 + first_image;

    std::string cascadeFrontalfilename = samples::findFile("/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/opencv/opencv/data/lbpcascades/lbpcascade_frontalface.xml");

    cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);

    if ( cascade->empty() )
    {
        printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
        return 2;
    }

    cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);

    if ( cascade->empty() )
    {
        printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
        return 2;
    }

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    int img_x, img_y;
    Mat ADCs [ADC::nb_ADCs_x][ADC::nb_ADCs_y];
    Mat CPUs [ADC::nb_CPUs_x][ADC::nb_CPUs_y];

    do {
        vector<Rect> Faces;
        facepoints = 0;

        for (int v=0; v < detect_img; v++) {
            //end of tracking
            if (first_image > number_images) {
                tracking_end = true;
                break;
            }

            //read image
            //path to image
            //convert int k to string
            stringstream ss;
            ss << first_image;
            string_k = ss.str();
            //get file path
            file = path_key[0] + string_k + path_key[1];

            img = imread(file,1);

            img_x = img.size().width;
            img_y = img.size().height;

            //divide imgage to ADCs
            for (int i=0; i<ADC::nb_ADCs_x; i++) {
                for (int j=0; j<ADC::nb_ADCs_y; j++) {
                    cv::Mat crop = img(Rect(i * ceil(img_x / ADC::nb_ADCs_x), j * ceil(img_y / ADC::nb_ADCs_y),
                            ceil(img_x / ADC::nb_ADCs_x), ceil(img_y / ADC::nb_ADCs_y)));
                    ADCs [i][j] = crop;
                }
            }

            //divide imgage to CPUs
            for (int i=0; i<ADC::nb_CPUs_x; i++) {
                for (int j=0; j<ADC::nb_CPUs_y; j++) {
                    cv::Mat crop = img(Rect(i * ceil(img_x / ADC::nb_CPUs_x), j * ceil(img_y / ADC::nb_CPUs_y),
                                            ceil(img_x / ADC::nb_CPUs_x), ceil(img_y / ADC::nb_CPUs_y)));
                    CPUs [i][j] = crop;
                }
            }

            imshow("LK Demo", ADCs [1][3]);
            waitKey(0);
            imshow("LK Demo", CPUs [1][0]);
            waitKey(0);

            /*
            //TODO LibRaw
            if (iProcessor.open_file(file.c_str()) != LIBRAW_SUCCESS) {
                fprintf(stderr, "Cannot open %s: %s\n", file.c_str(), libraw_strerror(iProcessor.open_file(file.c_str())));
            }
            iProcessor.unpack();
            iProcessor.dcraw_process();

            int ret = 0;
            libraw_processed_image_t *image = iProcessor.dcraw_make_mem_image(&ret);

            auto img = cv::Mat(image->height, image->width, CV_16UC3); //CV_16UC3

            //create image from raw to colour
            for (int i = 0; i < image->height; i++) {
                for (int j = 0; j < image->width; j++) {
                    int linindex = (i * image->width + j)*3;
                    cv::Vec3s tripel = cv::Vec3s((short)image->data[linindex+2]*256, (short)image->data[linindex +1]*256, (short)image->data[linindex ]*256);
                    img.at<cv::Vec3s>(i, j) = tripel;
                }
            }
            */

            //maybe with do-while
            /*
            do {
            } while (Faces.size() == 0);
            */
            for (int g=0; g<detection_loop; g++) {
                cvtColor(img, gray, COLOR_BGR2GRAY);
                Detector.process(gray);
                Detector.getObjects(Faces);

                for (size_t i = 0; i < Faces.size(); i++) {
                    rectangle(img, Faces[i], Scalar(255, 255, 0));
                }
            }

            if (Faces.size() == 0) {
                detect_img++;
            }

            if (detect_img > 20) {
                break;
            }
        }

        //cout << "image detected: " << detect_img << endl;

        if (Faces.size() == 0) {
            cerr << "Cannot detect face" << endl;
            first_image = detect_img + first_image;
            continue;
        } else {
            Faces_width = new int [Faces.size()] ();
            Faces_height = new int [Faces.size()] ();
            Faces_x = new int [Faces.size()] ();
            Faces_y = new int [Faces.size()] ();

            cout << "faces detected: " << Faces.size()  << endl;

            for (int p=0; p<Faces.size(); p++) {
                Faces_height[p] = Faces[p].height;
                Faces_width[p] = Faces[p].width;
                Faces_x[p] = Faces[p].x;
                Faces_y[p] = Faces[p].y;
                //cout << "face " << p+1 << " data:\nheight: " << Faces_height[p] << "\twidth: " << Faces_width[p] << "\n x: " << Faces_x[p] << "\ty: " << Faces_y[p] << endl;
            }

            //cout << "image size: " << img.size() << endl;
            imshow("LK Demo", img);
            waitKey(500);
        }

        //initialization for-loop tracking
        int m = detect_img + first_image;
        int count_img = 0;
        first_image = 0;
        char c;
        vector<Point2f> points[Faces.size()*2];

        for(;;) {
            //read image:
            //path to image
            //convert int k to string
            stringstream ss;
            ss << m;
            string_k = ss.str();
            //get file path
            file = path_key[0] + string_k + path_key[1];

            img = imread(file, 1);

            /*
            //TODO LibRaw
            if (iProcessor.open_file(file.c_str()) != LIBRAW_SUCCESS) {
                fprintf(stderr, "Cannot open %s: %s\n", file.c_str(), libraw_strerror(iProcessor.open_file(file.c_str())));
            }
            iProcessor.unpack();
            iProcessor.dcraw_process();

            int ret = 0;
            libraw_processed_image_t *image = iProcessor.dcraw_make_mem_image(&ret);

            auto img = cv::Mat(image->height, image->width, CV_16UC3); //CV_16UC3

            //create image from raw to colour
            for (int i = 0; i < image->height; i++) {
                for (int j = 0; j < image->width; j++) {
                    int linindex = (i * image->width + j)*3;
                    cv::Vec3s tripel = cv::Vec3s((short)image->data[linindex+2]*256, (short)image->data[linindex +1]*256, (short)image->data[linindex ]*256);
                    img.at<cv::Vec3s>(i, j) = tripel;
                }
            }
            */

            //tracking
            cvtColor(img, gray, COLOR_BGR2GRAY);

            for (int q = 0; q < (Faces.size()); q++) {
                //create ROI with detected face
                Mat mask = Mat::zeros(img.size(), CV_8UC1);
                Mat roi(mask, cv::Rect(Faces_x[q], Faces_y[q], Faces_width[q], Faces_height[q]));
                roi = Scalar(255, 255, 255);

                // automatic initialization
                goodFeaturesToTrack(gray, points[q * 2 + 1], MAX_COUNT, 0.01, 10, mask, 3, 3, 0, 0.04);
                cornerSubPix(gray, points[q * 2 + 1], subPixWinSize, Size(-1, -1), termcrit);

                facepoints = points[q*2+1].size();

                if (!points[q * 2].empty()) {
                    vector<uchar> status;
                    vector<float> err;
                    if (prevGray.empty())
                        gray.copyTo(prevGray);
                    calcOpticalFlowPyrLK(prevGray, gray, points[q * 2], points[q * 2 + 1], status, err, winSize, 3,
                                         termcrit, 0, 0.001);
                    size_t i, k;
                    for (i = k = 0; i < points[q * 2 + 1].size(); i++) {
                        if (!status[i])
                            continue;
                        points[q * 2 + 1][k++] = points[q * 2 + 1][i];
                        circle(img, points[q * 2 + 1][i], 3, Scalar(0, 255, 0), -1, 8);
                    }
                    points[q * 2 + 1].resize(k);
                }

                imshow("LK Demo", img);
                c = (char) waitKey(15);
            }

            if (c == 27) {
                break;
            }

            for (int i = 0; i < Faces.size(); i++) {
                std::swap(points[i * 2 + 1], points[i * 2]);
            }
            cv::swap(prevGray, gray);

            //rerun if facepoints lost or too many images used
            if (facepoints <= min_points || count_img >= max_img) {
                break;
            }

            //end of tracking
            if (m > number_images) {
                tracking_end = true;
                cout << "maximum of images reached" << endl;
                break;
            }
            m++;
            count_img++;
        }

        first_image = first_image +  m;
        waitKey(5);
    } while (!tracking_end);

    Detector.stop();
    iProcessor.recycle();

    return 0;
}

PacketFactory& PacketFactory::getInstance()
{
    static PacketFactory instance;
    return instance;
}

nt_packet_t* PacketFactory::createPacket(id_t id)
{
    /*
    auto p = new Packet(id);
    packets.push_back(p);
    return p;
     */
}

void PacketFactory::deletePacket(nt_packet_t* p)
{
    auto it = std::find(packets.begin(), packets.end(), p);
    delete(*it);
    packets.erase(it);
}

ADC& ADC::getInstance()
{
    static ADC instance;
    return instance;
}
