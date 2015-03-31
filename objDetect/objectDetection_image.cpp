#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect_c.h"
//#include "function.h"

using namespace std;
using namespace cv;

extern "C" double getScale(int width, int height);
std::vector<Rect> detectAndDisplay(CascadeClassifier& face_cascade, Mat& frame, bool isShow );
//double getScale(int width, int height);
string getRandomFileName(Rect rect);

int main(int argc,char* argv[])
{
    const char *keys={
        "{@image    |       | specify the image file name}"
	"{@cascade  |       | specify the cascade file name,eg:cascade.xml}"
	"{s show    |false  | show image}"
	"{w write   |false  | save image}"
	"{h help    |false  | print help.}"
    };
    CommandLineParser parser(argc, argv, keys);
    if (parser.get<bool>("help") || argc < 3){
        parser.printMessage();
	return 0;
    }

    string image_name = parser.get<string>("@image");
    string face_cascade_name = parser.get<string>("@cascade");//"/home/golden/Codes/opencv_xie/opencv/data/lbpcascades/lbpcascade_frontalface.xml";
    bool isShow = parser.get<bool>("show");
    bool isWrite = parser.get<bool>("write");

    
    Mat image = imread(image_name.c_str());
    if (!image.data){
        printf("Error: open %s faild!\n", image_name.c_str());
        return -1;
    }
    
    CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_name.c_str())){
        printf("Error: loading face cascade!\n");
        return -1;
    }

    std::vector<Rect> faces = detectAndDisplay(face_cascade, image, isShow);
    if (faces.size() > 0 && isWrite){
      int slash_idx = image_name.rfind("/");
      std::string new_name = image_name.substr(slash_idx+1);
      imwrite(new_name, image);
      
    }
    
    printf("%s\n", image_name.c_str()); 
    
}
std::vector<Rect> detectAndDisplay(CascadeClassifier& face_cascade, Mat& frame , 
		      bool isShow)
{
#define WIDTH 1440
#define HEIGHT 960

    std::vector<Rect> faces;
    Mat frame_gray;
    string window_name = "face detection with LBP";

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    Size* minSize =new Size(24, 24);
    float scaleFactor = 1.26;
    int flags = 0;// | CV_HAAR_SCALE_IMAGE ;// 0:scale feature; 2:scale image
    double tic = getTickCount();
    face_cascade.detectMultiScale( frame_gray, faces, scaleFactor, 2, flags, *minSize );
    double toc = getTickCount();
    if (flags == 0){
        printf("scale feature elapsed time is %f\n", (toc - tic)/getTickFrequency());
    }
    else{
        printf ("scale image elapsed time is %f\n", (toc-tic)/getTickFrequency());
    }
    delete minSize;
    for( size_t i = 0; i < faces.size(); i++ )
    {
        Mat faceROI = frame_gray( faces[i] );

	Rect rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
	Mat img_ex = frame(rect);
	string name = getRandomFileName(rect);
	imwrite(name.c_str(), img_ex);
#if 1
	Point p0( faces[i].x, faces[i].y);
	Point p1( faces[i].x + faces[i].width, faces[i].y + faces[i].height);
	rectangle( frame, p0, p1, Scalar(0, 0, 255), 2);
#endif
    }
    //-- Show what you got
    if (isShow){
        double scale = getScale(frame.cols, frame.rows);
	Mat im_r;
	if (scale > 1){
	    resize(frame, im_r, Size(frame.cols / scale ,frame.rows / scale));
	}
	else{
	    im_r = frame;
	}
        imshow( window_name.c_str(), im_r );
	waitKey(0);
    }
    return faces;
}
/*
double getScale(int width, int height)
{
    double scale = 1;
    int dstWidth = WIDTH;
    int dstHeight = HEIGHT;
    if (width >= height) {
        if (width > dstWidth  && height > dstHeight) {
            scale = max(width/(double)dstWidth, height/(double)dstHeight);
        }
    }
    else {
        if (width > dstHeight && height > dstWidth) {
            scale = max(width/(double)dstHeight, height/(double)dstWidth);
        }
    }
    if (scale < 1) 
        scale = 1;
    return scale;
}
*/
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
