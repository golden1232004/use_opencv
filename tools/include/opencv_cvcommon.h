#ifndef __CVCOMMON_H_
#define __CVCOMMON_H_

#include "opencv2/core.hpp"

#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/cxmisc.h"

#define __BEGIN__ __CV_BEGIN__
#define __END__  __CV_END__
#define EXIT __CV_EXIT__

#ifndef PATH_MAX
#define PATH_MAX 512
#endif /* PATH_MAX */

#define CV_RANDOM_INVERT 0x7FFFFFFF


int opencv_icvMkDir( const char* filename );

/* returns index at specified position from index matrix of any type.
   if matrix is NULL, then specified position is returned */
CV_INLINE
int opencv_icvGetIdxAt( CvMat* idx, int pos );

CV_INLINE
int opencv_icvGetIdxAt( CvMat* idx, int pos )
{
    if( idx == NULL )
    {
        return pos;
    }
    else
    {
        CvScalar sc;
        int type;

        type = CV_MAT_TYPE( idx->type );
        cvRawDataToScalar( idx->data.ptr + pos *
            ( (idx->rows == 1) ? CV_ELEM_SIZE( type ) : idx->step ), type, &sc );

        return (int) sc.val[0];
    }
}

/* debug functions */

#define CV_DEBUG_SAVE( ptr ) icvSave( ptr, __FILE__, __LINE__ );

void opencv_icvSave( const CvArr* ptr, const char* filename, int line );

#endif /* __CVCOMMON_H_ */
