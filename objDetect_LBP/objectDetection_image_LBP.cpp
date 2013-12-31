#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void detectAndDisplay( Mat frame );
double getScale(int width, int height);

CascadeClassifier face_cascade;

int main(int argc,char* argv[])
{
    if (argc < 2){
      fprintf(stderr, "input parameters must be equal 3!\n");
    }
    string image_name = argv[1];
    string face_cascade_name = "/home/golden/Codes/opencv_xie/opencv/data/lbpcascades/lbpcascade_frontalface.xml";

    
    Mat image = imread(image_name.c_str());
    if (!image.data){
        printf("Error: open %s faild!\n", image_name.c_str());
        return -1;
    }
    
    
    if (!face_cascade.load(face_cascade_name.c_str())){
        printf("Error: loading face cascade!\n");
        return -1;
    }
    detectAndDisplay( image);
     
    
}
void detectAndDisplay( Mat frame )
{
#define WIDTH 1440
#define HEIGHT 960

    std::vector<Rect> faces;
    Mat frame_gray;
    string window_name = "face detection with LBP";

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );

    for( size_t i = 0; i < faces.size(); i++ )
    {
        Mat faceROI = frame_gray( faces[i] );
        //-- Draw the face
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
    }
    //-- Show what you got

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

