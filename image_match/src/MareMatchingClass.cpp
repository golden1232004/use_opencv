#include "MareMatchingClass.h"
#include "opencv2/core.hpp"
//#include "opencv2/nonfree/nonfree.hpp"

bool compare(const pair<int, double> &a, const pair<int, double> &b )
{
	return a.second < b.second; //오름차순(ascending order)
	//return a.value > b.value; //내림차순(descending order)
}

CMareMatchingClass::CMareMatchingClass(void)
{	

	printf("//////////////////////////////////////////////////////////////////////////\n");

	printf("// Made by J.H.KIM, 2011 / feelmare@daum.net, feelmare@gmail.com        //\n");

	printf("// blog : http://feelmare.blogspot.com/2011/10/sift-matching-c-source-code-using.html\n");

	printf("// My Lab : VISLAB(http://me.pusan.ac.kr)                               //\n");

	printf("//////////////////////////////////////////////////////////////////////////)\n");


	params = cvSURFParams(1500, 0);
	BG_Keypoints = NULL;
	BG_Descriptors = NULL;	
	Patch_Keypoints = NULL;
	Patch_Descriptors = NULL;
	pMatchPair=NULL;
	BGpt1=NULL;
	PTpt2=NULL;

	MInterCornerPt=NULL;
	MInterTransformedCornerPt=NULL;	
	McornerPt=NULL;
	MTransformedCornerPt=NULL;

	
	mBestH = NULL;
	mBestCorner = NULL;	

	CreateBestH();

	
	BGstorage =cvCreateMemStorage(0);	
	PatchStorage =cvCreateMemStorage(0);

	inCount=0;
}

CMareMatchingClass::~CMareMatchingClass(void)
{	
	
	ReleaseBGsurfSeq();
	ReleasePatchsurfSeq();	
	ReleasepMatchPair();	
	ReleasepMatchPairPT();	
	ReleaseCornerMat();
	
}


void CMareMatchingClass::CreateBestH(void)
{
	ReleaseBestMat();

	double bestH[9] = {0,};
	mBestH = cvCreateMat(3,3, CV_64FC1);
	double BestCorner[12]={0,};
	mBestCorner = cvCreateMat(3,4, CV_64FC1);	

	memcpy(mBestH->data.db, bestH, sizeof(double)*9);
	memcpy(mBestCorner->data.db, BestCorner, sizeof(double)*12);


}


void CMareMatchingClass::ReleaseBestMat(void)
{
	if( mBestH != NULL)
	{
		cvReleaseMat(&mBestH);
		cvReleaseMat(&mBestCorner);

		mBestH = NULL;
		mBestCorner = NULL;	
	}
}

void CMareMatchingClass::ReleaseCornerMat(void)
{
	//Release Corner Matrix
	if( MInterCornerPt!=NULL)
	{
		cvReleaseMat(&McornerPt);
		cvReleaseMat(&MTransformedCornerPt);
		McornerPt=NULL;
		MTransformedCornerPt=NULL;

		cvReleaseMat(&MInterCornerPt);
		cvReleaseMat(&MInterTransformedCornerPt);
		MInterCornerPt=NULL;
		MInterTransformedCornerPt=NULL;		

	}	
}


void CMareMatchingClass::ReleaseBGsurfSeq(void)
{
	//release BackGround Surf Keypoint & descriptor
	if( BG_Keypoints != NULL)
	{
		cvReleaseMemStorage(&BGstorage);
		BGstorage=NULL;
		BG_Keypoints = NULL;
		BG_Descriptors = NULL;	

	}

}

void CMareMatchingClass::ReleasePatchsurfSeq(void)
{
	//release Patch Surf Keypoint & descriptor
	if(Patch_Keypoints != NULL)
	{
		cvReleaseMemStorage(&PatchStorage);

		
		PatchStorage=NULL;
		Patch_Keypoints = NULL;
		Patch_Descriptors = NULL;
	}
}

void CMareMatchingClass::ReleasepMatchPairPT(void)
{
	//release Pair point
	if(	BGpt1!= NULL )
	{
		delete[] BGpt1;
		delete[] PTpt2;

		BGpt1 = PTpt2 = NULL;
	}
}

