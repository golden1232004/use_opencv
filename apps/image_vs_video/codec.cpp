#include <iostream>
#include <string>
#include <sstream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "sak/ui/ui.hpp"

using namespace std;
using namespace cv;

#define FRAME_TOTAL  60680   //2G for 1920*1080

int code(int fps, bool gui)
{
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened()){
        cout << "cap is not open" << endl;
        return -1;
    }
 
	int fourcc = CV_FOURCC('X','V','I','D');
    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	VideoWriter video;
    Mat frame;
    unsigned int frameCnt = 0;
    unsigned int videoFileCnt = 0;
	while(true)
	{
        if (FRAME_TOTAL == frameCnt || 0 == frameCnt){
            frameCnt %= FRAME_TOTAL;
            char name[20] = {'\0'};
            sprintf(name, "%d", videoFileCnt);
            videoFileCnt++;
            string vn = name;
            vn = "./video" + vn + ".avi";
            video.open(vn.c_str(),fourcc,fps,cvSize(width,height));
            if (!video.isOpened()){
                cout << "video is not open " << endl;
                return -1;
            }
        }

        cap >> frame;
		if (frame.empty())
		{
			cout<<endl<<"image Empty"<<endl;
			break;
		}
		video << frame;
        frameCnt++;
        if (gui){
            imshow("record",frame);
        }
		char c = waitKey(30);
        if('q' == c){
            break;
        }
	}
    return 0;
}
int decode(string videoFile, bool gui)
{
    VideoCapture cap;
    cap.open(videoFile);
    if (!cap.isOpened()){
        cout << "open video file failed" << endl;
        return -1;
    }
    Mat frame;
    int cnt = 0;
    while(cap.read(frame)){
        char name[20] = {'\0'};
        sprintf(name, "%d", cnt);
        string fn = name;
        fn = "image" + fn + ".jpg";
        imwrite(fn.c_str(), frame);
        cnt++;
        if (gui){
            imshow("decode", frame);
            waitKey(30);
        }
    }
    cap.release();
    return 0;
}
int main(int argc, char* argv[])
{
    string opt_keys = 
        "{ @videoFile |      |video file for decode. }"
        "{ f function |code  |code(image2video) or decode(video2image). }"
        "{ p fps      |20    | set frame per second. }"
        "{ g gui      |false |Enable/disable GUI. }"
        "{ h help     |false |print help. }";
    sak::ui::CommandLineParser parser = sak::ui::CommandLineParser(argc, argv, opt_keys);

    if (parser.get<bool>("help")) {
        parser.printMessage();
        exit(0);
    }
    string fn   = parser.get<string>("@videoFile");
    string vn   = parser.get<string>("@videoFile");
    string func = parser.get<string>("function");
	int fps     = parser.get<int>("fps");
    bool gui    = parser.get<bool>("gui");

    int status = -1;
    if("code" == func){
        status = code(fps, gui);
        if (status < 0){
            printf(" code error\n");
            return -1;
        }
    }
    else if ("decode" == func){
        status = decode(vn, gui);
        if (status < 0) {
            printf(" decode error\n");
            return -1;
        }
    }
 
	return 0;
}

