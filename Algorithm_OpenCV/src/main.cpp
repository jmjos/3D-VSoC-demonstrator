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

const int detection_loop = 30;//number of loops for each face detection
const int MAX_COUNT = 100; //maximum number of feature points per face
const int max_img = 50; //number of images until new tracking
const int min_points = 35; //minimum number of points for tracking a face

//path to images
//const string path_key[2] = {"/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/RAWImages"
//                            "/example_",".dng"};
//const string path_key[2] = {"/home/mtzschoppe/Desktop/face images/3/filename000",".png"}; //zweistellige Zahlen
const string path_key[2] = {"/home/mtzschoppe/Desktop/face images/4/filename00",".png"}; //dreistellige Zahlen

int main(int arg_num, char *arg_vec[]) {
    //variable for image read
    unsigned long number_images;
    int first_image = 399; //0 oder 48 oder 449 oder 10
    int detect_img = 2; //maximum detect attempts for one image
    string file;
    string string_k;

    LibRaw iProcessor; //raw to png
    namedWindow("LK Demo", cv::WINDOW_NORMAL);

    ///intialization for tracking
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);
    Mat gray, prevGray, img, grey_b;
    int* Faces_height;
    int* Faces_width;
    int* Faces_x;
    int* Faces_y;
    int facepoints;
    bool tracking_end = false;

    ///data transfer