void CMareMatchingClass::ReleasepMatchPair(void)
{
	//release match index
	if( pMatchPair != NULL )
	{
		delete[] pMatchPair;
		pMatchPair = NULL;
	}
}

bool CMareMatchingClass::ExtractBGsurf(IplImage* img)
{
	//////////////////////////////////////////////////////////////////////////
	
	cvClearMemStorage(BGstorage);
	
	if(img->width < 0 || img->height<0)
		return 0;

	cvExtractSURF(img, NULL, &BG_Keypoints, &BG_Descriptors, BGstorage, params);

	if( BG_Keypoints->total == 0 )
		return 0;

	//////////////////////////////////////////////////////////////////////////
	printf("T1 Img - Keypoint: %d\n", BG_Keypoints->total);
	printf("T1 Img - Descriptors : %d\n", BG_Descriptors->total);

	BGWidth = img->width;
	BGHeight = img->height;

	return 1;
}

bool CMareMatchingClass::ExtractPatchSurf(IplImage* img)
{
	//////////////////////////////////////////////////////////////////////////
	

	cvClearMemStorage(PatchStorage);
	//	bool flag = cv::initModule_nonfree();
	cvExtractSURF(img, NULL, &Patch_Keypoints, &Patch_Descriptors, PatchStorage, params);

	if( Patch_Keypoints->total == 0 )
		return 0;

	printf("T2 Img - Keypoints : %d\n", Patch_Keypoints->total);
	printf("T2 Img - Descriptors : %d\n", Patch_Descriptors->total);

	PatchWidth = img->width;
	PatchHeight =img->height;

	printf("w=%d h=%d\n",PatchWidth, PatchHeight);

	//////////////////////////////////////////////////////////////////////////
	//Corner Pt
	ReleaseCornerMat();
	double cornerPt[12]={
		0, img->width, img->width, 0,
		0, 0, img->height, img->height,
		1, 1, 1, 1};
	//Inter Corner Pt
	double InterCornerPt[12]={
		img->width/2 , 0,                   img->width,    img->width/2,
		0,                  img->height/2,  img->height/2, img->height,
		1, 1, 1, 1};
	double InitM[12]={0,};

	McornerPt = cvCreateMat(3, 4, CV_64FC1);
	memcpy(McornerPt->data.db, cornerPt, sizeof(double)*12);
	MInterCornerPt = cvCreateMat(3, 4, CV_64FC1);
	memcpy(MInterCornerPt->data.db, InterCornerPt, sizeof(double)*12);
	MTransformedCornerPt = cvCreateMat(3,4, CV_64FC1);
	memcpy(MTransformedCornerPt->data.db, InitM, sizeof(double)*12);
	MInterTransformedCornerPt = cvCreateMat(3,4, CV_64FC1);
	memcpy(MInterTransformedCornerPt->data.db, InitM, sizeof(double)*12);


	ReleasepMatchPair();
	pMatchPair = new MATCH_PAIR[3000];

	return 1;		

}

int CMareMatchingClass::fFindMatchingPoints(void)
{
	if(BG_Keypoints == NULL ||  Patch_Keypoints == NULL)
		return 0;

	int descriptor_size = params.extended? 128 : 64;
	nMatchingCount = FindMatchingPoints(BG_Keypoints, BG_Descriptors, Patch_Keypoints, Patch_Descriptors, descriptor_size, pMatchPair);
	printf("matching count = %d\n", nMatchingCount);

	if(nMatchingCount == 0)
		return 0;

	//////////////////////////////////////////////////////////////////////////
	ReleasepMatchPairPT();
	BGpt1 = new CvPoint2D32f[nMatchingCount];
	PTpt2 = new CvPoint2D32f[nMatchingCount];
	
	CvSURFPoint* surf1, *surf2;
	for(int k=0; k<nMatchingCount; k++)
	{
		//매칭 k번째의 T1, T2에서의 키 포인트 정보 뽑기
		surf1 = (CvSURFPoint*) cvGetSeqElem(BG_Keypoints, pMatchPair[k].nA);
		surf2 = (CvSURFPoint*) cvGetSeqElem(Patch_Keypoints, pMatchPair[k].nB);

		BGpt1[k] = surf1->pt;
		PTpt2[k] = surf2->pt;		
	}

	return 1;
}

