#ifndef __OPENCV_FUNCTION_HPP__
#define __OPENCV_FUNCTION_HPP__

#include "opencv/cv.h"

void opencv_icvWriteVecHeader( FILE* file, int count, int width, int height );
void opencv_icvWriteVecSample( FILE* file, CvArr* sample );
int opencv_cvCreateTrainingSamplesFromInfo( const char* infoname, 
                                            const char* vecfilename,           
					    int num,
                                            int showsamples,
					    int winwidth, int winheight );


#endif  //__OPENCV_FUNCTION_HPP__ 