/*    Packet p2;
    Packet p3;
    Packet p4;
    Packet p1;
    Packet p_ADC;
    Packet p_CPU;
    Packet newP;
    p1.addr_src = 100;
    p1.dst = 99;
    p1.data.emplace_back(0b011110);
    cout << "p1:\t" << p1 << endl;

    for (auto i : p1.data){
        cout << "p1 data:\t" << i << endl;
    }

    std::ofstream ofs("filename"); //output -> store p1 to "filename"
    {
        boost::archive::text_oarchive oa(ofs);
        oa << p1;
    }

    std::ifstream ifs("filename"); //input -> from "filename" to newP
    {
        boost::archive::text_iarchive ia(ifs);
        ia >> newP;
    }

    cout << "newP:\t" << newP << endl;

    for (auto i : newP.data){
        cout << "newP data:\t"<< i << endl;
    }*/

    ///get maximum of images to read
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
    cout << number_images-first_image << " images to read, start with: " <<  path_key[0] << first_image <<  path_key[1] << " and end with: " << path_key[0] << number_images <<  path_key[1] << endl;


    std::string cascadeFrontalfilename = samples::findFile("/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator"
                                                           "/Algorithm_OpenCV/opencv/opencv/data/lbpcascades"
                                                           "/lbpcascade_frontalface.xml"); //get objects for face detection

    cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename); //perfect forwarding
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade); //manage correct copy from cascadeFrontalfilename to IDetector

    if ( cascade->empty() )
    {
        printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
        return 2;
    }

    cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename); //perfect forwarding
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade); //manage correct copy from cascadeFrontalfilename to IDetector

    if ( cascade->empty() )
    {
        printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
        return 2;
    }

    DetectionBasedTracker::Parameters params; //get parameter for detection: int maxTrackLifetime, int minDetectionPeriod,
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params); //function Detector with obejects for detection and parameters

    if (!Detector.run()) //Detector initialization
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    ///initialization for calc ADC/CPU
    int img_width, img_height;
    Mat ADCs_crop [ADC::nb_ADCs_x][ADC::nb_ADCs_y];
    Mat CPUs_crop [ADC::nb_CPUs_x][ADC::nb_CPUs_y];
    Point2i pts;
    vector<Point2i> CPU_points_begin;
    vector<Point2i> CPU_width_height;
    vector<Rect> ADC_coord;
    vector<Rect> CPU_coord;

    ///begin calc ADCs/CPUs
    //read image
    //path to image
    //convert int k to string
    stringstream ss;
    ss << first_image;
    string_k = ss.str();
    //get file path
    file = path_key[0] + string_k + path_key[1];

    img = imread(file,1); //read image

    img_width = img.size().width;
    img_height = img.size().height;

    cout << "image size: " << img.size() << endl;

    PacketFactory* data_trans_1 = PacketFactory::getInstance();

    int cnt_CPU = 0;

    //divide image to CPUs
    for (int i=0; i<ADC::nb_CPUs_x; i++) {
        for (int j=0; j<ADC::nb_CPUs_y; j++) {
            cnt_CPU++;
            cv::Mat crop = img(Rect(i * ceil(img_width / ADC::nb_CPUs_x), j * ceil(img_height / ADC::nb_CPUs_y),
                                    ceil(img_width / ADC::nb_CPUs_x), ceil(img_height / ADC::nb_CPUs_y))); //ceil(x) = roundup
            CPUs_crop [i][j] = crop;
            pts = Point2i(i * ceil(img_width / ADC::nb_CPUs_x), j * ceil(img_height / ADC::nb_CPUs_y));
            CPU_points_begin.push_back(pts);
            pts = Point2i(ceil(img_width / ADC::nb_CPUs_x), ceil(img_height / ADC::nb_CPUs_y));
            CPU_width_height.push_back(pts);
            cout << "CPU " << cnt_CPU << " P: [" << CPU_points_begin[i*ADC::nb_CPUs_y+j].x << "," << CPU_points_begin[i*ADC::nb_CPUs_y+j].y
                 << "]; \t\tsize: [" << CPU_width_height[i*ADC::nb_CPUs_y+j].x << " x " << CPU_width_height[i*ADC::nb_CPUs_y+j].y << "]" << endl;
//                    cout << "x coordinateCPU: " << CPU_points_begin[i*ADC::nb_CPUs_y+j].x << "\ny coordinateCPU: " << CPU_points_begin[i*ADC::nb_CPUs_y+j].y << endl;
//                    cout << "width: " << CPU_width_height[i*ADC::nb_CPUs_y+j].x << "\nheight: " << CPU_width_height[i*ADC::nb_CPUs_y+j].y << endl;
        }
    }

    //add max object size
    for (int i=0; i<CPU_width_height.size(); i++) {
        CPU_points_begin[i].x -= ceil(ADC::MAX_OBJECT_SIZE_x/2);
        CPU_points_begin[i].y -= ceil(ADC::MAX_OBJECT_SIZE_y/2);
        CPU_width_height[i].x += ceil(ADC::MAX_OBJECT_SIZE_x);
        CPU_width_height[i].y += ceil(ADC::MAX_OBJECT_SIZE_y);

        if (CPU_points_begin[i].x < 0){
            CPU_points_begin[i].x += ceil(ADC::MAX_OBJECT_SIZE_x/2);
            CPU_width_height[i].x -= ceil(ADC::MAX_OBJECT_SIZE_x/2);
        }
        if (CPU_points_begin[i].x + CPU_width_height[i].x > img_width) {
            CPU_width_height[i].x -= ceil(ADC::MAX_OBJECT_SIZE_x/2);
        }
        if (CPU_points_begin[i].y < 0){
            CPU_points_begin[i].y += ceil(ADC::MAX_OBJECT_SIZE_y/2);
            CPU_width_height[i].y -= ceil(ADC::MAX_OBJECT_SIZE_y/2);
        }
        if (CPU_points_begin[i].y + CPU_width_height[i].y > img_height) {
            CPU_width_height[i].y -= ceil(ADC::MAX_OBJECT_SIZE_y/2);
        }

        cout << "CPU_with_obj " << i+1 << " P: [" << CPU_points_begin[i].x << "," << CPU_points_begin[i].y
             << "]; \t size: [" << CPU_width_height[i].x << " x " << CPU_width_height[i].y << "]" << endl;
    }

    int cnt_packet = 0;

    //calculate ADCs for each CPU
    for (int i=0; i<CPU_points_begin.size(); i++) {
        cout << "ADCs for CPU " << i+1 << ":" << endl;

        int cnt_ADC = 0;

        //divide image to ADCs
        for (int k=0; k<ADC::nb_ADCs_x; k++) {
            for (int l=0; l<ADC::nb_ADCs_y; l++) {
                cnt_ADC++;
                cv::Mat crop = img(Rect(k * ceil(img_width / ADC::nb_ADCs_x), l * ceil(img_height / ADC::nb_ADCs_y),
                                        ceil(img_width / ADC::nb_ADCs_x), ceil(img_height / ADC::nb_ADCs_y))); //ceil(x) = roundup
                ADCs_crop [k][l] = crop;

                if (i < 1) {
                    ADC_coord.emplace_back(Rect(k * ceil(img_width / ADC::nb_ADCs_x), l * ceil(img_height / ADC::nb_ADCs_y),
                            ceil(img_width / ADC::nb_ADCs_x), ceil(img_height / ADC::nb_ADCs_y)));
                }

                //select ADCs
                if (( //CPU 2 and 3
                            k * ceil(img_width / ADC::nb_ADCs_x) < (CPU_points_begin[i].x + CPU_width_height[i].x)                                      //x_ADC_begin < x_CPU_end
                            && l * ceil(img_height / ADC::nb_ADCs_y) < (CPU_points_begin[i].y + CPU_width_height[i].y)                                  //y_ADC_begin < y_CPU_end
                            && (k * ceil(img_width / ADC::nb_ADCs_x) + ceil(img_width / ADC::nb_ADCs_x)) > CPU_points_begin[i].x                        //x_ADC_end > x_CPU_begin
                            && (l * ceil(img_height / ADC::nb_ADCs_y) + ceil(img_height / ADC::nb_ADCs_y)) > CPU_points_begin[i].y                      //y_ADC_end > y_CPU_begin
                    ) || ( //CPU 1 and 4
                            k * ceil(img_width / ADC::nb_ADCs_x) >= CPU_points_begin[i].x                                                                       //x_ADC_begin >= x_CPU_begin
                            && l * ceil(img_height / ADC::nb_ADCs_y) >= CPU_points_begin[i].y                                                                   //y_ADC_begin >= y_CPU_begin
                            && (k * ceil(img_width / ADC::nb_ADCs_x) + ceil(img_width / ADC::nb_ADCs_x)) <= (CPU_points_begin[i].x + CPU_width_height[i].x)     //x_ADC_end <= x_CPU_end
                            && (l * ceil(img_height / ADC::nb_ADCs_y) + ceil(img_height / ADC::nb_ADCs_y)) <= (CPU_points_begin[i].y + CPU_width_height[i].y)   //y_ADC_end <= y_CPU_end
                    )) {

                    data_trans_1->createPacket();
                    data_trans_1->packets[cnt_packet]->addr_src = cnt_ADC;
                    data_trans_1->packets[cnt_packet]->dst = i+1;
                    data_trans_1->packets[cnt_packet]->data.emplace_back('C');

                    cout << "\tADC " << cnt_ADC << endl;

//                    cout << "\tADC " << cnt_ADC << " P: [" << k * ceil(img_width / ADC::nb_ADCs_x) << "," << l * ceil(img_height / ADC::nb_ADCs_y)
//                         << "]; \t\tsize: [" << ceil(img_width / ADC::nb_ADCs_x) << " x " << ceil(img_height / ADC::nb_ADCs_y) << "]" << endl;
                    cnt_packet++;
                }
            }
        }
    }

    for (int i=0; i < data_trans_1->packets.size(); i++) {
        cout << "data_trans_1 Packet " << i+1 << ":\tID: " << data_trans_1->packets[i]->id << "\tSRC: "<<
                data_trans_1->packets[i]->addr_src << "\tDST: " << data_trans_1->packets[i]->dst << "\tData: " <<
                data_trans_1->packets[i]->data[0] << endl;
    }

    for (int i=0; i < ADC_coord.size(); i++) {
        cout << "\tADC " << i+1 << " P: " << ADC_coord[i].tl() << " \tsize: " << ADC_coord[i].size() << endl;
    }

    imshow("LK Demo", ADCs_crop [1][3]);
    waitKey(0);
