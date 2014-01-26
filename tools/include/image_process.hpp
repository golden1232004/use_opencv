#ifndef __IMAGE_PROCESS_HPP__
#define __IMAGE_PROCESS_HPP__

namespace tk{

#define IMAGE_MAX_WIDTH (600)
#define IMAGE_MAX_HEIGHT (450)

class ImageProcessor{

public:
    ImageProcessor();
    virtual ~ImageProcessor();
    
public:
    float getScale(const int src_width, const int src_height);

private:
    int m_width;
    int m_height;
    float m_scale;

};

}

#endif // __IMAGE_PROCESS_HPP__
