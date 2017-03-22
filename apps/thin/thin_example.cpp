#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace std;

void cvThin( IplImage*, IplImage*, int iterations);

//使用举例

int main(int argc, char* argv[])
{
 IplImage *pSrc = NULL,*pDst = NULL,*pTmp = NULL;

//传入一个灰度图像
 pSrc = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
 if(!pSrc)
 {
  return 0;
 }

 pTmp = cvCloneImage(pSrc);
 pDst = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
 cvZero(pDst);
 cvThreshold(pSrc,pTmp,68,255,CV_THRESH_BINARY_INV);//做二值处理，将图像转换成0，1格式
 cvSaveImage("binary.jpg", pTmp);

 cvThin(pTmp,pDst,8);//细化，通过修改iterations参数进一步细化
 cvSaveImage("thin.jpg", pDst);

 cvReleaseImage(&pSrc);
 cvReleaseImage(&pDst);
 cvReleaseImage(&pTmp);

 return 0;
}


void cvThin( IplImage* src, IplImage* dst, int iterations)
{
#define byte unsigned char

#define WHITE 255
#define BLACK 0
 //此时的src是一个二值化的图片
 CvSize size = cvGetSize(src);
 cvCopy(src, dst);

 int n = 0,i = 0,j = 0;
 for(n=0; n<iterations; n++)//开始进行迭代
 {
   IplImage* t_image = cvCloneImage(dst);
   for(i=0; i<size.height;  i++)
  {
   for(j=0; j<size.width; j++)
   {
    if(CV_IMAGE_ELEM(t_image,byte,i,j)==WHITE)
    {
     int ap=0;
     int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
     int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
     if (p2==0 && p3==WHITE)
     {
      ap++;
     }

     int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
     if(p3==0 && p4==WHITE)
     {
      ap++;
     }

     int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
     if(p4==0 && p5==WHITE)
     {
      ap++;
     }

     int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
     if(p5==0 && p6==WHITE)
     {
      ap++;
     }

     int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
     if(p6==0 && p7==WHITE)
     {
      ap++;
     }

     int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
     if(p7==0 && p8==WHITE)
     {
      ap++;
     }

     int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
     if(p8==0 && p9==WHITE)
     {
      ap++;
     }
     if(p9==0 && p2==WHITE)
     {
      ap++;
     }

     if((p2+p3+p4+p5+p6+p7+p8+p9)>1*WHITE && (p2+p3+p4+p5+p6+p7+p8+p9)<7*WHITE)
     {
      if(ap==1)
      {
       if(!(p2 && p4 && p6))
       {
        if(!(p4 && p6 && p8))
        {
         CV_IMAGE_ELEM(dst,byte,i,j)=BLACK;//设置目标图像中像素值为0的点
        }
       }
      }
     }

    }
   }
  }

  cvReleaseImage(&t_image);

  t_image = cvCloneImage(dst);
  for(i=0; i<size.height;  i++)
  {
   for(int j=0; j<size.width; j++)
   {
    if(CV_IMAGE_ELEM(t_image,byte,i,j)==WHITE)
    {
     int ap=0;
     int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
     int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
     if (p2==BLACK && p3==WHITE)
     {
      ap++;
     }
     int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
     if(p3==BLACK && p4==WHITE)
     {
      ap++;
     }
     int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
     if(p4==BLACK && p5==WHITE)
     {
      ap++;
     }
     int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
     if(p5==BLACK && p6==WHITE)
     {
      ap++;
     }
     int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
     if(p6==BLACK && p7==WHITE)
     {
      ap++;
     }
     int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
     if(p7==BLACK && p8==WHITE)
     {
      ap++;
     }
     int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
     if(p8==BLACK && p9==WHITE)
     {
      ap++;
     }
     if(p9==BLACK && p2==WHITE)
     {
      ap++;
     }
     if((p2+p3+p4+p5+p6+p7+p8+p9)>1*WHITE && (p2+p3+p4+p5+p6+p7+p8+p9)<7*WHITE)
     {
      if(ap==1)
      {
       if(p2*p4*p8==0)
       {
        if(p2*p6*p8==0)
        {
         CV_IMAGE_ELEM(dst, byte,i,j)=BLACK;
        }
       }
      }
     }
    }

   }

  }
  cvReleaseImage(&t_image);
 }

}
