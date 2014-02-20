#ifndef _OPENCV_IMAGESTORAGE_H_
#define _OPENCV_IMAGESTORAGE_H_

#include "opencv/highgui.h"



class CvCascadeImageReader
{
public:
    bool create( const std::string _posFilename, const std::string _negFilename, cv::Size _winSize );
    void restart() { posReader.restart(); }
    bool getNeg(cv::Mat &_img) { return negReader.get( _img ); }
    bool getPos(cv::Mat &_img) { return posReader.get( _img ); }

private:
    class PosReader
    {
    public:
        PosReader();
        virtual ~PosReader();
        bool create( const std::string _filename );
        bool get( cv::Mat &_img );
        void restart();

        short* vec;				/**< the current positive sample */
        FILE*  file;			/**< refer to the comment of the method of create(filename). */
        int    count;			/**< the number of the pos samples. */
        int    vecSize;			/**< a positive sample's size, i.e., winSize.cols * winSize.rows. */
        int    last;			/**< the index pointed to the next unreaded pos sample. */
        int    base;			/**< the offset of the data in the file and there's the file header
								 *   in front of it. */
    } posReader;

    class NegReader
    {
    public:
        NegReader();
        bool create( const std::string _filename, cv::Size _winSize );
        bool get( cv::Mat& _img );
        bool nextImg();

        cv::Mat     src, img;
        std::vector<std::string> imgFilenames;
        cv::Point   offset, point;
        float   scale;
        float   scaleFactor;
        float   stepFactor;
        size_t  last, round;
        cv::Size    winSize;
    } negReader;
};

#endif
