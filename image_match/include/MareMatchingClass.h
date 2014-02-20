#pragma once
//////////////////////////////////////////////////////////////////////////

// Made by J.H.KIM, 2011 / feelmare@daum.net, feelmare@gmail.com        //

// blog : http://feelmare.blogspot.com                                  //

// My Lab : VISLAB(http://me.pusan.ac.kr)                               //

//////////////////////////////////////////////////////////////////////////


#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <time.h>
#include <algorithm>

using namespace std;

typedef struct MATCH_PAIR
{
	int nA;
	int nB;
} MATCH_PAIR;

struct RectPT{
	double left;
	double right;
	double top;
	double bottom;
};

struct Pt{
	double x;
	double y;
};

struct Rect4Pt{
	Pt LeftTop;
	Pt RightTop;
	Pt LeftBottom;
	Pt RightBottom;
};

class CMareMatchingClass
{
public:
	CMareMatchingClass(void);
	~CMareMatchingClass(void);


	CvMemStorage* BGstorage;
	CvMemStorage* PatchStorage;
	
	int BGWidth;
	int BGHeight;

	int PatchWidth;
	int PatchHeight;



	CvSURFParams params;

	//Background Img
	CvSeq *BG_Keypoints;
	CvSeq *BG_Descriptors;	
	//Patch Img
	CvSeq *Patch_Keypoints;
	CvSeq *Patch_Descriptors;
	//Matching 
	MATCH_PAIR *pMatchPair;
	int nMatchingCount;
	CvPoint2D32f *BGpt1;
	CvPoint2D32f *PTpt2;
	//corner point
	CvMat* McornerPt;
	CvMat* MTransformedCornerPt;
	//Inter corner point
	CvMat* MInterCornerPt;	
	CvMat* MInterTransformedCornerPt;
	//solution	
	CvMat* mBestH;	
	CvMat* mBestCorner;	
	RectPT BestRectPt;

	void ReleaseBGsurfSeq(void);
	void ReleasePatchsurfSeq(void);
	void ReleasepMatchPair(void);
	void ReleasepMatchPairPT(void);
	void ReleaseCornerMat(void);
	void ReleaseBestMat(void);

	void CreateBestH(void);

	bool GetObjectRectAndBestH(IplImage* img, Rect4Pt* rect4Pt);

	bool ExtractBGsurf(IplImage* img);
	bool ExtractPatchSurf(IplImage* img);
	int fFindMatchingPoints(void);
	int FindMatchingPoints(const CvSeq* tKeypoints, const CvSeq* tDescriptors, const CvSeq* srcKeypoints, const CvSeq* srcDescriptors, int descriptor_size, MATCH_PAIR *pMatchPair);
	int FindNearestPoints(const float* pA, int laplacian, const CvSeq* srcKeypoints, const CvSeq* srcDescriptors, int descriptor_size);

	int FindBestH();
	void GetClosetPTN(int tRandI, CvPoint2D32f *BGpt1, int nMatchingCount, CvMat* M1, CvMat* M2, int n, CvPoint2D32f *BGpt, CvPoint2D32f *PTpt);
	void GetRandomPTN(int tRandI, CvPoint2D32f *BGpt1, int nMatchingCount, CvMat* M1, CvMat* M2, int n, CvPoint2D32f *BGpt, CvPoint2D32f *PTpt);

	bool HisOk(CvMat* tH, CvMat* tIH, int w, int h);

	bool IsInTheBound(double tx, double ty, double x0, double y0, double x2, double y2);
	bool IsOnTheLine(double x1, double y1, double x0, double y0, double x2, double y2);

	void GetRectPt( CvMat * M, RectPT* rectPt);
	double MatchingScore(const CvSeq* OriginKeyPt, const CvSeq * CompareKeyPt, CvMat* H, RectPT* rectPt);
	void CopyMat(CvMat* M1, CvMat* M2);

	void printMat(CvMat& M);
	void GetHMulRect(CvMat* M, Rect4Pt* rect4Pt);

	int inCount;

};