int CMareMatchingClass::FindMatchingPoints(const CvSeq* tKeypoints, const CvSeq* tDescriptors, const CvSeq* srcKeypoints, const CvSeq* srcDescriptors, int descriptor_size, MATCH_PAIR *pMatchPair)
{
	int i;
	float* pA;
	int nMatchB;
	CvSURFPoint* surfA;
	int k=0;
	for(i=0; i<tDescriptors->total; i++)
	{
		pA = (float*) cvGetSeqElem(tDescriptors, i);
		surfA = (CvSURFPoint*) cvGetSeqElem(tKeypoints, i);
		nMatchB = FindNearestPoints(pA, surfA->laplacian, srcKeypoints, srcDescriptors, descriptor_size);
		if(nMatchB > 0)
		{
			pMatchPair[k].nA=i;
			pMatchPair[k].nB = nMatchB;
			k++;
		}
	}

	return k;
}


int CMareMatchingClass::FindNearestPoints(const float* pA, int laplacian, const CvSeq* srcKeypoints, const CvSeq* srcDescriptors, int descriptor_size)
{
	int i, k;
	float* pB;
	CvSURFPoint *surfB;
	int nMatch = -1;
	double sum2, min1 = 10000, min2 = 10000;
	for(i=0; i<srcDescriptors->total; i++)
	{
		surfB = (CvSURFPoint*) cvGetSeqElem(srcKeypoints, i);
		pB = (float*) cvGetSeqElem(srcDescriptors, i);
		if(laplacian != surfB->laplacian)
			continue;

		sum2 = 0.0f;
		for(k=0; k<descriptor_size; k++)	{	sum2 +=(pA[k]-pB[k])*(pA[k]-pB[k]);	}

		if(sum2 < min1)
		{
			min2 = min1;
			min1 = sum2;
			nMatch = i;
		}
		else if(sum2<min2)	{	min2 = sum2;	}
	}
	if(min1<0.6*min2)
		return nMatch;

	return -1;
}
void CMareMatchingClass::GetHMulRect(CvMat* M, Rect4Pt* rect4Pt)
{
	CvMat* MTransformedCornerPt2 = cvCreateMat(3,3, CV_64FC1);
	double InitM[9]={0,};
	memcpy(MTransformedCornerPt2->data.db, InitM, sizeof(double)*9);

	cvMatMul(mBestH, M, MTransformedCornerPt2);

	for(int i=0; i<MTransformedCornerPt2->cols; ++i)
	{
		if( cvmGet(MTransformedCornerPt2, 2,i) != 1)
		{
			cvmSet(MTransformedCornerPt2, 0,i, cvmGet(MTransformedCornerPt2, 0,i) / cvmGet(MTransformedCornerPt2, 2,i) );
			cvmSet(MTransformedCornerPt2, 1,i, cvmGet(MTransformedCornerPt2, 1,i) / cvmGet(MTransformedCornerPt2, 2,i) );
			cvmSet(MTransformedCornerPt2, 2,i, cvmGet(MTransformedCornerPt2, 2,i) / cvmGet(MTransformedCornerPt2, 2,i) );
		}
	}	
	
	rect4Pt->LeftTop.x = cvmGet(MTransformedCornerPt2, 0,0);
	rect4Pt->LeftTop.y = cvmGet(MTransformedCornerPt2, 1,0);
	rect4Pt->RightTop.x = cvmGet(MTransformedCornerPt2, 0,1);
	rect4Pt->RightTop.y = cvmGet(MTransformedCornerPt2, 1,1);
	rect4Pt->RightBottom.x = cvmGet(MTransformedCornerPt2, 0,2);
	rect4Pt->RightBottom.y = cvmGet(MTransformedCornerPt2, 1,2);

	cvReleaseMat(&MTransformedCornerPt2);
}

