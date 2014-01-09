/*********************************************************************
 *
 *   implementation of abstract base class -- ChannelFeatureBuilder
 *
 *********************************************************************/
#include "opencv2/softcascade.hpp"
#include "opencv2/core.hpp"

using namespace cv::softcascade;
using namespace cv;

class ChannelFeatureBuilderImpl : public ChannelFeatureBuilder
{
public:
    ChannelFeatureBuilderImpl();
    virtual ~ChannelFeatureBuilderImpl();
    CV_WRAP_AS(compute) void operator()(InputArray src, OutputArray channels, cv::Size channelsSize = cv::Size()) const;

    CV_WRAP int totalChannels() const;
    virtual cv::AlgorithmInfo* info() const ;

    CV_WRAP static cv::Ptr<ChannelFeatureBuilderImpl> create(const String& featureType);
private:
    int m_totalChannels;
    
};
