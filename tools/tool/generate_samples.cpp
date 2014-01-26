#include <iostream>
#include <time.h>

#include "opencv2/opencv.hpp"
#include "random_generator.hpp"

using namespace cv;
using namespace std;
/*
typedef  struct rect_tag{
    int x;
    int y;
    int width;
    int height;
} tksRect;
*/
typedef cv::Rect tksRect;

static string dirName(const string& pathname, bool withLastSep)
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

int getStringFromFile(FILE *fFile, string match, vector<string> *vString);
tksRect getRoiFromInfo(string sFile, string sClassName);
tksRect getRectParamFromString(string sStr, char ch);
int processPositive(string lstPath, string imgLst, string infoLst, string className, int baseWin);
int processNegative(string lstPath, string fileLst, cv::Range range);

int main(int argc, char* argv[])
{
  
    string sLstPath;
    string sImgLst ;
    string sInfoLst;
    string sClassName;
    string sType; // positive or negative;
    string sSize; // such as 24x24;
    if ( argc == 7){
        sLstPath = dirName(argv[1], true);
	sImgLst = argv[2];
	sInfoLst = argv[3];
	sClassName = argv[4];
	sType = argv[5]; // positive or negative;
	sSize = argv[6]; // such as 24x24;
    }
    if (argc == 5){
        sLstPath = dirName(argv[1], true);
	sImgLst = argv[2];
	sType = argv[3]; // positive or negative;
	sSize = argv[4]; // such as 

    }
    size_t iPos = sSize.find_first_of("x");
    int iWidth = atoi(sSize.substr(0, iPos).c_str());
    int iHeight = atoi(sSize.substr(iPos+1).c_str());
    int baseWin = iWidth == iHeight? iWidth : 0;
    if (sType == "positive"){
        processPositive(sLstPath, sImgLst, sInfoLst, sClassName, baseWin);
    }
    if (sType == "negative"){
        cv::Range range(2*baseWin, 5*baseWin);
        processNegative(sLstPath, sImgLst, range);
    }
 

}
int processPositive(string lstPath, string imgLst, string infoLst, string className, int baseWin)
{
    FILE *pfImgLst = fopen((lstPath + imgLst).c_str(), "r");
    if (NULL == pfImgLst) {
        fprintf(stderr, "Error: load %s failed\n", (lstPath + imgLst).c_str());
        return -1;
    }
    FILE *pfInfoLst = fopen((lstPath + infoLst).c_str(), "r");
    if (NULL == pfInfoLst){
        fprintf(stderr, "Error: load %s faild\n", (lstPath + infoLst).c_str());
	return -1;
    }
    int iCnt = 0;
    const int iBufLen = 1000;
    char cStringName[iBufLen];
    while((!feof(pfImgLst)) && (!feof(pfInfoLst))){
        string vImgName; 
	if (fgets(cStringName, iBufLen, pfImgLst)){
            int len = strlen(cStringName);
            if (!isprint(cStringName[len-1]))
                cStringName[len-1]='\0';
	    vImgName = cStringName;
	}
	else{
	    fprintf(stderr, "Error: read image name error!\n");
	    return -1;
	}
        string vInfoName;
	memset(cStringName, iBufLen, sizeof(char));
	if (fgets(cStringName, iBufLen, pfInfoLst)){
            int len = strlen(cStringName);
            if (!isprint(cStringName[len-1]))
                cStringName[len-1]='\0';
	    vInfoName = cStringName;
	}
	else{
	    fprintf(stderr, "Error: read info name error!\n");
	    return -1;
	}
	memset(cStringName, iBufLen, sizeof(char));
	// ToDo, campare file name.
	string sImgName = lstPath + vImgName;
	Mat image = imread(sImgName);
	string sInfoName = lstPath + vInfoName;
	printf("%s\n", vImgName.c_str());
	printf("%s\n", vInfoName.c_str());
	tksRect vRect = getRoiFromInfo(sInfoName, className);
	Mat roiImg(image, vRect);
	if (vRect.width > baseWin && vRect.height > baseWin){
 	    Mat resize_img;
	    char cCnt[10] = {'\0'};
	    resize( roiImg, resize_img, Size(baseWin, baseWin), INTER_AREA);
	    sprintf(cCnt, "%d", iCnt);
            string objName = cCnt;
	    imwrite(objName + ".jpg", resize_img);
	    iCnt++;
	}
    }
    printf("%d %s are found!\n", iCnt, className.c_str());
    fclose(pfImgLst);
    fclose(pfInfoLst);
}
int processNegative(string lstPath, string fileLst, cv::Range range)
{
#define NUM_PER_IMAGE  5

    FILE *pfImgLst = fopen((lstPath + fileLst).c_str(), "r");
    if (NULL == pfImgLst) {
        fprintf(stderr, "Error: load %s failed\n", (lstPath + fileLst).c_str());
        return -1;
    }
    const int iBufLen = 1000;
    char cStringName[iBufLen]={'\0'};
    int cnt = 0;
    while((!feof(pfImgLst))){
        string vImgName; 
	if (fgets(cStringName, iBufLen, pfImgLst)){
            int len = strlen(cStringName);
            if (!isprint(cStringName[len-1]))
                cStringName[len-1]='\0';
	    vImgName = cStringName;
	}
	else{
	    fprintf(stderr, "Error: read image name error!\n");
	    return -1;
	}
	memset(cStringName, iBufLen, sizeof(char));
	printf("%s\n", vImgName.c_str());
	string sImgName = lstPath + vImgName;
	Mat image = imread(sImgName);
	int height = image.rows;
	int width = image.cols;
	Random rnd;
	char chNum[33]= {'\0'};
	for (int i = 0; i < NUM_PER_IMAGE; i++){
     	    float r = rnd.nextRand();
	    int win = (int)(range.start + r * (range.end - range.start)); 
            int maxH = height - win;
	    int maxW = width - win;
	    if (maxH < 0 || maxW < 0){
	        continue;
	    }
	    int x = (int)(maxW * rnd.nextRand());
	    int y = (int)(maxH * rnd.nextRand());
	    cv::Rect rect(x, y, win, win);
	    Mat roiImg(image, rect);
	    int num =(int)(r * pow(2,31));
	    sprintf(chNum, "%d", num);
	    string objName = chNum;
	    imwrite(objName + ".jpg", roiImg);
	    memset(chNum, 33, sizeof(chNum[0]));
	}
    }
    fclose(pfImgLst);
    return 1;
}