bool CMareMatchingClass::GetObjectRectAndBestH(IplImage* img, Rect4Pt* rect4Pt)
{

	if( ExtractBGsurf(img) == 0 )
		return 0;

	//Matching Start
	if( fFindMatchingPoints() == 0)
	{
		printf("no matching point!! \n");
		return 0;
	}	

	if(nMatchingCount < 50 )
		return 0;
	
	FindBestH();	

	if( inCount <= 5)
		return 0;
	
	//Drawing

	rect4Pt->LeftTop.x = cvmGet(mBestCorner, 0,0);
	rect4Pt->LeftTop.y = cvmGet(mBestCorner, 1,0);
	rect4Pt->RightTop.x = cvmGet(mBestCorner, 0,1);
	rect4Pt->RightTop.y  = cvmGet(mBestCorner, 1,1);
	rect4Pt->RightBottom.x = cvmGet(mBestCorner, 0,2);
	rect4Pt->RightBottom.y = cvmGet(mBestCorner, 1,2);
	rect4Pt->LeftBottom.x = cvmGet(mBestCorner, 0,3);
	rect4Pt->LeftBottom.y = cvmGet(mBestCorner, 1,3);


	return 1;

}

int CMareMatchingClass::FindBestH()
{	
	#define NN 6	
	CvMat M1, M2;	
	double tV1[NN*2]={0,};
	double tV2[NN*2]={0,};
	M1 = cvMat(NN,2, CV_32FC1,tV1);
	M2 = cvMat(NN,2, CV_32FC1,tV2);
	
	double H[9]={0,};
	CvMat mxH = cvMat(3, 3, CV_64F, H);
	srand( (unsigned int)time(NULL) );


	inCount=0;
	double Inliner=-100;
	int iteration=0;
	vector< int > RandV;
	while( iteration < 50 )
	{
		//printf("%d\n", iteration);
		
		iteration++;

		//한번 선택한 인덱스는 선택하지 않는다.
		int tRandI = rand() % nMatchingCount;
		int Bpoint=0;
		for(int i=0; i<RandV.size(); ++i)
		{
			if(RandV[i] == tRandI)
			{
				Bpoint=1;
				break;
			}
		}
		if(Bpoint)
			continue;
		
		////////////////////////////////////////////////////////////////////////////

		
		//printf("sampling\n");
		//Sampling
		//가까운점 찾는 함수, 전체를 비교하고 거리를 구하고, 소팅을 한다 계산시간이 복잡함.
		//GetClosetPTN(tRandI, BGpt1, nMatchingCount, &M1, &M2, NN, BGpt1, PTpt2);
		GetRandomPTN(tRandI, BGpt1, nMatchingCount, &M1, &M2, NN, BGpt1, PTpt2);

		
		//printMat(M1);
		//printMat(M2);
			
		//printf("Find Homo\n");
		//Homography
		if( !cvFindHomography(&M2, &M1, &mxH, CV_RANSAC, 2))
		{
			//printf("Find Homography Fail!\n");
			continue;
		}
		
		
		//////////////////////////////////////////////////////////////////////////
		//H inspection
		//printf("Multiply\n");
		cvMatMul(&mxH, McornerPt, MTransformedCornerPt);
		cvMatMul(&mxH, MInterCornerPt, MInterTransformedCornerPt);
		
		//printMat(*MTransformedCornerPt);
		//printMat(*MInterTransformedCornerPt);

		//printf("Normalization\n");		
		for(int i=0; i<MTransformedCornerPt->cols; ++i)
		{
			if( cvmGet(MTransformedCornerPt, 2,i) != 1)
			{
				cvmSet(MTransformedCornerPt, 0,i, cvmGet(MTransformedCornerPt, 0,i) / cvmGet(MTransformedCornerPt, 2,i) );
				cvmSet(MTransformedCornerPt, 1,i, cvmGet(MTransformedCornerPt, 1,i) / cvmGet(MTransformedCornerPt, 2,i) );
				cvmSet(MTransformedCornerPt, 2,i, cvmGet(MTransformedCornerPt, 2,i) / cvmGet(MTransformedCornerPt, 2,i) );
			}

			if(cvmGet(MInterTransformedCornerPt, 2,i) != 1)
			{
				cvmSet(MInterTransformedCornerPt, 0,i, cvmGet(MInterTransformedCornerPt, 0,i) / cvmGet(MInterTransformedCornerPt, 2,i) );
				cvmSet(MInterTransformedCornerPt, 1,i, cvmGet(MInterTransformedCornerPt, 1,i) / cvmGet(MInterTransformedCornerPt, 2,i) );
				cvmSet(MInterTransformedCornerPt, 2,i, cvmGet(MInterTransformedCornerPt, 2,i) / cvmGet(MInterTransformedCornerPt, 2,i) );
			}
		}
		
		//printMat(*MTransformedCornerPt);
		//printMat(*MInterTransformedCornerPt);
		//printf("Homo Ok?\n");
		if( HisOk(MTransformedCornerPt, MInterTransformedCornerPt,  BGWidth, BGHeight) == false )
		{
			//printf("Homography strengh!\n");
			continue;
		}

		RandV.push_back(tRandI);
		//printf("%d\n", tRandI);

		//////////////////////////////////////////////////////////////////////////		
		//cvWarpPerspective(PatchImg, WarpPathImg, &mxH);	

		//printf("Matching Score\n");
		RectPT rectPt;		
		GetRectPt(MTransformedCornerPt, &rectPt);
		double distSum = MatchingScore(BG_Keypoints, Patch_Keypoints, &mxH, &rectPt);

		
		if( Inliner < distSum)
		{
			printf("best update %d\n", inCount);
			//printMat(mxH);
			//printMat(*MTransformedCornerPt);
			//printMat(*mBestH);
			//printMat(*mBestCorner);

			Inliner = distSum;
			inCount = Inliner;
			CopyMat(&mxH, mBestH);
			CopyMat(MTransformedCornerPt, mBestCorner);
			BestRectPt = rectPt;

			//printMat(*mBestH);
			//printMat(*mBestCorner);

		}
		

		//printf("======================================\n");		
	}

	

	return 0;
}

