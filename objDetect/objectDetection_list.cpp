#include "opencv2/opencv.hpp"
#include "function.h"

using namespace std;
using namespace cv;

vector<Rect> detectAndDisplay(CascadeClassifier& face_cascade, Mat frame, bool isShow);

string dirName(const string& pathname, bool withLastSep)
{
    size_t pos;
    pos = pathname.rfind("/");
    if (pos == string::npos)
        return string("");
    else if (pos == 0) {
        if (withLastSep)
            return string("/");
        else
            return string("");
    }
    return pathname.substr(0, withLastSep ? pos+1 : pos);
}
string readLineFromListFile(FILE* fp)
{
    const int iBufLen = 1000;
    char str[iBufLen]={'\0'};
    if (!feof(fp)){
	if (fgets(str, iBufLen, fp)){
	    int len = strlen(str);
	    if (!isprint(str[len-1])){
	        str[len-1] = '\0';
	    }
	}
    }
    string lineStr = str;
    return lineStr;
}

int main(int argc,char* argv[])
{
    const char *keys={
        "{@list     |       | specify the image file name}"
	"{@cascade  |       | specify the cascade file name,eg:cascade.xml}"
        "{s show    |false  | show image}"
	"{h help    |false  | print help.}"
    };
    CommandLineParser parser(argc, argv, keys);
    if (parser.get<bool>("help") || argc < 2){
        parser.printMessage();
	return 0;
    }

    string list_name = parser.get<string>("@list");
    string face_cascade_name = parser.get<string>("@cascade");//"/home/golden/Codes/opencv_xie/opencv/data/lbpcascades/lbpcascade_frontalface.xml";
    bool isShow = parser.get<bool>("show");
    string dir = dirName(list_name, true);
    FILE* fp = fopen(list_name.c_str(), "r");
    CascadeClassifier face_cascade;
    if (!face_cascade.load(face_cascade_name.c_str())){
        printf("Error: loading face cascade!\n");
        return -1;
    }

    while (!feof(fp)){
        string image_name = readLineFromListFile(fp);
	string image_path = dir + image_name;
        Mat image = imread(image_path.c_str());
	if (image.empty()){
	    printf("Error: open %s faild!\n", image_name.c_str());
	    return -1;
	}
	int pos = image_name.find_first_of(".");
	string naked_name = image_name.substr(0, pos);
	printf("%s\n", naked_name.c_str());
	vector<Rect> vRect = detectAndDisplay(face_cascade, image, isShow);

    }
    fclose(fp);
    
}
vector<Rect> detectAndDisplay(CascadeClassifier& face_cascade, Mat frame, bool isShow)
{

    std::vector<Rect> faces;
    Mat frame_gray;
    string window_name = "face detection with LBP";

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    float scaleFactor = 1.1;
    face_cascade.detectMultiScale( frame_gray, faces, scaleFactor, 2, 0, Size(80, 80) );
    vector<Rect> vRect;
    for( size_t i = 0; i < faces.size(); i++ )
    {
        Mat faceROI = frame_gray( faces[i] );
        //-- Draw the face
       	Point p0( faces[i].x, faces[i].y);
	Point p1( faces[i].x + faces[i].width, faces[i].y + faces[i].height);
	rectangle( frame, p0, p1, Scalar(0, 0, 255), 2);
	vRect.push_back(faces[i]);
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
    return vRect;
}

