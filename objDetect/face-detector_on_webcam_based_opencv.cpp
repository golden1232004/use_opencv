/**
 *  @file    detect-gender-on-webcam_based_opencv.cpp
 *  @author  golden
 *  @data    TUE AUG 19 11:30:11 2014
 *  @brief   gender detector testbed based on opencv webcam
 **/

#include <string>
#include <stdio.h>
#include <opencv2/opencv.hpp>

#define WIN "detect-object"

using namespace std;
using namespace cv;
#define mstime(t0, t1) (double)((t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_nsec - t0.tv_nsec) / 1000000.0)

string getRandomFileName(Rect rect)
{
    // assert(rect.x > 0 && rect.y > 0 && rect.width > 0 && rect.height > 0);
    srand((int)time(NULL));
    int rand_num = rand();
    char name[100]={'\0'};
    char name_tmp[50] = {'\0'};
    int tmp;
    if (rect.x == 0)
        rect.x += rand() % 1000;
    tmp= rand_num % rect.x;
    sprintf(name_tmp, "%d", tmp);
    strcpy(name, name_tmp);
    memset(name_tmp, 0, strlen(name_tmp));
    tmp = rand_num / rect.x;
    sprintf(name_tmp, "%d", tmp);
    strcat(name, name_tmp);
    memset(name_tmp, 0, strlen(name_tmp));
    strcat(name, "_");
    sprintf(name_tmp, "%d", rect.x);
    strcat(name, name_tmp);
    memset(name_tmp, 0, strlen(name_tmp));
    strcat(name, "_");
    sprintf(name_tmp, "%d", rect.y);
    strcat(name, name_tmp);
    strcat(name, ".jpg");
 
    return string(name);
}

int detect_gender(CascadeClassifier &face_detector, Mat &showImg)
{
    int nObjects=0 ,  nObjects_sub=0;

    Mat gray_img;
    std::vector<Rect> faces;
    cvtColor(showImg, gray_img, COLOR_BGR2GRAY);
    face_detector.detectMultiScale( gray_img, faces, 1.1, 2, 0, Size(80, 80));
    nObjects = (int)faces.size();
    if(0 == nObjects)
    {
        Point pt(showImg.cols/2, showImg.rows/2);
        circle(showImg, pt, 10, Scalar(255,0,0),5);
	//        tsDetectObject_detect(gender_detector, "gender", 0); // to clear detected value under video
        return 0;
    }

#if 1
    for (int i =0; i<nObjects; i++)
    {
        Rect rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        // save face region
        Mat img_ex = showImg(rect);
        string name = getRandomFileName(rect);
        imwrite(name.c_str(), img_ex);
    }
#endif

    //    TPOINT feature[3]= {{0,0}};
    for (int i =0; i<nObjects; i++)
    {
        Rect rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        Point pt0(rect.x, rect.y);
        Point pt1(rect.x+rect.width, rect.y+rect.height);
        rectangle(showImg, pt0, pt1, Scalar(255,0,0), 3); // blue 
    }

    return 0;
}


int main(int argc, char** argv)
{
    int rVal = 0;
    const char* keys = {
        "{ @model          |        | face cascade detector width opencv. }"
	"{ mode            |1       | video or image. }"
	"{ c class         |face    | face gender. }"
        "{ h help          |false   | Print This Help. }"
    };
    CommandLineParser parser(argc, argv, keys);

    if (parser.get<bool>("help")) {
        parser.printMessage();
        return 0;
    }

    string model = parser.get<string>("@model");
    string className = parser.get<string>("class");
    int    isVideo = parser.get<int>("mode");

    CascadeClassifier face_detector;
    if (!face_detector.load(model.c_str())){
        printf("Error: load face cascade failed!\n");
    }

    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) // check if we succeeded
        return -1;
    namedWindow(WIN, 1);


    Mat im, resized, rotated;  /* 定义在外面防止内存增长 */

    for(;;)
    {
        char str[64];
        static int fnIdx = 0;
        cap >> im; // get a new frame from camera
        if (im.size().area() <= 0)
            break;
        resize(im, resized, Size(0,0), 0.5, 0.5);
        rotated = resized;
   
        /**************************************************
          Detect the object
        ***************************************************/

#if SAK_ENABLE_TIME
        double t1 = sakGetTime();
#endif
        if(0 == className.compare("face"))
        {
            struct timespec t0, t1;
            clock_gettime(CLOCK_MONOTONIC, &t0);
            if(0 != detect_gender(face_detector, rotated))
            {
                printf("ERROR:: Detect face failed\n");
                rVal = -1;
                goto EXIT;
            }
           clock_gettime(CLOCK_MONOTONIC, &t1);
            // printf("detection time  %.1f\n", mstime(t0, t1));

        }

#if SAK_ENABLE_TIME
        double t2 = sakGetTime();
        printf("detect time: %f ms\n", t2-t1);
#endif
        /**************************************************
          Show the detection result
        ***************************************************/
        imshow(WIN, rotated);
        char c = waitKey(30);
        if (c == 27) break;
        if (c == 'w') {
            sprintf(str, "%04d.jpg", fnIdx++);
            imwrite(str, im);
        }
    }   

 EXIT:

    return rVal;
}