void CMareMatchingClass::printMat(CvMat& M)
{
	printf("%d x %d\n", M.rows, M.cols);
	
	for(int i=0; i<M.rows; ++i)
	{
		for(int j=0;j<M.cols; ++j)
		{
			printf("%lf ", cvmGet(&M,i,j) );
		}
		printf("\n");
	}
}


void CMareMatchingClass::CopyMat(CvMat* M1, CvMat* M2)
{
	//printf("CopyMat\n");
	//printf("%d x %d\n", M1->rows, M1->cols);
	//printf("%d x %d\n", M2->rows, M2->cols);

	
	//memcpy(M2->data.db, M1->data.db, sizeof(CV_ELEM_SIZE(M1->type))*M1->rows*M1->cols);
	
	for(int i=0; i<M1->rows; ++i)
	{
		for(int j=0;j<M1->cols; ++j)
		{
			//printf("%d %d = %lf\n", i,j, cvmGet(M1,i,j));
			cvmSet(M2, i,j, cvmGet(M1,i,j) );
		}		
	}
}


double CMareMatchingClass::MatchingScore(const CvSeq* OriginKeyPt, const CvSeq * CompareKeyPt, CvMat* H, RectPT* rectPt)
{
	double aB[3]={1,1,1};
	CvMat mB = cvMat(3, 1, CV_64FC1, aB);
	double aA[3]={0,};
	CvMat mA = cvMat(3, 1, CV_64FC1, aA);

	double distanceSum=0;
	double thDist=1;
	double calDist;
	double x1,x2,y1,y2;

	CvSURFPoint* surfA;
	int count=0;
	for(int i=0; i<CompareKeyPt->total; ++i)
	{
		//A=HB
		surfA = (CvSURFPoint*) cvGetSeqElem(CompareKeyPt, i);
		cvmSet(&mB, 0,0, surfA->pt.x);
		cvmSet(&mB, 1,0, surfA->pt.y);		

		//H로 좌표 변환		
		cvMatMul(H, &mB, &mA);

		//정규화
		x1 = cvmGet(&mA,0,0)/cvmGet(&mA,2,0);
		y1 = cvmGet(&mA,1,0)/cvmGet(&mA,2,0);

		//printf("[%d]%lf %lf \n",i, x1, y1 );

		for(int j=0; j<OriginKeyPt->total; ++j)
		{
			surfA = (CvSURFPoint*) cvGetSeqElem(OriginKeyPt, j);

			//선택시 영역으로 잘라내기
			if( surfA->pt.x < rectPt->left || surfA->pt.x > rectPt->right)
				continue;
			if( surfA->pt.y < rectPt->top || surfA->pt.y > rectPt->bottom)
				continue;
			//OriginKeyPt에서 가까운 점 선택
			x2 = surfA->pt.x;
			y2 = surfA->pt.y;

			calDist = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );

			if( calDist < thDist )
			{
				distanceSum += calDist;
				count++;
				//printf("[%d]%lf %lf - [%d]%lf %lf=%lf\n",i, x1, y1,j, x2, y2, calDist);
			}			
		}

	}

	//printf("sum=%lf count=%d", distanceSum, count);
	//개수 리턴.
	//return distanceSum;
	return count;
}


