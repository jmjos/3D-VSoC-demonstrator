#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctype.h>

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

using namespace std;
using namespace cv;

//number of images in folder ../RAWImages/...
const int number_images = 6; //27
const int detection_loop = 150;
const int MAX_COUNT = 100;

const string path_key[2] = {"/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/RAWImages/example_",".dng"};
//const string path_key[2] = {"/home/mtzschoppe/Desktop/face images/filename000",".png"};

//const string WindowName = "Face Detection example";

//functions
static void help()
{
    // print a welcome message, and the OpenCV version
    cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
            "Using OpenCV version " << CV_VERSION << endl;
    cout << "\nIt uses camera by default, but you can provide a path to video as an argument.\n";
    cout << "\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tr - auto-initialize tracking\n"
            "\tc - delete all the points\n"
            "\tn - switch the \"night\" mode on/off\n"
            "To add/remove a feature point click it\n" << endl;
}
Point2f point;
bool addRemovePt = false;
static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == EVENT_LBUTTONDOWN )
    {
        point = Point2f((float)x, (float)y);
        addRemovePt = true;
    }
}

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
{
public:
    CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
    {
        CV_Assert(detector);
    }

    void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
    {
        Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
    }

    virtual ~CascadeDetectorAdapter() CV_OVERRIDE
    {}

private:
    CascadeDetectorAdapter();
    cv::Ptr<cv::CascadeClassifier> Detector;
};

int main(int arg_num, char *arg_vec[]) {

    LibRaw iProcessor;

    //namedWindow(WindowName, cv::WINDOW_NORMAL);

    string file;
    string string_k;

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

    //TODO feature points tracking:
    //VideoCapture cap("/home/mtzschoppe/Downloads/sample-videos/head-pose-face-detection-male.mp4");
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);
    bool needToInit = true;
    bool nightMode = false;
    help();

    vector<Rect> Faces;

    /*
    if( !cap.isOpened() )
    {
        cerr << "Could not initialize capturing...\n";
        return 0;
    }
    */
    namedWindow("LK Demo", cv::WINDOW_NORMAL);
    setMouseCallback( "LK Demo", onMouse, 0 );
    Mat gray, prevGray, frame;
    vector<Point2f> points[2];

    //path to image
    //convert int k to string
    stringstream ss;
    ss << 0; //48
    string_k = ss.str();
    //get file path
    file = path_key[0] + string_k + path_key[1];


    //LibRaw
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


    //Mat img = imread(file,4);

    //convert from 16 bit to 8 bit
    img.convertTo(img, CV_8UC1, 1/256.0);

    int Faces_height,Faces_width,Faces_x,Faces_y;

    for (int g=0; g<detection_loop; g++){
        cvtColor(img, gray, COLOR_BGR2GRAY);
        Detector.process(gray);
        Detector.getObjects(Faces);

        //draw rectangle around the face
        cout << "dedectionloop" << endl;


        for (size_t i = 0; i < Faces.size(); i++)
        {
            cout << Faces.size() << endl;
            cout << Faces[0].height << "\t" << Faces[0].width <<endl;
            cout << Faces[0].x << "\t" << Faces[0].y <<endl;
            rectangle(img, Faces[i], Scalar(255,255,0));
            cout << Faces[i] << endl;

        }

        Faces_height=Faces[0].height;
        Faces_width =Faces[0].width;
        Faces_x =Faces[0].x;
        Faces_y =Faces[0].y;
    }

    cout << "Eckdaten: " << Faces_height << "\t" << Faces_width << "\n" << Faces_x << "\t" << Faces_y << endl;
    imshow("LK Demo", img);
    waitKey(0);

        int m=1;

        //TODO tracking:
        for(;;)
        {

            //TODO read image:
            //path to image
            //convert int k to string
            stringstream ss;
            ss << m;
            string_k = ss.str();
            //get file path
            file = path_key[0] + string_k + path_key[1];


            //LibRaw
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


            //Mat img = imread(file,4);
            //convert from 16 bit to 8 bit
            img.convertTo(img, CV_8UC1, 1/256.0);

            //TODO begin tracking:
            cvtColor(img, gray, COLOR_BGR2GRAY);

            Mat image_2 = gray;
            //Rect roi = Faces;
            //Faces.width

            Mat crop = image_2(Rect(Faces_x, Faces_y, Faces_height, Faces_width));

            //crop.convertTo(crop, CV_8UC1, 1/256.0);

            imshow("LK Demo", crop);
            waitKey(0);

            if( nightMode )
                img = Scalar::all(0);


                // automatic initialization
                goodFeaturesToTrack(crop, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 3, 0, 0.04);
                cornerSubPix(gray, points[1], subPixWinSize, Size(-1,-1), termcrit);
                addRemovePt = false;

            if( !points[0].empty() )
            {
                vector<uchar> status;
                vector<float> err;
                if(prevGray.empty())
                    gray.copyTo(prevGray);
                calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);
                size_t i, k;
                for( i = k = 0; i < points[1].size(); i++ )
                {
                    if( addRemovePt )
                    {
                        if( norm(point - points[1][i]) <= 5 )
                        {
                            addRemovePt = false;
                            continue;
                        }
                    }
                    if( !status[i] )
                        continue;
                    points[1][k++] = points[1][i];
                    circle( img, points[1][i], 3, Scalar(0,255,0), -1, 8);
                }
                points[1].resize(k);
            }
            if( addRemovePt && points[1].size() < (size_t)MAX_COUNT )
            {
                vector<Point2f> tmp;
                tmp.push_back(point);
                cornerSubPix( gray, tmp, winSize, Size(-1,-1), termcrit);
                points[1].push_back(tmp[0]);
                addRemovePt = false;
            }
            needToInit = true;
            imshow("LK Demo", img);
            char c = (char)waitKey(500);
            if( c == 27 )
                break;
            switch( c )
            {
                case 'r':
                    needToInit = false;
                    break;
                case 'c':
                    points[0].clear();
                    points[1].clear();
                    break;
                case 'n':
                    nightMode = !nightMode;
                    break;
            }
            std::swap(points[1], points[0]);
            cv::swap(prevGray, gray);

            if (m >  number_images) {
                break;
            }
            m++;
        }

        //Mat ReferenceFrame;
        //Mat GrayFrame;

        //vector<Rect> Faces;


        //CascadeClassifier face_cascade;
        //face_cascade.load("/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/opencv/opencv/data/lbpcascades/lbpcascade_frontalface.xml");



        //loop for face detection
        /*
        for (int g=0; g<detection_loop; g++){
            //detect face
            cvtColor(img, GrayFrame, COLOR_BGR2GRAY);
            Detector.process(GrayFrame);
            Detector.getObjects(Faces);

            //face_cascade.detectMultiScale(img, Faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

            //draw rectangle around the face
            for (size_t i = 0; i < Faces.size(); i++)
            {
                rectangle(img, Faces[i], Scalar(255,255,0));
            }

        }

        imshow(WindowName, img);

        waitKey(0);
        */
        /*
        CascadeClassifier face_cascade;
        face_cascade.load("/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/opencv/opencv/data/lbpcascades/lbpcascade_frontalface.xml");

        if (!face_cascade.empty())
            face_cascade.detectMultiScale(img, Faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
        */





    Detector.stop();
    iProcessor.recycle();

    return 0;
}