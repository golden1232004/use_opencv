#ifndef _OPENCV_HAARFEATURES_H_
#define _OPENCV_HAARFEATURES_H_

#include "traincascade_features.h"

#define CV_HAAR_FEATURE_MAX      3

#define HFP_NAME "haarFeatureParams"

/**
 * CvHaarFeatureParams encapsulate the specified parameters for HAAR.
 * 
 */

class CvHaarFeatureParams : public CvFeatureParams
{
public:
    enum { BASIC = 0, CORE = 1, ALL = 2 };
     /* 0 - BASIC = Viola
     *  1 - CORE  = All upright
     *  2 - ALL   = All features */

    CvHaarFeatureParams();
    CvHaarFeatureParams( int _mode );

    virtual void init( const CvFeatureParams& fp );
    virtual void write( cv::FileStorage &fs ) const;
    virtual bool read( const cv::FileNode &node );

    virtual void printDefaults() const;
    virtual void printAttrs() const;
    virtual bool scanAttr( const std::string prm, const std::string val);

    int mode;					/**< BASIC/CORE/ALL */
};

class CvHaarEvaluator : public CvFeatureEvaluator
{
public:
    virtual void init(const CvFeatureParams *_featureParams,
        int _maxSampleCount, cv::Size _winSize );
    virtual void setImage(const cv::Mat& img, uchar clsLabel, int idx);
    virtual float operator()(int featureIdx, int sampleIdx) const;
    virtual void writeFeatures( cv::FileStorage &fs, const cv::Mat& featureMap ) const;
    void writeFeature( cv::FileStorage &fs, int fi ) const; // for old file fornat
protected:
    virtual void generateFeatures();

    /**
     * Feature is used to define a HAAR features, which is defined by
     * 2 or 3 rectangles with weights and a rotation angle.
     */
    class Feature
    {
    public:
        Feature();
        Feature( int offset, bool _tilted,
            int x0, int y0, int w0, int h0, float wt0,
            int x1, int y1, int w1, int h1, float wt1,
            int x2 = 0, int y2 = 0, int w2 = 0, int h2 = 0, float wt2 = 0.0F );
        float calc( const cv::Mat &sum, const cv::Mat &tilted, size_t y) const;
        void write( cv::FileStorage &fs ) const;

        bool  tilted;			/**< if it's 45 degree angle. */
        struct
        {
            cv::Rect r;
            float weight;
        } rect[CV_HAAR_FEATURE_MAX]; /**< rectangles used to define a HAAR feature. */

        struct
        {
            int p0, p1, p2, p3;
        } fastRect[CV_HAAR_FEATURE_MAX]; /**< The corresponding of the 'rect'. This member
                                          * variable is used for convenience and efficiency. */
    };

    std::vector<Feature> features;
	cv::Mat  sum;         /* sum images (NxM, N is the max sample count, M = (winSize.width+1)*(winSize.height+1). each row represents a integral image) */
	cv::Mat  tilted;      /* tilted sum images (NxM. each row represents a tilted integral image) */
	cv::Mat  normfactor;  /* normalization factor. 1xN */
};

/** 
 * @return return the responding value of the given feature.
 */
inline float CvHaarEvaluator::operator()(int featureIdx, int sampleIdx) const
{
    float nf = normfactor.at<float>(0, sampleIdx);
    return !nf ? 0.0f : (features[featureIdx].calc( sum, tilted, sampleIdx)/nf);
}

inline float CvHaarEvaluator::Feature::calc( const cv::Mat &_sum, const cv::Mat &_tilted, size_t y) const
{
    const int* img = tilted ? _tilted.ptr<int>((int)y) : _sum.ptr<int>((int)y);
    float ret = rect[0].weight * (img[fastRect[0].p0] - img[fastRect[0].p1] - img[fastRect[0].p2] + img[fastRect[0].p3] ) +
        rect[1].weight * (img[fastRect[1].p0] - img[fastRect[1].p1] - img[fastRect[1].p2] + img[fastRect[1].p3] );
    if( rect[2].weight != 0.0f )
        ret += rect[2].weight * (img[fastRect[2].p0] - img[fastRect[2].p1] - img[fastRect[2].p2] + img[fastRect[2].p3] );
    return ret;
}

#endif