void CMareMatchingClass::GetRectPt( CvMat * M, RectPT* rectPt)
{

	double l,r,t,b;
	l=t=10000;
	r=b=-10000;

	for(int i=0; i<M->cols; ++i)
	{
		double x = cvmGet(M, 0, i);
		double y = cvmGet(M, 1, i);

		if(x < l )
			l = x;
		if(x > r)
			r = x;
		if(y < t)
			t = y;
		if(y > b)
			b = y;
	}

	//printf("%lf %lf %lf %lf\n", l, r, t, b);

	rectPt->left = l;
	rectPt->right = r;
	rectPt->top = t;
	rectPt->bottom = b;
}

bool CMareMatchingClass::HisOk(CvMat* tH, CvMat* tIH, int w, int h)
{
	//check #1
	for(int c=0; c<tH->cols; ++c)
	{
		if( cvmGet(tH,0,c) <0 || cvmGet(tH,0,c) > w || cvmGet(tH,1,c) <0 || cvmGet(tH,1,c) > h)
		{			
			return false;
		}		
	}


	//check #2
	//left,top -> right,top
	double m,b;
	double x0, y0, x1, y1, x2, y2;
	double e;
	double xs, xl, ys, yl;

	x0 = cvmGet(tH,0,0);	
	y0 = cvmGet(tH,1,0);
	x2 = cvmGet(tH,0,1);
	y2 = cvmGet(tH,1,1);	
	x1 = cvmGet(tIH, 0,0);
	y1 = cvmGet(tIH, 1,0);

	if( IsOnTheLine(x1, y1, x0, y0, x2, y2) == false)
		return false;
	if( IsInTheBound(x1, y1, x0, y0, x2, y2) == false )
		return false;

	//left,top -> left, bottom
	x0 = cvmGet(tH,0,1);
	y0 = cvmGet(tH,1,1);
	x2 = cvmGet(tH,0,2);
	y2 = cvmGet(tH,1,2);	
	x1 = cvmGet(tIH, 0,2);
	y1 = cvmGet(tIH, 1,2);	

	if( IsOnTheLine(x1, y1, x0, y0, x2, y2) == false)
		return false;
	if( IsInTheBound(x1, y1, x0, y0, x2, y2) == false )
		return false;


	//right,top -> right,bottom
	x0 = cvmGet(tH,0,2);
	y0 = cvmGet(tH,1,2);
	x2 = cvmGet(tH,0,3);
	y2 = cvmGet(tH,1,3);
	x1 = cvmGet(tIH, 0,3);
	y1 = cvmGet(tIH, 1,3);

	if( IsOnTheLine(x1, y1, x0, y0, x2, y2) == false)
		return false;
	if( IsInTheBound(x1, y1, x0, y0, x2, y2) == false )
		return false;


	//left,bottom -> right, bottom
	x0 = cvmGet(tH,0,3);
	y0 = cvmGet(tH,1,3);
	x2 = cvmGet(tH,0,0);
	y2 = cvmGet(tH,1,0);
	x1 = cvmGet(tIH, 0,1);
	y1 = cvmGet(tIH, 1,1);

	if( IsOnTheLine(x1, y1, x0, y0, x2, y2) == false)
		return false;
	if( IsInTheBound(x1, y1, x0, y0, x2, y2) == false )
		return false;


	return true;
}

