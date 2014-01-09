#include "sft/ChannelFeatureBuilder_impl.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/core.hpp"
//#include "sft/softcascade_initialization.hpp"

ChannelFeatureBuilderImpl::ChannelFeatureBuilderImpl()
{
    m_totalChannels = 1;
}
ChannelFeatureBuilderImpl::~ChannelFeatureBuilderImpl()
{

}
cv::Ptr<ChannelFeatureBuilderImpl> ChannelFeatureBuilderImpl::create(const cv::String& featureType)
{
    bool flag = initModule_softcascade();
    cv::Ptr<ChannelFeatureBuilderImpl> alg = Algorithm::create<ChannelFeatureBuilderImpl>("Octave." + featureType);
    return alg;
    
}
void ChannelFeatureBuilderImpl::operator()(InputArray src, OutputArray channels, cv::Size channelsSize ) const
{

}
int ChannelFeatureBuilderImpl::totalChannels() const
{
    return m_totalChannels;
}
cv::AlgorithmInfo* ChannelFeatureBuilderImpl::info() const
{
}
