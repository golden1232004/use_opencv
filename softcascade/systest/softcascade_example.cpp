// softcascade detector

#include "opencv2/opencv.hpp"
#include "opencv2/softcascade.hpp"


using namespace cv;
using namespace cv::softcascade;
using namespace std;

int main(int argc, char* argv[])
{
  
    char *imgName = argv[1];
    char *cfgName = argv[2];

    Mat img = imread(imgName);
    FileStorage fs(cfgName, cv::FileStorage::READ);
    if (!fs.isOpened()){
        cout << "Configuration file" << cfgName << " can't be opened" << endl;
	return -1;
    }
    FileNode fileNode = fs["softcascade"];
    Detector detector;
    vector<Detection> detection;
    bool retFlag = detector.load(fileNode);
    vector<cv::Rect> vec;
    //   vec.push_back(cv::Rect(0,0, 20, 20));
    //    vec.push_back(cv::Rect(1,1,32,32));
    Mat roiImg(vec);
    bool isEmpty = roiImg.empty();
    detector.detect(img, roiImg, detection); // note: roiImg is empty.
    int size = detection.size();
    if (size > 0){
        for (int i = 0; i < size; i++){
	    Detection det = detection[i];
	    cv::Rect rect(det.x, det.y, det.w, det.h);
	    cv::Point p0(det.x, det.y);
	    cv::Point p1(det.x + det.w, det.y + det.h);
	    rectangle(img, p0, p1, Scalar(0,0,255), 3);
	}
	// resize;
	imshow("detect-face", img);
	waitKey(0);
    }
    else{
        printf("none face detected\n"); 
    }
  


}
