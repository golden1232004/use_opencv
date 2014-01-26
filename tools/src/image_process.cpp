#include "image_process.hpp"
#include <algorithm>

using namespace tk;
using namespace std;

ImageProcessor::ImageProcessor()
{
    m_width = IMAGE_MAX_WIDTH;
    m_height = IMAGE_MAX_HEIGHT;
}
float ImageProcessor::getScale(const int width, const int height)
{
    float scale = 1;
    if (width >= height) {
        scale = max(width/(float)m_width, height/(float)m_height);
    }
    else {
        scale = max(width/(float)m_height, height/(float)m_width);
    }
    if (scale < 1) 
        scale = 1;
    m_scale = scale;
    return scale;
}