void CMareMatchingClass::GetRandomPTN(int tRandI, CvPoint2D32f *BGpt1, int nMatchingCount, CvMat* M1, CvMat* M2, int n, CvPoint2D32f *BGpt, CvPoint2D32f *PTpt)
{

	vector< int > selectedIndex;
	//가까운점 N개 뽑기	
	selectedIndex.push_back(tRandI);

	int Bpoint;
	int newI;
	int count=1;
	while(1)
	{
		Bpoint=0;
		newI = rand() % nMatchingCount;
		
		for(int i=0; i<selectedIndex.size(); ++i)
		{
			if( selectedIndex[i] == newI)
			{
				Bpoint=1;
				break;
			}
		}
		if(Bpoint == 1)
			continue;

		count++;
		selectedIndex.push_back(newI);

		if( count >= n )
			break;
	}
	//sort
	
	//최상위 N개
	for(int i=0; i<selectedIndex.size(); ++i)
	{
		cvmSet(M1,i,0, BGpt[ selectedIndex[i] ].x );
		cvmSet(M1,i,1, BGpt[ selectedIndex[i] ].y );
		cvmSet(M2,i,0, PTpt[ selectedIndex[i] ].x );
		cvmSet(M2,i,1, PTpt[ selectedIndex[i] ].y );
	}

}

void CMareMatchingClass::GetClosetPTN(int tRandI, CvPoint2D32f *BGpt1, int nMatchingCount, CvMat* M1, CvMat* M2, int n, CvPoint2D32f *BGpt, CvPoint2D32f *PTpt){
	
	
	vector< pair<int, double> > ptDist;
	//가까운점 N개 뽑기

	ptDist.push_back( make_pair(tRandI, 0) );

	for(int i=0; i<nMatchingCount; ++i)
	{
		if(i==tRandI)
			continue;
		//거리 계산
		double dist = sqrt( (BGpt[tRandI].x-BGpt[i].x)*(BGpt[tRandI].x-BGpt[i].x) + (BGpt[tRandI].y-BGpt[i].y)*(BGpt[tRandI].y-BGpt[i].y) );
		ptDist.push_back( make_pair(i, dist) );
	}
	//sort
	sort(ptDist.begin(), ptDist.end(), compare);		
	//최상위 N개
	for(int i=0; i<n; ++i)
	{
		cvmSet(M1,i,0, BGpt[ ptDist[i].first ].x );
		cvmSet(M1,i,1, BGpt[ ptDist[i].first ].y );
		cvmSet(M2,i,0, PTpt[ ptDist[i].first ].x );
		cvmSet(M2,i,1, PTpt[ ptDist[i].first ].y );
	}
}


bool CMareMatchingClass::IsOnTheLine(double x1, double y1, double x0, double y0, double x2, double y2)
{
	double m, b, e;

	m = (y2-y0) / (x2-x0);
	b = -m*x0 + y0;

	e = (m*x1+b) - y1;	
	e = sqrt(e*e);


	if( e> 0.5)
   	    false;

	return true;
}

bool CMareMatchingClass::IsInTheBound(double tx, double ty, double x0, double y0, double x2, double y2)
{
	double xl, xr, yl, yr;

	if(x0 < x2)
	{
		xl = x0;
		xr = x2;
	}else{
		xl = x2;
		xr = x0;
	}


	if(y0 < y2)
	{
		yl = y0;
		yr = y2;
	}else{
		yl = y2;
		yr = y0;
	}

	if( !( (xl<=tx) && (xr>=tx) && (yl<=ty) && (yr>=ty) ) )
		return false;

	return true;
}