int getStringFromFile(FILE *fFile, string match, vector<string> *vString)
{
    const int iBufLen = 1000;
    char cStringName[iBufLen];
    char *cCh = const_cast<char*>(match.c_str());
    if ('\0' != *cCh){
        while (!feof(fFile)){
	  if (fgets(cStringName, iBufLen, fFile)){
		if (strstr(cStringName, match.c_str())){
                    vString->push_back(cStringName);
		}
	    }
	}
    }
    return 1;
}
tksRect getRoiFromInfo(string sFile, string sClassName)
{
    FILE *pfInfoFile = fopen(sFile.c_str(), "r");
    if (NULL == pfInfoFile){
        printf("Error:open file: %s\n", sFile.c_str());
        fprintf(stderr, "Error: open file %s!\n", sFile.c_str());
	exit(0);
    }
    vector<string> vString;
    getStringFromFile(pfInfoFile, sClassName, &vString);
    int iStrNum = vString.size();
    tksRect vRect(0,0,0,0);
    for (int i = 0; i < iStrNum; i++){
        string sStr = vString[i];
	vRect = getRectParamFromString(sStr, ':');
    }
    fclose(pfInfoFile);
    return vRect;

} 
tksRect getRectParamFromString(string sStr, char ch)
{
    string sLeft = sStr;
    char *cStr = const_cast<char*>(sStr.c_str());
    char cLeft[1000]={'\0'};
    strcpy(cLeft, cStr);
    int iLen = strlen(cLeft);
    vector<int> vNum;
    bool flag = true;
    while ( flag){
	flag = false;
	char *pcBegin = 0;
	char *pcEnd = 0;
	for (char *i = cLeft; *i != '\0' ; i++){
	    char cTemp = *i;
	    if ( cTemp >='0' && cTemp <= '9'){
	        if (false == flag){
		    pcBegin = i;
		    flag = true;
		}
	    }
	    else{
	        if (true == flag){
		    pcEnd = i; 
		    break;
	        }
	    }
	}
	if (true == flag){
 	    int n = pcEnd - pcBegin > 0 ? pcEnd - pcBegin : pcBegin - pcEnd;
	    n /= sizeof(*pcBegin);
            char cNum[9]={'\0'};
	    strncpy(cNum, pcBegin, n);
	    int iNum = atoi(cNum);
	    vNum.push_back(iNum);
	    memset(cNum, '\0', 9);
	}
	if (true == flag && pcEnd != NULL){
	    strcpy(cLeft, pcEnd); 
	    iLen = strlen(cLeft);
	}
    }
    tksRect rRect(0, 0, 0, 0);
    if (4 == vNum.size()){
        rRect.x = vNum[0];
	rRect.y = vNum[1];
	rRect.width = vNum[2] - vNum[0];
	rRect.height = vNum[3] - vNum[1];
    }
    return rRect;
}
