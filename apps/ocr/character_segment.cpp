//reference:http://blog.csdn.net/anqing715/article/details/16883863

#include "opencv/cv.h"  
#include "opencv/highgui.h"  
#include "opencv/cxcore.h"

#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

int segment_character(const char* path, vector<Rect>& rects);
int detect_contours(const char* name);

int main(int argc, char* argv[])  
{
    char* name = argv[1];
    vector<Rect> rects;
    segment_character(name, rects);
    //detect_contours(name);
    
    return 0;                                                                                                                                       
}
int segment_character(const char* path, vector<Rect>& rects)
{
    Mat src = imread(path);
    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
    imwrite("gray.bmp", gray);
    Mat binary;
    int blockSize = 9;
    adaptiveThreshold(gray, binary, 255.0, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, blockSize, 5);
    imwrite("binary.bmp", binary);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(binary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    Scalar color(0, 0, 255);
    int thick = 1;
    for (int i = 0; i < contours.size(); i++){
        //drawContours(src, contours, i, color, thick, 8, hierarchy, 0, Point());
        Rect r = boundingRect(contours[i]);
        if (r.width < 24 || r.height < 24){
            continue;
        }
        rectangle(src, r, color, thick);
    }
    imwrite("restult.bmp", src);
    //namedWindow("result", CV_WINDOW_AUTOSIZE);
    //imshow("result", src);
    return 1;
        
}
int detect_contours(const char* name)
{
    IplImage* imgSrc = cvLoadImage(name,CV_LOAD_IMAGE_COLOR);  
    IplImage* img_gray = cvCreateImage(cvGetSize(imgSrc), IPL_DEPTH_8U, 1);  
    cvCvtColor(imgSrc, img_gray, CV_BGR2GRAY);
#if 0
    cvThreshold(img_gray, img_gray,100, 255,CV_THRESH_BINARY_INV);// CV_THRESH_BINARY_INV使得背景为黑色，字符为白色，这样找到的最外层才是字符的最外层
#else
    cvAdaptiveThreshold(img_gray, img_gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 9, 5);
#endif
    cvSaveImage("cvBinary.bmp", img_gray);
    //cvShowImage("ThresholdImg",img_gray);  
    CvSeq* contours = NULL;  
    CvMemStorage* storage = cvCreateMemStorage(0);   
    // 上面源图片有瑕疵可以用腐蚀，膨胀来祛除  
    int count = cvFindContours(img_gray, storage, &contours,sizeof(CvContour),CV_RETR_EXTERNAL);  
    printf("轮廓个数：%d\n",count);  
    int idx = 0;  
    char szName[56] = {0};  
    int tempCount=0;  
    for (CvSeq* c = contours; c != NULL; c = c->h_next) {  
        CvRect rc =cvBoundingRect(c,0);                                                                                                                 
        if (rc.width < 24 || rc.height < 24)      
        {    
            continue;     //这里可以根据轮廓的大小进行筛选  
        }  
        cvDrawRect(imgSrc, cvPoint(rc.x, rc.y), cvPoint(rc.x + rc.width, rc.y + rc.height), CV_RGB(255, 0, 0));  
        IplImage* imgNo = cvCreateImage(cvSize(rc.width, rc.height), IPL_DEPTH_8U, 3);   
        cvSetImageROI(imgSrc, rc);  
        cvCopyImage(imgSrc, imgNo);  
        cvResetImageROI(imgSrc);   
        sprintf(szName, "wnd_%d", idx++);   
        //cvNamedWindow(szName);   
        //cvShowImage(szName, imgNo);   
        cvReleaseImage(&imgNo);   
    }  
    //cvNamedWindow("src");   
    //cvShowImage("src", imgSrc);  
    //cvWaitKey(0);   
    cvReleaseMemStorage(&storage);   
    cvReleaseImage(&imgSrc);   
    cvReleaseImage(&img_gray);   
    cvDestroyAllWindows();
}


