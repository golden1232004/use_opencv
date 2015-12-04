#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include <string>

using namespace std;


const int MAX_CORNERS = 500;

int main(int argc, char** argv)
{
    string filename = argv[1];
    string windowname = "find feature to track";
    IplImage* img = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
    assert(NULL != img);
    IplImage* img_gray = cvCreateImage(cvGetSize(img), 8, 1);
    cvCvtColor(img, img_gray, CV_BGR2GRAY);

    // create temporary image required by cvGoodFeaturesToTrack
    IplImage* img_temp = cvCreateImage(cvGetSize(img), 32, 1);
    IplImage* img_eigen = cvCreateImage(cvGetSize(img), 32, 1);

    // create the arra to store the points detected

    int count = 1000;
    CvPoint2D32f* corners = new CvPoint2D32f[count];

    //Find corners
    cvGoodFeaturesToTrack(img_gray, img_eigen, img_temp, corners, &count, 0.1, 10);

    // mark these corners on the original image
    for(int i = 0; i < count; i++){
        cvLine(img, cvPoint(corners[i].x, corners[i].y),
               cvPoint(corners[i].x, corners[i].y), CV_RGB(255, 0, 0), 5);
        
    }

    cvNamedWindow(windowname.c_str());
    cvShowImage(windowname.c_str(), img);

    printf("Detected Points : %d\n", count);

    cvWaitKey(0);
    return 0;

}
