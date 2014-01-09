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
    detector.read(fileNode);
    detector.detect(img, img, detection);
  


}
