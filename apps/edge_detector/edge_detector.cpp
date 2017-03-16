#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;
Mat do_canny(Mat& img)
{
    Mat dst;
    // simple way
    //Canny(img, dst, 150, 100, 3);
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);
    imwrite("gray.jpg", gray);
    threshold(gray, gray, 200, 255, CV_THRESH_BINARY);
    //denoise
    blur(gray, dst, Size(3, 3));
    Canny(dst, dst, 3, 9, 3);
    return dst;
}

Mat do_laplace(Mat& img)
{
    Mat src = img.clone();
    Mat dst;
    //use gaussion filter
    // GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
    Mat gray;
    cvtColor(src, gray, CV_BGR2GRAY);
    Laplacian(gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    Mat out;
    convertScaleAbs(dst, out);
    return out;
}

int main(int argc, char* argv[])
{
    string opt_keys =
        "{ @path      |      |image path. }"
        "{ m method   |canny |support canny, sobel, laplace, scharr}"
        "{ h help     |false |print help. }";
    CommandLineParser parser = CommandLineParser(argc, argv, opt_keys);

    if (parser.get<bool>("help")) {
        parser.printMessage();
        exit(0);
    }
    string fn   = parser.get<string>("@path");
    string method = parser.get<string>("method");
    Mat img = imread(fn.c_str());

    if ("canny" == method){
        Mat dst = do_canny(img);
        imwrite("do_canny.jpg", dst);
    }
    else if("sobel" == method){
    }
    else if ("laplace" == method){
        Mat dst = do_laplace(img);
        imwrite("do_laplace.jpg", dst);
    }
    else if ("scharr" == method){
    }
    else{
        printf("Please input right method\n");
    }



}
