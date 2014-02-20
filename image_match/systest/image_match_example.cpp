#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>


using namespace std;

#include "MareMatchingClass.h"


void printMat(CvMat& M)
{
	for(int i=0; i<M.rows; ++i)
	{
		for(int j=0;j<M.cols; ++j)
		{
			printf("%lf ", cvmGet(&M,i,j) );
		}
		printf("\n");
	}
}


int main(int argc, char* argv[])
{

	//Camera Version

	IplImage* PatchImg;
	if( (PatchImg = cvLoadImage("A.jpg", CV_LOAD_IMAGE_GRAYSCALE)) == NULL)
	{
		printf("B Img open error\n");
		return -1;
	}


	//Create Matching class
	CMareMatchingClass MMathing;	
	//Input PatchImg
	MMathing.ExtractPatchSurf(PatchImg);

	double cornerPt[9]={
		MMathing.PatchWidth/2,  0,                     MMathing.PatchWidth,
		0,                      MMathing.PatchHeight,  MMathing.PatchHeight,
		1,                      1,                     1};
	CvMat* McornerPt2 = cvCreateMat(3, 3, CV_64FC1);
	memcpy(McornerPt2->data.db, cornerPt, sizeof(double)*9);

	int c;
	IplImage* img;
	IplImage* BackGroundImg = NULL;
	CvCapture* capture = cvCaptureFromCAM(0);
	cvNamedWindow("mainWin",CV_WINDOW_AUTOSIZE);
	while(1)
	{
		cvGrabFrame(capture);
		img = cvRetrieveFrame(capture);
		
		if(BackGroundImg == NULL)
		{
			BackGroundImg = cvCreateImage(cvSize(img->width, img->height), 8, 1);
		}
		cvCvtColor(img, BackGroundImg, CV_RGB2GRAY);

		Rect4Pt rect4pt;
		if(MMathing.GetObjectRectAndBestH(BackGroundImg, &rect4pt) != 0)
		{
			cvLine(img, cvPoint( rect4pt.LeftTop.x, rect4pt.LeftTop.y), cvPoint(rect4pt.RightTop.x, rect4pt.RightTop.y), cvScalar(255,0,0), 2);
			cvLine(img, cvPoint(rect4pt.RightTop.x, rect4pt.RightTop.y), cvPoint(rect4pt.RightBottom.x, rect4pt.RightBottom.y), cvScalar(255,0,0), 2);
			cvLine(img, cvPoint(rect4pt.RightBottom.x, rect4pt.RightBottom.y), cvPoint(rect4pt.LeftBottom.x, rect4pt.LeftBottom.y), cvScalar(255,0,0), 2);
			cvLine(img, cvPoint(rect4pt.LeftBottom.x, rect4pt.LeftBottom.y), cvPoint( rect4pt.LeftTop.x, rect4pt.LeftTop.y), cvScalar(255,0,0), 2);

			Rect4Pt rect4ptZ;
			MMathing.GetHMulRect(McornerPt2,&rect4ptZ);
			cvLine(img, cvPoint(rect4ptZ.LeftTop.x, rect4ptZ.LeftTop.y), cvPoint(rect4ptZ.RightTop.x, rect4ptZ.RightTop.y), cvScalar(255,0,0), 2);
			cvLine(img, cvPoint(rect4ptZ.LeftTop.x, rect4ptZ.LeftTop.y), cvPoint(rect4ptZ.RightBottom.x, rect4ptZ.RightBottom.y), cvScalar(255,0,0), 2);			
		}		


		cvShowImage("mainWin",img);

		c = cvWaitKey(10);
		if(c == 27)
			break;
	}
	
	cvReleaseCapture(&capture);	
	cvDestroyAllWindows();	
	cvReleaseImage(&PatchImg);	
	cvReleaseImage(&BackGroundImg);	
	cvReleaseMat(&McornerPt2);


	//Image Version
	/*
	IplImage* PatchImg;
	if( (PatchImg = cvLoadImage("A.jpg", CV_LOAD_IMAGE_GRAYSCALE)) == NULL)
	{
		printf("B Img open error\n");
		return ;
	}

	IplImage* BackGroundImg = NULL;
	if( (BackGroundImg = cvLoadImage("backImg2.jpg", CV_LOAD_IMAGE_GRAYSCALE)) == NULL)
	{
		printf("B Img open error\n");
		return ;
	}
	//Create Matching class
	CMareMatchingClass MMathing;	
	//Input PatchImg
	MMathing.ExtractPatchSurf(PatchImg);	

	double cornerPt[9]={
		MMathing.PatchWidth/2,  0,                     MMathing.PatchWidth,
		0,                      MMathing.PatchHeight,  MMathing.PatchHeight,
		1,                      1,                     1};
		CvMat* McornerPt2 = cvCreateMat(3, 3, CV_64FC1);
		memcpy(McornerPt2->data.db, cornerPt, sizeof(double)*9);

	cvNamedWindow("mainWin",CV_WINDOW_AUTOSIZE);	
	Rect4Pt rect4pt;
	if(MMathing.GetObjectRectAndBestH(BackGroundImg, &rect4pt) != 0)
	{
		cvLine(BackGroundImg, cvPoint( rect4pt.LeftTop.x, rect4pt.LeftTop.y), cvPoint(rect4pt.RightTop.x, rect4pt.RightTop.y), cvScalar(255,0,0), 2);
		cvLine(BackGroundImg, cvPoint(rect4pt.RightTop.x, rect4pt.RightTop.y), cvPoint(rect4pt.RightBottom.x, rect4pt.RightBottom.y), cvScalar(255,0,0), 2);
		cvLine(BackGroundImg, cvPoint(rect4pt.RightBottom.x, rect4pt.RightBottom.y), cvPoint(rect4pt.LeftBottom.x, rect4pt.LeftBottom.y), cvScalar(255,0,0), 2);
		cvLine(BackGroundImg, cvPoint(rect4pt.LeftBottom.x, rect4pt.LeftBottom.y), cvPoint( rect4pt.LeftTop.x, rect4pt.LeftTop.y), cvScalar(255,0,0), 2);

		Rect4Pt rect4ptZ;
		MMathing.GetHMulRect(McornerPt2,&rect4ptZ);
		cvLine(BackGroundImg, cvPoint(rect4ptZ.LeftTop.x, rect4ptZ.LeftTop.y), cvPoint(rect4ptZ.RightTop.x, rect4ptZ.RightTop.y), cvScalar(255,0,0), 2);
		cvLine(BackGroundImg, cvPoint(rect4ptZ.LeftTop.x, rect4ptZ.LeftTop.y), cvPoint(rect4ptZ.RightBottom.x, rect4ptZ.RightBottom.y), cvScalar(255,0,0), 2);	

	}

	cvShowImage("mainWin",BackGroundImg);
	cvWaitKey(0);	
	cvDestroyAllWindows();
	cvReleaseImage(&PatchImg);	
	cvReleaseImage(&BackGroundImg);
	cvReleaseMat(&McornerPt2);
	*/
	
	
	return 1;
}