//        imshow("LK Demo", CPUs_crop [1][0]);
//        waitKey(0);

    do {
        vector<Rect> Faces;
        facepoints = 0;

        ///loop for face-detection
        for (int v=0; v < detect_img; v++) {
            //end of tracking
            if (first_image > number_images) { //break if maximum of images reached
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

            img = imread(file,1); //read image

            ///LibRaw
            /**
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
            cvtColor(img, gray, COLOR_BGR2GRAY); //make gray image (Mat) img to (Mat) gray

            for (int g=0; g<detection_loop; g++) {
                Detector.process(gray); //edit (Mat) gray, filtering? and
                Detector.getObjects(Faces); //search faces and get coordinates from faces -> (Vector<rect>) Faces, Returns a reference to a storage for the tracked objects

                for (size_t i = 0; i < Faces.size(); i++) {
                    rectangle(img, Faces[i], Scalar(255, 255, 0)); //draw rectangle to faces
                }

            }

            if (!Faces.empty()) {
                break; //if no face detected -> another attempt
            }
        }

        ///get information for each face
        if (Faces.empty()) {
            cerr << "Cannot detect face" << endl;
            first_image++;

            //end of tracking
            if (first_image > number_images) { //break if end number of images reached
                tracking_end = true;
                break;
            }

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

        ///initialization for-loop tracking
        int m = first_image + 1;
        int count_img = 0;
        first_image = 0;
        char c;
        vector<Point2f> points[Faces.size()*2]; //*2  for new and old feature points per face

        ///loop for tracking
        for(;;) {
            //read image:
            //path to image
            //convert int k to string
            stringstream ss;
            ss << m;
            string_k = ss.str();
            //get file path
            file = path_key[0] + string_k + path_key[1];

            img = imread(file, 1); //read image

            ///LibRaw
            /**
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

            cvtColor(img, gray, COLOR_BGR2GRAY);

            //loop for each face q
            for (int q = 0; q < Faces.size(); q++) {
                //info: points[q*2+1] -> current feature points from each face
                //      points[q*2] -> previous feature points from each face
                //create ROI with detected face
                Mat mask = Mat::zeros(img.size(), CV_8UC1); //create mask with zeros for img
                Mat roi(mask, cv::Rect(Faces_x[q], Faces_y[q], Faces_width[q], Faces_height[q])); //create region of interest for each face
                roi = Scalar(255, 255, 255);

                // automatic initialization
                goodFeaturesToTrack(gray, points[q * 2 + 1], MAX_COUNT, 0.01, 10, mask, 3, 3, 0, 0.04); //get feature points for each face
                cornerSubPix(gray, points[q * 2 + 1], subPixWinSize, Size(-1, -1), termcrit); //?

                facepoints = points[q*2+1].size(); //current number of feature points

                if (!points[q * 2].empty()) {
                    vector<uchar> status;
                    vector<float> err;
                    if (prevGray.empty())
                        gray.copyTo(prevGray);
                    calcOpticalFlowPyrLK(prevGray, gray, points[q * 2], points[q * 2 + 1], status, err, winSize, 3,
                                         termcrit, 0, 0.001); //move points to new feature points
                    size_t i, k;
                    for (i = k = 0; i < points[q * 2 + 1].size(); i++) {
                        if (!status[i])
                            continue;
                        points[q * 2 + 1][k++] = points[q * 2 + 1][i];
                        circle(img, points[q * 2 + 1][i], 3, Scalar(0, 255, 0), -1, 8); //draw points
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
                std::swap(points[i * 2 + 1], points[i * 2]); //current feature points -> previous feature points
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

        first_image = first_image + m;
        waitKey(5);
    } while (!tracking_end);

    Detector.stop();
    iProcessor.recycle();

    return 0;
}

PacketFactory* PacketFactory::getInstance() //only one instance of PacketFactory
{
    static PacketFactory instance;
    return &instance;
}

PacketFactory::PacketFactory() {}

Packet* PacketFactory::createPacket()
{
    auto p = new Packet;
    packets.push_back(p);
    return p;
}

void PacketFactory::deletePacket(Packet* p)
{
    auto it = std::find(packets.begin(), packets.end(), p);
    delete(*it);
    packets.erase(it);
}

ADC* ADC::getInstance() //only one instance of ADC
{
    static ADC instance;
    return &instance;
}

ADC::ADC() {}